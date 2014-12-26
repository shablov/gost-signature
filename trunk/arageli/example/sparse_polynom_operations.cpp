#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;
using std::boolalpha;


int main ()
{
    sparse_polynom<rational<> > f = "1/15*x^4+5/7*x^3+7*x";
    sparse_polynom<rational<> > g = "20/53*x^3-1/9*x-1/8";
    sparse_polynom<rational<> > h = "2/5*x^2+5/7*x-7/8";

    cout << "f = " << f << endl;
    cout << "g = " << g << endl;
    cout << "h = " << h << endl << endl;

    cout << "f + g * h = " << f + g * h << endl << endl;
    cout << "Division: f = q*h + r where" << endl;
    sparse_polynom<rational<> > q = f/h;
    sparse_polynom<rational<> > r = f%h;

    cout << "q = " << q << endl;
    cout << "r = " << r << endl;
    // Check the result
    cout << "It's " << boolalpha << (f == q*h + r) << endl << endl;

    typedef sparse_polynom<rational<> >::monom ratmonom;

    // Operations involving monomilas and scalars

    cout << "Let's divide f by x: " << endl << "f = "
        << (f /= ratmonom(1, 1)) << endl << endl;

    cout << "Let's divide f and g by their leading coefficients:" << endl;
    cout << "f = " << (f /= rational<>(20, 53)) << endl;
    cout << "g = " << (g /= rational<>(1, 15)) << endl << endl;
    cout << "Now f - g = " << f - g << endl;

    return 0;
}
