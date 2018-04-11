#ifndef RECORDVARIABLETESTSIMOBJECT_H_
#define RECORDVARIABLETESTSIMOBJECT_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"

class RecordVariableTestSimObject : public EvSimObject {
	EV_SIM_OBJECT( RecordVariableTestSimObject )

public:
	RecordVariableTestSimObject(const vector<double> &amp_levels = vector<double>())
	    : v_dbl(0), v_float(0), v_int(0), v_unsigned(0), curr_idx(0), amp_levels(amp_levels)
	{}

	virtual ~RecordVariableTestSimObject();

	virtual void reset(SimContext &sim_ctxt)
	{
		if (curr_idx < amp_levels.size()) {
			v_int = (int)amp_levels[0];
			v_dbl = amp_levels[0];
			v_float = amp_levels[0];
			v_unsigned = (unsigned)amp_levels[0];
		}
		curr_idx = 1;
	};


	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port)
	{
		if (curr_idx < amp_levels.size()) {
			v_int = (int)amp_levels[curr_idx];
			v_dbl = amp_levels[curr_idx];
			v_float = amp_levels[curr_idx];
			v_unsigned = (unsigned)amp_levels[curr_idx];
		}
		curr_idx++;
	};

protected:
	double v_dbl;
	float v_float;
	int v_int;
	unsigned v_unsigned;
	unsigned curr_idx;
	vector<double> amp_levels;
};


#endif /* RECORDVARIABLETESTSIMOBJECT_H_ */
