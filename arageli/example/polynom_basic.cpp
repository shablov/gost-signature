#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::boolalpha;


int main ()
{
    polynom<rational<> >
        f = "1/7*x^13-5*x^8-7*x^5+10",
        g = "-11*x^3+22/17*x^2-x";

    cout
        << "f(x) = " << f
        << "\ng(x) = " << g
        << "\nf(x)*g(x) = " << f*g
        << "\nf(x)+g(x) = " << f + g
        << "\nf(x)/g(x) = " << f/g
        << "\nf(x)%g(x) = " << f%g
        << "\nGCD(f(x), g(x)) = " << gcd(f, g)
        << "\ndividing is valid: " << boolalpha << ((f/g)*g + f%g == f)
        << "\n";

    return 0;
}
