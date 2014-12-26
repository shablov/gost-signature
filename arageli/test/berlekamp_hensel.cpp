/*****************************************************************************

    test/berlekamp_hensel.cpp

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
    \file berlekamp_hensel.cpp
    \brief This file includes test for factorize_berlekamp_hensel function.

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
bool simple_test1
(
    int maxdeg_a,
    int maxmodule,
    int seed,
    int coverage
)
{
    typedef typename P::coef_type T;
    typedef set::grid1<T> CS;
    typedef set::vector_fixed_size<vector<T>, CS> VS;
    typedef set::byvector<P, VS> PS;

    int num_of_err = 0;

    std::map<int, int> hdeg, hnumfact;
    std::map<P, int> pols;

    for
    (
        std::size_t maxdeg = 0;
        maxdeg < maxdeg_a && num_of_err < 10;
        maxdeg += std::max(std::size_t(1), std::size_t(maxdeg_a/10))
    )
    {
        CS cs(-maxmodule, maxmodule);
        PS ps(VS(maxdeg+1, cs));
        rnd::equiprob<PS> prnd(seed, ps);
        rnd::equiprob<CS> crnd(seed+1, cs);

        std::size_t n = Arageli::sqrt(coverage/100) + 3;
        P p;

        for(std::size_t i = 0; i < n && num_of_err < 10; ++i)try
        {
            p = prnd();
            //tout << "\np = " << p << "\n";
            ++pols[p];
            T m;

            vector<P> v = factorize_berlekamp_hensel(p);
            P prodv = product(v);

            if(prodv != p)
            {
                tout
                    << "Error at simple_test1:"
                    << "\n\tT = " << typeid(T).name()
                    << "\n\tp     = " << p
                    << "\n\tprodv = " << prodv
                    << "\n\tv = \n";
                output_aligned(tout, v);

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
                << "\n\tp = " << p << "\n";
        }
        catch(const std::exception& e)
        {
            tout << "\n" << e.what();
            ++num_of_err;
            tout
                << "\nWhere"
                << "\n\tT = " << typeid(T).name()
                << "\n\tp = " << p << "\n";
        }
        catch(...)
        {
            tout << "\nUnknown exception\n";
            ++num_of_err;
            tout
                << "\nWhere"
                << "\n\tT = " << typeid(T).name()
                << "\n\tp = " << p << "\n";
        }
    }

#ifdef ARAGELI_TEST_OUTPUT_STATISTICS
    tout << "\nStatistics for T = " << typeid(T).name() << "\n";
    tout << "Degrees:\n";
    output_map(hdeg);
    tout << "Number of factors:\n";
    output_map(hnumfact);
    tout << "Different polynomials (" << pols.size() << "):\n";
    output_map(pols);
#endif

    return num_of_err == 0;
}


template <typename P>
bool simple_test2 ()
{
    try
    {
        P p = "x^16+84*x^14+2346*x^12+30744*x^10+168941*x^8-60900*x^6-3607320*x^4-63000*x^2+100";
        vector<P> v = factorize_berlekamp_hensel(p);
        //std::cout<<v;
        if(v.size() != 1 || v.front() != p)
        {
            tout
                << "Error at simple_test2:"
                << "\n\tP = " << typeid(P).name()
                << "\n\tp     = " << p
                << "\n\tv = \n";
            output_aligned(tout, v);
        }
    }
    catch(const Arageli::exception& e)
    {
        tout << "\n" << e;
        return false;
    }

    try
    {
        P p = "400*x^2+360*x-180";
        vector<P> v = factorize_berlekamp_hensel(p);
        //std::cout<<v;
        if(v.size() == 0 || product(v) != v)
        {
            tout
                << "Error at simple_test2:"
                << "\n\tP = " << typeid(P).name()
                << "\n\tp     = " << p
                << "\n\tv = "
                << "\nproduct(v) = " << product(v) << "\n";
            output_aligned(tout, v);
        }
    }
    catch(const Arageli::exception& e)
    {
        tout << "\n" << e;
        return false;
    }
    return true;
}


}


TEST_FUNCTION(factorize_berlekamp_hensel, "A simple test for factorize_berlekamp_hensel function.")
{
    //return resHANG;    // buzz

    int seed = 1, coverage = ARAGELI_TESTSYS_COVERAGE_DEFAULT;

    bool is_ok = true;

    std::size_t maxdeg = 20;
    std::size_t m = 50;

    try
    {
        //is_ok &= simple_test<sparse_polynom<int> >(maxdeg, m, seed, coverage);
        //is_ok &= simple_test<sparse_polynom<int> >(maxdeg, m, seed, coverage);
        is_ok &= simple_test2<sparse_polynom<big_int> >();
        is_ok &= simple_test1<sparse_polynom<big_int> >(maxdeg, m, seed, coverage);

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
