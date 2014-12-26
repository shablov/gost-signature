#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    vector<rational<> > b;
    matrix< rational<> > A;

    // Let's define vector b
    b = "(2/3, -1/5, 4)";

    // Print b
    cout << "b (operator <<) = " << b << endl;

    // Prettier output
    cout << "b (output_aligned) = " << endl;
    output_aligned(cout, b, "|| ", " ||");

    // Output using latex notation
    cout << "b (output_latex) = " << endl;
    output_latex(cout, b);
    cout << endl;

    cout << "Size of b = " << b.size() << endl;
    cout << "Length of b = " << b.length() << endl;

    // Now let's define matrix A
    A = "((1/2, 2/3), (-5/7, 6), (8/11, -9/2))";

    // Print A
    cout << "A (operator <<) = " << A << endl;

    // Prettier output
    cout << "A (output_aligned) = " << endl;
    output_aligned(cout, A, "|| ", " ||", " ");

    // Output using latex notation
    cout << "A (output_latex) = " << endl;
    output_latex(cout, A);
    cout << endl;

    cout << "Cols in A = " << A.ncols() << endl;
    cout << "Rows in A = " << A.nrows() << endl;
    cout << "Size of A = " << A.size() << endl;
    cout << "Length of A = " << A.length() << endl;

    return 0;
}
