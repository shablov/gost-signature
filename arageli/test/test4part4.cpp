/*****************************************************************************

    test/test4part4.cpp

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


void test4_18 ()
{
    cout << "\n7->" << next_mersen_prime_degree(7);
    cout << "\n6->" << next_mersen_prime_degree(6) << "\n";

    typedef residue<int, Arageli::_Internal::module_2pm1<unsigned int, int> > T;

    {

        matrix<T> a = "((1, 2, 3), (4, 5, 6), (7, 8, 9))", b, q;

        for(matrix<T>::iterator i = a.begin(); i < a.end(); ++i)
        {
            i->module() = next_mersen_prime_degree(10);
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

}

void test4_19 ()
{
    cout << "\n\n ******* Intervals on double ******** \n\n";

    {
        typedef interval<double> T;

        matrix<T> a = "(((0.999, 1.001), (1.999, 2.001), (2.999, 3.001)), ((3.999, 4.001), (4.999, 5.001), (5.999, 6.001)), ((6.999, 7.001), (7.999, 8.001), (8.999, 9.001)))", b, q;
        vector<size_t> basis;
        T det;

        rref(a, b, q, basis, det, ctrl::make_rref_slog(cout));

        output_aligned(cout << "a =\n", a);
        output_aligned(cout << "\nb =\n", b);
        output_aligned(cout << "\nq =\n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        output_aligned(cout << "\nq * a =\n", q * a);
        output_aligned(cout << "\nb - q * a =\n", b - q * a);
        cout << "\nis it valid RREF?: " << (b == q * a);
    }

    cout << "\n\n ******* Intervals on rational ******** \n\n";

    {
        typedef interval<rational<> > T;

        matrix<rational<> > aa = "((1, 2, 3), (4, 5, 6), (7, 8, 9))";

        matrix<T> a = aa, b, q;

        for(matrix<T>::iterator i = a.begin(); i != a.end(); ++i)
        {
            i->first() -= rational<>(1, 1000);
            i->second() += rational<>(1, 1000);
        }

        vector<size_t> basis;
        T det;

        rref(a, b, q, basis, det, ctrl::make_rref_slog(cout));

        output_aligned(cout << "a =\n", a);
        output_aligned(cout << "\nb =\n", b);
        output_aligned(cout << "\nq =\n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        output_aligned(cout << "\nq * a =\n", q * a);
        output_aligned(cout << "\nb - q * a =\n", b - q * a);
        cout << "\nis it valid RREF?: " << (b == q * a);
    }

    cout << "\n\n ******* Intervals on rational functions ******** \n\n";

    {
        typedef interval<rational<sparse_polynom<rational<> > > > T;

        matrix<double> aa = "((1, 2, 3), (4, 5, 6), (7, 8, 9))";

        matrix<T> a = aa, b, q;

        for(matrix<T>::iterator i = a.begin(); i != a.end(); ++i)
        {
            i->first() -= rational<sparse_polynom<rational<> > >("x");
            i->second() += rational<sparse_polynom<rational<> > >("x");
        }

        vector<size_t> basis;
        T det;

        rref(a, b, q, basis, det, ctrl::make_rref_slog(cout));

        output_aligned(cout << "a =\n", a);
        output_aligned(cout << "\nb =\n", b);
        output_aligned(cout << "\nq =\n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        output_aligned(cout << "\nq * a =\n", q * a);
        output_aligned(cout << "\nb - q * a =\n", b - q * a);
        cout << "\nis it valid RREF?: " << (b == q * a);
    }

    cout << "\n\n *********** big_float *********** \n\n";

    {
        //big_float::set_global_precision(100);
        //big_float::set_round_mode(TO_NEAREST);

        cout.setf(std::ios::scientific, std::ios::floatfield);

        typedef big_float T;

        //matrix<T> a = "(((0.999, 1.001), (1.999, 2.001), (2.999, 3.001)), ((3.999, 4.001), (4.999, 5.001), (5.999, 6.001)), ((6.999, 7.001), (7.999, 8.001), (8.999, 9.001)))", b, q;
        matrix<T> a = "(((1, 1), (2, 2), (3, 3)), ((4, 4), (5, 5), (6, 6)), ((7, 7), (8, 8), (9, 9)))", b, q;
        vector<size_t> basis;
        T det;

        rref(a, b, q, basis, det, ctrl::make_rref_slog(cout));

        output_aligned(cout << "a =\n", a);
        output_aligned(cout << "\nb =\n", b);
        output_aligned(cout << "\nq =\n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        output_aligned(cout << "\nq * a =\n", q * a);
        output_aligned(cout << "\nb - q * a =\n", b - q * a);
        cout << "\nis it valid RREF?: " << (b == q * a);
    }
}


void test4_20 ()
{
    typedef algebraic<> T;

    vector<interval<rational<> > > segs;
    sparse_polynom<rational<> > p = "x^3+6*x^2+5*x-5";
    //sparse_polynom<rational<> > p = "x^2-2";
    sturm<rational<> >(p, segs);

    cout << "\np = " << p;
    cout << "\nthe number of roots = " << segs.size();

    vector<T> roots(segs.size());

    for(size_t i = 0; i < segs.size(); ++i)
        roots[i] = T(p, segs[i]);

    output_aligned(cout << "\nroots =\n", roots);

    vector<double> froots = roots;

    output_aligned(cout << "\ndouble(roots) =\n", froots);

    vector<double> frootvals = p.subs(froots);

    output_aligned(cout << "\np(double(roots)) =\n", frootvals);

    vector<T> rootvals = p.subs(roots);

    output_aligned(cout << "\np(roots) =\n", rootvals);

    vector<double> rootfvals = rootvals;

    output_aligned(cout << "\ndouble(p(roots)) =\n", rootfvals);

    cout << "\nis_null(p(roots)) = " << is_null(rootvals);


}


void test4_21 ()
{
    matrix<big_int> a = "((-2, 1, 1), (2, -1, -1))", f, q, e;
    skeleton(a, f, q, e);

    output_aligned(cout << "\na = \n", a);
    output_aligned(cout << "\nf = \n", f);
    output_aligned(cout << "\nq = \n", q);
    output_aligned(cout << "\ne = \n", e);
}


void test4_22 ()
{
    typedef big_int T;
    typedef matrix<T> MT;
    MT a = "((-1, 1, 1), (2, -1, -1), (0, 1, 0), (0, 0, 1))"; T res;
    intcount_barvinok(a, res);

    cout << "barvinok: " << res;
}


void test4_23 ()
{
    typedef sparse_polynom<big_int> P;
    P p = big_int(1); P x("x");
    for(int a = 1; a <= 50; ++a)
        p *= x - a;

    cout << p;
    vector<interval<rational<> > > lims;
    sturm<rational<> >(p, lims);

    output_aligned(cout << "\nlims =\n", lims);
}


void test4_24 ()
{
    typedef sparse_polynom<big_int> P;
    typedef interval<rational<> > SR;
    vector<P> ss;
    P p = "2*x-1";
    sturm_diff_sys(p, ss);
    SR seg;

    seg = "(1, -1)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(-1, -1)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(0, 1)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(-1, 0)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(1, 2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(1/2, 2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(-1, 1/2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(1/2, 1/2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    p *= P("x-10");

    seg = "(1, -1)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(-1, -1)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(0, 1)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(-1, 0)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(1, 2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(1/2, 2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(-1, 1/2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(1/2, 1/2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    p *= P("x+10");

    seg = "(1, -1)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(-1, -1)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(0, 1)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(-1, 0)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(1, 2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(1/2, 2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(-1, 1/2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);

    seg = "(1/2, 1/2)";
    cout << "\nseg = " << seg << ",  nr = " << sturm_number_roots(ss, seg);
}


void test4_25 ()
{
    typedef big_int T;
    typedef matrix<T, false> M;
    typedef vector<M::size_type, false> Basis;

    M a;

    a.resize(20, 20);
    for(M::iterator i = a.begin(); i != a.end(); ++i)
        *i = std::rand()/5000;

    output_aligned(cout << "A =\n", a);

    for(int i = 0; i < 2; ++i)
    {
        M b, q;
        Basis basis;
        M::size_type rank;
        T det;
        cout << "\n+++++++++ rref_gauss_bareiss +++++++++++";

        std::time_t tm = std::clock();

        rref_gauss_bareiss(a, b, q, basis, det);
        rank = basis.size();

        cout << "\ntime = " << std::clock() - tm;

        output_aligned(cout << "\nB = \n", b);
        output_aligned(cout << "\nQ = \n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        cout << "\nrank = " << rank;
    }

    for(int i = 0; i < 2; ++i)
    {
        M b, q;
        Basis basis;
        M::size_type rank;
        T det;
        cout << "\n+++++++++ bareiss +++++++++++";

        std::time_t tm = std::clock();

        bareiss(a, b, q, basis, det);
        rank = basis.size();

        cout << "\ntime = " << std::clock() - tm;

        output_aligned(cout << "\nB = \n", b);
        output_aligned(cout << "\nQ = \n", q);
        cout << "\nbasis = " << basis;
        cout << "\ndet = " << det;
        cout << "\nrank = " << rank;
    }

}


void test4_26 ()
{
    matrix<int> a = "((1, 1), (-1, -1))", f, q, e;
    //matrix<int> a, f, q, e;
    //a.resize(0, 2);

    skeleton(a, f, q, e);

    output_aligned(cout << "\na = \n", a);
    output_aligned(cout << "\nf = \n", f);
    output_aligned(cout << "\nq = \n", q);
    output_aligned(cout << "\ne = \n", e);
}
