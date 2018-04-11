#ifndef RANDOMENGINE_H_
#define RANDOMENGINE_H_

/*! \file RandomEngine.h
**  \brief Random number generators (based on the boost/random library)
* 
*   This provides a minimal run-time polymorphism based interface to the 
*   template based version provided by the boost/random library.
*/

#ifdef _MSC_VER
# pragma warning(disable:4244) // warning C4244: 'initializing' : conversion from 'boost::uint64_t' to 'boost::uint32_t', possible loss of data
#endif

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;

#include <boost/random.hpp>

#ifdef _MSC_VER
# undef min
# undef max
#endif

//! Interface definition for a random number generator
/** Generates random numbers (double precision) distributed uniformly over the interval [0,1) */
class RandomEngine
{

public:
    //! Let the boost implementation now what our return type is
    typedef double result_type;

    //! This NOOP makes the compile happy
    virtual ~RandomEngine() {};

    //! Returns a random number in the interval [0,1)
    virtual double operator()() = 0;

    //! Seed the random generator
    virtual void seed( unsigned s ) = 0;

    //! Seed the random generator based on the current time
    void seed( )
    {
        return seed( unsigned(microsec_clock::local_time().time_of_day().total_microseconds()) );
    };

    //! Returns 0.0
    double min() { return 0; };

    //! Returns 1.0
    double max() { return 1; };
};

#include <iostream>
using std::cerr;
using std::endl;

//! Template for easy construction of RandomEngines using boost::random random number generators
template< typename boost_engine >
class RandomEngineBoostImplementation : public RandomEngine
{
public:

    RandomEngineBoostImplementation() :  m_rng( boost_engine() )
    {
        m_rng.base().seed( unsigned(microsec_clock::local_time().time_of_day().total_microseconds()) );
    };

    virtual ~RandomEngineBoostImplementation() {};

    virtual double operator()()
    {
        return m_rng();
    };

    virtual void seed( unsigned s )
    {
        m_rng.base().seed( s );
    };


private:
    //! The random engine is wrapped by uniform_01 to ensure that we generate numbers in the interval [0,1)
    boost::uniform_01< boost_engine, double > m_rng;
};

// Here are several random number generator instantions
class MersenneTwister19937 : public RandomEngineBoostImplementation< boost::mt19937 > {};
class MersenneTwister11213b : public RandomEngineBoostImplementation< boost::mt11213b > {};
class LaggedFibonacci607 : public RandomEngineBoostImplementation< boost::lagged_fibonacci607 > {};
class LaggedFibonacci1279 : public RandomEngineBoostImplementation< boost::lagged_fibonacci1279 > {};
class LaggedFibonacci2281 : public RandomEngineBoostImplementation< boost::lagged_fibonacci2281 > {};
class LaggedFibonacci3217 : public RandomEngineBoostImplementation< boost::lagged_fibonacci3217 > {};
class LaggedFibonacci4423 : public RandomEngineBoostImplementation< boost::lagged_fibonacci4423 > {};
class LaggedFibonacci9689 : public RandomEngineBoostImplementation< boost::lagged_fibonacci9689 > {};
class LaggedFibonacci19937 : public RandomEngineBoostImplementation< boost::lagged_fibonacci19937 > {};
class LaggedFibonacci23209 : public RandomEngineBoostImplementation< boost::lagged_fibonacci23209 > {};
class LaggedFibonacci44497 : public RandomEngineBoostImplementation< boost::lagged_fibonacci44497 > {};

#ifdef _MSC_VER
# pragma warning(default:4244) // warning C4244: 'initializing' : conversion from 'boost::uint64_t' to 'boost::uint32_t', possible loss of data
#endif


#endif /*RANDOMENGINE_H_*/
