/*****************************************************************************

    test/test1part1.cpp

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


/*
template <typename P1, typename P2>
void sparse_polynom_tester (std::ostream& report)
{
    // Сейчас, в основном, только тесты на компилируемость.

    binsym_operator_tester<P1, P2>(report);
    binsym_operator_tester<typename P1::monom, typename P2::monom>(report);
    binsym_operator_tester<P1, typename P2::monom>(report);
    binsym_operator_tester<typename P1::coef_type, typename P2::coef_type>(report);
    binsym_operator_tester<P1, typename P2::coef_type>(report);
    binsym_operator_tester<typename P1::monom, typename P2::coef_type>(report);
    binassign_operator_tester<P1, P2>(report);
    binassign_operator_tester<typename P1::monom, typename P2::monom>(report);
    binassign_operator_tester<P1, typename P2::monom>(report);
    binassign_operator_tester<typename P1::coef_type, typename P2::coef_type>(report);
    binassign_operator_tester<P1, typename P2::coef_type>(report);
    binassign_operator_tester<typename P1::monom, typename P2::coef_type>(report);

    binsym_operator_tester<typename P1::monom, P2>(report);
    binsym_operator_tester<typename P1::coef_type, P2>(report);
    binsym_operator_tester<typename P1::coef_type, typename P2::monom>(report);

    binsym_operator_tester<P2, P1>(report);
    binsym_operator_tester<typename P2::monom, typename P1::monom>(report);
    binsym_operator_tester<P2, typename P1::monom>(report);
    binsym_operator_tester<typename P2::coef_type, typename P1::coef_type>(report);
    binsym_operator_tester<P2, typename P1::coef_type>(report);
    binsym_operator_tester<typename P2::monom, typename P1::coef_type>(report);
    binassign_operator_tester<P2, P1>(report);
    binassign_operator_tester<typename P2::monom, typename P1::monom>(report);
    binassign_operator_tester<P2, typename P1::monom>(report);
    binassign_operator_tester<typename P2::coef_type, typename P1::coef_type>(report);
    binassign_operator_tester<P2, typename P1::coef_type>(report);
    binassign_operator_tester<typename P2::monom, typename P1::coef_type>(report);

    binsym_operator_tester<typename P2::monom, P1>(report);
    binsym_operator_tester<typename P2::coef_type, P1>(report);
    binsym_operator_tester<typename P2::coef_type, typename P1::monom>(report);

}


template <typename T1, typename T2>
void binsym_operator_tester (std::ostream& report)
{
    T1 t1;
    T2 t2;

    t1 + t2;
    t1 - t2;
    t1 * t2;
    t1 / t2;
    //t1 % t2;
}


template <typename T1, typename T2>
void binassign_operator_tester (std::ostream& report)
{
    T1 t1;
    T2 t2;

    T1 t12 = t2;

    T1 t13(t2);

    t1 = t2;

    t1 += t2;
    t1 -= t2;
    t1 *= t2;
    t1 /= t2;
    //t1 %= t2;
}
*/


/*
template <typename T> void standard_sparse_polynom_tester_1 (std::ostream& report)
{
    sparse_polynom_tester<sparse_polynom<char>, T>(report);
    sparse_polynom_tester<sparse_polynom<signed char>, T>(report);
    sparse_polynom_tester<sparse_polynom<unsigned char>, T>(report);
    sparse_polynom_tester<sparse_polynom<short>, T>(report);
    sparse_polynom_tester<sparse_polynom<int>, T>(report);
    sparse_polynom_tester<sparse_polynom<long>, T>(report);
    sparse_polynom_tester<sparse_polynom<unsigned short>, T>(report);
    sparse_polynom_tester<sparse_polynom<unsigned int>, T>(report);
    sparse_polynom_tester<sparse_polynom<unsigned long>, T>(report);
    sparse_polynom_tester<sparse_polynom<double>, T>(report);
    sparse_polynom_tester<sparse_polynom<float>, T>(report);
    sparse_polynom_tester<sparse_polynom<long double>, T>(report);
}

void standard_sparse_polynom_tester_2 (std::ostream& report)
{
    //standard_sparse_polynom_tester_1<sparse_polynom<char> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<signed char> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<unsigned char> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<short> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<int> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<long> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<unsigned short> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<unsigned int> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<unsigned long> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<double> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<float> >(report);
    //standard_sparse_polynom_tester_1<sparse_polynom<long double> >(report);
}
*/


void test1_1 ()
{
    big_int a = 1;

    for(int i = 1; i < 1000; ++i)
        a *= i;

    std::cout << "\na = " << a << std::endl;

    rational<big_int> b = a;

    for(int i = 1; i < 1000; ++i)
        b /= i;

    std::cout << "\nb = " << b << std::endl;
}


void test1_2 ()
{
    big_int a;
    a = big_int(1) << big_int(10000);
    a <<= 10000;

    std::cout << "\na = " << a << std::endl;

    big_int b = power(big_int(2), 10000);
    b *= power(big_int(2), 10000);

    std::cout
        << "\nb = " << b << std::endl
        << "\nresults are equal: " << std::boolalpha << (a == b) << std::endl;
}


void test1_3 ()
{
    rational<big_int> b = power(rational<big_int>(2), 10000);
    b *= power(rational<big_int>(2), 10000);

    std::cout
        << "\nb = " << b << std::endl;
}


void test1_4 ()
{
    big_int a(1), b = "2";
    rational<big_int> c = 2, d(2, 7);
    int e = 10, f = 11;

    rational<int> g = a*c*f + 17.0 - d*2;

    std::cout
        << '\n' << a*c
        << '\n' << a*c*f
        << '\n' << a*c*f + 17
        << '\n' << d*2
        << '\n' << -d*2
        << '\n' << a*c*f + 17 - d*2;

    std::cout
        << "\ng = " << rational<big_int>(g) << std::endl
        << "\nresult is correct: " << std::boolalpha << (rational<big_int>(g) == rational<int>(39*7 - 4, 7)) << std::endl;


    std::cout << std::endl;
}


void test1_5 ()
{
    typedef rational<big_int> T;

    matrix<T> a(4), b(4), q(4);
    vector<size_t> basis(4);
    T det;
    std::generate(a.begin(), a.end(), std::rand);
    output_aligned(std::cout << "a == \n", a, "|| ", " ||");

    rref(a, b, q, basis, det);

    output_aligned(std::cout << "\n\na == \n", a, "|| ", " ||");
    output_aligned(std::cout << "\n\nb == \n", b, "|| ", " ||");
    output_aligned(std::cout << "\n\nq == \n", q, "|| ", " ||");
    output_aligned(std::cout << "\n\nbasis == \n", basis, "|| ", " ||");

    std::cout << "\n\ndet == " << det;

    matrix<T> aq = a*q;
    output_aligned(std::cout << "\n\na*q == \n", aq, "|| ", " ||");
    output_aligned(std::cout << "\n\n(a*q)/det == \n", aq/=det, "|| ", " ||");

    std::cout << std::endl;
}


void test1_6 ()
{
    typedef matrix<big_int> MB;
    typedef sparse_polynom<MB> PMB;

    PMB pmb1 = PMB(MB(2, 2, fromval), 2) + PMB(MB(2, 5, fromval));
    const char* nums[] = {"11111111111111111", "22222222", "-3333333333333333333", "4"};
    MB mb1(2);
    std::copy(nums, nums + 4, mb1.begin());

    std::cout
        << "mb1 == " << mb1
        << "\npmb1 == " << pmb1;

    pmb1 += mb1;

    std::cout << "\n\npmb1 += mb1 == " << pmb1;


    std::cout << std::endl;
}


void test1_7 ()
{
    big_int a(std::numeric_limits<int>::min());
    big_int b('1');
    big_int c(123345lu);
    big_int d(true);
    //big_int e(-9223372036854775807);
    big_int f1(1.1);
    big_int f2(-1.1);
    big_int f3(0.1);
    big_int f4(-0.1);
    big_int f5(0.0);
    big_int f6(+1.0);
    big_int f7(-1.0);
    big_int f8(1.0e9);
    big_int f9(1.0e19);
    big_int f10(1.23456789123456789123456789e9);
    big_int f11(1.23456789123456789123456789e19);
    big_int f12(std::numeric_limits<float>::max());
    big_int f13(std::numeric_limits<double>::max());
    big_int f14(std::numeric_limits<long double>::max());

    std::cout << a << ' ' << b << ' ' << c << ' ' << d << ' ' << "-9223372036854775807" << ' ' << "-9223372036854775808";

    std::cout
        << "\n\n"
        << f1 << '\n'
        << f2 << '\n'
        << f3 << '\n'
        << f4 << '\n'
        << f5 << '\n'
        << f6 << '\n'
        << f7 << '\n'
        << f8 << '\n'
        << f9 << '\n'
        << f10 << '\n'
        << f11 << '\n'
        << f12 << '\n'
        << f13 << '\n'
        << f14 << '\n';

    std::cout << '\n' << std::setprecision(300) << std::numeric_limits<double>::max();

    std::cout << std::endl;
}


void test1_8 ()
{
    matrix<rational<int> > mri;
    rational<int> ri;

    //mri = ri*mri;
    mri *= ri;

    sparse_polynom<matrix<int> > pmi;
    matrix<int> mi;

    pmi += mi;


    std::cout << std::endl;
}

void test1_9 ()
{
    big_int a;
    if(a != 0)std::cout << "\ntrue";
    else std::cout << "\nfalse";

    std::cout << std::endl;
}


void test1_10 ()
{
    big_int a(125l);
    int b = a;
    signed char c = a;
    std::cout << a << ' ' << b << ' ' << c;

    a = std::numeric_limits<int>::min();
    b = a;

    std::cout << "\n" << b << ' ' << a;

    a = std::numeric_limits<int>::max();
    b = a;

    std::cout << "\n" << b << ' ' << a;

    std::cout << std::endl;
}


void test1_11 ()
{
    big_int a("99834729384723984273984273498274972394872394729587359834795734958739573957398520934823094820492834098759793473984809284092840923840247884092412398753968701298312");
    float b = a;
    double c = a;
    long double d = a;

    std::cout << a << '\n' << b << '\n' << c << '\n' << d;

    std::cout << std::endl;
}


void test1_12 ()
{
    std::cout << big_int("2") * 12.3 - 2 + 7.0l / big_int(234);

    std::cout << std::endl;
}

void test1_13 ()
{
    big_int a; rational<big_int> b;
    std::cin >> a;
    std::cout << a;
    std::cin >> b;
    std::cout << b;
    std::cin.get();

    std::cout << std::endl;
}


void test1_14 ()
{
    sparse_polynom<int> a;
    std::cout << "Polynom Input Test. Please enter 13 to the next prompt if you want to exit.\n\n";

    do
    {
        std::cout << "Input sparse_polynom: ";
        std::cin >> a;

        std::cout << "State of std::cin is";
        if(std::cin.good())std::cout << " good";
        if(std::cin.eof())std::cout << " eof";
        if(std::cin.bad())std::cout << " bad";
        if(std::cin.fail())std::cout << " fail";

        std::cin.clear();
        std::cin.ignore(1);

        std::cout << "\nPolynom that has been read: " << a << "\n\n";
    }while(a != sparse_polynom<int>(13));

    std::cout << std::endl;
}


void test1_15 ()
{
    typedef matrix<sparse_polynom<matrix<rational<big_int> > > > Mpmrbi;
    typedef vector<int> Vi;

    Vi vi;
    std::cin >> vi;
    std::cout << vi;

    //Mpmrbi a(2, Mpmrbi::value_type("x"));

    //std::cout << a;

    std::cout << std::endl;
}


void test1_16 ()
{
    typedef matrix<sparse_polynom<matrix<rational<big_int> > > > Mpmrbi;

    Mpmrbi a(2, 2, Mpmrbi::value_type("x"));

    std::cout << a;

    std::cout << std::endl;
}


}
