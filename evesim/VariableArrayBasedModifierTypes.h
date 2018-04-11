#ifndef VARIABLEARRAYBASEDMODIFIERTYPES_H_
#define VARIABLEARRAYBASEDMODIFIERTYPES_H_

#include "VariableArrayBasedModifier.h"
#include "EvSimObjectTypeUtils.h"

class DoubleVariableModifier : public VariableArrayBasedModifier<double> {
	EV_SIM_OBJECT( DoubleVariableModifier )

	DoubleVariableModifier(const EvSimNetwork & net, const EvSimObject::ID & obj_id,
			                 const string & var_name, const std::vector<double> & values, bool cyclic=false)
		: VariableArrayBasedModifier<double>(net, obj_id, var_name, values, cyclic)
	{};



};

class IntVariableModifier: public VariableArrayBasedModifier<int> {
	EV_SIM_OBJECT( IntVariableModifier )
};

class FloatVariableModifier : public VariableArrayBasedModifier<float> {
	EV_SIM_OBJECT( FloatVariableModifier )
};

class UnsignedVariableModifier : public VariableArrayBasedModifier<unsigned> {
	EV_SIM_OBJECT( UnsignedVariableModifier )
};

class BoolVariableModifier : public VariableArrayBasedModifier<bool> {
	EV_SIM_OBJECT( BoolVariableModifier )
};

class ShortIntVariableModifier: public VariableArrayBasedModifier<short> {
	EV_SIM_OBJECT( ShortIntVariableModifier )
};



#endif /* VARIABLEARRAYBASEDMODIFIERTYPES_H_ */
