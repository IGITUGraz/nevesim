#ifndef VARIABLERECORDER_H_
#define VARIABLERECORDER_H_

#include "EvSimObject.h"
#include "SimException.h"
#include "EvSimObjectTypeUtils.h"

#include <vector>
using std::vector;

#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>

#ifndef SWIG

#include <typeinfo>
using std::type_info;

#endif


#include "evesim_hash.h"

class VariableRecorder : public EvSimObject {
public:

	virtual ~VariableRecorder();

	virtual void eventHit(SimContext & sim_info, Time time, port_id_t port) { /* NOOP */ };

	virtual void setRecordedSimObject(EvSimObject *obj, const string &var_name) { /* NOOP */ };

};


template <typename T>
class VariableRecorderImpl : public VariableRecorder {

public:
	typedef vector<T> ValuesVector;

	VariableRecorderImpl() {};

	virtual ~VariableRecorderImpl() {};

	virtual void eventHit(SimContext & sim_info, Time time, port_id_t port) {
		rec_values.push_back(*rec_var_ptr);
	}

	virtual void setRecordedSimObject(EvSimObject *obj, const string &var_name) {
		rec_var_ptr = (T *)obj->getVariablePtr(var_name);
	}


	const std::vector<T> & getRecordedValues() const {
		return rec_values;
	}

protected:
	T *rec_var_ptr;
	vector<T> rec_values;

};


#endif /* VARIABLERECORDER_H_ */
