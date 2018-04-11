#ifndef RANDOMDISTRIBUTION_H_
#define RANDOMDISTRIBUTION_H_

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

#include "RandomDistributionBase.h"

//! Bernoulli distribution
class BernoulliDistribution : public RandomDistributionBoostImplementation< boost::bernoulli_distribution<double> >
{
public:
    BernoulliDistribution( double p=0 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t( p ) ) {};
};

//! Binomial distribution
class BinomialDistribution : public RandomDistributionBoostImplementation< boost::binomial_distribution<int,double> >
{
public:
    BinomialDistribution( int n=1, double p=0.5 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t(n, p) ) {};
};

//! Cauchy distribution
class CauchyDistribution : public RandomDistributionBoostImplementation< boost::cauchy_distribution<double> >
{
public:
    CauchyDistribution( double median=0, double sigma=1 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t(median, sigma) ) {};
};

//! A "distribution" which always returns a specified value
class ConstantNumber : public RandomDistribution
{
public:

    ConstantNumber( double c=1 ) : c(c) {};
    virtual ~ConstantNumber() {};

    virtual double get( RandomEngine &eng )
    {
        return c;
    };

    virtual RandomDistribution *clone(void) const
    {
        return new ConstantNumber( c );
    };

private:
    double c;
};

//! Exponential distribution
class ExponentialDistribution : public RandomDistributionBoostImplementation< boost::exponential_distribution<double> >
{
public:
    ExponentialDistribution( double lambda=1 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t(lambda) ) {};
};

//! Gamma distribution
class GammaDistribution : public RandomDistributionBoostImplementation< boost::gamma_distribution<double> >
{
public:
    GammaDistribution( double alpha=1 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t(alpha) ) {};
};

//! Geometric distribution
class GeometricDistribution : public RandomDistributionBoostImplementation< boost::geometric_distribution<int,double> >
{
public:
    GeometricDistribution( double p=0.5 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t(p) ) {};
};

//! Log-normal distribution
class LogNormalDistribution : public RandomDistributionBoostImplementation< boost::lognormal_distribution<double> >
{
public:
    LogNormalDistribution( double mean=0, double sigma=1 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t( mean, sigma ) ) {};
};

//! Normal distribution
class NormalDistribution : public RandomDistributionBoostImplementation< boost::normal_distribution<double> >
{
public:
    NormalDistribution( double mean=0, double sigma=1 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t( mean, sigma ) ) {};
};

//! Poisson distribution
class PoissonDistribution : public RandomDistributionBoostImplementation< boost::poisson_distribution<int,double> >
{
public:
    PoissonDistribution( double mean=0 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t( mean ) ) {};
};

//! Triangle distribution
/** The returned floating-point values x satisfy a <= x <= c;
 * x has a triangle distribution, where b is the most probable value for x.
 */
class TriangleDistribution : public RandomDistributionBoostImplementation< boost::triangle_distribution<double> >
{
public:
    TriangleDistribution( double a=0, double b=0.5, double c=1 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t( a, b, c ) ) {};
};

//! Uniform distribution in the interval [a,b)
class UniformDistribution : public RandomDistributionBoostImplementation< boost::uniform_real<double> >
{
public:
    UniformDistribution( double a=0, double b=0.5 ) : RandomDistributionBoostImplementation<boost_dist_t>( boost_dist_t( a, b ) ) {};
};

//! Uniform distributed integers in the range [a,b]
class UniformIntegerDistribution : public RandomDistribution
{
public:
    UniformIntegerDistribution( int a=0, int b=1 ) : a(a), b(b), range(b-a+1) {};
    virtual ~UniformIntegerDistribution() {};
    virtual double get( RandomEngine &eng );
    virtual RandomDistribution *clone(void) const
    {
        return new UniformIntegerDistribution(a,b);
    };
private:
    int a, b, range;
};

class STLRandomNumberGenerator
{
public:
	STLRandomNumberGenerator( RandomEngine &eng );

	unsigned operator()(unsigned N);
protected:

	RandomEngine &eng;
};

//! Uniform distributed integers in the range [a,b]
class ClippedDistribution : public RandomDistribution
{
public:
    ClippedDistribution( RandomDistribution const& dist, double a, double b, size_t n=1 );
    virtual ~ClippedDistribution();
    virtual double get( RandomEngine &eng );
    virtual RandomDistribution *clone(void) const
    {
        return new ClippedDistribution(*dist,a,b,n);
    };
private:
    RandomDistribution *dist;
    double a,b,range;
    size_t n;
};


//! Gamma distribution with 2 parameters (additional scaling parameter)
class Gamma2Distribution : public GammaDistribution
{
public:
    Gamma2Distribution(double a, double b);
    virtual ~Gamma2Distribution()
    {
    }

    virtual double get(RandomEngine &eng);

    virtual RandomDistribution *clone(void) const
    {
        return new Gamma2Distribution(a,b);
    };

protected:
    double a, b;
};


//! Bounded Gamma distribution (similar to the bndgamma in the circuit toolbox)
class BndGammaDistribution : public Gamma2Distribution
{
public:
    BndGammaDistribution(double mu, double cv, double upperBound);
    virtual ~BndGammaDistribution()
    {
    }

    virtual double get(RandomEngine &eng);

    virtual RandomDistribution *clone(void) const
    {
        return new BndGammaDistribution(mu, cv, ub);
    };

protected:
    UniformDistribution uniform;
    double mu, cv, ub;
};


//! Bounded Normal distribution (similar to the bndnormal in the circuit toolbox)
class BndNormalDistribution : public NormalDistribution
{
public:
    BndNormalDistribution(double mu, double cv, double lowerBound, double upperBound);
    virtual ~BndNormalDistribution()
    {
    }

    virtual double get(RandomEngine &eng);

    virtual RandomDistribution *clone(void) const
    {
        return new BndNormalDistribution(mu, cv, lb, ub);
    };

protected:
    UniformDistribution uniform;
    double mu, cv, lb, ub;
};


//! Quadratic distribution
class QuadDistribution : public UniformDistribution
{
public:
    QuadDistribution(double a, double b);
    virtual ~QuadDistribution()
    {
    }

    virtual double get(RandomEngine &eng);

    virtual RandomDistribution *clone(void) const
    {
        return new QuadDistribution(a, b);
    };

protected:
    double a,b;
};

#endif /*RANDOMDISTRIBUTION_H_*/
