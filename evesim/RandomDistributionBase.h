#ifndef RANDOMDISTRIBUTIONBASE_H_
#define RANDOMDISTRIBUTIONBASE_H_

/*! \file RandomDistribution.h
**  \brief Random number distributions (based on the boost/random library)
* 
*   This provides a minimal run-time polymorphism based interface to the 
*   template based version provided by the boost/random library.
*/

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <boost/random.hpp>

#include <vector>

#include "RandomEngine.h"

//! Interface for random number distributions
class RandomDistribution
{
public:
    virtual ~RandomDistribution() {};
    //! Return a random number specified by this distribution given a RandomEngine
    /** \arg eng uniform random number generator. It is assumed that eng() returns a random number in the interval [0,1) */
    double operator()(RandomEngine &eng ) { return get( eng ); };
    shared_ptr< std::vector<double> > operator()(RandomEngine &eng, size_t n );
    virtual RandomDistribution *clone(void) const = 0;
protected:
    virtual double get( RandomEngine &eng ) { return 0; };
};

//! This template allos an easy construction of a RandomDistribution based on a boost::random based distribution
template< typename boost_dist >
class RandomDistributionBoostImplementation : public RandomDistribution
{
protected:
    typedef boost_dist boost_dist_t;

public:
    RandomDistributionBoostImplementation( boost_dist const& d ) : m_dist( d ) {};
    virtual ~RandomDistributionBoostImplementation() {};

    virtual double get( RandomEngine &eng )
    {
        return m_dist( eng );
    };

    virtual RandomDistribution *clone(void) const
    {
        return new RandomDistributionBoostImplementation( m_dist );
    };

private:
    boost_dist m_dist;

};
#endif
