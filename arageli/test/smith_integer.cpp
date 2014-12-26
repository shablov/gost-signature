/*****************************************************************************

    test/smith_integer.cpp

    This file is a part of the Arageli library test base.

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
    \file smith_integer.cpp
    \brief This file includes test for integer smith function for integer matrices.

    Test for Smith's normal diagonal form of an integer matrix.
    It generates in cycle several matrices and build for each of them
    the Smith's normal form, and check some test on the result.
*/


#include "stdafx.hpp"
#include "matrix_ops.hpp"

using namespace Arageli;


namespace
{

//#define ARAGELI_TEST_OUTPUT_STATISTICS


template <typename M, typename SRnd, typename VRnd>
void random_matrix (M& a, SRnd& srnd, VRnd& vrnd)
{
    typedef typename M::size_type size_type;
    size_type m = srnd(), n = srnd();
    a.resize(m, n);

    for(size_type i = 0; i < m; ++i)
        for(size_type j = 0; j < n; ++j)
            a(i, j) = vrnd();
}


template <typename M, typename Rank, typename Det>
bool is_smith (const M& b, Rank rank, const Det& det)
{
    typedef typename M::size_type size_type;
    typename M::element_type proddiag;
    proddiag = unit(proddiag);

    for(size_type i = 0; i < b.nrows(); ++i)
        for(size_type j = 0; j < b.ncols(); ++j)
        {
            if
            (
                i == j &&
                (
                    i < rank && is_null(b(i, j)) ||
                    i >= rank && !is_null(b(i, j))
                ) ||
                i != j && !is_null(b(i, j))
            )
                return false;

            if(i == j && i < rank)proddiag *= b(i, j);

            if(i == j && i > 0 && !is_divisible(b(i, i), b(i-1, i-1)))
                return false;
        }

    if(proddiag != abs(det))return false;
    return true;
}


template <typename T>
bool random_test_1 (std::size_t maxsize, const T& maxabsval, std::size_t n)
{
    typedef rnd::lcseq_fine1 BBasernd;
    typedef rnd::ref<BBasernd> Basernd;
    typedef matrix<T> Matrix;
    typedef typename Matrix::size_type size_type;
    typedef set::ipositive<size_type> Sizeset;
    typedef rnd::equiprob<Sizeset, Basernd> Sizernd;
    typedef set::grid1<T> Valset;
    typedef rnd::equiprob<Valset, Basernd> Valrnd;

    BBasernd bbrnd;
    Sizeset sizeset(maxsize);
    Sizernd sizernd(Basernd(bbrnd), sizeset);
    Valset valset(-maxabsval, maxabsval);
    Valrnd valrnd(Basernd(bbrnd), valset);
    Matrix a, p, b, q;
    size_type rank;
    T det;

    // for statistics

    std::map<Matrix, int> stat_a;
    std::map<Matrix, int> stat_b;
    std::map<size_type, int> stat_rank;
    std::map<T, int> stat_det;

    int num_of_err = 0;

    for(std::size_t i = 0; i < n && num_of_err < 10; ++i)
    {
        random_matrix(a, sizernd, valrnd);
        smith(a, b, p, q, rank, det);

        ++stat_a[a];
        ++stat_b[b];
        ++stat_rank[rank];
        ++stat_det[det];

        if(a.is_square())
        {
            if(rank == a.nrows() && det != Arageli::det_int(a))
            {
                tout
                    << "Error at DET test:"
                    << "\n\tT = " << typeid(T).name()
                    << "\n\tb = " << b
                    << "\n\tp = " << p
                    << "\n\ta = " << a
                    << "\n\tq = " << q
                    << "\n\trank = " << rank
                    << "\n\tdet = " << det
                    << "\n\tREAL det = " << Arageli::det_int(a)
                    << "\n\tp*a*q = " << p*a*q
                    << "\n";

                ++num_of_err;
            }

        }

        if(rank != Arageli::rank_int(a))
        {
            tout
                << "Error at DET test:"
                << "\n\tT = " << typeid(T).name()
                << "\n\tb = " << b
                << "\n\tp = " << p
                << "\n\ta = " << a
                << "\n\tq = " << q
                << "\n\trank = " << rank
                << "\n\tREAL rank = " << Arageli::rank_int(a)
                << "\n\tdet = " << det
                << "\n\tp*a*q = " << p*a*q
                << "\n";

            ++num_of_err;
        }

        if(b != p * a * q || !is_unit(abs(det_int(p))) || !is_unit(abs(det_int(q))))
        {
            tout
                << "Error at \"B = PBQ, |P| = 1, |Q| = 1\" test:"
                << "\n\tT = " << typeid(T).name()
                << "\n\tb = " << b
                << "\n\tp = " << p
                << "\n\ta = " << a
                << "\n\tq = " << q
                << "\n\tdet_int(p) = " << abs(det_int(p))
                << "\n\tdet_int(q) = " << abs(det_int(q))
                << "\n\trank = " << rank
                << "\n\tdet = " << det
                << "\n\tp*a*q = " << p*a*q
                << "\n";

            ++num_of_err;
        }

        if(!is_smith(b, rank, det))
        {
            tout
                << "Error at \"is_smith(b)\" test:"
                << "\n\tT = " << typeid(T).name()
                << "\n\tb = " << b
                << "\n\tp = " << p
                << "\n\ta = " << a
                << "\n\tq = " << q
                << "\n\trank = " << rank
                << "\n\tdet = " << det
                << "\n";

            ++num_of_err;
        }
    }

#ifdef ARAGELI_TEST_OUTPUT_STATISTICS
    tout << "Original matrices: (" << stat_a.size() << "):\n";
    output_map(stat_a);
    tout << "Smith's normal matrices: (" << stat_b.size() << "):\n";
    output_map(stat_b);
    tout << "Ranks: (" << stat_rank.size() << "):\n";
    output_map(stat_rank);
    tout << "Dets: (" << stat_det.size() << "):\n";
    output_map(stat_det);
#endif

    return num_of_err == 0;
}

}


TEST_FUNCTION
(
    smith_integer,
    "Test for smith function for integer matrices."
)
{
    bool is_ok = true;

    try
    {
        is_ok &= random_test_1<int>(3, 3, 10000);
        is_ok &= random_test_1<big_int>(3, 3, 10000);
        is_ok &= random_test_1<big_int>(3, 10, 10000);
        is_ok &= random_test_1<big_int>(5, 100, 10000);
        is_ok &= random_test_1<big_int>(10, 1000, 1000);
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
