#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


sparse_polynom<rational<> > Lagrange (rational<> *x, rational<> *y, int n)
{
    rational<> tmpDenom; // denominator
    sparse_polynom<rational<> >
        poly, // result
        tmpPolyNumer(1), // numerator
        mono("x");

    // a blank for numerator
    for (int i = 0; i < n; i++)
        tmpPolyNumer *= mono - x[i];

    for (int j = 0; j < n; j++)
    {
        // computing a denominator
        tmpDenom = 1;
        for (int k = 0; k < n; k++)
            if (k != j)
                tmpDenom *= x[j]-x[k];

        // next addand
        poly += (tmpPolyNumer / (mono - x[j])) * (y[j] / tmpDenom);
    }

    return poly;
}


int main ()
{
    int n = 7;
    rational<> x[] = {0, 1, 2, 3, 4, 5, 6};
    rational<> y[] = {rational<>(1,3), -1, 0, 6, 7, -3, -7};

    sparse_polynom<rational<> > L = Lagrange(x, y, n);

    cout << "Interpolating polynomial" << endl << "L(x) = " << L << endl << endl;

    cout << "Let's check it" << endl;

    bool ok = true;

    for (int i = 0; i < n; i++)
    {
        rational<> yy = L.subs(x[i]);
        cout << "L(" << x[i] << ") = " << yy << endl;

        if (yy != y[i])
        {
            ok = false;
            cout << "Error!" << endl;
        }
    }

    if (ok)
        cout << "All is Ok" << endl;

    return 0;
}
