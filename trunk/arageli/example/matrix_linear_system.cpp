#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;
using std::boolalpha;

/*
    Solving the system of linear equations $Ax=b$,
    $ A = \left(\begin{array}{rrr} -1/2 & 2/3 & 3/6 \\ 5/7 & -6 & 7/5\\ -8/11 & 9/2 & -11\end{array}\right)$,
    $ b = \left(\begin{array}{ccc} \phantom{+}2/3 \\ -1/5 \\ \phantom{+}4 \end{array}\right)$
*/

int main ()
{
    matrix<rational<> > A;
    vector<rational<> > b,x;

    A = "((-1/2, 2/3, 3/6), (5/7, -6, 7/5), (-8/11, 9/2, -11))";
    b = "(2/3, -1/5, 4)";

    cout << "A = " << endl;
    output_aligned(cout, A);
    cout << endl;
    cout << "b = " << endl;
    output_aligned(cout, b);
    cout << endl;

    try
    {
        x = solve_linsys(A, b);

        cout << "x = " << endl;
        output_aligned(cout, x);
        cout << endl;
        cout << "The result is " << boolalpha << (A*x == b) << endl;
    }
    catch(matrix_is_singular)
    {
        cout << "Error! Matrix is singular!" << endl;
    }

    return 0;
}
