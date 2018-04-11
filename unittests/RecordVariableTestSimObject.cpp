#include "RecordVariableTestSimObject.h"

EV_SIM_OBJECT_POST( RecordVariableTestSimObject )

FIELD_REGISTRATOR( RecordVariableTestSimObject )
{
	REGISTER_FIELD(RecordVariableTestSimObject, v_int )
	REGISTER_FIELD(RecordVariableTestSimObject, v_dbl )
	REGISTER_FIELD(RecordVariableTestSimObject, v_float )
	REGISTER_FIELD(RecordVariableTestSimObject, v_unsigned )
}

RecordVariableTestSimObject::~RecordVariableTestSimObject() {
	// TODO Auto-generated destructor stub
}

