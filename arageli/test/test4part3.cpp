/*****************************************************************************

    test/test4part3.cpp

    This file is a part of the Arageli library.

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

#include "stdafx.hpp"

#pragma warning (disable : 4503)


using namespace Arageli;
using namespace Arageli::simplex_method;
using namespace Arageli::ctrl;
using namespace Arageli::ctrl::simplex_method;
using Arageli::vector;

using std::ostream;
using std::cout;
using std::cin;
using std::ofstream;
using std::ifstream;
using std::cerr;
using std::endl;
using std::string;


template <typename A>
void norm_first_col (A& a)
{
    for(size_t i = 0; i < a.nrows(); ++i)
        a.div_row(i, safe_reference(a(i, 0)));
}


void test4_9 ()
{
    typedef big_int T;
    matrix<T> a = "((18, 0, 0), (0, 18, 0), (0, 0, 18), (37, 7, 6), (40, 1, -21), (41, -1, 12))",
        f, q, e;
    skeleton(a, f, q, e, ctrl::make_skeleton_slog(cout, false));
    matrix<rational<> > qq;

    qq.insert_col(0, q.copy_col(3));
    qq.insert_col(1, q.copy_col(1));
    qq.insert_col(2, q.copy_col(4));
    qq.insert_col(3, q.copy_col(5));
    qq.insert_col(4, q.copy_col(2));

    qq.insert_col(0, q.copy_col(0));

    norm_first_col(qq);
    output_aligned(cout, qq);
}


void test4_10 ()
{
    cout << "\n\n ******* Integer Ring ******** \n\n";

    {
        typedef int T;

        matrix<T> a = "((1, 2, 3), (4, 5, 6), (7, 8, 9))", b, q;
        vector<size_t> basis;
        T det;

        rref(a, b, q, basis, det);

        output_aligned(cout << "a =\n", a);
        output_aligned(cout << "\nb =\n", b);
        output_aligned(cout << "\nq =\n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        cout << "\nis it valid RREF?: " << (b == q * a);
    }

    cout << "\n\n ******* Rational Field ******** \n\n";

    {
        typedef rational<int> T;

        matrix<T> a = "((1, 2, 3), (4, 5, 6), (7, 8, 9))", b, q;
        vector<size_t> basis;
        T det;

        rref(a, b, q, basis, det);

        output_aligned(cout << "a =\n", a);
        output_aligned(cout << "\nb =\n", b);
        output_aligned(cout << "\nq =\n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        cout << "\nis it valid RREF?: " << (b == q * a);
    }

    cout << "\n\n ******* Finite Field ******** \n\n";

    {
        typedef residue<int> T;

        matrix<T> a = "((1, 2, 3), (4, 5, 6), (7, 8, 9))", b, q;

        for(matrix<T>::iterator i = a.begin(); i < a.end(); ++i)
        {
            i->module() = (1 << next_mersen_prime_degree(10)) - 1;//11;
            i->normalize();
        }

        vector<size_t> basis;
        T det;

        rref(a, b, q, basis, det);

        output_aligned(cout << "a =\n", a);
        output_aligned(cout << "\nb =\n", b);
        output_aligned(cout << "\nq =\n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        cout << "\nis it valid RREF?: " << (b == q * a);
    }

    cout << "\n\n *********** double *********** \n\n";

    {
        typedef double T;

        matrix<T> a = "((1, 2, 3), (4, 5, 6), (7, 8, 9))", b, q;

        vector<size_t> basis;
        T det;

        rref(a, b, q, basis, det);

        output_aligned(cout << "a =\n", a);
        output_aligned(cout << "\nb =\n", b);
        output_aligned(cout << "\nq =\n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        cout << "\nis it valid RREF?: " << (b == q * a);
    }

    cout << "\n\n *********** big_float *********** \n\n";

    {
        //big_float::set_global_precision(100);
        //big_float::set_round_mode(TO_NEAREST);

        cout.setf(std::ios::scientific, std::ios::floatfield);

        typedef big_float T;

        matrix<T> a = "((1, 2, 3), (4, 5, 6), (7, 8, 9))", b, q;

        vector<size_t> basis;
        T det;

        rref(a, b, q, basis, det);

        output_aligned(cout << "a =\n", a);
        output_aligned(cout << "\nb =\n", b);
        output_aligned(cout << "\nq =\n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        cout << "\nis it valid RREF?: " << (b == q * a);
    }
}


void test4_11 ()
{
    vector<rational<> > a = "(1, 1/2, 1/3, 1/4,    1/5)";
    vector<int> index = "(1, 2, 3, 3, 0, 1)";
    output_list(cout << '\n', a);
    output_list(cout << '\n', index);
    output_list(cout << '\n', a[index]);

    index = "(0, 2)";
    a[index] = a[index + 1];

    output_list(cout << "\n\n", a);

    //output_list(cout << '\n', a[index][index]);
    //output_list(cout << '\n', a[index][index][index]);

    //a[index][2] = 100;

    //output_list(cout << "\n\n", a);
    //output_list(cout << "\n\n", a[index]);
    //output_list(cout << '\n', a[index][index]);
    //output_list(cout << '\n', a[index][index][index]);

}


void test4_12 ()
{
    typedef residue<int> T;
    typedef matrix<T, false> M;

    M qi =
        "((1,  0,  0,  0,  0,  0,  0,  0),"
        " (2,  1,  7, 11, 10, 12,  5, 11),"
        " (3,  6,  4,  3,  0,  4,  7,  2),"
        " (4,  3,  6,  5,  1,  6,  2,  3),"
        " (2, 11,  8,  8,  3,  1,  3, 11),"
        " (6, 11,  8,  6,  2,  7, 10,  9),"
        " (5, 11,  7, 10,  0, 11,  7, 12),"
        " (3,  3, 12,  5,  0, 11,  9, 12))";

    for(M::iterator i = qi.begin(); i < qi.end(); ++i)
    {
        i->module() = 13;
        i->normalize();
    }

    qi -= M(qi.nrows(), eye);

    output_aligned(cout << "Q - I=\n", qi);

    M rreftqi, invqi;
    vector<int> basis;

    rref(transpose(qi), rreftqi, invqi, basis);
    rreftqi.erase_rows(vector<int>("(5, 6, 7)"));

    output_aligned(cout << "\nRREF((Q - I)^T) =\n", rreftqi);

    M bm = rreftqi.take_cols(basis);
    rreftqi.opposite();

    output_aligned(cout << "\nbasis matrix =\n", bm);
    output_aligned(cout << "\nnonbasis matrix =\n", rreftqi);


    for(size_t i = 0; i < rreftqi.ncols(); ++i)
    {
        vector<T> x(rreftqi.ncols());
        x[i] = 1;
        cout << "\nxbasis = " << x;
        cout << "\nxnonbasis = " << rreftqi*x;
    }
}


void test4_13 ()
{
    cout
        << "\n" << vector<int>("()")
        << "\n" << vector<int>("(1)")
        << "\n" << vector<int>("(1, 2, 3)")
        << "\n" << vector<int>("(1:5, 100)")
        << "\n" << vector<int>("(200, 5:-2)")
        << "\n" << vector<int>("(100, 0:5:25, 200)")
        << "\n" << vector<sparse_polynom<int> >("(0, x : x^2 : 3*x^2+x, 1)")
        << "\n" << vector<sparse_polynom<int> >("(x:x^2:3*x^2+x)")
        << "\n" << vector<sparse_polynom<int> >("(x-2:x+3)")
        << "\n" << vector<sparse_polynom<int> >("(x : x)");
    cout << "\n";

    vector<rational<> > v;
    cout << "\n" << v;
    v.insert(0, "(3, 2, 1)");
    cout << "\n" << v;
    v.insert(1, vector<rational<> >("(4/5:-1/5:1/5)") + 2);
    cout << "\n" << v;
    v.insert(v.size(), 3, rational<big_int>(-1));
    cout << "\n" << v;
}


void test4_14 ()
{
    //big_float::set_global_precision(100);
    //big_float::set_round_mode(TO_NEAREST);

    big_float
        a = "1.1234567890987654321",
        b = "82736418265418217823000000.0000000000000000000000000000000001111111";

    cout.setf ( std::ios::scientific, std::ios::floatfield );
    cout << "\na = " << a << "\nb = " << b << "\na + b = " << a + b;

    //cout << big_int(std::numeric_limits<__int64>::min());
}


void test4_15 ()
{
    matrix<int> m(32, 3200), b, p, q;
    std::size_t rank;
    int det;

    m(0, 0) = 345;
    m(0, 1) = -23;
    m(1, 0) = 77;
    m(1, 199) = 31;
    m(29, 310) = 11;

    smith(m, b, p, q, rank, det);

    output_aligned(cout, b.copy_rows(vector<int>("(1, 2, 3, 4, 5)")-1).copy_cols(vector<int>("(1, 2, 3, 4, 5)")-1));
}


void test4_16 ()
{
    typedef sparse_polynom<big_int, big_int> P1; typedef sparse_polynom<big_int> P2;
    output_aligned(cout, sylvester(P1("3*x^2-1"), P2("x^4+10*x^2-7")));
    cout << "\nres = " << resultant(P1("3*x^2-1"), P2("x^4+10*x^2-7"));

    typedef sparse_polynom<rational<> > P3;
    vector<P3> f;
    cout << "\n" << sqrfree_factorize_poly_rational(pow(P3("x^10-12*x^5+4*x^4-3*x^3-190"), 1), f);
    cout << "\n" << sqrfree_factorize_poly_rational(pow(P3("x^10-12*x^5+4*x^4-3*x^3-190"), 2), f);
    cout << "\n" << sqrfree_factorize_poly_rational(pow(P3("x^10-12*x^5+4*x^4-3*x^3-190"), 3), f);

    typedef sparse_polynom<P3> P4;

    P3 a = "2*x^3-x+17", b = "x^2+5";

    cout << "\n" << a.subs(P4("((x)-x)"));
    output_aligned(cout << "\n", sylvester(a.subs(P4("((x)-x)")), b));
    cout << resultant(a.subs(P4("((x)-x)")), b);


}


//template <typename P, typename Seg>
//double algebraic_to_double (const P& p, const Seg& seg)
//{
//    interval<double> fseg = seg;
//    interval_root_precise(p, fseg, 0.00000000001);
//    return fseg.first;
//}


void test4_17 ()
{
    typedef rational<big_int> T;
    typedef sparse_polynom<T> P;
    //typedef sparse_polynom<P> Pxy;
    typedef interval<T> Seg;
    //typedef vector<P, false> Polyvec;
    //typedef vector<Seg, false> Segvec;

    //Polyvec polyvec = "(x^2-2, x^2-2)";
    //Segvec segvec = "((-1, 2), (-2, 0))";
    //P p; Seg seg;
    //
    //P rslt = resultant(polyvec[0].subs(Pxy("((x)-x)")), polyvec[1]);

    //algebraic_func_rslt(polyvec, segvec, rslt, p, seg, my_func<Segvec>());

    //P p; Seg seg;

    //P ap = "x^2-2"; Seg as = "(1, 2)";
    //P bp = "x^3-3"; Seg bs = "(-2, -1)";

    //cout << std::setprecision(15);

    //algebraic_plus(ap, as, bp, bs, p, seg);
    //cout << "\n" << algebraic_to_double(p, seg);
    //algebraic_multiplies(p, seg, P("11*x-10"), Seg("10/11", "10/11"), p, seg);
    //cout << "\n" << algebraic_to_double(p, seg);
    //algebraic_divides(p, seg, P("x^5-12"), Seg(0, 12), p, seg);
    //cout << "\n" << algebraic_to_double(p, seg);

    std::time_t tm = time(0);

    algebraic<T, T>
        a("x^2-2", "(1, 2)"),
        b("x^3+3", "(-2, -1)"),
        c("11*x-10", "(10/11, 10/11)"),
        d("x^5-12", "(1, 12)");

    cout << "\na + b = " << a << " + " << b;
    a += b;
    cout << "\na + b = " << a << " = " << double(a);

    cout << "\na * c = " << a << " * " << c;
    a *= c;
    cout << "\na * c = " << a << " = " << double(a);

    cout << "\na / d = " << a << " / " << d;
    a /= d;
    cout << "\na / d = " << a << " = " << double(a);

    cout << "\na / a = " << a << " / " << a;
    a /= a;
    cout << "\na / a = " << a << " = " << double(a);
    cout << "\nis_null(a) = " << a.is_null();

    cout << "\n\nTime: " << time(0) - tm;
}
