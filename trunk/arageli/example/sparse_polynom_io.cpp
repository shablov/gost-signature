#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::cin;
using std::endl;


int main ()
{
    sparse_polynom<int> S;
    cout << "Enter a polynomial with integer coefficients "
        << endl << endl;

    cin >> S;

    cout << "Standard form: " << S << endl;

    return 0;
}
