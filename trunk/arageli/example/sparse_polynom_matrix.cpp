#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;

// $  \left(\begin{array}{rrr} 3 & -5 \\  0 &  7 \end{array}\right) x^8 + \left(\begin{array}{rrr} 0 &  1 \\  0 & -8 \end{array}\right) x^3 + \left(\begin{array}{rrr}-1 &  9 \\ 13 &  2 \end{array}\right) \leftrightarrow$
// $  \left(\begin{array}{ccc} 3x^8-1 & -5x^8+x^3+9 \\  13 &  7x^8-8x^3+2 \end{array}\right)$

int main ()
{
    sparse_polynom<matrix<int> >
        f = "(((3,-5),(0,7))*x^8+((0,1),(0,-8))*x^3+((-1,9),(13,2)))";

    matrix<sparse_polynom<int> >
        x = "((x,0),(0,x))";

    cout << "f(x) = " << f << endl << endl;
    cout << "Let x = " << x << endl << endl;
    cout << "Then the resulting matrix is" << endl;

    output_aligned(cout, f.subs(x), "|| ", " ||", " ");

    return 0;
}
