#ifndef SharedArray_H_
#define SharedArray_H_

#include <cstdlib>
#include <vector>
using std::vector;

#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<vector<double> > shared_dbl_vector_ptr;

template<typename T>
class SharedArray
{
public:
	SharedArray(const T v[], size_t size);

	SharedArray(const std::vector<T> &v = std::vector<T>());

	const T operator[] (size_t n) const;

	size_t size() const;

	const vector<T> * get() const
	{
		return &el_vec;
	}

protected:
	vector<T> el_vec;
};

template<typename T>
SharedArray<T>::SharedArray(const T v[], size_t size)
	: el_vec(v,v+size)
{}

template<typename T>
SharedArray<T>::SharedArray(const std::vector<T> &v)
  : el_vec(v){ }


template<typename T>
const T SharedArray<T>::operator[] (size_t n) const
{
	return el_vec[n];
}

template<typename T>
size_t SharedArray<T>::size() const
{
	return el_vec.size();
}


#endif /*SharedArray_H_*/
