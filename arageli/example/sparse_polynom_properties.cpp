#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    sparse_polynom<rational<> > S =
        "2/55*x^55+5/567*x^28-56/997*x^5+1/18122005*x^2+567";
    cout << "S = " << S << endl << endl;

    if (S.is_normal())
        cout << "S is in canonical form" << endl;
    else
    {
        cout << "S is not in canonical form!" << endl;
        // This would mean that there is a bug in Arageli!
        S.normalize();
    }

    if (S.is_null())
        cout << "S = 0" << endl;
    else
        cout << "S is not zero" << endl;

    if (S.is_x())
        cout << "S = x" << endl;
    else
        cout << "S is not x" << endl;

    if (S.is_const())
        cout << "S is rational number" << endl;
    else
        cout << "S is not a scalar" << endl;

    // Two ways to determine the leading coefficient
    cout
        << "The leading coefficient is "
        << S.leading_coef() << " = "
        << S.leading_monom().coef() << endl;

    // Two ways to determine the degree of the polynomial
    cout
        << "The degree of S is "
        << S.degree() << " = "
        << S.leading_monom().degree() << endl;

    // The value of the polynomial at the point
    cout << "S(1/2) = " << S.subs(rational<>(1,2)) << endl;
    cout << "S(0) = " << S.subs(rational<>(0)) << endl;

    return 0;
}
