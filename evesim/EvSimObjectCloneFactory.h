#ifndef EvSimObjectCloneFactory_H_
#define EvSimObjectCloneFactory_H_

#include "EvSimObjectFactory.h"

//! This class provides the functionality for cloning EvSimObject instances
template< class T >
class EvSimObjectCloneFactory : public EvSimObjectFactory
{

public:

    EvSimObjectCloneFactory(T &src) : source(src)
    { /* NOOP */ };

    EvSimObjectCloneFactory(void)
    { /* NOOP */ };

    virtual ~EvSimObjectCloneFactory()
    { /* NOOP */ };

    virtual EvSimObject* create(void) const
    {
        return new T(source);
    };

    virtual EvSimObject* create( RandomEngine *  ) const
    {
        return new T(source);
    };

protected:
    T source;

};

#endif /*EvSimObjectCloneFactory_H_*/
