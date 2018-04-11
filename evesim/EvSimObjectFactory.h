#ifndef EvSIMOBJECTFACTORY_H_
#define EvSIMOBJECTFACTORY_H_

#include "globals.h"

class EvSimObject;
class RandomEngine;

//! This class provides the interface for creating SimObject instances
class EvSimObjectFactory
{

public:

    virtual ~EvSimObjectFactory()
    { /* NOOP */ };

    //! Derived classes implement this function to create clones
    virtual EvSimObject* create( void ) const { return 0; };
    
    //! Derived classes implement this function to create clones
    virtual EvSimObject* create( RandomEngine *rng ) const { return 0; };

};

#endif /*EvSIMOBJECTFACTORY_H_*/
