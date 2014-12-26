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
    MT B, H;

    cout << endl << "Initial basis (in columns) A =" << endl;
    output_aligned(cout, A);

    B = A;
    if (lll_reduction(B, H))
    {
        cout << endl << "Reduced bases (in columns) B =" << endl;
        output_aligned(cout, B);

        cout << endl << "Transformation matrix H =" << endl;
        output_aligned(cout, H);

        cout << endl << "A*H = " << endl;
        output_aligned(cout, A*H);

        cout << endl << "Check B == A*H? -> " << boolalpha << (B == A*H) << endl;

        cout << endl << "det(H) = " << det(H) << endl;
    }
    else
    {
        cout << "Columns in A don't form a basis" << endl;
        cout << "(they are linear dependent)" << endl;
    }

    return 0;
}
