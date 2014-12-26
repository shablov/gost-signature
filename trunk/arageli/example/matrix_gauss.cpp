#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;
using std::boolalpha;


int main ()
{
    matrix<rational<> > A, A_inv;
    rational<> d;

    A = "((2/3, -3/5, 1) , (-4/7, 5/8, 1/12) , (-3, 5, -6/7))";

    cout << "A = " << endl;
    output_aligned(cout, A, "|| ", " ||", " ");
    cout << endl;

    if (A.is_square())
    {
        d = det(A);
        cout << "det(A) = " << d << endl << endl;

        if (d != 0)
        {
            A_inv = inverse(A);

            cout << "Inverse to A = " << endl;
            output_aligned(cout, A_inv, "|| ", " ||", " ");
            cout
                << endl << "The result is " << boolalpha
                << (A*A_inv).is_unit() << endl << endl;
        }
    }

    matrix< big_int > B;
    big_int delta;

    B = "((1, 2, 3) , (3, 2, 3) , (0, 1, 3))";

    cout << "B = " << endl;
    output_aligned(cout, B, "|| ", " ||", " ");
    cout << endl;

    if (B.is_square())
    {
        delta = det_int(B);
        cout << "det(B) = " << delta << endl;
    }

    return 0;
}
