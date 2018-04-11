#ifndef SIMPARAMETERS_H_
#define SIMPARAMETERS_H_

#include "globals.h"

class SimParameters {
public:
	SimParameters(unsigned rnd_seed = 12341231, Time default_clock_tick = 1e-3) : rnd_seed(rnd_seed),
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 default_clock_tick(default_clock_tick) {};

	unsigned rnd_seed;
	Time default_clock_tick;
};

#endif /* SIMPARAMETERS_H_ */
