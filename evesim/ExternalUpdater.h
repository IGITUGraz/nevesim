/*
 * ExternalUpdater.h
 *
 *  Created on: 26.09.2012
 *      Author: kappel
 */

#ifndef EXTERNALUPDATER_H_
#define EXTERNALUPDATER_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"

class ExternalUpdater : public EvSimObject {

	EV_SIM_OBJECT( ExternalUpdater )

public:
	ExternalUpdater() {};
	virtual ~ExternalUpdater();

	virtual unsigned numInputPorts() const
	{
		return 1;
	};
};

#endif /* EXTERNALUPDATER_H_ */
