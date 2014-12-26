#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    typedef sparse_polynom<big_int> poly;
    typedef poly::coef_iterator coefs;
    typedef poly::degree_iterator degrees;
    typedef poly::monom_const_iterator monoms;

    poly S = "213*x^3443+532*x^4432-744*x^44-4235*x^15+292*x+34254";
    cout << "S = " << S << endl << endl;

    // Let's find the minimal non-zero coefficient
    big_int min_coeff = S.leading_coef();
    for(coefs ci = S.coefs_begin(), cj = S.coefs_end(); ci != cj; ++ci)
        if(min_coeff > *ci) min_coeff = *ci;
    cout << "Minimal non-zero coefficient in S is " << min_coeff
        << endl << endl;

    // Find the sum of all degrees of S
    // We'll use the STL algorithms
    int dsum = std::accumulate(S.degrees_begin(), S.degrees_end(), 0);
    cout << "The sum of all degrees of S is "
        << dsum << endl << endl;

    // Pick out all monomials with odd degrees
    poly oddS;
    for(monoms mi = S.monoms_begin(), mj = S.monoms_end(); mi != mj; ++mi)
        if(is_odd(mi->degree()))
            oddS.insert(oddS.monoms_end(), *mi);
    cout << "The polynomial with only odd degrees = " << oddS << endl << endl;

    // Replace all degrees by their residues modulo 5
    for(degrees di = S.degrees_begin(), dj = S.degrees_end(); di != dj; ++di)
        *di %= 5;

    // Now S can contain monomials with equal degrees
    // We have to reduce S to its canonical form
    S.normalize();
    cout << "All degrees modulo 5 = " << S << endl;

    return 0;
}
