#include "RecorderSelector.h"

RecorderSelector recorderSelector;

RecorderSelector::~RecorderSelector()
{
	RecorderRegistryType::iterator it;
	for (it = recorder_registry.begin() ; it != recorder_registry.end() ; ++it)
		delete it->second;
}
