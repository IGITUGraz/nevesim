#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"
#include <cstdlib>

EvSimObject::EvSimObject()
{

}

EvSimObject::~EvSimObject()
{

}


const EvSimObject::ID INVALID_EVSIMOBJECT_ID = std::numeric_limits<EvSimObject::ID>::max();


EvSimObject::BlockAllocator EvSimObject::defaultAllocator;
EvSimObject::BlockAllocator *EvSimObject::currentAllocator = &EvSimObject::defaultAllocator;


EvSimObject *EvSimObject::copy() const {
	if (this->getSimObjTypeUtils())
		return this->getSimObjTypeUtils()->copy(this);
	return 0;
}


const string & EvSimObject::getTypeName() const {
	return this->getSimObjTypeUtils()->getTypeName();
}

EvSimObject *EvSimObject::create() const {
     return this->copy();
}


void * const EvSimObject::getVariablePtr(const string &var_name) const
{
	return this->getSimObjTypeUtils()->getVariablePtr(this, var_name);
}



const type_info & EvSimObject::getVariableType(const string &var_name) const
{
	return this->getSimObjTypeUtils()->getVariableType(this, var_name);
}



EvSimObject::BlockAllocator::BlockAllocator( size_t blockSize  ) :
        blk_sz( blockSize )
{
    mem_block.resize(1);
    free_pos = (char *)malloc( blk_sz );
    mem_block[0] = free_pos;
    num_free_bytes = blk_sz;
}

EvSimObject::BlockAllocator::~BlockAllocator()
{
    for( size_t i=0; i < mem_block.size(); i++ ) {
        free( mem_block[i] );
    }
}

void *EvSimObject::BlockAllocator::allocate( size_t size )
{
    void *p;
    if( num_free_bytes < size ) {
        if( size > blk_sz ) {
            free_pos = (char *)malloc( size );
            num_free_bytes = size;
        } else {
            free_pos = (char *)malloc( blk_sz );
            num_free_bytes = blk_sz;
        }
        //printf("--------------------- new block of size %i\n", num_free_bytes);
        mem_block.push_back( free_pos );
    }
    p = free_pos;
    free_pos += size;
    num_free_bytes -= size;
    //printf("- new obj of size %i (%i) at %p\n", size, num_free_bytes, p);
    return p;
}

void EvSimObject::setAllocator( BlockAllocator & ba )
{
    currentAllocator = &ba;
}

void *EvSimObject::operator new(size_t sz)
{
    void *p = currentAllocator->allocate(sz);
    currentAllocator = &defaultAllocator;
    return p;
}

void EvSimObject::operator delete(void *p, size_t size)
{
    //printf("- delete obj of size %i at %p\n", size, p);
    // WE DO NOTHING HERE SINCE THE EvSimObject::BlockAllocator FREES ANY MEMORY
}

void *EvSimObject::operator new[](size_t sz)
{
    void *p = currentAllocator->allocate(sz);
    currentAllocator = &defaultAllocator;
    return p;
}

void EvSimObject::operator delete[](void *, size_t)
{
    // WE DO NOTHING HERE SINCE THE EvSimObject::BlockAllocator FREES ANY MEMORY
}

void EvSimObject::setVariable(const string &var_name, double new_value)
{
    void *var_ptr = getVariablePtr(var_name);
    if (!var_ptr) {
        throw( std::invalid_argument(std::string("Object has no variable named '")+var_name+"'.") );                        
    }
    const type_info &typ = getVariableType(var_name);
    if (typ == typeid(double))
        *static_cast<double*>(var_ptr) = static_cast<const double>(new_value);
    else if (typ == typeid(float))
        *static_cast<float*>(var_ptr) = static_cast<const float>(new_value);
    else if (typ == typeid(int))
        *static_cast<int*>(var_ptr) = static_cast<const int>(new_value);
    else if (typ == typeid(unsigned int))
        *static_cast<unsigned int*>(var_ptr) = static_cast<const unsigned int>(new_value);
    else if (typ == typeid(char))
        *static_cast<char*>(var_ptr) = static_cast<const char>(new_value);
    else if (typ == typeid(unsigned char))
        *static_cast<unsigned char*>(var_ptr) = static_cast<const unsigned char>(new_value);
    else
        throw( std::invalid_argument(std::string("Unexpected field type '")+typ.name()+"'.") );
}	
	
double EvSimObject::getVariable(const string &var_name) const
{
    void *var_ptr = getVariablePtr(var_name);
    if (!var_ptr) {
        throw( std::invalid_argument(std::string("Object has no variable named '")+var_name+"'.") );
    }
    const type_info &typ = getVariableType(var_name);
    if (typ == typeid(double))
        return *static_cast<double*>(var_ptr);
    else if (typ == typeid(float))
        return *static_cast<float*>(var_ptr);
    else if (typ == typeid(int))
        return *static_cast<int*>(var_ptr);
    else if (typ == typeid(unsigned int))
        return *static_cast<unsigned int*>(var_ptr);
    else if (typ == typeid(char))
        return *static_cast<char*>(var_ptr);
    else if (typ == typeid(unsigned char))
        return *static_cast<unsigned char*>(var_ptr);
    else
        throw( std::invalid_argument(std::string("Unexpected field type '")+typ.name()+"'.") ); 
}

