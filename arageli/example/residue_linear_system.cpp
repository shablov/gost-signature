#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;
using std::boolalpha;


int main ()
{
    typedef residue<int> T;

    int mod = 7;

    matrix<T> A = "((3, 1, 2), (1, 2, 3), (4, 3, 2))";
    vector<T> b = "(1, 1, 1)";
    vector<T> x;

    for(matrix<T>::iterator i = A.begin(); i < A.end(); ++i)
    {
        i->module() = mod;
        i->normalize();
    }

    for(matrix<T>::iterator i = b.begin(); i < b.end(); ++i)
    {
        i->module() = mod;
        i->normalize();
    }

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
        cout << "Check the result: " << boolalpha << (A*x == b) << endl;
    }
    catch(matrix_is_singular)
    {
        cout << "Error! Matrix is singular!" << endl;
    }

    return 0;
}
