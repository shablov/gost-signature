/*****************************************************************************

    test/test4part7.cpp

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


void test4_44 ()
{

    typedef rational<big_int> T;
    typedef set::grid1<T> BS;
    typedef vector<T> V;
    typedef set::vector_upto_size<V, BS> S;
    typedef enumer::fast<S> E;

    for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
    {
        S s(i, BS(-1, 1, T(1, (j+1))));
        cout << "i = " << i << "\n";

        for(E e(enumer::begin, s); !e.is_end(); ++e)
            cout << *e << "\n";

        cout << "\n";

        rnd::equiprob<S> r(s);
        std::map<V, int> hist;
        int n = 1000;
        for(int k = 0; k < n; ++k)
            ++hist[r()];

        for(std::map<V, int>::iterator e = hist.begin(); e != hist.end(); ++e)
            cout << "[" << e->first << "] = " << e->second << "\n";

        cout << "\n";
    }
}


void test4_45 ()
{
    typedef rational<big_int> T;
    typedef set::grid1<T> BS;
    typedef vector<T> V;
    typedef set::vector_fixed_size<V, BS> VS;
    typedef set::byvector<sparse_polynom<T>, VS> S;
    typedef enumer::fast<S> E;

    for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
    {
        S s(VS(i, BS(-1, 1, T(1, (j+1)))));
        cout << "i = " << i << "\n";

        for(E e(enumer::begin, s); !e.is_end(); ++e)
            cout << *e << "\n";

        cout << "\n";

        rnd::equiprob<S> r(s);
        std::map<sparse_polynom<T>, int> hist;
        int n = 1000;
        for(int k = 0; k < n; ++k)
            ++hist[r()];

        for(std::map<sparse_polynom<T>, int>::iterator e = hist.begin(); e != hist.end(); ++e)
            cout << "[" << e->first << "] = " << e->second << "\n";

        cout << "\n";
    }
}


void test4_46 ()
{
    sparse_polynom<residue <big_int> > P = "((1 (mod 3))*x)";
    std::cout << P;
}


void test4_47 ()
{
    typedef int T;
    rnd::equiprob_alter<set::inonnegative<T> > myrnd;

    for(int i = 0; i < 5; ++i)
    {
        std::map<int, int> hist;
        for(int j = 0; j < 100; ++j)
            ++hist[myrnd(i)];

        for(std::map<int, int>::iterator e = hist.begin(); e != hist.end(); ++e)
            cout << "[" << e->first << "] = " << e->second << "\n";

        cout << "\n";
    }
}


void test4_48 ()
{
    typedef int T;
    typedef set::inonnegative<T> Set;
    typedef set::subsets_fixnum<Set> Subsets;
    typedef rnd::equiprob<Subsets> Rnd_subsets;

    for(int i = 0; i <= 6; ++i)
    {
        Subsets ss(Set(6), i);
        Rnd_subsets myrnd(ss);

        typedef std::map<Subsets::value_type, int> Hist;
        Hist hist;
        for(int j = 0; j < 10000; ++j)
            ++hist[myrnd()];

        for(Hist::iterator e = hist.begin(); e != hist.end(); ++e)
            cout << "[" << e->first << "] = " << e->second << "\n";

        cout << "\n";
    }
}


void test4_49 ()
{
    {
        typedef int T;
        typedef set::inonnegative<T> Set;
        typedef set::subsets_fixnum<Set> Subsets;
        typedef rnd::equiprob<Subsets>::base_type Basernd;
        typedef rnd::equiprob<Subsets, Basernd> Rnd_subsets;
        typedef rnd::equiprob<Set, Basernd> Rnd_other;

        Set os(100);
        Subsets ss(Set(15), 4);

        Basernd basernd;
        Rnd_subsets myrnd1(basernd, ss);
        Rnd_other myrnd2(basernd, os);

        for(int i = 0; i < 5; ++i)
            cout << "myrnd1() = " << myrnd1() << "\nmyrnd2() = " << myrnd2() << "\n";

        cout << "\n";
    }

    {
        typedef int T;
        typedef set::inonnegative<T> Set;
        typedef set::subsets_fixnum<Set> Subsets;
        typedef rnd::equiprob<Subsets>::base_type Basernd;
        typedef rnd::equiprob<Subsets, Basernd> Rnd_subsets;
        typedef rnd::equiprob<Set, Basernd> Rnd_other;

        Set os(100);
        Subsets ss(Set(15), 4);

        Basernd basernd(2);
        Rnd_subsets myrnd1(basernd, ss);
        Rnd_other myrnd2(basernd, os);

        for(int i = 0; i < 5; ++i)
            cout << "myrnd1() = " << myrnd1() << "\nmyrnd2() = " << myrnd2() << "\n";

        cout << "\n";
    }

    {
        typedef int T;
        typedef set::inonnegative<T> Set;
        typedef set::subsets_fixnum<Set> Subsets;
        typedef rnd::equiprob<Subsets>::base_type Basernd;
        typedef rnd::equiprob<Subsets, rnd::ref<Basernd> > Rnd_subsets;
        typedef rnd::equiprob<Set, rnd::ref<Basernd> > Rnd_other;

        Set os(100);
        Subsets ss(Set(15), 4);

        Basernd basernd;
        Rnd_subsets myrnd1(rnd::ref<Basernd>(basernd), ss);
        Rnd_other myrnd2(rnd::ref<Basernd>(basernd), os);

        for(int i = 0; i < 5; ++i)
            cout << "myrnd1() = " << myrnd1() << "\nmyrnd2() = " << myrnd2() << "\n";

        cout << "\n";
    }

    {
        typedef int T;
        typedef set::inonnegative<T> Set;
        typedef set::subsets_fixnum<Set> Subsets;
        typedef rnd::equiprob<Subsets>::base_type Basernd;
        typedef rnd::equiprob<Subsets, rnd::ref<Basernd> > Rnd_subsets;
        typedef rnd::equiprob<Set, rnd::ref<Basernd> > Rnd_other;

        Set os(100);
        Subsets ss(Set(15), 4);

        Basernd basernd(2);
        Rnd_subsets myrnd1(rnd::ref<Basernd>(basernd), ss);
        Rnd_other myrnd2(rnd::ref<Basernd>(basernd), os);

        for(int i = 0; i < 5; ++i)
            cout << "myrnd1() = " << myrnd1() << "\nmyrnd2() = " << myrnd2() << "\n";

        cout << "\n";
    }
}


void test4_50 ()
{
    typedef algebraic<> T;
    T
        a("x^4+10*x^2-10", "(-1, 0)"),
        b("x^4+11*x^2-10", "(0, 1)");

    std::cout << "a = " << a << " = " << double(a) << "\n";
    std::cout << "b = " << b << " = " << double(b) << "\n";
    std::cout << "-a = " << -a << " = " << double(-a) << "\n";
    std::cout << "-b = " << -b << " = " << double(-b) << "\n";
    std::cout << "a + b = " << a + b << " = " << double(a + b) << "\n";
    std::cout << "a - b = " << a - b << " = " << double(a - b) << "\n";
    std::cout << "a * b = " << a * b << " = " << double(a*b) << "\n";
    std::cout << "a / b = " << a / b << " = " << double(a/b) << "\n";
}


void test4_51 ()
{
    generic_type x;
    std::cout << x << "\n";
    x = new generic_big_int(5);
    generic_type y = new generic_big_int(6);

    std::cout << -x << "\n";
    std::cout << -y << "\n";
    std::cout << x + y << "\n";
    std::cout << x - y << "\n";
    std::cout << x * y << "\n";
    std::cout << x / y << "\n";
    std::cout << x % y << "\n";

    Arageli::vector<generic_type> vx, vy(15, y);
    std::cout << vx << "\n" << vy << "\n";
    vx.resize(15, x);
    std::cout << vx + vy << "\n";

    big_int xx = 2, yy = 2;
    std::cout << Arageli::power(xx, yy);
}


void test4_52 ()
{
    typedef std::complex<rational<> > C;
    typedef sparse_polynom<rational<> > P;
    typedef sparse_polynom<sparse_polynom<C> > PPC;

    PPC ppc;
    ppc += PPC::monom(PPC::coef_type::monom(C(1)), 1);
    ppc += PPC::monom(PPC::coef_type::monom(C(0, 1), 1), 0);
    P p = "2*x^3-17*x+5";
    std::cout
        << "ppc = " << ppc << "\n"
        << "p = " << p << "\n";
    PPC subres = p.subs(ppc);
    std::cout << "p.subs(ppc) = " << subres;
}

void test4_53 ()
{
    timer tm;
    for(big_int i = 0; i < big_int("10000"); ++i);
    std::cout << tm.time() << '\n' << tm.precision();
    tm.stop();
    std::cout << "\n" << tm;
    std::cin >> tm;
    std::cout << tm.time() << '\n' << tm;

}

void test4_54 ()
{
    vector<int> b, c = "(1, 2)";
    matrix<int> matr = "((1, 2), (3, 4))";
    b = solve_linsys(matr, c);
    output_aligned(std::cout, b);
    std::cout << "(matr*b == c) = " << (matr*b == c);
}

void test4_55 ()
{
    matrix<big_int> m = "((1, 0), (0, 1))";
    cone<> c1(m, fromgen);
    std::cout << f_vector_cone(c1);
}

void do_some_time (int i)
{
    static volatile int x = 0;
    for(int j = 0; j < i; ++j)
        x += j;
}

void test4_56 ()
{
    double precision;
    std::cout << "precision: ";
    std::cin >> precision;

    const int SIZE = 10;
    typedef Arageli::vector<double> V;
    V times(SIZE), x(SIZE), prec(SIZE);

    for(int i = 0; i < SIZE; ++i)
    {
        int j = 0;
        timer tm;
        do { do_some_time(1000000*i); ++j; } while(j < precision/*tm.precision() > precision*/);
        tm.stop();
        times[i] = tm.time()/j;
        prec[i] = tm.precision();
        x[i] = i;
    }

    typedef cartesian2d_chart_line<V, V> Line;
    typedef Arageli::vector<Line> Lines;
    Lines lines;
    lines.push_back(Line("time", x, times, "[linecolor=black]"));
    lines.push_back(Line("precision", x, times*prec, "[linecolor=green]"));
    std::ofstream file("timer.times.chart.tex");

    cartesian2d_chart chart
    (
        16, 20, 1, 1.5,
        "промежуток", "Время, сек."
    );

    output_pstricks_cartesian2d(file, 0, 0, chart, lines);
}

void test4_57 ()
{
    multimonom<rational<int> >
        mm1(1, vector<rational<> >("(1, 2, 3)")),
        mm2(1, vector<rational<> >("(3, 2, 1)")),
        mm_gcd = gcd(mm1, mm2),
        mm_lcm = lcm(mm1, mm2);

    std::cout
        << "mm1.multidegree() = " << mm1.multidegree() << '\n'
        << "mm2.multidegree() = " << mm2.multidegree() << '\n'
        << "mm_gcd.multidegree() = " << mm_gcd.multidegree() << '\n'
        << "mm_lcm.multidegree() = " << mm_lcm.multidegree();
}


void test4_58 ()
{
    typedef vector<int> MD;
    typedef std::complex<rational<> > F;
    typedef sparse_multipolynom<F> P;

    Arageli::vector<P> vp;
    //// P(F(), MD("()"))
    //vp.push_back(P(F(1), MD("(2)")) + P(F(2), MD("(1)")) + P(F(1), MD("(0)")));
    //vp.push_back(P(F(1), MD("(1)")) + P(F(1), MD("(0)")));

    //// P(F(1), MD("(, , , )"))
    //vp.push_back(P(F(1), MD("(4, 0, 0, 0)")) - P(F(1), MD("(0, 1, 0, 0)")));
    //vp.push_back(P(F(1), MD("(3, 0, 0, 0)")) - P(F(1), MD("(0, 0, 1, 0)")));
    //vp.push_back(P(F(1), MD("(2, 0, 0, 0)")) - P(F(1), MD("(0, 0, 0, 1)")));

    //// P(F(1), MD("(, )"))
    //vp.push_back(P(F(1), MD("(7, 0)")) - P(F(1), MD("(1, 0)")) - P(F(1), MD("(0, 0)")));
    //vp.push_back(P(F(1), MD("(3, 1)")) - P(F(4), MD("(1, 0)")) + P(F(1), MD("(0, 0)")));

    // P(F(1), MD("(, , )"))
    //vp.push_back(P(F(12), MD("(1, 0, 0)")) - P(F(1), MD("(1, 1, 0)")));
    //vp.push_back(P(F(0, 7), MD("(0, 0, 3)")) - P(F(1), MD("(3, 1, 1)")));
    //vp.push_back(P(F(1), MD("(4, 0, 1)")) - P(F(1), MD("(0, 1, 0)")));

    vp.push_back(P("-x^(1, 1, 0)+(12,0)*x^(1, 0, 0)"));
    vp.push_back(P("-x^(3, 1, 1)+(0,7)*x^(0, 0, 3)"));
    vp.push_back(P("x^(4, 0, 1)-x^(0, 1, 0)"));

    output_aligned(std::cout, vp);
    //std::cout << "\n";

    //typedef Arageli::_Internal::gbasis<P> GB;
    //Arageli::_Internal::gbasis<P> gb(vp);

    std::cout << "\n\n";

    output_aligned(std::cout, groebner_basis(vp));
    //for(GB::iterator i = gb.begin(); i != gb.end(); ++i)
    //    std::cout << i->first << " : " << i->second/P(i->second.leading_coef(), MD("(0, 0, 0)")) << "\n";
}


void test4_59 ()
{
    typedef sparse_polynom<double, double> PD;
    //PD p1 = "x^14-2.8+3*x^-0.3", p2 = "5*x^7.2+2.4*x^-3.1", p3 = "x^550";
    PD p1 = "x^2-1", p2 = "x-2", p3 = "x^50";
    p1 *= p3;
    std::cout
        << "p1 = " << p1 << "\n"
        << "p2 = " << p2 << "\n"
        << "p1 + p2 = " << p1 + p2 << "\n"
        << "p1 * p2 = " << p1 * p2 << "\n"
        << "p1 / p2 = " << p1 / p2 << "\n";
    PD p4 = p1/p2;
    std::cout << "\n\n";
    std::copy(p4.degrees_begin(), p4.degrees_end(), std::ostream_iterator<double>(std::cout, "\n"));
    std::cout << "\n\n";
    vector<double> degrees(p4.size(), p4.degrees_begin(), fromseq);
    std::cout << degrees;
    std::cout << "\n\n";
    std::adjacent_difference(degrees.begin(), degrees.end(), degrees.begin());
    std::cout << "\n\n";
    std::cout << degrees;
}


void test4_60 ()
{
    typedef Arageli::rational<> R;
    typedef Arageli::rational<sparse_multipolynom<R> > F;
    typedef sparse_multipolynom<F> P;

    Arageli::vector<P> vp;

    //Arageli::vector<P> vp =
    //"("
    //    "-x^(1, 1, 0)+(12,0)*x^(1, 0, 0),"
    //    "-x^(3, 1, 1)+(0,7)*x^(0, 0, 3)),"
    //    "x^(4, 0, 1)-x^(0, 1, 0)"
    //")";

    std::cin >> vp;

    output_aligned(std::cout, vp);
    std::cout << "\n\n";
    output_aligned(std::cout, groebner_basis(vp));
}
