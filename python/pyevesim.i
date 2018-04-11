/* pyevesim.i

   Created on: Jul 11, 2012
     Author: dejan
*/

%module pyevesim




%{
#define SWIG_PYTHON_CAST_MODE
#define SWIG_FILE_WITH_INIT
#include "NeurSamplingNetwork.h"
#include "EvSimObjectTypeUtils.h"
#include "EventTimesRecorder.h"
#include "ArrayInputNeuron.h"
#include "EvSimRegularClock.h"
#include "ExternalEventGenerator.h"
#include "ExternalUpdater.h"
#include "VariableRecorder.h"
#include "VariableRecorderTypes.h"
#include "VariableArrayBasedModifier.h"
#include "VariableArrayBasedModifierTypes.h"
#include "GenericPoissonNeuron.h"
#include "GenericPLPoissonNeuron.h"
#include "ExpPoissonNeuron.h"
#include "PLExpPoissonNeuron.h"
#include "BasicSynapse.h"
#include "PLBasicSynapse.h"
#include "BinomSEMSynapse.h"
#include "BinomNSamplSEMSynapse.h"
#include "PopulCodeSEMSynapse.h"
#include "PopulCodeNSamplSEMSynapse.h"
#include "NSHebbianSynapse.h"
#include "BiasHomeostasisMechanism.h"
#include "BiasAsymmHomeostMechanism.h"
#include "BiasAdaptiveHomeostMechanism.h"
#include "BiasWakeSleepPlasticityMechanism.h"
#include "BiasSEMPlasticityMechanism.h"
#include "BinaryArrayInputSynapse.h"
#include "BinaryArrayPulseGenerator.h"
#include "MultiValArrayPulseGenerator.h"
#include "BinaryInputSynapse.h"
#include "ResetRectSpikeResponse.h"
#include "KernelSpikeResponseBase.h"
#include "PLAdditiveKernelSpikeResponse.h"
#include "ResetKernelSpikeResponse.h"
#include "PLResetKernelSpikeResponse.h"
#include "DblExpSpikeResponse.h"
#include "CompositeSynapse.h"
#include "SharedArray.h"
#include "SharedArrayTypes.h"
%}

%exception {
	try {
		$action
	} catch(evesim::Exception &e) {
		PyErr_SetString( PyExc_RuntimeError, e.message().c_str() );
		SWIG_fail;
	} 
}


%include <pyfragments.swg>
%include <numpy.i>

%init %{
import_array();
%}

%include <boost_shared_ptr.i>
%include <std_vector.i>

namespace std {
   %template(IntValuesVector) vector<int>;
   %template(DoubleValuesVector) vector<double>;
   %template(FloatValuesVector) vector<float>;
   %template(BoolValuesVector) vector<bool>;
   %template(UnsignedValuesVector) vector<unsigned>;
   %template(ShortIntValuesVector) vector<short>;
}

%shared_ptr(DoubleValuesVector);

%include <std_string.i>
%include <std_pair.i>


%rename(__getitem__) SharedDblArray::operator[];
%rename(__getitem__) SharedFloatArray::operator[];
%rename(__getitem__) SharedIntArray::operator[];
%rename(__getitem__) SharedShortIntArray::operator[];
%rename(__getitem__) SharedBoolArray::operator[];


%fragment("NumPy_Fragments");

%typemap(out) (EvSimObject *) {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIG_TypeQuery($1->getTypeNameWithPtr().c_str()), 0 |  0 );
}


%typemap(out) const std::vector<double> & {
 npy_intp length = (npy_intp)$1->size();
 $result = PyArray_SimpleNew(1, &length, PyArray_DOUBLE);
 if (length)
 	memcpy(PyArray_DATA($result),&((*$1)[0]),sizeof(double)*length);
}


%typemap(in) const std::vector<double> & {
    int is_new_object = 0;
    PyArrayObject *array = obj_to_array_contiguous_allow_conversion(
                                            $input, NPY_DOUBLE, &is_new_object);
    npy_intp size[1] = {-1};
    if (!array || !require_dimensions(array, 1) || !require_size(array, size, 1)) { 
    	if (is_new_object && array) Py_DECREF(array);
        SWIG_fail;
    }
	std::vector<double> *temp_vec = new std::vector<double>(array->dimensions[0]);
	if (array->dimensions[0])
		memcpy(&(*temp_vec)[0], PyArray_DATA(array), sizeof(double)*array->dimensions[0]);
	if (is_new_object && array) Py_DECREF(array);
	$1 = temp_vec;
}

%typemap(freearg) const std::vector<double> & {
   free($1);
}




%typemap(in) std::vector<bool> const & {
    int is_new_object = 0;
    PyArrayObject *array = obj_to_array_contiguous_allow_conversion(
                                            $input, NPY_BOOL, &is_new_object);
    npy_intp size[1] = {-1};
    if (!array || !require_dimensions(array, 1) || !require_size(array, size, 1)) { 
    	if (is_new_object && array) Py_DECREF(array);
        SWIG_fail;
    }
	std::vector<bool> *temp_vec = new std::vector<bool>(array->dimensions[0]);
	if (array->dimensions[0]) {
		std::copy((npy_bool *)PyArray_DATA(array), (npy_bool *)PyArray_DATA(array) + array->dimensions[0], 
			temp_vec->begin());
	}
	if (is_new_object && array) Py_DECREF(array);
	$1 = temp_vec;
}

%typemap(freearg) std::vector<bool> const & {
   free($1);
}


%typemap(in) std::vector<unsigned short> const & {
    int is_new_object = 0;
    PyArrayObject *array = obj_to_array_contiguous_allow_conversion(
                                            $input, NPY_UINT16, &is_new_object);
    npy_intp size[1] = {-1};
    if (!array || !require_dimensions(array, 1) || !require_size(array, size, 1)) { 
    	if (is_new_object && array) Py_DECREF(array);
        SWIG_fail;
    }
	std::vector<unsigned short> *temp_vec = new std::vector<unsigned short>(array->dimensions[0]);
	if (array->dimensions[0]) {
		std::copy((npy_ushort *)PyArray_DATA(array), (npy_ushort *)PyArray_DATA(array) + array->dimensions[0], 
			temp_vec->begin());
	}
	if (is_new_object && array) Py_DECREF(array);
	$1 = temp_vec;
}

%typemap(freearg) std::vector<unsigned short> const & {
   free($1);
}

%typemap(out) const std::vector<unsigned short> & {
 npy_intp length = (npy_intp)$1->size();
 $result = PyArray_SimpleNew(1, &length, PyArray_UINT16);
 if (length)
 	memcpy(PyArray_DATA($result),&((*$1)[0]),sizeof(unsigned short)*length);
}





%typemap(in) EvSimObject::IDVector {
  	int is_new_object = 0;
  	PyArrayObject *array = obj_to_array_contiguous_allow_conversion(
	                                        $input, NPY_INT, &is_new_object);
	npy_intp size[1] = {-1};
   	if (!array || !require_dimensions(array, 1) || !require_size(array, size, 1)) { 
		if (is_new_object && array) Py_DECREF(array);
	    SWIG_fail;
	}
	shared_ptr< std::vector<EvSimObject::ID> > temp_vec = 
	           shared_ptr< std::vector<EvSimObject::ID> >(new std::vector<EvSimObject::ID>(array->dimensions[0]));
	memcpy(&(*temp_vec)[0], PyArray_DATA(array), sizeof(EvSimObject::ID)*array->dimensions[0]);
	if (is_new_object && array) Py_DECREF(array);
	$1 = temp_vec;
}


%typemap(in) EvSimObject::IDVector & (EvSimObject::IDVector temp_vec
                             = EvSimObject::IDVector(new vector<EvSimObject::ID>())) 
{
	$1 = &temp_vec ;
}

%typemap(argout) EvSimObject::IDVector & {
	npy_intp length = (npy_intp)(*$1)->size();
	$result = PyArray_SimpleNew(1, &length, PyArray_INT);
	memcpy(PyArray_DATA($result),&((**$1)[0]),sizeof(EvSimObject::ID)*length);
}


%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) EvSimObject::IDVector {
	$1 = (PySequence_Check($input)) ? 1 : 0;  
}

%typemap(out) EvSimObject::IDVector {
 npy_intp length = (npy_intp)$1->size();
 $result = PyArray_SimpleNew(1, &length, NPY_INT);
 memcpy(PyArray_DATA($result),&((*$1)[0]),sizeof(int)*length);    
}






%rename("$ignore", regextarget=1) "field_registrator_func";
%rename("$ignore", regextarget=1) "EventSender";

%include "globals.h"


%extend SharedArray {
  const T __getitem__(size_t n) const {
  	T val = (*$self)[n];
  	return val;
  }
  
  %ignore operator[];
}


%include "SharedArray.h"



%template(SharedDblArray) SharedArray<double>;
%template(SharedBoolArray) SharedArray<bool>;
%template(SharedFloatArray) SharedArray<float>;
%template(SharedIntArray) SharedArray<int>;
%template(SharedShortIntArray) SharedArray<short>;


%include "DirectEventSender.h"
%include "ProjectedEventSender.h"

%include "EvSimObjectTypeUtils.h"
%include "EvSimObjectFactory.h"
%include "EvSimObject.h"

%include "EvSimNetwork.h"
%include "NeurSamplingNetwork.h"
%include "SimParameters.h"

%include "EvSimRegularClock.h"
%include "ExternalEventGenerator.h"
%include "ExternalUpdater.h"
%include "BinaryArrayPulseGenerator.h"
%include "MultiValArrayPulseGenerator.h"

%include "Neuron.h"
%include "GenericPoissonNeuron.h"

%template(Generic_Fast_PoissonNeuron) GenericPoissNeuronFast<PoissonNeuronBase>;
%template(Generic_Exp_Fast_PoissonNeuron) GenericExpPoissonNeuron<GenericPoissNeuronFast<PoissonNeuronBase> >;
%template(Generic_Complete_Exp_Fast_PoissonNeuron) GenericPoissonNeuronComplete<GenericExpPoissonNeuron<GenericPoissNeuronFast<PoissonNeuronBase> > >;
 
%include "GenericPLPoissonNeuron.h"

%template(Generic_PL_Fast_PoissonNeuron) GenericPLPoissNeuronFast<PoissonNeuronBase>;
%template(Generic_PL_Exp_Fast_PoissonNeuron) GenericPLExpPoissonNeuron<GenericPLPoissNeuronFast<PoissonNeuronBase> >;
%template(Generic_Complete_PL_Exp_Fast_PoissonNeuron) GenericPoissonNeuronComplete< GenericPLExpPoissonNeuron< GenericPLPoissNeuronFast<PoissonNeuronBase> > >;

%include "ExpPoissonNeuron.h"
%include "PLExpPoissonNeuron.h"

%include "EventTimesRecorder.h"
%include "ArrayInputNeuron.h"

%include "VariableRecorder.h"

%template(VariableRecorderImpl_double) VariableRecorderImpl<double>;
%template(VariableRecorderImpl_int) VariableRecorderImpl<int>;
%template(VariableRecorderImpl_unsigned) VariableRecorderImpl<unsigned>;
%template(VariableRecorderImpl_float) VariableRecorderImpl<float>;
%template(VariableRecorderImpl_bool) VariableRecorderImpl<bool>;
%template(VariableRecorderImpl_short) VariableRecorderImpl<short>;

%include "VariableRecorderTypes.h"

%include "SpikeResponse.h"
%include "ResetRectSpikeResponse.h"
%include "KernelSpikeResponseBase.h"
%include "ResetKernelSpikeResponse.h"
%include "PLResetKernelSpikeResponse.h"
%include "PLAdditiveKernelSpikeResponse.h"
%include "DblExpSpikeResponse.h"

%include "Synapse.h"
%include "PlasticSynapse.h"

%include "GenericBasicSynapse.h"

%template(GenBasicSynapse__Synapse) GenericBasicSynapse<Synapse >;
%template(GenBasicSynapse__ActiveSynapse) GenericBasicSynapse<ActiveSynapse >;

%include "BasicSynapse.h"

%template(GenBasicSynapse__PlasticNSSyn) GenericBasicSynapse<PlasticNSSynapse >;
%template(GenBasicSynapse__PlasicNSActiveSyn) GenericBasicSynapse<PlasticNSActiveSynapse >;

%include "GenericPLBasicSynapse.h"

%template(GenPLBasicSynapse__Synapse) GenericPLBasicSynapse< PLSynapse >;
%template(GenPLBasicSynapse__ActiveSynapse) GenericPLBasicSynapse< PLActiveSynapse >;

%include "PLBasicSynapse.h"

%include "GenericBinomSEMSynapse.h"
 
%template (GenBinomSEMSynapse__BasicSynapse__Syn) GenericBinomSEMSynapse< GenericBasicSynapse< PlasticNSSynapse > >;
%template (GenBinomSEMSynapse__BasicSynapse__ActiveSyn) GenericBinomSEMSynapse< GenericBasicSynapse< PlasticNSActiveSynapse > >;

%include "BinomSEMSynapse.h"

%include "GenericBinomNSamplSEMSynapse.h"
 
%template (GenBinomNSamplSEMSynapse__BasicSynapse__Syn) GenericBinomNSamplSEMSynapse< GenericBasicSynapse< PlasticNSSynapse > >;
%template (GenBinomNSamplSEMSynapse__BasicSynapse__ActiveSyn) GenericBinomNSamplSEMSynapse< GenericBasicSynapse< PlasticNSActiveSynapse > >;

%include "BinomNSamplSEMSynapse.h"


%include "GenericPopulCodeSEMSynapse.h"
 
%template (GenPopulCodeSEMSynapse__BasicSynapse__Syn) GenericPopulCodeSEMSynapse< GenericBasicSynapse< PlasticNSSynapse > >;
%template (GenPopulCodeSEMSynapse__BasicSynapse__ActiveSyn) GenericPopulCodeSEMSynapse< GenericBasicSynapse< PlasticNSActiveSynapse > >;

%include "PopulCodeSEMSynapse.h"

%include "GenericPopulCodeNSamplSEMSynapse.h"
 
%template (GenPopulCodeNSamplSEMSynapse__BasicSynapse__Syn) GenericPopulCodeNSamplSEMSynapse< GenericBasicSynapse< PlasticNSSynapse > >;
%template (GenPopulCodeNSamplSEMSynapse__BasicSynapse__ActiveSyn) GenericPopulCodeNSamplSEMSynapse< GenericBasicSynapse< PlasticNSActiveSynapse > >;

%include "PopulCodeNSamplSEMSynapse.h"


%include "GenericNSHebbianSynapse.h"

%template (GenNSHebbianSynapse__BasicSynapse__Syn) GenericNSHebbianSynapse< GenericBasicSynapse< PlasticNSSynapse > > ;
%template (GenNSHebbianSynapse__BasicSynapse__ActiveSyn) GenericNSHebbianSynapse< GenericBasicSynapse< PlasticNSActiveSynapse > >;

%include "NSHebbianSynapse.h"

%include "GenericBinaryArrayInputSynapse.h"

%template(GenBinaryArrayInputSynapse__Synapse) GenericBinaryArrayInputSynapse<Synapse > ;
%template(GenBinaryArrayInputSynapse__ActiveSynapse) GenericBinaryArrayInputSynapse<ActiveSynapse >;

%include "BinaryArrayInputSynapse.h"

%include "GenericBinaryInputSynapse.h"

%template(GenBinaryInputSynapse__Synapse) GenericBinaryInputSynapse<Synapse > ;
%template(GenBinaryInputSynapse__ActiveSynapse) GenericBinaryInputSynapse<ActiveSynapse >;

%include "BinaryInputSynapse.h"

%include "BiasHomeostasisMechanism.h"
%include "BiasAsymmHomeostMechanism.h"
%include "BiasAdaptiveHomeostMechanism.h"
%include "BiasWakeSleepPlasticityMechanism.h"
%include "BiasSEMPlasticityMechanism.h"

%include "CompositeSynapse.h"

%include "VariableArrayBasedModifier.h"

%template(VariableArrayBasedModifier_double) VariableArrayBasedModifier<double>;
%template(VariableArrayBasedModifier_int) VariableArrayBasedModifier<int>;
%template(VariableArrayBasedModifier_unsigned) VariableArrayBasedModifier<unsigned>;
%template(VariableArrayBasedModifier_float) VariableArrayBasedModifier<float>;
%template(VariableArrayBasedModifier_bool) VariableArrayBasedModifier<bool>;
%template(VariableArrayBasedModifier_short) VariableArrayBasedModifier<short>;

%include "VariableArrayBasedModifierTypes.h"

