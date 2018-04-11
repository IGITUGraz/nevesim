#include "RandomEngine.h"
#include "RandomDistribution.h"
#include "SimException.h"

/*
#include <iostream>
using std::cerr;
using std::endl;
*/

shared_ptr< std::vector<double> > RandomDistribution::operator()(RandomEngine &eng, size_t n )
{
    shared_ptr< std::vector<double> > v( new std::vector<double>(n) );
    for( size_t i=0; i<n; i++ ) {
        (*v)[i] = (*this)( eng );
    }
    return v;
}

double UniformIntegerDistribution::get( RandomEngine &eng )
{
    return a + eng() * range;
}

STLRandomNumberGenerator::STLRandomNumberGenerator( RandomEngine &eng )
  : eng(eng)
{
}

unsigned STLRandomNumberGenerator::operator()(unsigned N)
{
	return unsigned(eng()*N);
}


ClippedDistribution::ClippedDistribution( RandomDistribution const& dist, double a, double b, size_t n ) :
        dist(NULL), a(a), b(b), n(n)
{
    this->dist = dist.clone();
    range = b-a;
    if( range <= 0 ) {
        throw( evesim::Exception( "ClippedDistribution", "Invalid boundaries." ) );
    }
}

ClippedDistribution::~ClippedDistribution()
{
    if( dist ) delete dist;
}

double ClippedDistribution::get( RandomEngine &eng )
{
    double v = (*dist)( eng );
    size_t c = 1;
    bool invalid = v < a || v > b;
    while( c < n && invalid ) {
        v = (*dist)( eng );
        invalid = v < a || v > b;
        c++;
    }
    if( invalid ) {
        v = a + eng() * range;
    }
    return v;
}


Gamma2Distribution::Gamma2Distribution(double a, double b)
	:GammaDistribution(a), a(a), b(b)
{
}


double Gamma2Distribution::get( RandomEngine &eng )
{
    return GammaDistribution::get(eng)*b;
}




BndGammaDistribution::BndGammaDistribution(double mu, double cv, double upperBound)
    :Gamma2Distribution(1/(cv*cv), fabs(mu*cv*cv)), mu(mu), cv(cv), ub(upperBound)
{
}


double BndGammaDistribution::get( RandomEngine &eng )
{
    if(mu == 0.0 || cv <= 0.0)				// bad style
        return mu;

    double v=Gamma2Distribution::get(eng);
    double lb=0;

    if (v > ub) {
        double maxd = std::min(std::min(mu-lb, ub-mu), fabs(5*cv*mu));
        double clb = mu-maxd;
        double cub = mu+maxd;

        v = clb + fabs(cub-clb)*uniform.get(eng);
    }

    return v;
}


BndNormalDistribution::BndNormalDistribution(double mu, double cv, double lowerBound, double upperBound)
    :NormalDistribution(mu, fabs(cv*mu)), mu(mu), cv(cv), lb(lowerBound), ub(upperBound)
{
}


double BndNormalDistribution::get( RandomEngine &eng )
{
    if(mu == 0.0 || cv <= 0.0)               // bad style
        return mu;

    double v=NormalDistribution::get(eng);

    if (v < lb || v > ub) {
        double maxd = std::min(std::min(mu-lb,ub-mu), fabs(5*cv*mu));
        double clb = mu-maxd;
        double cub = mu+maxd;

        v = clb + fabs(cub-clb)*uniform.get(eng);
    }

//    v = 1e-4 * round(v * 1e-4);
    return v;
}


QuadDistribution::QuadDistribution(double a, double b)
    :UniformDistribution(0,1), a(a), b(b)
{
}


double QuadDistribution::get( RandomEngine &eng )
{
    double v=UniformDistribution::get(eng);

    return a + b*v*v;
}


double foo(void)
{
    MersenneTwister19937 mt;
    mt.seed( 1234 );
    double x = mt();

    MersenneTwister11213b mt2;
    mt2.seed( 12345 );
    x = mt2();

    LaggedFibonacci4423 lf;
    lf.seed( 3456 );
    x = lf();

    BernoulliDistribution bd;
    x = bd( mt2 );

    BinomialDistribution bind;
    x = bind( lf );

    CauchyDistribution cd;
    x = cd( lf );
    x = cd( mt2 );

    ConstantNumber cn( 4 );
    x = cn( lf );

    ExponentialDistribution ed( 2.3 );
    x = ed( lf );

    GammaDistribution ad( 3.4 );
    x = ad( mt2 );

    LogNormalDistribution lnd( 4, 5 );
    x = lnd( lf );

    NormalDistribution nd( 1, 2);
    x = nd( mt );

    PoissonDistribution pd( 10 );
    x = pd( mt );

    TriangleDistribution td( 1, 2, 3 );
    x= td( lf );

    UniformDistribution cud( 3, 4 );
    x = cud( mt );

    UniformIntegerDistribution iud( 1, 100 );
    x = iud( lf );

    return x;
}
