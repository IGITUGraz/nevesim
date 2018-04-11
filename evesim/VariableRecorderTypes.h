#ifndef VARIABLERECORDERTYPES_H_
#define VARIABLERECORDERTYPES_H_

#include "VariableRecorder.h"

class DoubleVariableRecorder : public VariableRecorderImpl<double> {
	EV_SIM_OBJECT( DoubleVariableRecorder )
};

class IntVariableRecorder: public VariableRecorderImpl<int> {
	EV_SIM_OBJECT( IntVariableRecorder )
};

class FloatVariableRecorder : public VariableRecorderImpl<float> {
	EV_SIM_OBJECT( FloatVariableRecorder )
};

class UnsignedVariableRecorder : public VariableRecorderImpl<unsigned> {
	EV_SIM_OBJECT( UnsignedVariableRecorder )
};

class BoolVariableRecorder : public VariableRecorderImpl<bool> {
	EV_SIM_OBJECT( BoolVariableRecorder )
};

class ShortIntVariableRecorder: public VariableRecorderImpl<short> {
	EV_SIM_OBJECT( ShortIntVariableRecorder )
};



typedef DoubleVariableRecorder::ValuesVector DoubleValuesVector;
typedef FloatVariableRecorder::ValuesVector FloatValuesVector;
typedef IntVariableRecorder::ValuesVector IntValuesVector;
typedef UnsignedVariableRecorder::ValuesVector UnsignedValuesVector;
typedef BoolVariableRecorder::ValuesVector BoolValuesVector;
typedef ShortIntVariableRecorder::ValuesVector ShortIntValuesVector;


#endif /* VARIABLERECORDERTYPES_H_ */
