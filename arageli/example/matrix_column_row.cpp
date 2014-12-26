#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    matrix< rational<> > A;
    vector< rational<> > b;

    int k;

    A = "((-2/5, -1/5, 3/5, 1/2),"
        "(2/5, -1/4, 2/6, 1/3),"
        "(-5/2, 5/6, -6/7, 1/4))";
    b = "(-2/3, 5/2, -1/6)";
    k = 5;

    cout << "A = " << endl;
    output_aligned(cout, A);
    cout << endl;

    cout << "The first row in A = " << A.copy_row(0) << endl;
    cout << "The second row in A = " << A.copy_row(1) << endl;
    cout << "The first column in A = " << A.copy_col(0) << endl;
    cout << "The second column in A = " << A.copy_col(1) << endl;

    cout << endl << "A[0] = " << k << "*A[1] (rows)" << endl;
    A.addmult_rows(0, 1, k);
    cout << "Result = " << endl;
    output_aligned(cout, A);

    cout << endl << "A[1] = A[1] / A(2, 1) (columns)" << endl;
    A.div_col(1, safe_reference(A(2, 1)));
    cout << "Result = \n" << endl;
    output_aligned(cout, A);

    cout << endl << "Erase two columns = " << endl;
    A.erase_cols(2, 2);
    output_aligned(cout, A);
    cout << endl;

    cout << "Vector b = " << b << endl;
    cout << "Insert b into A = " << endl;
    A.insert_col(2, b);
    output_aligned(cout, A);
    cout << endl;

    A.swap_rows(0, 2);
    cout<<"Swap in A the first and the third rows = " << endl;
    output_aligned(cout, A);

    return 0;
}
