#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    // Let's create some polynomials
    sparse_polynom<int> S = "2*x^2+5*x-7+3*x";
    cout << "An integer polynomial = "
        << endl << S << endl << endl;

    sparse_polynom<big_int>
        B = "1234567891011121314151617181920*x^777"
                "+112233445566778899";
    cout << "A big integer polynomial = "
        << endl << B << endl << endl;

    sparse_polynom<double>
        D = "1.12345*x-1e25*x^2+1234.5678*x^3-0.000002334";
    cout << "A real polynomial = "
        << endl << D << endl << endl;

    sparse_polynom<rational<> >
        R = "1234/56781*x^321+7/9*x+3*x^2-4/256";
    cout << "Rational polynomial = "
        << endl << R << endl << endl;

    // It is posible to create a polynomial with matrix coefficients
    // Pay attention to extra parentheses
    sparse_polynom<matrix<int> > M =
        "(((1,2),(3,4))*x^55-((1,2),(4,5))*x+((5,-1),(4,0)))";
    cout << "Polynomial with matrix coefficients = "
        << endl << M << endl << endl;

    // Now let's create a polynomial from separate monomials
    big_int num = 1, den = 1;
    int degree = 0;
        sparse_polynom<rational<> > F;
    for(int i = 0; i < 6; i++)
    {
        F += sparse_polynom<rational<> >::
            monom(rational<>(num, den), degree);
        degree++;
        num += den;
        den += num;
    }

    cout
        << "A polynomial constructed from separate monomials = "
        << endl << F << endl << endl;

    // Converting types of polynomials
    sparse_polynom<big_int> BD = D;
    cout << "A polynomial converted from another = "
        << endl << BD << endl << endl;

    sparse_polynom<double> FR = R;
    cout << "A polynomial converted from another = "
        << endl << FR << endl;

    return 0;
}

