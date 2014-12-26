#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;
using std::boolalpha;


int main()
{
    sparse_polynom<rational<> >
        P = "x^6+x^4-x^2-1",
        Q = "x^3-2*x^2-x+2";
    cout << "P = " << P << endl;
    cout << "Q = " << Q << endl << endl;

    if (is_coprime(P, Q))
        cout << "P and Q are coprime" << endl << endl;
    else
        cout << "P and Q are not coprime" << endl << endl;

    sparse_polynom<rational<> > dP = diff(P);
    cout << "The first derivative dP/dx = " << dP << endl;
    cout << "The second derivative d2P/dx2 = " << diff(dP) << endl << endl;

    cout << "GCD(P, Q) = " << gcd(P, Q) << endl;
    cout << "LCM(P, Q) = " << lcm(P, Q) << endl << endl;

    sparse_polynom<rational<> > U, V, pq;
    pq = euclid_bezout(P, Q, U, V);
    cout << "U = " << U << endl;
    cout << "V = " << V << endl << endl;
    cout << "Check for gcd(P, Q) == P*U + Q*V: ";
    cout << boolalpha << (pq == P*U + Q*V);
    cout << endl;

    return 0;
}
