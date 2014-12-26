#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;
using std::boolalpha;


int main ()
{
    matrix< rational<> > A, A_inv, B;
    vector< rational<> > basis;
    rational<> d = 0;

    A = "((2/3, -3/5, 1) , (-4/7, 5/8, 1/12) , (-3, 5, -6/7))";

    cout << "A = " << endl;
    output_aligned(cout, A, "|| ", " ||", " ");
    cout << endl;

    rref (A, B, A_inv, basis, d);

    cout << "The row reduced echelon form of A = " << endl;
    output_aligned(cout, B, "|| ", " ||", " ");
    cout << endl;

    cout << "The inverse to A = " << endl;
    output_aligned(cout, A_inv, "|| ", " ||", " ");
    cout << endl;

    cout << "det(A) = " << d << endl << endl;

    cout << "The result is " << boolalpha << (A*A_inv).is_unit();
    cout << endl;

    return 0;
}
