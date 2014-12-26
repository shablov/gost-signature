#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    // Compute a factorial and a root
    big_int f = factorial(big_int(16));
    cout << "16! = " << f << endl;
    big_int sf = intsqrt(f);
    cout << "Integer part of sqrt(16!) = " << sf << endl;
    cout << sf << "^2 = " << sf*sf << endl;

    // Find the first composite Fermat's number
    big_int pow = 1;
    big_int fermas_number;
    cout << "Fermat's numbers: " << endl;
    while(is_prime(fermas_number = (big_int(1) << pow) + 1))
    {
        cout << fermas_number << " is prime" << endl;
        pow <<= 1;
    }
    cout << fermas_number << " is composite" << endl;

    // Find its fractorisation
    vector<big_int> factorization;
    factorize(fermas_number, factorization);
    cout << fermas_number << " = ";
    output_list(cout, factorization, "", "", "*");
    cout << endl;

    return 0;
}
