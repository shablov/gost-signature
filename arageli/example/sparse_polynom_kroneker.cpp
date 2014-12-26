#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


bool findoneroot
(
    sparse_polynom<rational<> > &poly,
    big_int &num,
    big_int &den
)
{
    // Factorization
    vector<big_int> num_factorization, den_factorization;
    factorize(num < 0 ? -num : num, num_factorization);
    factorize(den < 0 ? -den : den, den_factorization);

    // All combinations of numerators and denominators
    // Substitute them into poly
    big_int den_variants = power(2, den_factorization.size()),
            num_variants = power(2, num_factorization.size());
    typedef vector<big_int>::iterator pfactor;

    for(big_int den_mask = 0; den_mask <= den_variants; ++den_mask)
        for(big_int num_mask = 0; num_mask <= num_variants; ++num_mask)
        {
            // Constructin a numerator
            big_int j = num_mask;
            big_int trial_num = 1;

            for
            (
                pfactor
                    factor = num_factorization.begin(),
                    end = num_factorization.end();
                factor != end && j != 0;
                ++factor
            )
            {
                if(j.is_odd())
                    trial_num *= *factor;
                j >>= 1;
            }

            // Constructing a denomiator
            big_int i = den_mask;
            big_int trial_den = 1;

            for
            (
                pfactor
                    factor = den_factorization.begin(),
                    end = den_factorization.end();
                factor != end && i != 0;
                ++factor
            )
            {
                if(i.is_odd())
                    trial_den *= *factor;
                i >>= 1;
            }

            // Substitute a trial root into poly
            if(poly.subs(rational<>(trial_num,trial_den)) == 0)
            {
                num = trial_num;
                den = trial_den;
                return true;
            }

            if(poly.subs(rational<>(-trial_num,trial_den)) == 0)
            {
                num = -trial_num;
                den = trial_den;
                return true;
            }
        }

    return false;
}

void findroots(sparse_polynom<rational<> > poly, vector<rational<> > &ret)
{
    ret.resize(0); // for the present there are no roots found

    // First find zero roots
    while(poly.subs(rational<>(0)) == 0)
    {
        ret.push_back(rational<>(0,1));
        poly /= sparse_polynom<rational<> >(rational<>(1,1),1);
    }

    if (poly.is_const()) return;

    // Find common denominator
    typedef sparse_polynom<rational<> >::monom_iterator pmonom;
    big_int nok = 1;
    for (pmonom i = poly.monoms_begin(), j = poly.monoms_end(); i != j; ++i)
        nok *= i->coef().denominator() / gcd(nok,i->coef().denominator());
    for (pmonom i = poly.monoms_begin(), j = poly.monoms_end(); i != j; ++i)
        i->coef() *= nok;

    big_int num = poly.monoms_begin()->coef().numerator();
    big_int den = (--poly.monoms_end())->coef().numerator();

    // Find all roots
    while(!poly.is_const() && findoneroot(poly, num, den))
    {
        // Store tye root found
        ret.push_back(rational<>(num, den));

        // Reduce the degree of the polynomial
        sparse_polynom<rational<> > tmp(rational<>(den, 1), 1);
        tmp += rational<>(-num, 1);
        poly /= tmp;

        // Take numerator and denominator for the next root
        num = poly.monoms_begin()->coef().numerator();
        den = (--poly.monoms_end())->coef().numerator();
    }
}

int main ()
{
    sparse_polynom<rational<> >
        P = "x^7+167/15*x^6-221/20*x^5-91/12*x^4+27/10*x^3+6/5*x^2";
    cout << "P = " << P << endl << endl;

    vector<rational<> > roots;
    findroots(P,roots);

    cout << "Rational roots: " << roots << endl;
    return 0;
}

