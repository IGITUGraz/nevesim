#ifndef GENERICBINARYARRAYINPUTSYNAPSE_H_
#define GENERICBINARYARRAYINPUTSYNAPSE_H_

#include "EvSimObjectTypeUtils.h"
#include "Synapse.h"
#include "SharedArrayTypes.h"

#include <vector>

#ifndef SWIG

#include <iostream>
using std::cerr;
using std::endl;

#endif

#include "SharedArrayTypes.h"
#include "SimContext.h"


template<class BaseSyn>
class GenericBinaryArrayInputSynapse : public BaseSyn {

public:
	GenericBinaryArrayInputSynapse(SharedBoolArray & shared_bin_array,
								   float psr_level_hi = 5,
								   float psr_level_lo = 0,
								   bool isActive = true) {
		this->bin_array = shared_bin_array.get();
		is_arr_owner = false;
		this->psr_level_hi = psr_level_hi;
		this->psr_level_lo = psr_level_lo;
		this->isActive = isActive;
	}

	float psr_level_hi;

	float psr_level_lo;

	GenericBinaryArrayInputSynapse(const vector<bool> & bin_array,
								   float psr_level_hi = 5,
								   float psr_level_lo = 0,
								   bool isActive = true) {
		this->bin_array = new vector<bool>(bin_array);
		is_arr_owner = true;
		this->psr_level_hi = psr_level_hi;
		this->psr_level_lo = psr_level_lo;
		this->isActive = isActive;
	}

	const std::vector<bool> & getBinaryArray() {
		return *bin_array;
	}

	void setBinaryArray(const vector<bool> & bin_array) {
		if (is_arr_owner) {
			if (this->bin_array)
				delete this->bin_array;
		}
		is_arr_owner = true;
		this->bin_array = new vector<bool>(bin_array);
	}

	void setBinaryArray(SharedBoolArray & shared_bin_array) {
		if (is_arr_owner) {
			if (bin_array)
				delete bin_array;
		}
		is_arr_owner = false;
		this->bin_array = shared_bin_array.get();
	}

	virtual ~GenericBinaryArrayInputSynapse() {
		if (is_arr_owner)
			delete bin_array;
	}

	virtual EvSimObject *copy() const {
		GenericBinaryArrayInputSynapse<BaseSyn> *obj =
				     dynamic_cast<GenericBinaryArrayInputSynapse<BaseSyn> *>(this->EvSimObject::copy());
		if (is_arr_owner)
			obj->bin_array = new vector<bool>(*bin_array);
		return obj;
	}

	virtual void reset(SimContext &sim_ctxt);

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);

protected:
	double psr;
	const vector<bool> *bin_array;
	unsigned curr_idx;
	bool is_arr_owner;
	bool isActive;
};

template<class BaseSyn>
void GenericBinaryArrayInputSynapse<BaseSyn>::eventHit(SimContext & sim_ctxt, Time time, port_id_t port)
{
	if (port == 0) {
		if (curr_idx == bin_array->size())
			curr_idx = 0;
		if (isActive)
			BaseSyn::updatePSR(((*bin_array)[curr_idx++]) ? psr_level_hi : psr_level_lo, time);
	}
	else if (port == 1) {
		isActive = !isActive;
		if (!isActive)
			BaseSyn::updatePSR(0, time);
		else
			BaseSyn::updatePSR(((*bin_array)[curr_idx]) ? psr_level_hi : psr_level_lo, time);
	}
}


template<class BaseSyn>
void GenericBinaryArrayInputSynapse<BaseSyn>::reset(SimContext &sim_ctxt)
{
	BaseSyn::resetPSRs();
	curr_idx = 0;
}

#endif /* GENERICBINARYARRAYINPUTSYNAPSE_H_ */
