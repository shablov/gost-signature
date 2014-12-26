/*****************************************************************************

    test/dense_sparse_polynom.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 2005--2007 Sergey S. Lyalin

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
    \file dense_sparse_polynom.cpp
    \brief Testing sparse_polynom and polynom template classes together.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{


/// Relative test polynom and sparse_polynom over ring.
template <typename CoefSet>
bool poly_test1
(
    std::size_t maxdeg,
    const CoefSet& cs,
    int seed,
    int coverage
)
{
    typedef typename CoefSet::value_type T;
    typedef set::vector_upto_size<vector<T>, CoefSet> VSet;
    typedef sparse_polynom<T> SP;
    typedef polynom<T> P;
    typedef set::byvector<sparse_polynom<T>, VSet> SPSet;
    typedef set::byvector<polynom<T>, VSet> PSet;
    typedef rnd::equiprob<SPSet> SPRnd;
    typedef rnd::equiprob<PSet> PRnd;

    VSet vset(maxdeg+1, cs);

    SPSet spset(vset);
    PSet pset(vset);

    SPRnd sprnd(seed, spset);
    PRnd prnd(seed, pset);

    int num_of_err = 0;

    int n = coverage/10 + 10;

    for(int i = 0; i < n && num_of_err < 10; ++i)
    {
        SP sp1 = sprnd(), sp2 = sprnd();
        P   p1 =  prnd(),  p2 =  prnd();

        if
        (
            SP(p1) != sp1 ||
            SP(p2) != sp2 ||
            P(sp1) != p1 ||
            P(sp2) != p2    // WARNING! ERROR!
        )
        {
            tout
                << "Error at Stage 1 of poly_test1:"
                << "\n\tsp1 = " << sp1
                << "\n\tsp2 = " << sp2
                << "\n\tp1 = " << sp1
                << "\n\tp2 = " << sp2
                << "\n\tP(sp1) = " << P(sp1)
                << "\n\tP(sp2) = " << P(sp2)
                << "\n\tSP(p1) = " << SP(sp1)
                << "\n\tSP(p2) = " << SP(sp2)
                << "\n\t(SP(p1) != sp1) = " << (SP(p1) != sp1)
                << "\n\t(SP(p2) != sp2) = " << (SP(p2) != sp2)
                << "\n\t(P(sp1) != p1) = " << (P(sp1) != p1)
                << "\n\t(P(sp2) != p2) = " << (P(sp2) != p2)
                << std::endl;

            ++num_of_err;
        }

        SP spuplus = +sp1;
        P  puplus = +p1;

        if(SP(puplus) != spuplus || P(spuplus) != puplus)
        {
            tout
                << "Error at Stage UPLUS of poly_test1:"
                << "\n\tsp1 = " << sp1
                << "\n\tp1 = " << p1
                << "\n\tspuplus = " << spuplus
                << "\n\tpuplus = " << puplus
                << "\n\t(SP(puplus) != spuplus) = " << (SP(puplus) != spuplus)
                << "\n\t(P(spuplus) != puplus) = " << (P(spuplus) != puplus)
                << std::endl;

            ++num_of_err;
        }


        SP spuminus = -sp1;
        P  puminus = -p1;

        if(SP(puminus) != spuminus || P(spuminus) != puminus)
        {
            tout
                << "Error at Stage UMINUS of poly_test1:"
                << "\n\tsp1 = " << sp1
                << "\n\tp1 = " << p1
                << "\n\tspuminus = " << spuminus
                << "\n\tpuminus = " << puminus
                << "\n\t(SP(puminus) != spuminus) = " << (SP(puminus) != spuminus)
                << "\n\t(P(spuminus) != puminus) = " << (P(spuminus) != puminus)
                << std::endl;

            ++num_of_err;
        }


        SP spbplus = sp1 + sp2;
        P  pbplus = p1 + p2;

        if(SP(pbplus) != spbplus || P(spbplus) != pbplus)
        {
            tout
                << "Error at Stage BPLUS of poly_test1:"
                << "\n\tsp1 = " << sp1
                << "\n\tp1 = " << p1
                << "\n\tsp2 = " << sp2
                << "\n\tp2 = " << p2
                << "\n\tspbplus = " << spbplus
                << "\n\tpbplus = " << pbplus
                << "\n\t(SP(pbplus) != spbplus) = " << (SP(pbplus) != spbplus)
                << "\n\t(P(spbplus) != pbplus) = " << (P(spbplus) != pbplus)
                << std::endl;

            ++num_of_err;
        }


        SP spbminus = sp1 - sp2;
        P  pbminus = p1 - p2;

        if(SP(pbminus) != spbminus || P(spbminus) != pbminus)
        {
            tout
                << "Error at Stage BMINUS of poly_test1:"
                << "\n\tsp1 = " << sp1
                << "\n\tp1 = " << p1
                << "\n\tsp2 = " << sp2
                << "\n\tp2 = " << p2
                << "\n\tspbminus = " << spbminus
                << "\n\tpbminus = " << pbminus
                << "\n\t(SP(pbminus) != spbminus) = " << (SP(pbminus) != spbminus)
                << "\n\t(P(spbminus) != pbminus) = " << (P(spbminus) != pbminus)
                << std::endl;

            ++num_of_err;
        }


        SP spbmult = sp1 * sp2;
        P  pbmult = p1 * p2;

        if(SP(pbmult) != spbmult || P(spbmult) != pbmult)
        {
            tout
                << "Error at Stage BMULT of poly_test1:"
                << "\n\tsp1 = " << sp1
                << "\n\tp1 = " << p1
                << "\n\tsp2 = " << sp2
                << "\n\tp2 = " << p2
                << "\n\tspbmult = " << spbmult
                << "\n\tpbmult = " << pbmult
                << "\n\t(SP(pbmult) != spbmult) = " << (SP(pbmult) != spbmult)
                << "\n\t(P(spbmult) != pbmult) = " << (P(spbmult) != pbmult)
                << std::endl;

            ++num_of_err;
        }

        if(!type_traits<T>::is_field)continue;

        SP spbquot = sp1 / sp2;
        P  pbquot = p1 / p2;

        if(SP(pbquot) != spbquot || P(spbquot) != pbquot)
        {
            tout
                << "Error at Stage BQUOT of poly_test1:"
                << "\n\tsp1 = " << sp1
                << "\n\tp1 = " << p1
                << "\n\tsp2 = " << sp2
                << "\n\tp2 = " << p2
                << "\n\tspbquot = " << spbquot
                << "\n\tpbquot = " << pbquot
                << "\n\t(SP(pbquot) != spbquot) = " << (SP(pbquot) != spbquot)
                << "\n\t(P(spbquot) != pbquot) = " << (P(spbquot) != pbquot)
                << std::endl;

            ++num_of_err;
        }


        SP spbrem = sp1 % sp2;
        P  pbrem = p1 % p2;

        if(SP(pbrem) != spbrem || P(spbrem) != pbrem)
        {
            tout
                << "Error at Stage BREM of poly_test1:"
                << "\n\tsp1 = " << sp1
                << "\n\tp1 = " << p1
                << "\n\tsp2 = " << sp2
                << "\n\tp2 = " << p2
                << "\n\tspbrem = " << spbrem
                << "\n\tpbrem = " << pbrem
                << "\n\t(SP(pbrem) != spbrem) = " << (SP(pbrem) != spbrem)
                << "\n\t(P(spbrem) != pbrem) = " << (P(spbrem) != pbrem)
                << std::endl;

            ++num_of_err;
        }


    }

    return num_of_err == 0;
}


}


TEST_FUNCTION(dense_sparse_polynom, "Testing sparse_polynom and polynom template classes together.")
{
    int seed = 1, coverage = ARAGELI_TESTSYS_COVERAGE_DEFAULT;

    std::size_t maxdeg = 4;
    int intlim = 5;

    bool is_ok = true;

    try
    {
        #define CALL_TEST_1(T, T1)    \
            is_ok &= poly_test1    \
            (    \
                maxdeg,    \
                set::make_grid1<T >(-T1(intlim), T1(intlim)),    \
                seed, coverage    \
            );

        CALL_TEST_1(int, int)
        CALL_TEST_1(big_int, big_int)
        CALL_TEST_1(rational<int>, int)
        CALL_TEST_1(rational<big_int>, big_int)


        #define CALL_TEST_2(P, T, T1)    \
            is_ok &= poly_test1    \
            (    \
                maxdeg,    \
                set::make_byvector<P<T > >    \
                (    \
                    set::make_vector_fixed_size<vector<T > >    \
                    (    \
                        maxdeg/2,    \
                        set::make_grid1<T >(-T1(intlim/2), T1(intlim/2))    \
                    )    \
                ),    \
                seed, coverage    \
            );

        CALL_TEST_2(polynom, int, int)
        CALL_TEST_2(polynom, big_int, big_int)
        CALL_TEST_2(polynom, rational<int>, int)
        CALL_TEST_2(polynom, rational<big_int>, big_int)
        CALL_TEST_2(sparse_polynom, int, int)
        CALL_TEST_2(sparse_polynom, big_int, big_int)
        CALL_TEST_2(sparse_polynom, rational<int>, int)
        CALL_TEST_2(sparse_polynom, rational<big_int>, big_int)

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
