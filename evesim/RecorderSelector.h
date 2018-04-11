#ifndef RECORDERSELECTOR_H_
#define RECORDERSELECTOR_H_

#include "VariableRecorder.h"
#include "VariableRecorderTypes.h"

class RecorderSelector {
public:

	RecorderSelector();

	virtual ~RecorderSelector();

	void registerRecorder(const string & type, VariableRecorder * const rec);

	VariableRecorder & selectAppropriateRecorder(const type_info & type) const;

protected:
	typedef unordered_map<string, VariableRecorder *, evesim::hash<string> > RecorderRegistryType;
	RecorderRegistryType recorder_registry;
};


inline RecorderSelector::RecorderSelector()
{
	recorder_registry.insert(make_pair(string(typeid(int).name()), (VariableRecorder *)(new IntVariableRecorder())));
	recorder_registry.insert(make_pair(string(typeid(short).name()), (VariableRecorder *)(new ShortIntVariableRecorder())));
	recorder_registry.insert(make_pair(string(typeid(double).name()), (VariableRecorder *)(new DoubleVariableRecorder())));
	recorder_registry.insert(make_pair(string(typeid(float).name()), (VariableRecorder *)(new FloatVariableRecorder())));
	recorder_registry.insert(make_pair(string(typeid(unsigned).name()), (VariableRecorder *)(new UnsignedVariableRecorder())));
	recorder_registry.insert(make_pair(string(typeid(bool).name()), (VariableRecorder *)(new BoolVariableRecorder())));
}


inline void RecorderSelector::registerRecorder(const string & type, VariableRecorder * const rec)
{
	recorder_registry.insert(make_pair(type, rec));
}


inline VariableRecorder & RecorderSelector::selectAppropriateRecorder(const type_info & type) const
{
	RecorderRegistryType::const_iterator it = recorder_registry.find(string(type.name()));
	if (it == recorder_registry.end()) {
		throw ( evesim::ConstructionException("RecorderSelector::getAppropriateRecorder",
									  boost::str( boost::format("Can not find an appropriate recorder for a variable of type %s") % type.name()) ) );
	}
	return *it->second;
}

extern RecorderSelector recorderSelector;

#endif /* RECORDERSELECTOR_H_ */
