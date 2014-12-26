/*****************************************************************************

    test/test1part2.cpp

    This file is a part of Arageli library.

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

#define _ARAGELI_TEST_VECTOR_SWAP_BUG

namespace Arageli
{


void test1_17 ()
{
    sparse_polynom<big_int> a("x^2-4*x+4"), b("x-2");
    std::cout << "\n\ngcd(" << a << ", " << b << ") == " << gcd(a, b) << "\n\n";

    matrix<sparse_polynom<rational<big_int> > > mprb(2);

    mprb(0, 0) = sparse_polynom<rational<big_int> >(a)/7;
    mprb(1, 0) = b;
    mprb(0, 1) = a*b;
    mprb(1, 1) = a*a;

    output_aligned(std::cout, mprb, "|| ", " ||", "  ");

    std::cout << std::endl;
}


void test1_18 ()
{
    vector<double, false> a(10u, 2.0);
    vector<double, false> b = std::sin(a);
    b = std::sin(a);

    output_aligned(std::cout, a);
    std::cout << std::endl;
}


void test1_19 ()
{
    vector<double, false> a(5);
    for(size_t i = 0; i < a.size(); ++i)
        a[i] = std::rand();

    matrix<double, false> res(100, 5, fromsize);

    for(size_t i = 0; i < res.nrows(); ++i)
    {
        a = std::sin(a);
        for(size_t j = 0; j < a.size(); ++j)
            res(i, j) = a[j];
    }

    output_aligned(std::cout, res);

    std::cout << std::endl;
}


void test1_m ()
{
    typedef sparse_polynom<matrix<rational<big_int> > > P;
    P p1 = P(P::coef_type(2, rational<big_int>(3, 7), fromval), 2) + P(P::coef_type(2, rational<big_int>(1, 2), fromval), 5);
    std::cout << p1 << std::endl;
    p1.leading_coef()(1, 1) -= 5;
    std::cout
        << p1 << "\n\n" << p1*p1 << "\n\n"
        << p1.subs(matrix<sparse_polynom<rational<big_int> > >(2, sparse_polynom<rational<big_int> >(big_int(1), 1), diag)) << '\n'
        << (p1*p1).subs(matrix<sparse_polynom<rational<big_int> > >(2, sparse_polynom<rational<big_int> >(big_int(1), 1), diag)) << '\n';

    std::cout << "\n";

    vector<sparse_polynom<big_int> > vpb1(3u, big_int("123456789123456789"));
    vector<sparse_polynom<int> > vpi1(3u, 123);

    std::cout << '\n' << vpb1 << '\n' << vpi1 << '\n';

    vpb1 += vpi1;

    std::cout << vpb1;

    vpb1 -= vpi1;

    std::cout << vpb1;

    std::cout << std::endl;
}


void test1_20 ()
{
    std::cout << typeid(_Internal::digit).name() << "\n";

    sparse_polynom<rational<big_int> > a, b;
    a = rational<big_int>(24238428);
    a *= a; a *= a;
    b = rational<big_int>(2042343243);
    //sparse_polynom<big_int> a, b;
    //a = big_int(24238428);
    //a *= a; a *= a;
    //b = big_int(23984923749980293);
    //sparse_polynom<long long> a, b;
    //a = long long(24238428);
    //a *= a; a *= a;
    //b = long long(23984923749980293);
    a % b;
    std::cout
        << a << "\n" << b << "\n"
        << a + b << "\n"
        << a * b << "\n"
        << a / b << "\n"
        << a - b << "\n"
        << a % b << "\n";

    std::cout << gcd(a, b);

    std::cout << std::endl;
}


void test1_21 ()
{
    typedef big_int E;
    typedef vector<E> V;
    V v = "(345345345, 204982394723948729847892759573987325,  23432423424234235625)";
    std::cout << v << "\n" << lcm(v);

    std::cout << std::endl;
}


void test1_22 ()
{
    sparse_polynom<rational<big_int> > a;
    while(a != sparse_polynom<rational<big_int> >(13))
    {
        std::cin.clear();
        std::cin >> a;
        if(!std::cin)std::cout << "std::cin is not good\n";
        else std::cout << a << '\n';
    }

    std::cout << std::endl;
}


void test1_23 ()
{
    sparse_polynom<sparse_polynom<rational<> > > v /*= "(x^3 - 12*x)*x^7 + (7)*x^2 - (x)*x"*/;
    //sparse_polynom<rational<> > v /*= "(x^3 - 12*x)*x^7 + (7)*x^2 - (x)*x"*/;
    //input_list(std::cin, v, "", ".", ";", "", "", ",");
    std::cin >> v;
    std::cout << v;

    std::cout << std::endl;
}


void test1_23_5 ()
{
    vector<big_int, true> a;
    vector<big_int, false> b;
    std::swap(a, b);
#ifndef _ARAGELI_TEST_VECTOR_SWAP_BUG
    swap(a, b);
#endif
}


void test1_24 ()
{
    refcntr_proxy<vector<big_int, true> > a;
    refcntr_proxy<vector<big_int, true> , false> b;

    refcntr_proxy<vector<big_int, false> > c;
    refcntr_proxy<vector<big_int, false> , false> d;

    std::swap(a, b);
    std::swap(b, a);
    std::swap(a, a);
    std::swap(b, b);
#ifndef _ARAGELI_TEST_VECTOR_SWAP_BUG
    std::swap(a, c);
    std::swap(b, c);
    std::swap(a, d);
    std::swap(b, d);
    std::swap(c, b);
    std::swap(c, a);
    std::swap(d, a);
    std::swap(d, b);
#endif

    std::cout << std::endl;
}


void test1_25 ()
{
    sparse_polynom<big_int> a("234*x^123-923874*x+23432-x^1");
    sparse_polynom<int> b("1111111*x+22222222*x^2-333333*x^3"), c;

    std::cout << "a = " << a << "\nb = " << b << "\nswapping b <--> a\n";

    std::swap(a, b);
    std::cout << "a = " << a << "\nb = " << b;

    std::cout << "\na = " << a << "\nb = " << b << "\nswapping b <--> c\n";

    std::swap(b, c);
    std::cout << "a = " << a << "\nb = " << b << "\nc = " << c;


    std::cout << std::endl;
}


void test1_26 ()
{
    vector<int> a = "(1, 2, 3)";
    std::cin >> a;
    std::cout << a;

    std::cout << std::endl;
}


void test1_27 ()
{
    rational<int> i;
    rational<big_int> b;
    std::cout << typeid(i + b).name();

    std::cout << std::endl;
}


void test1_28 ()
{
    matrix<sparse_polynom<rational<> > > a, b, p, q;
    //random_int_matrix(a, 2, 2);
    a = "((x, x^3+5), (x^2-x-4, x^4-x^3-4*x^2+5*x-5))";
    output_aligned(std::cout << "a = \n", a);
    size_t rank; sparse_polynom<rational<> > det;
    smith(a, b, p, q, rank, det, ctrl::make_smith_slog(std::cout));

    // normalization pass, actually smith does not this
    for(size_t i = 0; i < b.nrows() && !b(i, i).is_null(); ++i)
    {
        p.div_row(i, b(i, i).leading_coef());
        b(i, i) /= b(i, i).leading_coef();
    }

    output_aligned(std::cout << "\nb = \n", b, "|| ", " ||", "  ");
    output_aligned(std::cout << "\np = \n", p, "|| ", " ||", "  ");
    output_aligned(std::cout << "\nq = \n", q, "|| ", " ||", "  ");
    std::cout << "\nrank = " << rank << "\ndet = " << det;

    matrix<sparse_polynom<rational<> > > paq = p*a*q;
    output_aligned(std::cout << "\np*a*q = \n", paq);
    std::cout << "\nb == p*a*q: " << std::boolalpha << (b == paq);

    std::cout << std::endl;
}




void test1_29 ()
{
    typedef std::map<size_t, int> Lens;

    //{
    //    Lens lens;
    //    for(int i = 0; i < 10000; ++i)
    //        ++lens[big_int::random_in_range(1).length()];


    //    std::copy(lens.begin(), lens.end(), std::ostream_iterator<Lens::value_type>(std::cout, "\n"));

    //    std::cout << "\n";
    //}

    //{
    //    Lens lens;
    //    for(int i = 0; i < 10000; ++i)
    //        ++lens[big_int::random_with_length(50).length()];


    //    std::copy(lens.begin(), lens.end(), std::ostream_iterator<Lens::value_type>(std::cout, "\n"));

    //    std::cout << "\n";
    //}

    {
        Lens lens;
        for(int i = 0; i < 1000000; ++i)
            ++lens[big_int::random_with_length_or_less(50).length()];


        std::copy(lens.begin(), lens.end(), std::ostream_iterator<Lens::value_type>(std::cout, "\n"));
    }
}


void test1_30 ()
{
    Timing tmg;
    big_int res_factorial = factorial_even_odd_multiplication(big_int(20000));
    std::cout << "\nFactorial computation time is " << tmg.time() << " seconds.";

    //big_int res2 = factorial_successive_multiplication(big_int(10000));
    //std::cout << res2;
    //if(res_factorial == res2)
    //    std::cout << "OK";
    //else std::cout << "ERROR";
    //for(big_int i = 0; i < 100; ++i)
    //    if(factorial_by_successive_multiplication(i)
    //        != factorial_by_even_odd_multiplication(i))
    //    {
    //        std::cout
    //            << "\nError: "
    //            << factorial_by_successive_multiplication(i)
    //            << ", " << factorial_by_even_odd_multiplication(i);
    //    }
    //tmg.off();
    //std::cout << "\nFactorial computation time is " << tmg.time() << " seconds.";
    //std::cout << "\nEnd, press Enter...";
    //std::cin.get();
    //std::ofstream file("factorial(100000).txt");
    ////tmg.on();
    //file << res_factorial;
    ////tmg.off();
    ////std::cout << "\nFactorial result outputting time is " << tmg.time() << " seconds.";
    //std::cout << "\nEnd, press Enter...";
    //std::cin.get();

    std::cout << std::endl;
}


void test1_31 ()
{
    vector<big_int> a = "(1,2,3,4,5,6)";

    for(int i = 0; i < 20; ++i)
        std::random_shuffle(a), std::cout << a << '\n';

    std::cout << std::endl;
}


template <typename Pair>
struct Less_second_pair
{
    bool operator() (const Pair& a, const Pair&b) const
    { return a.second < b.second; }
};

template <typename Pair>
struct Outputter_pair
{
    void operator() (const Pair& v) const
    { std::cout << '\n' << v.first << " : " << v.second; }
};


struct Omit_const_first_pair
{
    template <typename T1, typename T2>
    std::pair<T1, T2> operator() (const std::pair<const T1, T2>& a) const
    { return std::pair<T1, T2>(a.first, a.second); }

    template <typename T1, typename T2>
    std::pair<T1, T2> operator() (const std::pair<T1, T2>& a) const
    { return a; }
};


void test1_32 ()
{
    typedef vector<big_int> V;
    typedef std::map<V, big_int> MV;

    V a = "(1, 2, 3, 4, 5)";
    std::cout << "Permutations of a = " << a << std::endl;

    const int
        nech = 10,
        np = factorial(a.size()),
        n = np*nech;

    MV mv;

    for(int i = 0; i < n; ++i)
        ++mv[std::random_shuffle(a), a];

    std::cout
        << "\nTheoretical number of permutations is " << factorial(a.size())
        << "\nEmpirical number of permutations is " << mv.size()
        << "\nAverage value for each chunk is " << nech;

    std::cout << "\n\nDistribution by permutations:\n";
    std::for_each(mv.begin(), mv.end(), Outputter_pair<MV::value_type>());
    typedef vector<std::pair<V, big_int> > VMV;
    VMV vmv;
    std::transform(mv.begin(), mv.end(), std::back_inserter(vmv), Omit_const_first_pair());
    std::sort(vmv, Less_second_pair<VMV::value_type>());
    std::cout << "\n\nSorted by values:\n";
    std::for_each(vmv, Outputter_pair<VMV::value_type>());

    std::cout << std::endl;
}


void test1_33 ()
{
    vector<rational<> > a = "(1, 1/2, 1/3, 2/3, 1/4, 2/4, 3/4, 1/5, 2/5, 3/5, 4/5)";
    std::cout << "\na = " << a;
    std::sort(a);
    std::cout << "\na after sorting = " << a;
    vector<rational<> >::iterator i = std::unique(a);
    std::cout << "\na after unique = " << a;
    a.erase(i, a.end());
    std::cout << "\na after erase = " << a;
    std::cout << "\na + 1 = " << a + 1;
    std::cout << "\na + 1.0 = " << a + 1.0;

    std::cout << std::endl;
}


}
