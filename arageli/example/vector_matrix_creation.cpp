#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    // Let's create vector with rational entries
    vector<rational<> > a = "(21/3, 3, 4)";
    cout << "Vector defined by its string representation = " << endl;
    output_aligned(cout, a);
    cout << endl;

    // Create a vector populated by a value
    vector<int> b(3, 777, fromval);
    cout << "Vector populated by a value = " << endl;
    output_aligned(cout, b);
    cout << endl;

    // Create a zero vector
    vector<double> c(3);
    cout << "A zero vector = " << endl;
    output_aligned(cout, c);
    cout << endl;

    // Create matrix with rational entries
    matrix<rational<> > A = "((21/3, 3, 4), (3335, 6/5, 75), (81, 9, 10/7))";
    cout << "Matrix defined by its string representation = " << endl;
    output_aligned(cout, A);
    cout << endl;

    // Create 3x3 identity matrix
    matrix<rational<> > E(3, eye);
    cout << "Identity matrix = " << endl;
    output_aligned(cout, E);
    cout << endl;

    // Create 3x3 square matrix populated by a value
    matrix<sparse_polynom<rational<> > >
        B(3, sparse_polynom<rational<> >("3/2*x^2"), fromval);
    cout << "Square matrix populated by a value = " << endl;
    output_aligned(cout, B);
    cout << endl;

    // Create 3x4 matrix
    matrix<double> C(3, 4, fromsize);
    cout << "Rectangle matrix = " << endl;
    output_aligned(cout, C);
    cout << endl;

    // Create char matrix!
    matrix<char> D(3, '*', fromval);
    cout << "Char matrix = " << endl;
    output_aligned(cout, D);

    return 0;
}
