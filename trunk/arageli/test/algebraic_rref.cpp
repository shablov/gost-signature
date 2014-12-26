/*****************************************************************************

    test/algebraic_rref.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    REPLACE OR/AND REFERENCE ADDITIONAL COPYRIGHTS HERE

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
    \file algebraic_rref.cpp
    \brief This file includes test for algebraic class with rref function.

    Random polynomials are generating for creating algebraic objects and
    filling matrices. Then rref is calling on such matrices.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

//#define ARAGELI_INLINE_DEBUG_OUTPUT
//#define ARAGELI_TEST_OUTPUT_STATISTICS


template <typename M>
void output_map (const M& m)
{
    for(typename M::const_iterator i = m.begin(); i != m.end(); ++i)
        tout << "[" << i->first << "] = " << i->second << "\n";
}


template <typename P>
bool test1
(
    std::size_t maxdeg_a,
    std::size_t maxsize,
    int maxval,
    int seed,
    int coverage
)
{
    typedef typename P::coef_type T;
    typedef set::grid1<T> CS;
    typedef set::ipositive<std::size_t> SS;
    typedef set::vector_fixed_size<vector<T>, CS> VS;
    typedef set::byvector<P, VS> PS;

    typedef algebraic<rational<>, rational<>, P> Algebr;

    int num_of_err = 0;

    std::map<Algebr, int> coefs, dets;
    std::map<vector<std::size_t>, int> sizes;
    std::map<std::size_t, int> ranks;
    typedef matrix<Algebr, false> Mat;
    Mat mat;

    for
    (
        std::size_t maxdeg = 1;
        maxdeg < maxdeg_a && num_of_err < 10;
        maxdeg += std::max(std::size_t(1), std::size_t(maxdeg_a/10))
    )
    {
        CS cs(-maxval, maxval);
        PS ps(VS(maxdeg+1, cs));
        SS ss(maxsize);
        rnd::equiprob<PS> prnd(seed, ps);
        rnd::equiprob<CS> crnd(seed+1, cs);
        rnd::equiprob<SS> srnd(seed+2, ss);

        std::size_t n = Arageli::sqrt(Arageli::sqrt(coverage/100)) + 2;
        P p;

        for(std::size_t i = 0; i < n && num_of_err < 10; ++i)try
        {
            std::size_t rows = srnd(), cols = srnd();
            ++sizes[make_vector(rows, cols)];
            mat.assign(rows, cols);

            for(typename Mat::iterator i = mat.begin(); i != mat.end(); ++i)
            {
                for(;;)
                {
                    while((p = prnd()).is_const() && !p.is_null());
                    vector<interval<T>, false> lims;
                    p = sqrfree_poly_rational(p);    // WARNING! Only for rationals.
                    sturm<T>(p, lims);
                    if(!lims.is_empty())break;
                }

                *i = p;
                ++coefs[*i];
            }
#ifdef ARAGELI_INLINE_DEBUG_OUTPUT
            output_aligned(std::cerr << "\nmat =\n", mat);
            output_aligned(std::cerr << "\ndouble(mat) =\n", matrix<double>(mat));
#endif

            Mat b, q;
            vector<std::size_t, false> basis;
            Algebr det;
            rref_field
            (
                mat, b, q, basis, det
#ifdef ARAGELI_INLINE_DEBUG_OUTPUT
                , ctrl::make_rref_field_slog(std::cerr, false, false)
#endif
            );
            ++dets[det];
            ++ranks[basis.size()];
            Mat matq = q*mat;

            if(b != matq)
            {
                tout
                    << "Error at test1: b != matq"
                    << "\n\tP = " << typeid(P).name();
                output_aligned(tout << "\nmat =\n", mat);
                output_aligned(tout << "\nmatq =\n", matq);
                output_aligned(tout << "\nb =\n", b);
                output_aligned(tout << "\nq =\n", q);
                tout
                    << "\n\tbasis = " << basis
                    << "\n\tdet = " << det;

                ++num_of_err;
            }

        }
        catch(Arageli::exception& e)
        {
            ARAGELI_EXCEPT_LOC(e);
            tout << "\n" << e;
            ++num_of_err;
            tout
                << "\nWhere"
                << "\n\tT = " << typeid(T).name()
                << "\n\tmat = " << mat << "\n";
        }
        catch(const std::exception& e)
        {
            tout << "\n" << e.what();
            ++num_of_err;
            tout
                << "\nWhere"
                << "\n\tT = " << typeid(T).name()
                << "\n\tmat = " << mat << "\n";
        }
        catch(...)
        {
            tout << "\nUnknown exception\n";
            ++num_of_err;
            tout
                << "\nWhere"
                << "\n\tT = " << typeid(T).name()
                << "\n\tmat = " << mat << "\n";
        }
    }

#ifdef ARAGELI_TEST_OUTPUT_STATISTICS
    tout << "\nStatistics for P = " << typeid(P).name() << "\n";
    tout << "Coefficiens (" << coefs.size() << "):\n";
    output_map(coefs);
    tout << "Determinats (" << dets.size() << "):\n";
    output_map(dets);
    tout << "Sizes (" << sizes.size() << "):\n";
    output_map(sizes);
    tout << "Ranks (" << ranks.size() << "):\n";
    output_map(ranks);
#endif

    return num_of_err == 0;
}


template <typename P>
bool test_0_multiplies_0 ()
{
    typedef rational<> T;
    typedef algebraic<T, T, P> Algebr;

    {
        Algebr a, b;

        ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN
            a*b;
        ARAGELI_EXCEPT_LOCCTRL_REGION_END
    }

    {
        Algebr a(null<T>()), b(null<T>());

        ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN
            a*b;
        ARAGELI_EXCEPT_LOCCTRL_REGION_END
    }

    {
        Algebr a(null<T>()), b;

        ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN
            a*b;
        ARAGELI_EXCEPT_LOCCTRL_REGION_END
    }

    {
        Algebr a, b(null<T>());

        ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN
            a*b;
        ARAGELI_EXCEPT_LOCCTRL_REGION_END
    }

    return true;
}


template <typename P>
bool test_2 ()
{
    typedef rational<> T;
    typedef algebraic<T, T, P> Algebr;
    typedef typename Algebr::interval_type Interv;

    ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN
    Algebr
        a
        (
            P("8*x^0-195*x^1"),
            Interv("(-3344/6847,3344/6847)")
        ),
        b
        (
            P("2145*x^0-2252*x^1"),
            Interv("(4950/6847,9900/6847)")
        );

        ARAGELI_ASSERT_ALWAYS(a.is_normal());
        ARAGELI_ASSERT_ALWAYS(b.is_normal());

        a*b;
    ARAGELI_EXCEPT_LOCCTRL_REGION_END

    return true;
}


}


TEST(algebraic, four_arithmetic_operations, "Test algebraic via rref.")
{
    //return resHANG;    // too long
    int seed = 1, coverage = ARAGELI_TESTSYS_COVERAGE_DEFAULT;

    bool is_ok = true;

    std::size_t maxdeg = 3;
    std::size_t maxval = 20;
    std::size_t maxsize = 3;

    try
    {
        is_ok &= test_0_multiplies_0<sparse_polynom<rational<> > >();
        is_ok &= test_2<sparse_polynom<rational<> > >();
        is_ok &= test1<sparse_polynom<rational<> > >(maxdeg,  maxsize, maxval,seed, coverage);
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
