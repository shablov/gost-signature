#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    {
        // Working in ${\bf Z}\sb{5} = {\bf Z}/5{\bf Z}$

        residue<int> a = "2 (mod 5)";
        residue<int> b = "3 (mod 5)";

        cout << "a = " << a << endl;
        cout << "b = " << b << endl;
        cout << "a + b = " << a + b << endl;
        cout << "a - b = " << a - b << endl;
        cout << "a * b = " << a * b << endl;
        cout << "a / b = " << a / b << endl << endl;
    }

    {
        // Working in ${\bf Q}/(x^2 + 1){\bf Q}$

        typedef residue<sparse_polynom<rational<> > > T;

        T a = "((1+x) (mod (x^2+1)))";
        T b = "((1-x) (mod (x^2+1)))";

        cout << "a = " << a << endl;
        cout << "b = " << b << endl;
        cout << "a + b = " << a + b << endl;
        cout << "a - b = " << a - b << endl;
        cout << "a * b = " << a * b << endl;
        cout << "a / b = " << a / b << endl << endl;
    }

    {
        // Working in ${\bf Z}\sb{3}/(x^2 + 1){\bf Z}\sb{3}$

        typedef residue<sparse_polynom<residue<int> > > T;

        T a = "( ((1(mod 3))*x+(1(mod 3))) (mod ((1(mod 3))*x^2+(1(mod 3)))) )";
        T b = "( ((1(mod 3))*x+(2(mod 3))) (mod ((1(mod 3))*x^2+(1(mod 3)))) )";

        cout << "a = " << a << endl;
        cout << "b = " << b << endl;
        cout << "a + b = " << a + b << endl;
        cout << "a - b = " << a - b << endl;
        cout << "a * b = " << a * b << endl;
        cout << "a / b = " << a / b << endl;
    }

    return 0;
}
