#ifndef EvSimObjectTypeUtils_H_
#define EvSimObjectTypeUtils_H_

#include "EvSimObject.h"
#include "EvSimObjectCloneFactory.h"
#include "evesim_hash.h"

#include <string>
using std::string;

#include <cstddef>

#include <iostream>

#include <boost/format.hpp>


#define EV_SIM_OBJECT(type)                                                      \
public:                                                                          \
   static EvSimObjectTypeUtils<type> simobj_utils;                               \
	                                                                             \
   friend void field_registrator_func<type>(type *);                             \
                                                                                 \
  virtual const EvSimObjectTypeUtilsBase * getSimObjTypeUtils() const;           \



#define EV_SIM_OBJECT_POST(OBJECTTYPE)                                           \
      EvSimObjectTypeUtils<OBJECTTYPE > OBJECTTYPE::simobj_utils =               \
				EvSimObjectTypeUtils<OBJECTTYPE >(#OBJECTTYPE);                  \
												                   		         \
      const EvSimObjectTypeUtilsBase * OBJECTTYPE::getSimObjTypeUtils() const    \
      {                                                                          \
	      return &OBJECTTYPE::simobj_utils;                                      \
      }                                                                          \


#define FIELD_REGISTRATOR(OBJECTTYPE)                                            \
    template<>                                                                   \
    void field_registrator_func<OBJECTTYPE>(OBJECTTYPE *obj_ptr)                 \


#define REGISTER_FIELD(OBJECTTYPE, FIELDNAME)                                    \
	OBJECTTYPE::simobj_utils.registerVariable(string(#FIELDNAME),                \
			(char *)&obj_ptr->FIELDNAME - (char *)obj_ptr,                       \
            typeid(obj_ptr->FIELDNAME));                                         \



class EvSimObjectTypeUtilsBase {
public:
	EvSimObjectTypeUtilsBase(const string &type_name) : type_name(type_name) {};

	virtual ~EvSimObjectTypeUtilsBase() {};

	virtual EvSimObject* copy(const EvSimObject * source) const = 0;

	virtual void registerVariable(const string &var_name, ptrdiff_t offset, const type_info &t_info) = 0;

	virtual void * const getVariablePtr(const EvSimObject *obj, const string &var_name) const = 0;

	virtual const type_info & getVariableType(const EvSimObject *obj, const string &var_name) const = 0;

	virtual const string &getTypeName() const {
		return type_name;
	}

protected:
	string type_name;
};


template<class T>
class EvSimObjectTypeUtils : public EvSimObjectTypeUtilsBase {
public:

	EvSimObjectTypeUtils<T>(const string &type_name) : EvSimObjectTypeUtilsBase(type_name) {
		T *example_obj = (T *)200000;
		field_registrator_func(example_obj);
	};

	virtual ~EvSimObjectTypeUtils<T>() {};

	virtual EvSimObject* copy(const EvSimObject * source) const
	{
		return new T(*dynamic_cast<const T*>(source));
	};

	virtual void registerVariable(const string &var_name, ptrdiff_t offset, const type_info &t_info) {
		field_offsets.insert(make_pair(var_name, offset));
		field_types.insert(make_pair(var_name, &t_info));
	}

	virtual void * const getVariablePtr(const EvSimObject *obj, const string &var_name) const {
		const T *downcast_obj = dynamic_cast<const T *>(obj);
		field_offsets_map_type::const_iterator it;
		if ((it = field_offsets.find(var_name)) != field_offsets.end())
			return reinterpret_cast<void * const>((char *)downcast_obj + it->second);
		return 0;
	};

	virtual const type_info & getVariableType(const EvSimObject *obj, const string &var_name) const
	{
		field_types_map_type::const_iterator it;
		if ((it = field_types.find(var_name)) != field_types.end())
			return *it->second;
		return typeid(NoVariableFound);
	};

protected:
	typedef unordered_map< string, ptrdiff_t, evesim::hash<string> > field_offsets_map_type;
	typedef unordered_map< string, const type_info *, evesim::hash<string> > field_types_map_type;

	field_offsets_map_type field_offsets;
	field_types_map_type field_types;
};


template<typename T>
void field_registrator_func(T * ) {}

#endif /* EvSimObjectTypeUtils_H_ */
