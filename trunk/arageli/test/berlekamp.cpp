/*****************************************************************************

    test/berlekamp.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2006--2007 Sergey S. Lyalin

    The Arageli Library is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    The Arageli Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

    We are also open for dual licensing for the whole library or
    for its particular part. If you are interested to get the library
    in this way, i.e. not under the GNU General Public License,
    please contact Arageli Support Service support.arageli@gmail.com.

*****************************************************************************/

/**
    \file berlekamp.cpp
    \brief This file includes test for factorize_berlekamp function.

    Just random polynomial is being generated and factorized, after that
    the factors are being multiplied and the result is being compared with
    the original polynomial.
*/


#include "stdafx.hpp"
//#include "matrix_ops.hpp"

using namespace Arageli;


namespace
{

//#define ARAGELI_TEST_OUTPUT_STATISTICS


template <typename P>
bool simple_test
(
    std::size_t maxdeg_a,
    const typename P::coef_type& maxmodule,
    int seed,
    int coverage
)
{
    typedef typename P::coef_type T;
    typedef set::inonnegative<T> CS;
    typedef set::vector_fixed_size<vector<T>, CS> VS;
    typedef set::byvector<P, VS> PS;
    typedef set::grid1<T> MS;

    int num_of_err = 0;

    std::map<int, int> hdeg, hmodule, hnumfact;
    std::map<P, int> pols;

    for
    (
        std::size_t maxdeg = 0;
        maxdeg < maxdeg_a && num_of_err < 10;
        maxdeg += std::max(std::size_t(1), std::size_t(maxdeg_a/10))
    )
    {
        CS cs(maxmodule-1);
        MS ms(2, maxmodule);
        PS ps(VS(maxdeg+1, cs));
        rnd::equiprob<PS> prnd(seed, ps);
        rnd::equiprob<CS> crnd(seed+1, cs);
        rnd::equiprob<MS> mrnd(seed+2, ms);

        std::size_t n = Arageli::sqrt(coverage/1000) + 1;

        for(std::size_t i = 0; i < n && num_of_err < 10; ++i)
        {
            P p = prnd();
            //tout << "\np = " << p << "\n";
            ++pols[p];
            T m;
            P redp;

            for(std::size_t j = 0; j < n && num_of_err < 10; ++j)try
            {
                m = next_prime(mrnd());
                if(m > maxmodule)m = 2;    // WARNING! probability of 2 is greater
                //tout << "m = " << m << "\n";
                ++hmodule[m];

                redp = p;
                sparse_polynom_reduction_mod(redp, m);
                ++hdeg[redp.degree()];
                vector<P> pv = factorize_berlekamp(redp, m);
                ++hnumfact[pv.size()];
                typedef typename P::template other_coef<big_int>::type BP;
                BP bprodpv = product(vector<BP>(pv));
                BP bredprodpv = bprodpv;
                sparse_polynom_reduction_mod(bredprodpv, m);
                P redprodpv = bredprodpv;

                if(redprodpv != redp)
                {
                    tout
                        << "Error at P-Stage of simple_test:"
                        << "\n\tT = " << typeid(T).name()
                        << "\n\tp = " << p
                        << "\n\tm = " << m
                        << "\n\tredp      = " << redp
                        << "\n\tprodpv = " << bprodpv
                        << "\n\tredprodpv = " << redprodpv
                        << "\n\tpv = \n";
                    output_aligned(tout, pv);

                    ++num_of_err;
                }

                typedef residue<T> R;
                typedef typename P::template other_coef<R>::type RP;
                RP rp = p;
                rp *= residue<T>(unit<T>(), m);    // apply module
                vector<RP> rv = factorize_berlekamp(rp);
                RP prodrv = product(rv);

                if(prodrv != rp)
                {
                    tout
                        << "Error at R-Stage of simple_test:"
                        << "\n\tT = " << typeid(T).name()
                        << "\n\tp = " << p
                        << "\n\tm = " << m
                        << "\n\trp     = " << rp
                        << "\n\tprodrv = " << prodrv
                        << "\n\trv = \n";
                    output_aligned(tout, rv);

                    ++num_of_err;
                }
            }
            catch(const Arageli::exception& e)
            {
                tout << "\n" << e;
                ++num_of_err;
                tout
                    << "\nWhere"
                    << "\n\tT = " << typeid(T).name()
                    << "\n\tp = " << p
                    << "\n\tm = " << m
                    << "\n\tredp      = " << redp;
            }
            catch(const std::exception& e)
            {
                tout << "\n" << e.what();
                ++num_of_err;
                tout
                    << "\nWhere"
                    << "\n\tT = " << typeid(T).name()
                    << "\n\tp = " << p
                    << "\n\tm = " << m
                    << "\n\tredp      = " << redp;
            }
            catch(...)
            {
                tout << "\nUnknown exception\n";
                ++num_of_err;
                tout
                    << "\nWhere"
                    << "\n\tT = " << typeid(T).name()
                    << "\n\tp = " << p
                    << "\n\tm = " << m
                    << "\n\tredp      = " << redp;
            }

        }
    }

#ifdef ARAGELI_TEST_OUTPUT_STATISTICS
    tout << "\nStatistics for T = " << typeid(T).name() << "\n";
    tout << "Degrees:\n";
    output_map(hdeg);
    tout << "Modules:\n";
    output_map(hmodule);
    tout << "Number of factors:\n";
    output_map(hnumfact);
    tout << "Different polynomials (" << pols.size() << "):";
    output_map(pols);
#endif

    return num_of_err == 0;
}

}


TEST_FUNCTION(factorize_berlekamp, "A simple test for factorize_berlekamp function.")
{
    int seed = 1, coverage = ARAGELI_TESTSYS_COVERAGE_DEFAULT;

    bool is_ok = true;

    std::size_t maxdeg = 20;
    std::size_t m = 50;

    try
    {
        is_ok &= simple_test<sparse_polynom<int> >(maxdeg, m, seed, coverage);
        //is_ok &= simple_test<sparse_polynom<__int64> >(maxdeg, m, seed, coverage);
        is_ok &= simple_test<sparse_polynom<big_int> >(maxdeg, m, seed, coverage);
    }
    catch(const Arageli::exception& e)
    {
        tout << e;
        return resEXCEPT;
    }
    catch(const std::exception& e)
    {
        tout << e.what();
        return resEXCEPT;
    }

    return is_ok ? resOK : resFAIL;

    return resOK;
}
