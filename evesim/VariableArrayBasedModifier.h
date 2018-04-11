/*
 * VariableArrayBasedModifier.h
 *
 *  Created on: Aug 2, 2012
 *      Author: dejan
 */

#ifndef VARIABLEARRAYBASEDMODIFIER_H_
#define VARIABLEARRAYBASEDMODIFIER_H_

#include "EvSimObject.h"
#include "EvSimNetwork.h"
#include "SharedArray.h"

#include <stdexcept>

#ifndef SWIG

#include <iostream>
using std::cerr; using std::endl;

#endif




template<typename T>
class VariableArrayBasedModifier : public EvSimObject {

public:

	VariableArrayBasedModifier() : owns_array(false), values_arr(0), is_cyclic(false)
	{}

	VariableArrayBasedModifier(const VariableArrayBasedModifier & copy_obj)
	{
		is_cyclic = copy_obj.is_cyclic;
		owns_array = copy_obj.owns_array;
		mod_var_ptr = copy_obj.mod_var_ptr;
		values_arr = copy_obj.values_arr;
		const_cast<VariableArrayBasedModifier &>(copy_obj).owns_array = false;
	}

	VariableArrayBasedModifier(const EvSimNetwork & net, const EvSimObject::ID & obj_id, const string &var_name, const vector<T> & values, const bool cyclic=false)
		: is_cyclic(cyclic)
	{
		values_arr = new vector<T>(values);
		owns_array = true;
		EvSimObject *obj = net.getObject(obj_id);
		mod_var_ptr = (T *)obj->getVariablePtr(var_name);
	}

	VariableArrayBasedModifier(const SharedArray<T> & values, bool cyclic=false)
	    : owns_array(false), values_arr(values.get()), is_cyclic(cyclic)
	{}


	virtual ~VariableArrayBasedModifier() {
		if (owns_array) {
			delete values_arr;
		}
	};


	void setValuesArray(const SharedArray<T> & values) {
		values_arr = values.get();
		owns_array = false;
	}

	virtual void eventHit(SimContext & sim_info, Time time, port_id_t port) {
		if (port == 1) {
			curr_idx=0;
		}
		if (port == 0 || port == 1) {
			(*mod_var_ptr) = (*values_arr)[curr_idx];
			if (curr_idx < values_arr->size() - 1)
				curr_idx++;
			else if (is_cyclic)
				curr_idx=0;
		}
	}

	virtual void reset(SimContext &sim_ctxt)
	{
		curr_idx = 0;
	}

	virtual unsigned numInputPorts() const
	{
		return 2;
	};

	void setModifiedVariable(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID obj_id, const string &var_name, update_id_t upd_id = -1) {
		EvSimObject *obj = net.getObject(obj_id);
		std::cout << "I am here!" << std::endl;
		setModifiedVariable(obj, var_name);
		if (upd_id >= 0)
			net.causalUpdateLink(self, obj_id, upd_id);
	}

	void setModifiedVariable(EvSimObject *obj, const string &var_name) {
		mod_var_ptr = (T *)obj->getVariablePtr(var_name);
		if (!mod_var_ptr)
		{
			throw( std::invalid_argument(std::string("VariableArrayBasedModifier::setModifiedVariable: Object has no variable named '")+var_name+"'." ) );
		}
		if (obj->getVariableType(var_name) != typeid(T))
		{
			throw( std::invalid_argument(std::string("VariableArrayBasedModifier::setModifiedVariable: Modified variable and modifier must be of same type. Source type: ")+
                                                      typeid(T).name()+" destination type ('"+var_name+"'): "+obj->getVariableType(var_name).name()+"." ) );
		}		
	}

	void setCyclic(bool cyclic) {
		is_cyclic = cyclic;
	}

protected:
	T *mod_var_ptr;
	bool owns_array;
	unsigned curr_idx;
	const vector<T> * values_arr;
	bool is_cyclic;
};

#endif /* VARIABLEARRAYBASEDMODIFIER_H_ */
