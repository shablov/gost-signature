/*****************************************************************************

    test/test3.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2005--2008 Sergey S. Lyalin

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

// WARNING! THIS FILE TEMPORARY IS TURNED OFF FROM THE COMPILATION DUE
// TO TOO MANY ERRORS WITH UNINVESTIGATED CAUSE.
#if 0

using namespace std;
using namespace Arageli;
using Arageli::vector;


void test3_1 ()
{
    // using namespace Arageli;

    std::ofstream texout("from_arageli.tex");
    typedef rational<> Rational;

    texout << "Несколько примеров вывода.";
    output_latex(texout << "\\par", 209373);
    output_latex(texout << "\\par", big_int("223098430928409238409309373"));
    output_latex(texout << "\\par", Rational(9));
    output_latex(texout << "\\par", Rational(9, 2));
    output_latex(texout << "\\par", Rational(9, 23));
    output_latex(texout << "\\par", Rational(9, 233));
    output_latex(texout << "\\par", Rational(-9));
    output_latex(texout << "\\par", Rational(-9, 2));
    output_latex(texout << "\\par", Rational(-9, 23));
    output_latex(texout << "\\par", Rational(-9, 233));

    using Arageli::vector;

    Arageli::vector<big_int> a = "(9287423984230984, 2039784, 928734, 298734, 93, 9, 1)";
    Arageli::vector<Rational > b = "(92874239/84230984, 20/39784, 9287/34, 2/98734, 93, 9, 1)";
    Arageli::vector<Rational > c = "(9, 9/2, 9/23, 9/233, -9, -9/2, -9/23, -9/233)";

    output_latex(texout << "\\par", a, false, eep_alone, true);
    output_latex(texout << "\\par", b, false, eep_alone, true);
    output_latex(texout << "\\par", c, false, eep_alone, true);
    output_latex(texout << "\\par", a, false, eep_alone, false);
    output_latex(texout << "\\par", b, false, eep_alone, false);
    output_latex(texout << "\\par", c, false, eep_alone, false);

    typedef sparse_polynom<rational<> > Sprbi;

    output_latex(texout << "\\par", Sprbi("0"));
    output_latex(texout << "\\par", Sprbi("1"));
    output_latex(texout << "\\par", Sprbi("-1"));
    output_latex(texout << "\\par", Sprbi("x"));
    output_latex(texout << "\\par", Sprbi("-x"));
    output_latex(texout << "\\par", Sprbi("2*x"));
    output_latex(texout << "\\par", Sprbi("2"));
    output_latex(texout << "\\par", Sprbi("-234/234425"));
    output_latex(texout << "\\par", Sprbi("23/123*x"));
    output_latex(texout << "\\par", Sprbi("-1+x"));
    output_latex(texout << "\\par", Sprbi("x^12-12/7*x"));
    output_latex(texout << "\\par", Sprbi("-x+123/234345-1/5*x"));
    output_latex(texout << "\\par", Sprbi("2*x^2-1+12/5*x"));
    output_latex(texout << "\\par", Sprbi("2+1/1000*x^8-1/3*x"));
    output_latex(texout << "\\par", Sprbi("-234/234425*x^123432-x^2313+x^3242-1"));
    output_latex(texout << "\\par", Sprbi("-23/123*x"));

    typedef matrix<Sprbi> Msprbi;

    output_latex(texout << "\\par", Msprbi("((1, 0), (-1, x))"));
    output_latex(texout << "\\par", Msprbi("((-x, 2*x), (2, -234/234425))"));
    output_latex(texout << "\\par", Msprbi("((23/123*x, -1+x), (x^12-12/7*x, -x+123/234345))"));
    output_latex(texout << "\\par", Msprbi("((2*x^2-1, 2+1/1000*x^8), (-234/234425*x^123432-x^2313+x^3242-1, -23/123*x))"));
    output_latex(texout << "\\par", Msprbi("((-x, 2+1/1000*x^8), (-234/234425*x^123432-x^2313+x^3242-1, -23/123*x))"));

    typedef sparse_polynom<matrix<rational<> > > Spmrbi;

    output_latex(texout << "\\par", Spmrbi("(((1, 0), (-1, 34))-x)"));
    output_latex(texout << "\\par", Spmrbi("(((-1, 2), (2, -234/234425))*x^2)"));
    output_latex(texout << "\\par", Spmrbi("(((23/123, -1), (12, 123/234345))*x^12-((1, 2), (2, 3), (23/121, 0))*x^2-((1/2, 1/3, 1/4)))"));
    output_latex(texout << "\\par", Spmrbi("(((23/123, -1, -1, -1), (12, 123/234345, -1, -1))*x-((1, 2), (2, 3), (23/121, 0))*x^2-((1/2, 1/3, 1/4)))"));

    typedef matrix<matrix<int> > Mmi;

    output_latex(texout << "\\par", Mmi("((((1, -2), (3, 4))), (((5, 6), (-7, 8))))"));
    output_latex(texout << "\\par", Mmi("((((2341, 2), (3, 23452344))), (((-25, 6), (-7, 8))))"));
    output_latex(texout << "\\par", Mmi("((((2341, 2), (3, 23452344)), ((-25, 6), (7, 8))))"));
    output_latex(texout << "\\par", Mmi("((((2341, 2), (3, -23452344)), ((25, 6), (-7, 8))), (((3, 23452344, 1, -1)), ((25, 6))))"));
}


void test3_2 ()
{
    // using namespace Arageli;

    {
        matrix<rational<> > a = "((1, 234, 21), (23, 34, 2), (2, 2, 345353))", b, q;
        Arageli::vector<size_t> basis;
        rational<> det;

        rref(a, b, q, basis, det);
        output_aligned_hor_pair(std::cout, b, q);

        std::cout << "\n\n*****************************************\n";
        rref(a, b, q, basis, det, ctrl::make_rref_slog(std::cout));
    }

    std::cout << "\n\n//////////////////////////////////////////\n\n";

    {
        matrix<big_int > a = "((1, 234, 21), (23, 34, 2), (2, 2, 345353))", b, q;
        Arageli::vector<size_t> basis;
        big_int det;

        rref_int(a, b, q, basis, det);
        output_aligned_hor_pair(std::cout, b, q);

        std::cout << "\n\n*****************************************\n";
        rref_int(a, b, q, basis, det, ctrl::make_rref_int_slog(std::cout));
    }
}


void test3_3 ()
{
    // using namespace Arageli;

    {
        matrix<big_int > a = "((1, 234, 21), (23, 34, 2), (2, 2, 345353))", b, q, p;
        Arageli::vector<size_t> basis;
        big_int det;
        size_t rank;

        smith(a, b, p, q, rank, det);
        output_aligned_corner_triplet_br(std::cout, q, b, p);

        std::cout << "\n\n*****************************************\n";
        smith(a, b, p, q, rank, det, ctrl::make_smith_slog(std::cout));
    }

    std::cout << "\n\n//////////////////////////////////////////\n\n";

    {
        matrix<sparse_polynom<rational<> > > a =
            "((x^3-x, 2*x^2), (x^2+5*x, 3*x))", b, q, p;
        Arageli::vector<size_t> basis;
        sparse_polynom<rational<> > det;
        size_t rank;

        smith(a, b, p, q, rank, det);
        output_aligned_corner_triplet_br(std::cout, q, b, p);

        std::cout << "\n\n*****************************************\n";
        smith(a, b, p, q, rank, det, ctrl::make_smith_slog(std::cout));
    }

    std::cout << "\n\n//////////////////////////////////////////\n\n";

    {
        matrix<sparse_polynom<rational<> > > a =
            "((x, x^3+5), (x^2-x-4, x^4-x^3-4*x^2+5*x-5))", b, q, p;
        Arageli::vector<size_t> basis;
        sparse_polynom<rational<> > det;
        size_t rank;

        smith(a, b, p, q, rank, det);
        output_aligned_corner_triplet_br(std::cout, q, b, p);

        std::cout << "\n\n*****************************************\n";
        smith(a, b, p, q, rank, det, ctrl::make_smith_slog(std::cout));
    }


}


void test3_4 ()
{
    // using namespace Arageli;

    Arageli::vector<rational<> > a;
    a *= rational<>(3);
    a *= big_int(234);
    a *= Arageli::vector<rational<> >();
    Arageli::vector<rational<> >() * rational<>();
    //Arageli::vector<Arageli::vector<rational<> > >() * Arageli::vector<rational<> >();
    Arageli::vector<sparse_polynom<rational<> > >() + sparse_polynom<rational<> >();

    Arageli::vector<Arageli::vector<rational<> > > b;
    b *= rational<>(4);
    b *= big_int(234);
    b *= Arageli::vector<rational<> >();
    b *= Arageli::vector<Arageli::vector<rational<> > >();
}


void test3_5 ()
{
    // using namespace Arageli;

    matrix<rational<> > a;
    a *= rational<>(3);
    a *= big_int(234);
    a *= matrix<rational<> >();
    a * rational<>(3);
    a * big_int(234);
    a * matrix<rational<> >();
    rational<>(3) * a;
    big_int(234) * a;
    matrix<rational<> >() * a;

    matrix<matrix<rational<> > > b;
    b *= rational<>(3);
    b *= big_int(234);
    b *= matrix<rational<> >();
    b *= matrix<matrix<rational<> > >();
    //b * rational<>(3);
    b * big_int(234);
    //b * matrix<rational<> >();
    b * matrix<matrix<rational<> > >();
    //rational<>(3) * b;
    big_int(234) * b;
    //matrix<rational<> >() * b;
    matrix<matrix<rational<> > >() * b;
}


void test3_6 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace iomanip;

    cout
        << matrix_output_aligned()
        << (rational<>(1, 3) * matrix<rational<> >("((1, 2), (3, 4))"))
        << endl
        << matrix_output_aligned()
        << (2 * matrix<rational<> >("((1, 2), (3, 4))"))
        << endl
        << matrix_output_aligned()
        << (big_int("323498727") * matrix<rational<> >("((1, 2), (3, 4))"))
        << endl
        << matrix_output_aligned()
        << (matrix<rational<> >("((1, 2), (3, 4))") * rational<>(1, 3))
        << endl
        ;
    cout
        << matrix_output_aligned()
        << (sparse_polynom<rational<> >(1, 3) * matrix<sparse_polynom<rational<> > >("((1, 2), (3, 4))"))
        << endl
        << matrix_output_aligned()
        << (2 * matrix<sparse_polynom<rational<> > >("((1, 2), (3, 4))"))
        << endl
        << matrix_output_aligned()
        << (big_int("323498727") * matrix<sparse_polynom<rational<> > >("((1, 2), (3, 4))"))
        << endl
        << matrix_output_aligned()
        << (matrix<sparse_polynom<rational<> > >("((1, 2), (3, 4))") * sparse_polynom<rational<> >(1, 3))
        << endl
        ;

    cout << sparse_polynom_output_aligned() << sparse_polynom<int>();
}


void test3_7 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace iomanip;

    sparse_polynom<rational<> > a;
    rational<> b;
    a * b;
}


void test3_8 ()
{
    // using namespace Arageli;
    // using namespace std;

    typedef matrix<sparse_polynom<rational<> > > Mspr;
    Mspr m1 =    "((1, 0, x-123/123345, -12/7*x^4000), "
                " (x^2-2*x+1, x-1, 2*x, 12),"
                " (1/7, 777777777/999999999999, x, -x))";

    ofstream texfile("gentex.inc.tex");

    //texfile << "$$";
    output_latex(texfile, m1);
    output_latex(texfile << "\\par", m1, false, eep_alone, false, "[", "]");
    output_latex(texfile << "\\par", m1, false, eep_alone, true, "|", "|");

    matrix_line h(1), v(1);
    std::list<matrix_box> boxes;
    boxes.push_back(matrix_box(1, 1));
    boxes.push_back(matrix_box(1, 3));
    boxes.push_back(matrix_box(2, 2));

    output_latex_matrix_frames
    (
        texfile << "\\par", m1,
        make_matrix_frames(&h, &h + 1, &v, &v + 1, boxes.begin(), boxes.end())
    );
    //texfile << "$$";
}


void test3_9 ()
{
    // using namespace Arageli;
    // using namespace std;

    typedef matrix<rational<> > Mspr;
    Mspr m1 =    "((  1,                       0,  123/123345,  -12/4000), "
                " (  3,                      -1,           2,        12),"
                " (1/7,  777777777/999999999999,           1,        -1))",
        b, q;
    Arageli::vector<rational<> > basis;
    rational<> det;

    ofstream texfile("gentex.inc.tex");
    rref(m1, b, q, basis, det, ctrl::make_rref_latexlog(texfile));

    texfile << " \\par {\\bf Transposed}\\par ";
    m1.transpose();
    rref(m1, b, q, basis, det, ctrl::make_rref_latexlog(texfile));
}


void test3_10 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace simplex_method;

    matrix<rational<> > m_orig =
        "((0,0,0,16/3,0,0,-625/3,4700/3,10/3), "
        " (0,0,1,2/25,0,0,-7/4,14,1/25), "
        " (0,1,0,-2/225,0,0,2/9,-16/9,-2/225), "
        " (0,0,0,2/75,0,1,-1/6,7/3,1/150), "
        " (1,0,0,0,1,0,0,0,1))";

    Arageli::vector<int> basis_orig = "(2,1,5,4)";

    {
        matrix<rational<> > m = m_orig;
        Arageli::vector<int> basis = basis_orig;

        output_aligned(cout, m);
        output_aligned(cout << "\n", basis);

        primal_row_iters(m, basis, rule_s_primal_first(),
            rule_r_primal_lex(), ctrl::simplex_method::primal_row_iters_idler());

        output_aligned(cout << "-------------\n", m);
        output_aligned(cout << "\n", basis);
    }

    cout << "\n**********************\n";

    {
        matrix<rational<> > m = m_orig;
        Arageli::vector<int> basis = basis_orig;

        //output_aligned(cout, m);
        //output_aligned(cout << "\n", basis);

        primal_row_iters(m, basis, rule_s_primal_first(),
            rule_r_primal_lex(), ctrl::simplex_method::primal_row_iters_slog<ostream>(cout));

        //output_aligned(cout << "-------------\n", m);
        //output_aligned(cout << "\n", basis);
    }

}

void test3_11 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace simplex_method;
    // using Arageli::vector;

    typedef rational<> T;
    Arageli::vector<T> c;
    matrix<T> a;
    Arageli::vector<T> b;
    Arageli::vector<T> bx;
    Arageli::vector<size_t> basis;
    T res;

    //c = "(-1, -1, 0, -5)";
    //a = "((1, 1, -1, 3), (1, -2, 3, -1), (5, -4, 7, 3))";
    //b = "(1, 1, 5)";

    c = "(0, 0, 0, 0, 200, 175, -1100, -2)";
    a =
        "((0, 1, 0, 0, -3, -5/4, 7, 1/50),"
        " (1, 0, 0, 0, 1/3, 1/6, -1, -1/150),"
        " (0, 0, 1, 0, 75/2, -25/4, 175/2, 1/4),"
        " (0, 0, 0, 1, 0, 0, 0, 1))";
    b = "(0, 0, 0, 1)";


    const char* status[] = {"FOUND", "EMPTY", "INFINITE", "NONOPTIMAL"};

    cout << "\n\nresult = " <<
        status
        [
            primal_row_with_artificial_basis
            (
                a, b, c, bx, basis, res, rule_s_primal_first(), rule_r_primal_lex(),
                ctrl::simplex_method::primal_row_with_artificial_basis_slog<ostream>(cout)
            )
        ];

    cout << "\n*********************************************\n";

    cout << "\n\nresult = " <<
        status[primal_row_with_artificial_basis(a, b, c, bx, basis, res)];
}


void test3_12 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace simplex_method;
    // using Arageli::vector;

    ifstream in("in.lpt.txt");

    for(;;)
    {
        linear_prog_task<rational<> > lpt;
        input_list(in, lpt);
        if(!in)break;
        simplex_method::output_aligned(cout, lpt);
    }
}


void test3_13 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace simplex_method;
    // using Arageli::vector;

    typedef rational<> T;
    Arageli::vector<T> c;
    matrix<T> a;
    Arageli::vector<T> b;
    Arageli::vector<T> bx;
    Arageli::vector<size_t> basis;
    T res;

    c = "(-1, -1, 0, -5)";
    a = "((1, 1, -1, 3), (1, -2, 3, -1), (5, -4, 7, 3))";
    b = "(1, 1, 5)";

    //c = "(0, 0, 0, 0, 200, 175, -1100, -2)";
    //a =
    //    "((0, 1, 0, 0, -3, -5/4, 7, 1/50),"
    //    " (1, 0, 0, 0, 1/3, 1/6, -1, -1/150),"
    //    " (0, 0, 1, 0, 75/2, -25/4, 175/2, 1/4),"
    //    " (0, 0, 0, 1, 0, 0, 0, 1))";
    //b = "(0, 0, 0, 1)";


    const char* status[] = {"FOUND", "EMPTY", "INFINITE", "NONOPTIMAL"};


    matrix<T> q;
    row_table_create(a, b, q);
    artificial_basis_create(q, basis);

    output_aligned(cout << "Q =\n", q);
    cout << "basis = " << basis << "\n\n";
    row_to_col_table(q, basis);
    output_aligned(cout << "T =\n", q);
    cout << "nonbasis = " << basis << "\n";


    //cout << "\n\nresult = " <<
    //    status
    //    [
    //        primal_row_with_artificial_basis
    //        (
    //            a, b, c, bx, basis, res, rule_s_primal_first(), rule_r_primal_lex(),
    //            primal_row_with_artificial_basis_ctrl_slog<ostream>(cout)
    //        )
    //    ];
    //
    //cout << "\n*********************************************\n";
    //
    //cout << "\n\nresult = " <<
    //    status[primal_row_with_artificial_basis(a, b, c, bx, basis, res)];
}


void test3_14 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace simplex_method;
    // using Arageli::vector;

    typedef rational<> T;
    Arageli::vector<T> c;
    matrix<T> a;
    Arageli::vector<T> b;
    Arageli::vector<T> bx;
    //Arageli::vector<size_t> basis;
    T res;

    const char* status[] = {"FOUND", "EMPTY", "INFINITE", "NONOPTIMAL"};

    matrix<T> q =

        //"((0,  1,  1, 1, 1),"
        //" (0, -1, 0, 0, 0),"
        //" (0,  0,-1, 0, 0),"
        //" (0,  0, 0,-1, 0),"
        //" (0,  0, 0, 0,-1),"
        //" (-1,-1, 1,-2, 2),"
        //" (-2, 1,-2, 3,-4))"

        //"((0, -1, -1, 0, 0),"
        //" (6,  2,  1, 1, 0),"
        //" (6,  1,  2, 0, 1))"

        "((0,  1,  1,  1,  1,  0,  0),"
        " (1,  1, -1,  2, -2, -1,  0),"
        " (2, -1,  2, -3,  4,  0, -1))"
        ;

    //Arageli::vector<size_t> nonbasis = "(1, 2, 3, 4)";
    Arageli::vector<size_t> basis = "(3, 4)";

    cout << "The original table (row, primal):\n";
    output_aligned(cout, q);
    cout << "basis = " << basis;

    cout << "\n\nSolved by primal_row_iters, table is\n";
    matrix<T> q1 = q; Arageli::vector<size_t> basis1 = basis;
    primal_row_iters(q1, basis1);
    output_aligned(cout, q1);
    cout << "basis = " << basis1;

    cout << "\n\nCorresponding dual row table is\n";
    matrix<T> q2; Arageli::vector<size_t> basis2;
    primal_row_to_dual_row_table(q1, basis1, q2, basis2);
    output_aligned(cout, q2);
    cout << "basis = " << basis2;

    cout << "\n\nAfter (1, 2)-pivoting\n";
    matrix<T> q23 = q2; Arageli::vector<size_t> basis23 = basis2;
    gauss_field_row_iter(q23, 1, 1);
    gauss_field_row_iter(q23, 2, 2);
    output_aligned(cout, q23);
    cout << "basis = " << basis23;

    cout << "\n\n#######Corresponding dual column table is\n";
    matrix<T> q22; Arageli::vector<size_t> basis22;
    row_to_col_table(q2, basis2, q22, basis22);
    output_aligned(cout, q22);
    cout << "basis = " << basis22;

    cout << "\n\nDual dual row table is\n";
    matrix<T> q21; Arageli::vector<size_t> basis21;
    primal_row_to_dual_row_table(q2, basis2, q21, basis21);
    output_aligned(cout, q21);
    cout << "basis = " << basis21;

    cout << "\n\nDual row table by original table is\n";
    matrix<T> q3; Arageli::vector<size_t> basis3;
    primal_row_to_dual_row_table(q, basis, q3, basis3);
    output_aligned(cout, q3);
    cout << "basis = " << basis3;

    //cout << "\n\nSolved by primal_row_iters";

    cout << "\n\nCorresponding column table\n";
    row_to_col_table(q3, basis3);
    output_aligned(cout, q3);
    cout << "basis = " << basis3;

    cout << "\n\nSolved by dual_col_iters\n";
    dual_col_iters(q3, basis3);
    output_aligned(cout, q3);
    cout << "basis = " << basis3;


    //dual_col_iters
    //(
    //    q, nonbasis,
    //    rule_s_dual_first(),
    //    rule_r_dual_first(),
    //    dual_col_iters_ctrl_slog<std::ostream>(std::cout)
    //);
}



void test3_15 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace simplex_method;
    // using Arageli::vector;

    typedef rational<> T;

    matrix<T> table =
        //"(( 0, -3, -2),"
        //" ( 0, -1,  0),"
        //" ( 0,  0, -1),"
        //" (52, 13, -2),"
        //" (11,  1,  2),"
        //" (16, -1,  4))";
        "(( 5, 1, 1),"
        " ( 11/5, 3/5,  2/5),"
        " ( 14/5, 2/5,  3/5),"
        " ( 0,  0, -1),"
        " ( 0, -1, 0))";
    //vector<size_t> nonbasis = "(1, 2)";
    Arageli::vector<size_t> nonbasis = "(4, 3)";

    cout << "\n\nOriginal column table\n";
    output_aligned(cout, table);
    cout << "nonbasis = " << nonbasis;

    cout << "\n\nAfter primal_col_iters\n";
    primal_col_iters(table, nonbasis);
    output_aligned(cout, table);
    cout << "nonbasis = " << nonbasis;

    gomory1_iters(table, nonbasis);

    const char* status[] = {"FOUND", "EMPTY", "INFINITE", "NONOPTIMAL"};

}


void test3_16 ()
{
    //// using namespace Arageli;
    //// using namespace std;
    //using namespace simplex_method;
    //// using Arageli::vector;

    //typedef rational<> T;

    //matrix<T> table =
    //    //"(( 0, -3, -2),"
    //    //" ( 0, -1,  0),"
    //    //" ( 0,  0, -1),"
    //    //" (52, 13, -2),"
    //    //" (11,  1,  2),"
    //    //" (16, -1,  4))";
    //    "((0,  10,  3,  1, 0, 0, 0),"
    //    " (-3, -3, -1,  0, 1, 0, 0),"
    //    " (-5, -6, -2, -1, 0, 1, 0),"
    //    " (11, 11,  4,  2, 0, 0, 1))";
    ////vector<size_t> nonbasis = "(1, 2)";
    ////vector<size_t> basis = "(5, 2)";

    //
    //
    //cout << "\n\nOriginal column table\n";
    //output_aligned(cout, table);
    //cout << "basis = " << basis;

    //cout << "\n\nDual row table by original table is\n";
    //matrix<T> table2; vector<size_t> basis2;
    //primal_row_to_dual_row_table(table, basis, table2, basis2);
    //output_aligned(cout, table2);
    //cout << "basis = " << basis2;

    //cout << "\n\nDual dual row table by original table is\n";
    //matrix<T> table3; vector<size_t> basis3;
    //primal_row_to_dual_row_table(table2, basis2, table3, basis3);
    //output_aligned(cout, table3);
    //cout << "basis = " << basis3;

    //const char* status[] = {"FOUND", "EMPTY", "INFINITE", "NONOPTIMAL"};

}

// using namespace Arageli;
// using Arageli::vector;


template <typename T, bool REFCNT>
void rand_lpt (matrix<T, REFCNT>& a, Arageli::vector<T, REFCNT>& c, Arageli::vector<T, REFCNT>& b)
{
    size_t m = std::rand()%3 + 2;
    size_t n = m + 1 + std::rand()%3;

    a.assign(m, n);
    c.resize(n);
    b.resize(m);

    for(size_t i = 0; i < m; ++i)
    {
        for(size_t j = 0; j < n; ++j)
        {
            a(i, j) = std::rand()%100;
            c[j] = std::rand()%100;
        }

        b[i] = std::rand()%100;
    }
}

template <typename T, bool REFCNT>
void dual_canonical
(
    matrix<T, REFCNT>&  a, Arageli::vector<T, REFCNT>&  c, Arageli::vector<T, REFCNT>&  b,
    matrix<T, REFCNT>& da, Arageli::vector<T, REFCNT>& dc, Arageli::vector<T, REFCNT>& db
)
{
    da = a;
    da.transpose();
    dc = -b;
    db = c;

    ARAGELI_ASSERT_0(da.ncols() == dc.size());

    size_t n = da.ncols();
    for(size_t j = 0; j < n; ++j)
    {
        da.insert_col(n-j, null<T>());
        for(size_t i = 0; i < da.nrows(); ++i)
            da(i, n-j) = -da(i, n-j-1);
        dc.insert(n-j, -dc[n-j-1]);
    }

    ARAGELI_ASSERT_1(da.ncols() == dc.size());

    da.insert_cols(da.ncols(), da.nrows(), null<T>());
    dc.insert(dc.size(), da.nrows(), null<T>());
    for(size_t i = 0; i < da.nrows(); ++i)
        da(i, da.ncols() - da.nrows() + i) = -1;

    std::cout << "\n******************\n";
    output_aligned(std::cout << "original A\n", a);
    output_aligned(std::cout << "\n\ndual A\n", da);
    std::cout << "\n******************\n";


}


template <typename T1, bool REFCNT1, typename T2, bool REFCNT2>
Arageli::vector<T1, REFCNT1> my_subvector
(const Arageli::vector<T1, REFCNT1>& v, const Arageli::vector<T2, REFCNT2>& indexes)
{
    Arageli::vector<T1, REFCNT1> res(indexes.size());
    for(size_t i = 0; i < res.size(); ++i)
        res[i] = v[indexes[i]];

    return res;
}


template <typename T1, bool REFCNT1, typename T2, bool REFCNT2>
matrix<T1, REFCNT1> my_col_submatrix
(const matrix<T1, REFCNT1>& m, const Arageli::vector<T2, REFCNT2>& indexes)
{
    matrix<T1, REFCNT1> res(m.nrows(), indexes.size());
    for(size_t i = 0; i < indexes.size(); ++i)
    {
        for(size_t r = 0; r < res.nrows(); ++r)
            res(r, i) = m(r, indexes[i]);
    }

    return res;
}


void test3_17 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace simplex_method;
    // using Arageli::vector;

    typedef rational<> T;

    matrix<T> table, dtable, a, da;
    Arageli::vector<T> b, db, c, dc, basis_x, dbasis_x;
    Arageli::vector<size_t> basis, dbasis;
    T res, dres;

    for(size_t i = 0; i < 10; ++i)
    {
        rand_lpt(a, c, b);
        output_aligned(cout << "\n\nA =\n", a);
        cout << "\nc = " << c << "\nb = " << b << '\n';
        if
        (
            rk_found != primal_row_with_artificial_basis
            (
                a, b, c, basis_x, basis, res/*,
                rule_s_primal_first(), rule_r_primal_lex(),
                primal_row_with_artificial_basis_ctrl_slog<std::ostream>(cout)*/
            )
        )
        {
            cout << "\nnot rk_found";
            continue;
        }

        cout << "\nbasis_x = " << basis_x;
        cout << "\nbasis = " << basis << "\n";
        dual_canonical(a, c, b, da, dc, db);
        cout << "\nDual canonical\n";
        primal_row_with_artificial_basis(da, db, dc, dbasis_x, dbasis, dres);
        cout << "\ndbasis_x = " << dbasis_x;
        cout << "\ndbasis = " << dbasis;

        Arageli::vector<T> u = my_subvector(c, basis - 1)*inverse(my_col_submatrix(a, basis - 1));
        cout << "\nu = " << u << '\n';
    }
}


void test3_18 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace simplex_method;
    // using Arageli::vector;

    typedef rational<> T;

    matrix<T> da, a =
        "((-3, -1,  0, 1, 0, 0),"
        " (-6, -2, -1, 0, 1, 0),"
        " (11,  4,  2, 0, 0, 1))";

    Arageli::vector<T> db, b = "(-3, -5, 11)";
    Arageli::vector<T> dc, c = "(-10, -3, -1, 0, 0, 0)";

    output_aligned(std::cout << "\nA original:\n", a);
    std::cout << "\nb original: " << b;
    std::cout << "\nc original: " << c;

    matrix<T> q, t;
    Arageli::vector<size_t> basis, nonbasis;

    // The first variant. Basis is known, dual column simplex method:
    row_table_create(a, b, c, q);

    output_aligned(std::cout << "\n\nq:\n", q);

    basis = "(4, 5, 6)";
    row_to_col_table(q, basis, t, nonbasis);

    output_aligned(std::cout << "\n\nt:\n", t);
    std::cout << "\nnonbasis: " << nonbasis;

    dual_col_iters(t, nonbasis);

    output_aligned(std::cout << "\n\nt after dual_col_iters:\n", t);
    std::cout << "\nnonbasis after dual_col_iters: " << nonbasis;

    primal_to_dual_canonical(a, b, c, da, db, dc, basis);
    row_table_create(da, db, dc, q);

    output_aligned(std::cout << "\nA dual:\n", da);
    std::cout << "\nb dual: " << db;
    std::cout << "\nc dual: " << dc;

    output_aligned(std::cout << "\n\nq dual:\n", q);
    std::cout << "\nbasis dual: " << basis;

    primal_row_iters(q, basis);

    output_aligned(std::cout << "\n\nq dual after primal_row_iters:\n", q);
    std::cout << "\nbasis dual after primal_row_iters: " << basis;
}


void test3_19 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace simplex_method;
    // using Arageli::vector;

    typedef rational<> T;

    //matrix<T> da, a =
    //    "((-3, -1,  0, 1, 0, 0),"
    //    " (-6, -2, -1, 0, 1, 0),"
    //    " (11,  4,  2, 0, 0, 1))";

    //Arageli::vector<T> db, b = "(-3, -5, 11)";
    //Arageli::vector<T> dc, c = "(-10, -3, -1, 0, 0, 0)";

    matrix<T> da, bsuba, a =
        "((2,  1,  0,  1),"
        " (1,  2,  1,  0))";

    Arageli::vector<T> db, b = "(6, 6)";
    Arageli::vector<T> dc, bsubc, c = "(0, 2, -1, 1)";
    Arageli::vector<size_t> basis = "(4, 3)";

    output_aligned(std::cout << "\nA original:\n", a);
    std::cout << "\nb original: " << b;
    std::cout << "\nc original: " << c;

    //primal_to_dual_canonical_discr(a, b, c, basis, da, db, dc, bsuba, bsubc);

    output_aligned(std::cout << "\nA original:\n", da);
    std::cout << "\nb original: " << db;
    std::cout << "\nc original: " << dc;

    output_aligned(std::cout << "\nbsuba:\n", bsuba);
    std::cout << "\nbsubc = " << bsubc;
}


void test3_20 ()
{
    // using namespace Arageli;
    // using namespace std;
    using namespace simplex_method;
    // using Arageli::vector;

    typedef rational<> T;

    //matrix<T> da, a =
    //    "((-3, -1,  0, 1, 0, 0),"
    //    " (-6, -2, -1, 0, 1, 0),"
    //    " (11,  4,  2, 0, 0, 1))";

    //Arageli::vector<T> db, b = "(-3, -5, 11)";
    //Arageli::vector<T> dc, c = "(-10, -3, -1, 0, 0, 0)";

    ifstream tasks("tasks.txt");
    matrix<T> a, t, q, bsuba, da;
    Arageli::vector<T> b, c, db, dc, bsubc;
    Arageli::vector<size_t> basis, nonbasis;

    for(;;)
    {
        string stask;
        getline(tasks, stask);
        if(!tasks && stask.empty())break;
        istringstream buftask(stask);

        buftask >> a >> b >> c;

        std::cout << "****************  Task  *******************\n";
        output_aligned(std::cout << "\nA original:\n", a);
        std::cout << "\nb original: " << b;
        std::cout << "\nc original: " << c;

        row_table_create(a, b, q);
        ARAGELI_ASSERT_1(is_null(q(0, 0)));

        basis_artificial(q, basis);
        output_aligned(std::cout << "\n\nArtificial q:\n", q);
        std::cout << "\nbasis: " << basis;
        //row_table_split(a, b, q);

        output_aligned(std::cout << "\nA with basis:\n", a);
        std::cout << "\nb with basis: " << b;
        std::cout << "\nc with basis: " << c;

        T res_offset;
        primal_to_dual_standard_discr(a, b, c, basis, da, db, dc, res_offset, bsuba, bsubc);

        output_aligned(std::cout << "\n\nBasis submatrix after artificial:\n", bsuba);
        std::cout << "\nbasis subvector c after aftificial: " << bsubc;

        output_aligned(std::cout << "\n\nA dual:\n", da);
        std::cout << "\nb dual: " << db;
        std::cout << "\nc dual: " << dc;

        for(size_t i = 0; i < c.size(); ++i)
            q(0, i+1) = -c[i];

        for(size_t i = 0; i < basis.size(); ++i)
            q.addmult_rows(0, i+1, -q(0, basis[i]));

        output_aligned(std::cout << "\n\nThe first allowable q:\n", q);
        primal_row_iters(q, basis);
        output_aligned(std::cout << "\n\nOptimal q:\n", q);
        std::cout << "\noptimal basis: " << basis;

        row_to_col_table(q, basis, t, nonbasis);
        output_aligned(std::cout << "\n\nDirect column table:\n", t);
        std::cout << "\nnonbasis: " << nonbasis;

        gomory1_iters(t, nonbasis);

        output_aligned(std::cout << "\n\nInteger column table:\n", t);
        std::cout << "\nnonbasis: " << nonbasis;

        Arageli::vector<T> x(a.ncols());
        for(size_t i = 0; i < a.nrows(); ++i)
            x[basis[i]-1] = q(i+1, 0);

        std::cout << "\nx: " << x;

        col_table_create_by_standard(da, db, dc, t, nonbasis);
        t(0, 0) += res_offset;

        output_aligned(std::cout << "\n\nDual column table:\n", t);
        std::cout << "\nnonbasis: " << nonbasis;

        if(is_dual_allow(t))
        {
            dual_col_iters(t, nonbasis);

            output_aligned(std::cout << "\n\nOptimal dual column table:\n", t);
            std::cout << "\nnonbasis: " << nonbasis;

            Arageli::vector<T> y(da.ncols());
            for(size_t i = 0; i < y.size(); ++i)
                y[i] = t(i+1, 0);

            std::cout << "\ny: " << y;
            //std::cout << "\nbsubc: " << bsubc;
            //std::cout << "\ninverse(bsuba): " << inverse(bsuba);
            Arageli::vector<T> u = (y + bsubc)*inverse(bsuba);
            std::cout << "\nu: " << u;

            gomory1_iters(t, nonbasis);
            for(size_t i = 0; i < y.size(); ++i)
                y[i] = t(i+1, 0);

            output_aligned(std::cout << "\n\nInteger dual column table:\n", t);
            std::cout << "\nnonbasis: " << nonbasis;
            std::cout << "\ny: " << y;
            u = (y + bsubc)*inverse(bsuba);
            std::cout << "\nu: " << u;
        }
        else
        {
            std::cout << "\nDual table is not dual allowable!";
        }
    }


    //col_table_create_by_standard(a, b, c, t, basis);

    //output_aligned(std::cout << "\nt:\n", t);

    //Arageli::vector<size_t> nonbasis;
    //basis_to_nonbasis(basis, nonbasis, t.nrows()-1);
    //dual_col_iters(t, nonbasis);

    //output_aligned(std::cout << "\nt optimal:\n", t);
    //std::cout << "\nnonbasis = " << nonbasis;
}


void test3_21 ()
{
    //std::cout << is_prime(big_int(next_prime(big_int("10000"))));
    std::cout << is_prime_solovey_strassen(big_int("82746817691284628867"), 10);

    //std::cout << "\n" << jacobi(91, 1291) << "\n" << jacobi(big_int(91), big_int(1291));
}


void test3_22 ()
{
    Arageli::vector<size_t> m(11);

    for(int i = 0; i < 100; ++i)
        m[big_int::random_in_range(big_int("10"))]++;

    for(size_t i = 0; i < m.size(); ++i)
        std::cout << "\ni = " << i << ", n = " << m[i];
}


void test3_24 ()
{
    // using namespace std;
    // using namespace Arageli;
    // using Arageli::vector;
    using namespace ctrl;

    typedef rational<> T;

    matrix<T>
        a = "((5, -2, -3, 5, 0, 23, 234, 2, 24, -2), "
        "(5, -2, -3, 5, 0, 3, 234, 2, 24, -2), "
        "(5, 2, -3, 8, 0, 3, 2, 2, 24, -2), "
        "(5, -2, -3, 5, 0, 23, 234, 2, 24, -2), "
        "(-5, -2, 234, 6, 2, 23, 4, 12, 44, 2), "
        "(5, -2, -3, 4, 0, 19, 234, 2, 24, 10))",
        b, q;
    Arageli::vector<size_t> basis;
    T det;

    ofstream file;

    file.open("rref.output.with_additionals.txt");
    rref(a, b, q, basis, det, rref_slog<ostream>(file));
    file.close();

    file.open("rref.output.without_additionals.txt");
    rref(a, b, q, basis, det, rref_slog<ostream>(file, false, false));
    file.close();

    file.open("rref.output.with_additionals.tex");
    rref(a, b, q, basis, det, rref_latexlog<ostream>(file));
    file.close();

    file.open("rref.output.without_additionals.tex");
    rref(a, b, q, basis, det, rref_latexlog<ostream>(file, false, false));
    file.close();
}


void test3_25 ()
{
    // using namespace std;
    // using namespace Arageli;
    // using Arageli::vector;
    Arageli::vector<int> a,b;
    a = (4, 6, 16, 8);
    b = (2, 3, 8, 6);
    std::cout<<"\nGCD for a and b = " << gcd(a,b);
    std::cout<<"\nLCM for a and b = " << lcm(a,b);


}

// using namespace std;

template <typename T, bool REFCNT>
void print_rref (const matrix<T, REFCNT>& a)
{
    matrix<T, false> b, q;
    Arageli::vector<size_t, false> basis;
    T det;

    rref(a, b, q, basis, det, ctrl::rref_slog<ostream>(cout));

    output_aligned(cout, b);
}


void test3_26 ()
{
    // using namespace std;
    // using namespace Arageli;
    // using Arageli::vector;
    using namespace Arageli::simplex_method;

    typedef rational<> T;

    matrix<T>
        //q = "((0, 0,  -3, 0, 0, -23, -234, 0, 24, 0), "
        //    "(5,  0,  -3, 0, 0,   3,  234, 1, 24, 0), "
        //    "(1,  0,  -3, 0, 0,   3,    2, 0, 31, 1), "
        //    "(3,  0,  -3, 1, 0,  23,  234, 0, 24, 0), "
        //    "(2,  0, 234, 0, 1,  23,    4, 0, 44, 0), "
        //    "(4,  1,  -3, 0, 0,  19,  234, 0, 24, 0))",
        q = "((0, 1,  1,  0,  5), "
            "( 1, 1,  1, -1,  3), "
            "( 1, 1, -2,  3, -1), "
            "( 5, 5, -4,  7,  3))",
        qtemp = q;

    Arageli::vector<size_t> basis;
    basis_artificial(q, basis, ctrl::simplex_method::basis_artificial_slog<ostream>(cout, true, true));

    cout << "\n*****************************\n";

    //q = qtemp;
    //basis = "(7, 9, 3, 4, 1)";
    Arageli::vector<T> c;
    row_table_extract_c(c, qtemp);
    row_table_place_c(c, q);
    row_table_pivot_basis_c(q, basis);

    primal_row_iters(q, basis, ctrl::simplex_method::primal_row_iters_slog<ostream>(cout, true, true));

}

void test3_27 ()
{
    // using namespace std;
    // using namespace Arageli;

    float a = 2;
    float b = a + rational<>(12);
    float c = rational<>(3);

    cout << a << ' ' << b << ' ' << c;
}

void test3_28 ()
{
    // using namespace std;
    // using namespace Arageli;

    monom<rational<> > m = "-2/5*x^17";
    cout << m;
}


void test3_29 ()
{
    // using namespace std;
    // using namespace Arageli;

    sparse_polynom<double> a =
        "1.12345*x^3-1e45*x^2+1234.5678*x-0.000002334";
    sparse_polynom<big_int> b = a;

    copy
    (
        b.coefs_begin(), b.coefs_end(),
        ostream_iterator<sparse_polynom<big_int>::coef_type>(cout, "\n")
    );

    cout << "\n\n" << b;

}

void test3_30 ()
{
    typedef sparse_polynom<big_int> poly;

    poly S = "213*x^3443+532*x^4432-744*x^44-4235*x^15+292*x-34254";
    poly::coef_const_iterator ci = S.coefs_begin();//error
    poly::degree_const_iterator di = S.degrees_begin();//error
    //poly::coef_iterator i = ci;
}


void test3_31 ()
{

    sparse_polynom<rational<> > S = "2/55*x^255+567";
    cout << "S(0) = " << S.subs(0) << endl; // output: S(0) = 1
    cout << "S(0) = " << S.subs(rational<>(0)) << endl; // output: S(0) = 567
    cout << rational<>("1/3") + big_int("1") << endl;
    cout << rational<>("1/3") + __int64(1) << endl;
}


void test3_32 ()
{
    int j;
    rational<> *y;
    rational<> tmpDenom;
    sparse_polynom<rational<> > poly, tmpPolyNumer(1);
    poly += (y[j]/tmpDenom) * tmpPolyNumer;// - НЕ компилируется !!!
}


void test3_33 ()
{
    sparse_polynom<rational<> > S = "x";
    sparse_polynom<rational<> > T = S * rational<>(0);//error

    cout << S << '\n' << T;
}


void test3_34 ()
{
    matrix<rational<> > v = "((1, 2, 3/4))";
    ofstream file("test3_34.output.txt");
    file << v;
}


void test3_35 ()
{
    sparse_polynom<rational<> > P,Q,U,V, pq;
    pq = euclid_bezout(P,Q,U,V);    //error
}


void test3_36 ()
{
    sparse_polynom<rational<> > P;
    cout << inverse(P) << endl;
}


void test3_37 ()
{
    sparse_polynom<int> S = "2*x^2+5*x-7";
    output_aligned(cout,S);//error
}


void test3_38 ()
{
    sparse_polynom<int> q = "x^2+1", w = "x-1";
    sparse_polynom<int> qw = gcd(q,w);//здесь мы «висим»
    cout << qw;
}


void test3_39 ()
{
    for(big_int i = 1; i < 100; ++i)
        cout << factorize_division(i) << '\n';
    cout << '\n';
    for(int i = 1; i < 100; ++i)
        cout << factorize_division(i) << '\n';
}


void test3_40 ()
{
    matrix<rational<> > A(3, unit<rational<> >(), fromval);
    output_aligned(cout, A);
}


void test3_41 ()
{
    Arageli::vector<rational<> > v = "(2, 3, 4, 5, 6, 7, 8)";
    Arageli::vector<short> i = "(4, 2, 5, 2)";

    cout
        << "\nv = " << v
        << "\ni = " << i;
    cout
        << "\nv.take_subvector(i) = " << v.take_subvector(i);
    cout
        << "\nv after taking = " << v;
}


void test3_42 ()
{
    {
        matrix<int, true> a = "((-3, 5, 0), (1, 3, -3), (0, -3, 4))", b, p, q;
        size_t rank; int det;
        ofstream file("smith_b.output.txt");

        Timing tm;
        for(int i = 0; i < 100000; ++i)
            smith(a, b, p, q, rank, det/*, Arageli::ctrl::smith_slog<ofstream>(file)*/);
        std::cout << "\nTime for Smith (refcounter is on): " << tm.time() << "\nMatrixes are:\n";

        output_aligned_corner_triplet_br(cout, b, p, q);
    }
    {
        matrix<int, false> a = "((-3, 5, 0), (1, 3, -3), (0, -3, 4))", b, p, q;
        size_t rank; int det;
        ofstream file("smith_b.output.txt");

        Timing tm;
        for(int i = 0; i < 100000; ++i)
            smith(a, b, p, q, rank, det/*, Arageli::ctrl::smith_slog<ofstream>(file)*/);
        std::cout << "\nTime for Smith (refcounter is off): " << tm.time() << "\nMatrixes are:\n";

        output_aligned_corner_triplet_br(cout, b, p, q);
    }
}


void test3_43 ()
{
    Arageli::vector<int, true> rs = "(1, 1, 3)", cs = "(0, 2)";
    matrix<int> m = "((1, 2, 3, 4), (4, 5, 6, 7), (7, 8, 9, 10), (11, 12, 13, 14))";
    output_aligned(cout << "\nM =\n", m);
    output_aligned(cout << "\nM.take_submatrix(rs, cs) =\n", m.take_submatrix(rs, cs));
    output_aligned(cout << "\nM after the operation:\n", m);
}


void test3_44 ()
{
    matrix<int>
        a1(2, 3, eye),
        a2(3, 2, eye),
        a3(3, 3, eye);

    output_aligned(cout << "\na1 = \n", a1);
    output_aligned(cout << "\na2 = \n", a2);
    output_aligned(cout << "\na3 = \n", a3);
    output_aligned(cout << "\ntranspose(a1) = \n", transpose(a1));
    output_aligned(cout << "\ntranspose(a2) = \n", transpose(a2));
    output_aligned(cout << "\ntranspose(a3) = \n", transpose(a3));
}


void test3_45 ()
{
    sparse_polynom<rational<> > sp = "x^5+2*x^4-5*x^3+8*x^2-7*x-3";
    cout << root_upper_bound_cauchy<float>(sp);
}


void test3_46 ()
{
    {
        sparse_polynom<rational<> > sp = "x^5+2*x^4-5*x^3+8*x^2-7*x-3";
        Arageli::vector<interval<rational<> > > lims;
        sturm<rational<> >(sp, lims);
    }
    {
        sparse_polynom<rational<> > sp = "x^3+3*x^2-1";
        sp *= sparse_polynom<rational<> >("x-2");
        Arageli::vector<interval<rational<> > > lims;
        sturm<rational<> >(sp, lims);
    }
}


void test3_47 ()
{
    typedef rational<> T;
    const size_t num_roots = 4;
    const T e(1, 10);

    for(int i = 0; i < 100; ++i)
    {
        Arageli::vector<T, false> exact_roots(num_roots);
        std::generate(exact_roots.begin(), exact_roots.end(), std::rand);
        std::sort(exact_roots);
        output_aligned(std::cout << "\nExact roots:\n", exact_roots);

        typedef sparse_polynom<T, big_int, false> Poly;
        Poly prime_poly(T(1), 1);
        prime_poly.insert(prime_poly.monoms_begin(), Poly::monom());    // placeholder
        Poly p = unit<Poly>();

        for(size_t i = 0; i < num_roots; ++i)
        {
            if(is_null(exact_roots[i]))p *= Poly::monom(1, 1);
            else
            {
                *prime_poly.monoms_begin() = -exact_roots[i];
                //std::cout << "\nprime_poly = " << prime_poly;
                p *= prime_poly;
                //std::cout << "\np = " << p;
            }
        }

        std::cout << "\np = " << p;

        Arageli::vector<T, false> roots, prec;
        roots_poly_real(p, roots, prec, e);

        output_aligned(std::cout << "\nComputed roots:\n", roots);
        output_aligned(std::cout << "\nevalf(Computed roots):\n", Arageli::vector<double>(roots));
        output_aligned(std::cout << "\nPrecisions:\n", prec);
        prec = std::abs(roots - exact_roots);
        output_aligned(std::cout << "\nExact precisions:\n", prec);
        std::cout
            << "\nIt's OK: " << std::boolalpha
            << all_less(prec, e) << std::noboolalpha;
    }
}


void test3_48 ()
{
    matrix<rational<> > H, B =
        "(( 1, 0, 0, 0, 0, 0, -366 ),"
        "( 0, 1, 0, 0, 0, 0, -385 ),"
        "( 0, 0, 1, 0, 0, 0, -392 ),"
        "( 0, 0, 0, 1, 0, 0, -401 ),"
        "( 0, 0, 0, 0, 1, 0, -422 ),"
        "( 0, 0, 0, 0, 0, 1, -437 ),"
        "( 0, 0, 0, 0, 0, 0, 1215 ))";

    output_aligned(cout << "B =\n", B);
    lll_reduction(B, H);
    output_aligned(cout << "\noutput B =\n", B);
    output_aligned(cout << "\noutput H =\n", H);
}


void test3_49 ()
{
    matrix<rational<> > a = "((11, 12, 13, 14), (21, 22, 23, 24), (31, 32, 33, 34))";
    output_aligned(cout << "\n", a);
    a.swap_cols(1, 2);
    output_aligned(cout << "\n", a);
    a.swap_cols(0, 3);
    output_aligned(cout << "\n", a);
    a.assign_col(1, a.copy_col(2)*3);
    output_aligned(cout << "\n", a);
}

void fff1 (const Arageli::vector<int>& x)
{
    for(int i = 0; i < 3; ++i)
        std::cout << x[i];
}

void fff2 (const Arageli::vector<int> x)
{
    for(int i = 0; i < 3; ++i)
        std::cout << x[i];
}

void fff3 (const Arageli::vector<int> xx)
{
    Arageli::vector<int>::const_iterator x = xx.begin();
    for(int i = 0; i < 3; ++i)
        std::cout << x[i];
}

void test3_50 ()
{
    Arageli::vector<int> x(3, 555);
    fff1(x);
    fff2(x);
    fff3(x);
}


void test3_51 ()
{
    typedef sparse_polynom<rational<> > Poly;

    Poly a, b;

    a = oldrnd<Poly>::rand(); b = oldrnd<Poly>::rand();

    for(int i = 0; i < 0; ++i)
        a *= oldrnd<Poly>::rand(), b *= oldrnd<Poly>::rand();

    std::cout << a << "\n\n" << b;

    Poly c;

    for(int i = 0; i < 30; ++i)
        c = gcd(a, b);

    std::cout << "\n\n" << c;
}


void test3_52 ()
{
    matrix<int> a(2, 3, fromval);
    matrix<int> b(2, 3, 4);
    matrix<int> c(2, 3, fromsize);


    output_latex(std::cout << "\n", a, true);
    output_latex(std::cout << "\n", b, true);
    output_latex(std::cout << "\n", c, true);

    Arageli::vector<big_int> v = "(1, -1, 2, -2)";
    matrix<rational<> > x = "((1), (1/2), (1/3), (1/4))";
    matrix<rational<> > y = "((1, 2, 3, 4))";
    matrix<sparse_polynom<int> > A = "((1, x-2), (x^3-17, 2*x^5+1))";

    output_latex(std::cout << "\n\n", v, true, eep_alone, false);
    output_latex(std::cout << "\n", x, true);
    output_latex(std::cout << "\n", y, true);
    output_latex(std::cout << "\n", A, true);
}


void test3_53 ()
{
    Arageli::vector<int> x(3);
    matrix<int> a(2), b(2, 3);
    matrix<int> c(2, size_t(3));

    output_latex(std::cout << "\n", x, true);
    output_latex(std::cout << "\n", a, true);
    output_latex(std::cout << "\n", b, true);
    output_aligned(std::cout << "\n", c);
}


void test3_54 ()
{
    int array[] = {1, 2, 4, 8, 16, 32};

    Arageli::vector<big_int> x1(2, &array[0]);
    Arageli::vector<big_int> x2(2, &array[0], fromseq);

    if(x1 != x2)
    {
        std::cerr << "x\n" << x1 << ",\n" << x2 << "\n\n";
    }

    Arageli::vector<int> y1(2, rational<>(7));
    Arageli::vector<int> y2(2, rational<>(7), fromval);

    if(y1 != y2)
    {
        std::cerr << "y\n" << y1 << ",\n" << y2 << "\n\n";
    }

    matrix<int> a1(2, &array[0]);
    matrix<int> a2(2, &array[0], fromseq);

    if(a1 != a2)
    {
        std::cerr << "a\n" << a1 << ",\n" << a2 << "\n\n";
    }

    matrix<rational<int> > b1(2, 9);
    matrix<rational<int> > b2(2, 9, fromval);

    if(b1 != b2)
    {
        std::cerr << "b\n" << b1 << ",\n" << b2 << "\n\n";
    }

    matrix<rational<big_int> > c1(3, 2, &array[0]);
    matrix<rational<big_int> > c2(3, 2, &array[0], fromseq);

    if(c1 != c2)
    {
        std::cerr << "c\n" << c1 << ",\n" << c2 << "\n\n";
    }

    matrix<rational<short> > d1(3, 2, rational<int>("1/7"));
    matrix<rational<short> > d2(3, 2, rational<int>("1/7"), fromval);

    if(d1 != d2)
    {
        std::cerr << "d\n" << d1 << ",\n" << d2 << "\n\n";
    }

    output_latex(std::cout << "\nx = ", x1, true); std::cout << ", \\quad";
    output_latex(std::cout << "\ny = ", y1, true); std::cout << ", \\quad";
    output_latex(std::cout << "\na = ", a1, true); std::cout << ", \\quad";
    output_latex(std::cout << "\nb = ", b1, true); std::cout << ", \\quad";
    output_latex(std::cout << "\nc = ", c1, true); std::cout << ", \\quad";
    output_latex(std::cout << "\nd = ", d1, true);
}


void test3_55 ()
{
    sparse_polynom<big_int> a = "36*x^10+15*x^2-2*x^+6";
    std::cout << a;
    sparse_polynom_reduction_mod(a, 6);
    std::cout << "reduction modulo 6 = " << a;
}


void test3_56 ()
{
rational<int> a(4,44);
rational<big_int> b(11,8);
a*b;
Arageli::vector<int> vi(10);

cout<<vi;

}


void test3_57 ()
{
    sparse_polynom<big_int> p1 = "x-1", p2 = "x^23-1",
        p3 = power_mod(p1, big_int(23), p2);
    std::cout << p3;
    sparse_polynom_reduction_mod(p3, big_int(23));
    std::cout << "\n\n" << p3;
}


void test3_58 ()
{
    matrix<int> A("((1,2),(3,4))");
    A.each_inverse();

    std::cout << "A = \n";
    output_aligned(std::cout, A,"|| "," ||"," ");
}

#endif

int test3 ()
{
    //test3_2();
    //std::cout << "\n\n ++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
    //test3_3();
    //test3_4();
    //test3_6();
    //test3_48();
    //print_rref(matrix<rational<> >("((1, 2, 3), (4, 5, 6), (7, 8, 9))"));

    return 0;
}
