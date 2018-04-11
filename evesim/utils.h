#ifndef UTILS_H_
#define UTILS_H_

#include <ostream>
#include <utility>

template< typename T1, typename T2 >
std::ostream& operator<< (std::ostream &out, const std::pair<T1, T2> &p)
{
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}


#endif /* UTILS_H_ */
