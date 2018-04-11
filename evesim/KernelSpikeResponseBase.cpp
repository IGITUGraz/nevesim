#include "KernelSpikeResponseBase.h"

void KernelSpikeResponseBase::postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self)
{
	net.connect(self, 0, self, 1);
}

