#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    sparse_polynom<rational<> >
        P = "1/2*x^8-47/12*x^7-359/84*x^5+349/126*x^4+94/9*x^3+55/14*x^2+10*x",
        Q = "-99/13*x^7+834/13*x^6-141/4*x^5+22171/390*x^4-5699/180*x^3+2*x";

    cout << "P = " << P << endl;
    cout << "Q = " << Q << endl << endl;

    int P_vs_Q = cmp(P,Q);
    if (P_vs_Q < 0) cout << "P < Q" << endl;
    if (P_vs_Q > 0) cout << "P > Q" << endl;
    if (P_vs_Q == 0) cout << "P == Q" << endl;

    int P_vs_P = cmp(P,P);
    if (P_vs_P < 0) cout << "P < P" << endl;
    if (P_vs_P > 0) cout << "P > P" << endl;
    if (P_vs_P == 0) cout << "P == P" << endl;
    cout << endl;

    cout << "Before swapping P and Q:" << endl;
    cout << "P = " << P << endl;
    cout << "Q = " << Q << endl << endl;
    swap(P, Q);
    cout << "After swapping:" << endl;
    cout << "P = " << P << endl;
    cout << "Q = " << Q << endl << endl;

    cout << "-P = " << opposite(P) << endl;

    return 0;
}
