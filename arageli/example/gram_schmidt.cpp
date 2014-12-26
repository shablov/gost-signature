#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;
using std::boolalpha;


int main ()
{
    typedef rational<> T;
    typedef matrix<T> MT;

    MT A = "((17,0,81),(1,17,0),(0,1,0),(5,1,12))";
    MT B, Mu;

    cout << endl << "Initial basis (in columns) A =" << endl;
    output_aligned(cout, A);

    B = A;
    if (gram_schmidt(B, Mu))
    {
        cout << endl << "Orthogonal bases (in columns) B =" << endl;
        output_aligned(cout, B);

        cout << endl << "transpose(B)*B =" << endl;
        output_aligned(cout, transpose(B)*B);

        for (size_t j = 0; j < Mu.ncols(); j++)
            Mu(j, j) = 1;

        cout << endl << "Transformation (from B to A) matrix Mu =" << endl;
        output_aligned(cout, Mu);

        cout << endl << "B*Mu = " << endl;
        output_aligned(cout, B*Mu);

        cout << endl << "Check A == B*Mu? -> " << boolalpha << (A == B*Mu) << endl;
    }
    else
    {
        cout << "Columns in A don't form a basis" << endl;
        cout << "(They are linear dependent)" << endl;
    }

    return 0;
}
