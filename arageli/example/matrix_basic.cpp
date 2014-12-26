#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::boolalpha;


int main ()
{
    // create matrix with rational elements from string
    matrix<rational<> > A = "((21/3, 3, 4), (3335, 6/5, 75), (81, 9, 10/7))";
    cout << "A = \n";
    output_aligned(cout, A, "|| ", " ||", "  ");

    // calculate inverse matrix
    matrix<rational<> > AInv(inverse(A));
    cout << "\ninversion of A = \n";
    output_aligned(cout, AInv, "|| ", " ||", "  ");

    // check inverse operation result
    cout
        << "\n\nthe inversion is valid: "
        << boolalpha << (A*AInv).is_unit();

    // create 3x3 identity matrix
    matrix<rational<> > E(3, eye);
    cout << "\nidentity matrix E = \n";
    output_aligned(cout, E, "|| ", " ||", "  ");

    // create 3x3 square matrix
    matrix<sparse_polynom<rational<> > >
        SQ(3, sparse_polynom<rational<> >("3/2*x^2"), fromval);
    cout << "\nsquare matrix SQ = \n";
    output_aligned(cout, SQ, "|| ", " ||", "  ");

    // create 3x4 fromsize matrix
    matrix<double> NSQ(3, 4, fromsize);
    cout << "\nnonsquare matrix NSQ = \n";
    output_aligned(cout, NSQ, "|| ", " ||", "  ");

    // create 3x3 diagonal matrix
    matrix<char> D(3, 1, diag);
    cout << "\ndiagonal matrix D = \n";
    output_aligned(cout, D, "|| ", " ||", "  ");

    return 0;
}
