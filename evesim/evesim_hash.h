#ifndef _EVESIM_HASH_H_
#define _EVESIM_HASH_H_

#include <boost/functional/hash.hpp>

#if defined(__GNUC__)
  #if (__GNUC__ < 4)
  #warning "gcc versions less than 4.0 may not be fully supported!"
  #include <ext/hash_map>
  #include <ext/hash_set>
  using __gnu_cxx::hash_map;
  using __gnu_cxx::hash_set;
  template <class Key, class T, class Hash>
    class unordered_map : public __gnu_cxx::hash_set<Key,T,Hash> {};
  template <class Key, class T, class Hash>
    class unordered_set : public __gnu_cxx::hash_set<Key,T,Hash> {};

  #elif (__GNUC__ == 4) && (__GNUC_MINOR__ < 2)
  #include <tr1/unordered_map>
  #include <tr1/unordered_set>
  using std::tr1::unordered_map;
  using std::tr1::unordered_set;

  #else
  #include <unordered_map>
  #include <unordered_set>
  using std::unordered_map;
  using std::unordered_set;

  #endif
#else
  #include <unordered_map>
  #include <unordered_set>
  using std::unordered_map;
  using std::unordered_set;
#endif 

namespace evesim
{

	template<class T>
	class hash : public boost::hash<T>
	{
	};

}


#endif /* _EVESIM_HASH_H_ */
