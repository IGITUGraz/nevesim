#ifndef SIMEXCEPTION_H_
#define SIMEXCEPTION_H_

#include <string>
using std::string;

namespace evesim
{

    class Exception
    {
    public:
        Exception( string method, string msg ) : _method(method), _MSG( msg ) {};
        virtual ~Exception() {};
        string message() const;
        virtual string name() const { return string("nevesim::Exception"); };
    protected:
        string _method;
        string _MSG;
    };

    class ConstructionException : public Exception
    {
    public:
        ConstructionException( string method, string msg ) :  Exception( method, msg ) {};
        virtual string name() const { return string("nevesim::ConstructionException"); };
    };

    class NotFoundException : public Exception
    {
    public:
        NotFoundException( string method, string msg ) :  Exception( method, msg ) {};
        virtual string name() const { return string("nevesim::NotFoundException"); };
    };

}

inline string evesim::Exception::message() const
{
    return name() + " in function " + _method + ": " + _MSG;
}

#endif /*SIMEXCEPTION_H_*/
