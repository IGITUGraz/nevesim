#ifndef BINARYARRAYINPUTSYNAPSE_H_
#define BINARYARRAYINPUTSYNAPSE_H_

#include "Synapse.h"
#include "GenericBinaryArrayInputSynapse.h"
#include "EvSimObjectTypeUtils.h"

#include <vector>

class BinaryArrayInputSynapse : public GenericBinaryArrayInputSynapse<Synapse> {

	EV_SIM_OBJECT( BinaryArrayInputSynapse )

public:
	BinaryArrayInputSynapse(SharedBoolArray & shared_bin_array,
			   	   	   	    float psr_level_hi = 2,
			   	   	   	    float psr_level_lo = 0,
			   	   	   	    bool isActive = true) :
		GenericBinaryArrayInputSynapse<Synapse>(shared_bin_array, psr_level_hi, psr_level_lo, isActive)
	{}

	BinaryArrayInputSynapse(const std::vector<bool> & bin_array = std::vector<bool>(),
	   	   	    			float psr_level_hi = 2,
	   	   	    			float psr_level_lo = 0,
	   	   	    			bool isActive = true) :
	   	GenericBinaryArrayInputSynapse<Synapse>(bin_array, psr_level_hi, psr_level_lo, isActive)
	{}

};

class BinaryArrInpActiveSynapse : public GenericBinaryArrayInputSynapse<ActiveSynapse> {

	EV_SIM_OBJECT( BinaryArrInpActiveSynapse )

public:
	BinaryArrInpActiveSynapse(SharedBoolArray & shared_bin_array,
			   	   	   	    float psr_level_hi = 5,
			   	   	   	    float psr_level_lo = 0,
			   	   	   	    bool isActive = true) :
		GenericBinaryArrayInputSynapse<ActiveSynapse>(shared_bin_array, psr_level_hi, psr_level_lo, isActive)
	{}

	BinaryArrInpActiveSynapse(const std::vector<bool> & bin_array = std::vector<bool>(),
	   	   	    			float psr_level_hi = 5,
	   	   	    			float psr_level_lo = 0,
	   	   	    			bool isActive = true) :
	   	GenericBinaryArrayInputSynapse<ActiveSynapse>(bin_array, psr_level_hi, psr_level_lo, isActive)
	{}

};


#endif /* BINARYARRAYINPUTSYNAPSE_H_ */
