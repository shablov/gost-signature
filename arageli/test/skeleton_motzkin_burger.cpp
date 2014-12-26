/*****************************************************************************

    test/skeleton_motzkin_burger.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
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

#include "stdafx.hpp"


using namespace Arageli;
using Arageli::vector;

namespace
{
using Arageli::vector;

// lexicographical sort of rows of a matrix
template <typename M>
void matrix_rows_sort (M& m)
{
    typedef typename M::element_type T;
    typedef vector<vector<T>, false> VVT;

    VVT rows;
    for(size_t i = 0; i < m.nrows(); ++i)
        rows.push_back(m.copy_row(i));

    std::sort(rows);

    for(size_t i = 0; i < m.nrows(); ++i)
        m.assign_row(i, rows[i]);
}

template <typename T>
bool skeleton_prof_correct
(
    const char* a_orig,
    const char* a_out,
    const char* f_out,
    const char* e_out
)
{
    bool fail = false;

    matrix<T, false> a1(a_orig), f1, q1, e1, a2(a_orig), f2, q2, e2;
    std::ostringstream buffer;

    skeleton(a1, f1, q1, e1);
    buffer << "skeleton(a2, f2, q2, e2, ...)\n";
    skeleton(a2, f2, q2, e2, ctrl::make_skeleton_slog(buffer));

    matrix<T, true> a3(a_orig), f3, q3, e3, a4(a_orig), f4, q4, e4;

    skeleton(a3, f3, q3, e3);
    buffer << "skeleton(a4, f4, q4, e4, ...)\n";
    skeleton(a4, f4, q4, e4, ctrl::make_skeleton_slog(buffer));

    matrix<T, false> a_res(a_out), f_res(f_out), e_res(e_out);

    #define SIMPLE_CHECK(EXPR)    \
        if(!(EXPR)){ tout << "ERROR: !(" << #EXPR << ")\n"; fail = true; }

    matrix_rows_sort(a1);
    matrix_rows_sort(a2);
    matrix_rows_sort(a3);
    matrix_rows_sort(a4);
    matrix_rows_sort(a_res);

    matrix_rows_sort(q1);
    matrix_rows_sort(q2);
    matrix_rows_sort(q3);
    matrix_rows_sort(q4);

    matrix_rows_sort(f1);
    matrix_rows_sort(f2);
    matrix_rows_sort(f3);
    matrix_rows_sort(f4);
    matrix_rows_sort(f_res);

    matrix_rows_sort(e1);
    matrix_rows_sort(e2);
    matrix_rows_sort(e3);
    matrix_rows_sort(e4);
    matrix_rows_sort(e_res);

    SIMPLE_CHECK(a1 == a2)
    SIMPLE_CHECK(a1 == a3)
    SIMPLE_CHECK(a1 == a4)
    SIMPLE_CHECK(a1 == a_res)

    SIMPLE_CHECK(f1 == f2)
    SIMPLE_CHECK(f1 == f3)
    SIMPLE_CHECK(f1 == f4)
    SIMPLE_CHECK(f1 == f_res)

    SIMPLE_CHECK(q1 == q2)
    SIMPLE_CHECK(q1 == q3)
    SIMPLE_CHECK(q1 == q4)

    SIMPLE_CHECK(e1.is_empty() && e2.is_empty() || e1 == e2)
    SIMPLE_CHECK(e1.is_empty() && e3.is_empty() || e1 == e3)
    SIMPLE_CHECK(e1.is_empty() && e4.is_empty() || e1 == e4)
    SIMPLE_CHECK(e1.is_empty() && e_res.is_empty() || e1 == e_res)

    if(fail)
    {
        tout << "\nThe matrices are:";

        output_aligned(tout << "\na1 =\n", a1);
        output_aligned(tout << "\na2 =\n", a2);
        output_aligned(tout << "\na3 =\n", a3);
        output_aligned(tout << "\na4 =\n", a4);
        output_aligned(tout << "\na_res =\n", a_res);

        output_aligned(tout << "\nf1 =\n", f1);
        output_aligned(tout << "\nf2 =\n", f2);
        output_aligned(tout << "\nf3 =\n", f3);
        output_aligned(tout << "\nf4 =\n", f4);
        output_aligned(tout << "\nf_res =\n", f_res);

        output_aligned(tout << "\nq1 =\n", q1);
        output_aligned(tout << "\nq2 =\n", q2);
        output_aligned(tout << "\nq3 =\n", q3);
        output_aligned(tout << "\nq4 =\n", q4);

        output_aligned(tout << "\ne1 =\n", e1);
        output_aligned(tout << "\ne2 =\n", e2);
        output_aligned(tout << "\ne3 =\n", e3);
        output_aligned(tout << "\ne4 =\n", e4);
        output_aligned(tout << "\ne_res =\n", e_res);

        tout << "\n ************ BUFFER *************\n";
        tout << buffer.str();
        tout << "\n ********** END BUFFER ***********\n";
    }

    return !fail;
}


template <typename T>
bool simple_convex_test ()
{
    bool fail = false;

    // TEMPORARY PROBLEM
    //fail |= !skeleton_prof_correct<T>
    //(
    //    "((0))",    // A orig
    //    "((0))",    // A out
    //    "(())",    // F out
    //    "((1))"    // E out
    //);

    fail |= !skeleton_prof_correct<T>
    (
        "((1))",    // A orig
        "((1))",    // A out
        "((1))",    // F out
        "(())"    // E out
    );

    fail |= !skeleton_prof_correct<T>
    (
        "((0, 1))",    // A orig
        "((0, 1))",    // A out
        "((0, 1))",    // F out
        "((1, 0))"    // E out
    );

    fail |= !skeleton_prof_correct<T>
    (
        "((1, -1), (1, 1))",    // A orig
        "((1, -1), (1, 1))",    // A out
        "((1, -1), (1, 1))",    // F out
        "(())"    // E out
    );

    fail |= !skeleton_prof_correct<T>
    (
        "((1, -1))",    // A orig
        "((1, -1))",    // A out
        "((1, 0))",    // F out
        "((1, 1))"    // E out
    );

    return !fail;
}


bool old_test ()
{
    const int test_amount = 100;
    const int max_fails = 10;

    int stage = 0;

    std::ostringstream buffer;

    try
    {
        matrix<big_int> a;
        matrix<big_int> f, q, e;

        ////////////////////////////////////////////////////////////////////

        a =
            "((1, 0, 0, 0), "
            "(-2, -1, 3, 1), "
            "(0, 2, 5, -1), "
            "(-1, 1, 1, 0), "
            "(1, 0, 1, 0), "
            "(-1, 0, 2, 1))";


        skeleton_motzkin_burger(a, f, q, e, ctrl::make_skeleton_motzkin_burger_slog(buffer));

        a = "((0, 0, 0))";

        skeleton_motzkin_burger(a, f, q, e, ctrl::make_skeleton_motzkin_burger_slog(buffer));

        a = "((1, 0, 0), (0, 1, 0), (0, 0, 1), (1, -1, -1))";

        skeleton_motzkin_burger(a, f, q, e, ctrl::make_skeleton_motzkin_burger_slog(buffer));

        a =
            "((2, 1, -2), "
            "(10, -2, -5), "
            "(1, -1, 1),"
            "(0, 1, 0),"
            "(0, 0, 1),"
            "(1, 0, 0))";


        skeleton_motzkin_burger(a, f, q, e, ctrl::make_skeleton_motzkin_burger_slog(buffer));

        a =
            "((0, 0, 0), "
            "(1, -1, -1), "
            "(-2, 1, 1),"
            "(0, 1, 0))";


        skeleton_motzkin_burger(a, f, q, e, ctrl::make_skeleton_motzkin_burger_slog(buffer));

        ////////////////////////////////////////////////////////////////////

        a =
            "((1, 0, 0, 0), "
            "(-2, -1, 3, 1), "
            "(0, 2, 5, -1), "
            "(-1, 1, 1, 0), "
            "(1, 0, 1, 0), "
            "(-1, 0, 2, 1))";

        skeleton_motzkin_burger(a, f, q, e);

        a = "((0, 0, 0))";

        skeleton_motzkin_burger(a, f, q, e);

        a = "((1, 0, 0), (0, 1, 0), (0, 0, 1), (1, -1, -1))";

        skeleton_motzkin_burger(a, f, q, e);

        a =
            "((2, 1, -2), "
            "(10, -2, -5), "
            "(1, -1, 1),"
            "(0, 1, 0),"
            "(0, 0, 1),"
            "(1, 0, 0))";


        skeleton_motzkin_burger(a, f, q, e);

        a =
            "((0, 0, 0), "
            "(1, -1, -1), "
            "(-2, 1, 1),"
            "(0, 1, 0))";


        skeleton_motzkin_burger(a, f, q, e);

        ////////////////////////////////////////////////////////////////////

        a =
            "((1, 0, 0, 0), "
            "(-2, -1, 3, 1), "
            "(0, 2, 5, -1), "
            "(-1, 1, 1, 0), "
            "(1, 0, 1, 0), "
            "(-1, 0, 2, 1))";

        skeleton(a, f, q, e, ctrl::make_skeleton_slog(buffer));

        a = "((0, 0, 0))";

        skeleton(a, f, q, e, ctrl::make_skeleton_slog(buffer));

        a = "((1, 0, 0), (0, 1, 0), (0, 0, 1), (1, -1, -1))";

        skeleton(a, f, q, e, ctrl::make_skeleton_slog(buffer));

        a =
            "((2, 1, -2), "
            "(10, -2, -5), "
            "(1, -1, 1),"
            "(0, 1, 0),"
            "(0, 0, 1),"
            "(1, 0, 0))";


        skeleton(a, f, q, e, ctrl::make_skeleton_slog(buffer));

        a =
            "((0, 0, 0), "
            "(1, -1, -1), "
            "(-2, 1, 1),"
            "(0, 1, 0))";


        skeleton(a, f, q, e, ctrl::make_skeleton_slog(buffer));

        ////////////////////////////////////////////////////////////////////

        a =
            "((1, 0, 0, 0), "
            "(-2, -1, 3, 1), "
            "(0, 2, 5, -1), "
            "(-1, 1, 1, 0), "
            "(1, 0, 1, 0), "
            "(-1, 0, 2, 1))";

        skeleton(a, f, q, e);

        a = "((0, 0, 0))";

        skeleton(a, f, q, e);

        a = "((1, 0, 0), (0, 1, 0), (0, 0, 1), (1, -1, -1))";

        skeleton(a, f, q, e);

        a =
            "((2, 1, -2), "
            "(10, -2, -5), "
            "(1, -1, 1),"
            "(0, 1, 0),"
            "(0, 0, 1),"
            "(1, 0, 0))";


        skeleton(a, f, q, e);

        a =
            "((0, 0, 0), "
            "(1, -1, -1), "
            "(-2, 1, 1),"
            "(0, 1, 0))";


        skeleton(a, f, q, e);
    }
    catch(const Arageli::exception& e)
    {
        tout
            << '\n' << e.msg()
            << "\n\tstage = " << stage << '\n'
            << "\n *********** BUFFER *************\n"
            << buffer.str() << '\n';
        return false;
    }

    return true;
}


// The skeleton worked incorrectly on this matrix.
// It has been found by Ekaterina Shchukina.
bool note_from_Catherine ()
{
    matrix<big_int> a = "((2, -1, -2), (1, 1, 1), (0, -4, -5))";
    matrix<big_int> f, q, e;
    std::ostringstream buffer;
    skeleton(a, f, q, e, ctrl::make_skeleton_slog(buffer));
    matrix<big_int> f_correct = "((-1, 5, -4), (3, -10, 8), (1, -4, 3))";
    matrix<big_int> q_correct(3, eye);

    if(f != f_correct)
    {
        tout
            << "note_from_Catherine was failed:"
            << "\nf output  = " << f
            << "\nf correct = " << f_correct << '\n'
            << "\n *********** BUFFER *************\n"
            << buffer.str() << '\n';

        return false;
    }

    if(q != q_correct)
    {
        tout
            << "note_from_Catherine was failed:"
            << "\nq output  = " << q
            << "\nq correct = " << q_correct << '\n'
            << "\n *********** BUFFER *************\n"
            << buffer.str() << '\n';

        return false;
    }

    return true;
}


}

TEST_FUNCTION(skeleton_motzkin_burger, "Test skeleton_motzkin_burger function.")
{
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN

    bool fail = false;
    fail |= !old_test();
    fail |= !simple_convex_test<int>();
    fail |= !simple_convex_test<big_int>();
    fail |= !simple_convex_test<rational<int> >();
    fail |= !simple_convex_test<rational<big_int> >();
    fail |= !note_from_Catherine();

    return fail ? resFAIL : resOK;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END
}
