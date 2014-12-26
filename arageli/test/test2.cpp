/*****************************************************************************

    test/test2.cpp

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

namespace Arageli
{

void test_2_1 ()
{
    big_int
        maxint = std::numeric_limits<int>::max(),
        maxfloat = std::numeric_limits<float>::max(),
        maxdouble = std::numeric_limits<double>::max();

    std::cout
        << "maximum int value is " << maxint
        << "\nmaximum float value is " << maxfloat
        << "\nmaximum double value is " << maxdouble
        << "\nbig_int(1e100) = " << big_int(1e100);

    std::cout << std::endl;
    //std::cin.get();
}


void test_2_2 ()
{
    matrix<rational<> > A = "((21, 3, 4), (3335, 6, 75), (81, 9, 10))";

    std::cout << "A = \n";
    output_aligned(std::cout, A, "|| ", " ||", "  ");
    std::cout << "\ninversion of A = \n";
    output_aligned(std::cout, inverse(A), "|| ", " ||", "  ");

    std::cout
        << "\nthe inversion is valid: "
        << std::boolalpha << (A*inverse(A)).is_unit();

    std::cout << std::endl;
    //std::cin.get();
}


void test_2_3 ()
{
    sparse_polynom<matrix<int> >
        f = "(((3,-5),(0,7))*x^8+((0,1),(0,-8))*x^3+((-1,9),(13,2)))";

    matrix<sparse_polynom<int> >
        x = "((x,0),(0,x))";

    std::cout
        << "f(x) = " << f
        << "\nx = " << x
        << "\nthe resulting matrix is \n";

    output_aligned(std::cout, f.subs(x), "||", "||");

    std::cout << std::endl;
    //std::cin.get();
}


/*

/   1  13     5   12     8    \
|  ---x  + 25x - ----   ---x + 14  |
|   7             7      3         |
|                                  |
|                           1      |
|        x - 12            ----    |
\                           17     /

*/


void test_2_4 ()
{
    sparse_polynom<rational<> >
        f = "1/7*x^13-5*x^8-7*x^5+10",
        g = "-11*x^3+22/17*x^2-x";

    std::cout
        << "f(x) = " << f
        << "\ng(x) = " << g
        << "\nf(x)*g(x) = " << f*g
        << "\nf(x)+g(x) = " << f + g
        << "\nf(x)/g(x) = " << f/g
        << "\nf(x)%g(x) = " << f%g
        << "\nGCD(f(x), g(x)) = " << gcd(f, g)
        << "\ndividing is valid: " << std::boolalpha << ((f/g)*g + f%g == f);

    std::cout << std::endl;
    //std::cin.get();
}


void test_2_5 ()
{
    std::cout
        << "0! = " << factorial(0)
        << "\n1! = " << factorial(1)
        << "\n2! = " << factorial(2)
        << "\n3! = " << factorial(3)
        << "\n4! = " << factorial(4)
        << "\n100! = " << factorial(big_int(100));

    std::cout << std::endl;
    //std::cin.get();
}


void test_2_6 ()
{
    rational<> a = "98172349273/128374921387491247918247";
    big_int b = "23749129472394712";
    std::cout << a*b + 2*std::pow(big_int(10), 100) - a/b + 1;

    std::cout << std::endl;
    //std::cin.get();
}


void test_2_7 ()
{
    matrix<big_int>
        a = "((1, 2), (3, 4), (5, -6))",
        b = "((1, 2), (3, 4), (5, -6), (2, 7))";

    std::cout << (a == b) << ' ' << (a != b);

    std::cout << std::endl;
    //std::cin.get();
}


void test_2_8 ()
{
    matrix<rational<> >
        a = "((1, 2, 3), (4, 5, 6), (7, 8, 9))",
        b = "((21, 3, 4), (3335, 6, 75), (81, 9, 10))";

    std::cout << "a = \n";
    output_aligned(std::cout, a);
    std::cout << "\nb = \n";
    output_aligned(std::cout, b);

    std::cout << "\na/b = \n";
    output_aligned(std::cout, a/b);

    std::cout << "\n(a/b)*b = \n";
    output_aligned(std::cout, (a/b)*b);

    std::cout << "\na*b/b = \n";
    output_aligned(std::cout, (a*b)/b);

    std::cout << std::endl;
    //std::cin.get();
}


void test_2_9 ()
{
    matrix<int> a;
    matrix<rational<> > b;
    random_int_matrix(a, 10, 10);
    random_int_matrix(b, 10, 10);
    b /= 10000;

    std::cout << "a = \n";
    output_aligned(std::cout, a);
    std::cout << "\nb = \n";
    output_aligned(std::cout, b);

    std::cout << "\ndet(a) = " << det(matrix<big_int>(a));
    std::cout << "\ndet(b) = " << det(b);

    std::cout << std::endl;
    //std::cin.get();
}


void test_2_10 ()
{
    matrix<big_int> a = "((1, 2), (3, 4), (5, 6))";
    matrix<rational<> > b = "((1/3, 2/3, 1), (2, 7/2, 20/9))";

    std::cout << "a = \n";
    output_aligned(std::cout, a);
    std::cout << "\nb = \n";
    output_aligned(std::cout, b);

    std::cout << "\nswapping\n";
    std::swap(a, b);

    std::cout << "\na = \n";
    output_aligned(std::cout, a);
    std::cout << "\nb = \n";
    output_aligned(std::cout, b);

    std::cout << std::endl;
    //std::cin.get();
}


void test_2_11 ()
{
    vector<rational<> > a = "(2/7, 12/9182734913857923597, 1/13,2034978247)";
    std::cout << "a = " << a;
    std::cout << "\nelements 1/13 has been found at " << std::find(a, rational<>(1, 13)) - a.begin();
    std::cout << "\nelement 2/13 has been fount at " << std::find(a, rational<>(2, 13)) - a.begin();

    std::cout << std::endl;
}


void test_2_12 ()
{
    typedef rational<> Base;

    matrix<Base> a;
    vector<Base> b = "(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)";
    vector<Base> x;

    random_int_matrix(a, 10, 10);

    std::cout << "\na = \n";
    output_aligned(std::cout, a);
    std::cout << "\nb = " << b;

    try
    {
        solve_linsys(a, b, x);
        std::cout << "\nx = \n";
        output_aligned(std::cout, x);
        std::cout << "\nsolution is valid: " << std::boolalpha << (a*x == b);
    }
    catch(const matrix_is_singular&)
    { std::cerr << "Sorry, the matrix is singular."; }

    std::cout << std::endl;
}


void test_2_13 ()
{
    vector<big_int, false> a = "(298374927, 4927289492, 298347, 29472, 293874239, 98734, 982734, 872)";

    for(int i = 0; i < 10000; ++i)
        -a;

    std::cout << std::endl;
}


void test_2_14 ()
{
    matrix<sparse_polynom<rational<> > >
        a = "((1/7*x^13+25*x^5-12/7, 8/3*x+14), (x-12, 1/17))";
    output_aligned(std::cout, a);

    std::cout << std::endl;
}


void test_2_15 ()
{
    big_int a = factorial(big_int(10000));

    {
        std::ofstream file("factorial(10000).txt");
        file << a;
    }

    big_int acopy;

    {
        std::ifstream file("factorial(10000).txt");
        file >> acopy;
    }

    std::cout << "big_int storing is valid: " << std::boolalpha << (a == acopy);

    std::cout << std::endl;
}


void test_2_16 ()
{
    const int nnums = 1000;
    const int base = 10000000;
    int lastprime = next_prime(base);

    std::cout << "First " << nnums << " prime numbers greater then "
        << base << ": " << lastprime;

    for(int i = 0; i < nnums - 1; ++i)
        std::cout << ", " << (lastprime = next_prime(lastprime));

    std::cout << std::endl;
}


void test_2_17 ()
{
    std::cout << next_prime(big_int("1000000000000"));
    std::cout << std::endl;
}


void test_2_18 ()
{
    for(big_int i = 987654321; i <= 987654321 + 5000; ++i)
    {
        std::cout << i << " = " << factorize(i);
        if(is_prime(i))std::cout << "; it is prime";
        std::cout << std::endl;
    }

    std::cout << std::endl;
}


void test_2_19 ()
{
    big_int a = "98550", b = "1458";
    a = next_prime(a); b = next_prime(b);

    std::cout
        << "a = " << a << "\nb = " << b
        << "\na*b = " << a*b
        << "\nfactorization: " << factorize(a*b);

    std::cout << std::endl;
}


void test_2_20 ()
{
    //matrix<sparse_polynom<rational<> > >
    //    a = "((x^12-5*x^2+x-13/7, 234-x^10+x^2), (13, x^20-x^19+x^17-x^16+x^15))",
    //    b, q;

    matrix<sparse_polynom<rational<> > >
        a = "((5*x^2+x-13/7, x^2), (13, x^17))",
        b, q;

    sparse_polynom<rational<> > det;

    //matrix<sparse_polynom<rational<> > >
    //    a = "((12, 234), (13, 15))",
    //    b, q;

    //sparse_polynom<sparse_polynom<rational<> > > det;

    //matrix<rational<> >
    //    a = "((12, 234), (13, 15))",
    //    b, q;

    //sparse_polynom<rational<> > det;

    vector<size_t> basis;
    std::cout << "a = \n";
    output_aligned(std::cout, a, "|| ", " ||");

    rref_int(a, b, q, basis, det);

    std::cout << "\na after rref_int = \n";
    output_aligned(std::cout, a, "|| ", " ||");
    std::cout << "\nb = \n";
    output_aligned(std::cout, b, "|| ", " ||");
    std::cout << "\nq = \n";
    output_aligned(std::cout, q, "|| ", " ||");
    std::cout << "\nbasis = \n";
    output_aligned(std::cout, basis, "|| ", " ||");
    std::cout << "\ndet = " << det;
    std::cout << std::endl;
}


template <typename F, typename I, bool REFCNT>
void random_int_sparse_polynom
(sparse_polynom<F, I, REFCNT>& p, size_t maxdegree, double nzmonoms)
{
    size_t degree = double(std::rand())/RAND_MAX * maxdegree;
    p = monom<F, I>(std::rand(), degree);

    for(size_t i = 0; i < nzmonoms*degree; ++i)
    {
        F coef = std::rand();
        I dergee = double(std::rand())/RAND_MAX * degree;
        p += monom<F, I>(coef, dergee);
    }
}


void test_2_21 ()
{
    vector<sparse_polynom<int> > vp(100);
    for(size_t i = 0; i < vp.size(); ++i)
        random_int_sparse_polynom(vp[i], 30, 0.3);

    output_aligned(std::cout, vp);

    sparse_polynom<big_int> pm1 =
        product(vector<sparse_polynom<big_int> >(vp));

    std::cout << "\n";
    std::sort(vp.begin(), vp.end());
    output_aligned(std::cout, vp);

    sparse_polynom<big_int> pm2 =
        product(vector<sparse_polynom<big_int> >(vp));

    std::cout << "\n" << pm1 << "\n\nis valid: " << (pm1 == pm2);
    std::cout << std::endl;
}


void test_2_22 ()
{
    vector<char> s1 = "(a, b, c, d, e, f, g)", s2 = "(c, d, e)";

    std::cout << "s1 = " << s1 << "\ns2 = " << s2;
    std::cout << "\nsearch(s1, s2) - s1.begin() = " <<
        std::search(s1, s2) - s1.begin();
    std::cout << std::endl;
}

#if 0

template <typename T = type_traits<T> >
struct Quadratic_equation_solution
{
    // Represents solution of quadratic equation with rational coefficients
    // as a+d*sqrt(c), where a, d is rational<T>, c is positive integer T.
    // Where d may have a form b*i, where i is imaginary unit.

    Quadratic_equation_solution
    (
        const rational<T>& ta = rational<T>(),
        const rational<T>& tb = rational<T>(),
        bool ti = false,
        const T& tc = unit<T>()
    ) : a(ta), b(tb), i(ti), c(tc) {}

    rational<T> a, b;
    T c;    // if b != 0 then c != 0, if b == 0 then c is any
    bool i;    // if imaginary unit is in solution
};


template <typename T>
inline std::ostream& operator<<
(
    std::ostream& out,
    const Quadratic_equation_solution<T>& x
)
{
    assert(x.b.is_null() || !is_null(x.c));

    if(!x.a.is_null())out << x.a;

    if(!x.b.is_null())
    {
        if(x.b.is_unit())
        {
            if(!x.a.is_null())out << "+";
            if(x.i)
            {
                out << "i";
                if(!is_unit(x.c))out << "*";
            }
        }
        else if(x.b.is_opposite_unit())
        {
            out << "-";
            if(x.i)
            {
                out << "i";
                if(!is_unit(x.c))out << "*";
            }
        }
        else
        {
            out << std::showpos << x.b << std::noshowpos;
            if(x.i)out << "*i";
            if(!is_unit(x.c))out << "*";
        }

        if(!is_unit(x.c))out << "sqrt(" << x.c << ")";
    }

    if(x.a.is_null() && x.b.is_null())
        out << "0";

    return out;
}


void test_2_23()
{
    std::cout
        << "\n" << Quadratic_equation_solution<big_int>()
        << "\n" << Quadratic_equation_solution<big_int>(1)
        << "\n" << Quadratic_equation_solution<big_int>(1, 1, false, 3)
        << "\n" << Quadratic_equation_solution<big_int>(1, 1, true)
        << "\n" << Quadratic_equation_solution<big_int>(0, -1, true)
        << "\n" << Quadratic_equation_solution<big_int>(0, -1, false, 5)
        << "\n" << Quadratic_equation_solution<big_int>(0, -1, true)
        << "\n" << Quadratic_equation_solution<big_int>(0, +1, false, 2)
        << "\n" << Quadratic_equation_solution<big_int>(2, -1, true)
        << "\n" << Quadratic_equation_solution<big_int>(0-5, +1, false, 2)
        << "\n" << Quadratic_equation_solution<big_int>(2, -3, true)
        << "\n" << Quadratic_equation_solution<big_int>(-5, +5, false, 2);
    std::cout << std::endl;
}

#endif

template <typename T, bool REFCNT>
void random_int_matrix_rs (matrix<T, REFCNT>& res, size_t maxrows, size_t maxcols)
{
    size_t rows = std::rand()%maxrows + 1;
    size_t cols = std::rand()%maxcols + 1;
    res.assign_fromsize(rows, cols);
    std::generate(res.begin(), res.end(), std::rand);
}


void test_2_24 ()
{
    vector<matrix<int> > vmi(10);
    for(size_t i = 0; i < vmi.size(); ++i)
        random_int_matrix_rs(vmi[i], 3, 3);

    output_aligned(std::cout << "\nvmi before sorting:\n", vmi);
    std::sort(vmi);
    output_aligned(std::cout << "\nvmi after sorting:\n", vmi);
    std::cout << std::endl;
}

template <typename T>
struct Risp
{
    sparse_polynom<T> operator() () const
    {
        sparse_polynom<T> p;
        random_int_sparse_polynom(p, 20, 0.3);
        return p;
    }

};

namespace
{
    int _my_counter_1 = 0, _my_counter_2 = 0;
}


void test_2_25 ()
{
    matrix<sparse_polynom<rational<> > > a(3u, 3u), b, p, q;
    std::generate(a.begin(), a.end(), Risp<rational<> >());
    output_aligned(std::cout << "a = \n", a);
    size_t rank; sparse_polynom<rational<> > det;

    Timing tm;
    smith(a, b, p, q, rank, det/*, ctrl::make_smith_slog(std::cout)*/);
    std::cout << "\nsmith is done, time = " << tm.time();
    std::cout
        << "\nall calls: " << _my_counter_1 + _my_counter_2
        << "\nfast calls: " << _my_counter_1
        << "\nrelative fast calls: " << double(_my_counter_1) / (_my_counter_1 + _my_counter_2);

    // normalization pass, actually smith does not doing this
    for(size_t i = 0; i < b.nrows() && !b(i, i).is_null(); ++i)
    {
        p.div_row(i, b(i, i).leading_coef());
        b(i, i) /= b(i, i).leading_coef();
    }

    std::ofstream smithout("smith_proof.txt");
    output_aligned(smithout << "\nb = \n", b, "|| ", " ||", "  ");
    output_aligned(smithout << "\np = \n", p, "|| ", " ||", "  ");
    output_aligned(smithout << "\nq = \n", q, "|| ", " ||", "  ");
    //std::cout << "\nrank = " << rank << "\ndet = " << det;

    //matrix<sparse_polynom<rational<> > > paq = p*a*q;
    //output_aligned(std::cout << "\np*a*q = \n", paq);
    //std::cout << "\nb == p*a*q: " << std::boolalpha << (b == paq);

    //std::cout << std::endl;
}



int test2 ()
{
    //test_2_1();
    //test_2_2();
    //test_2_3();
    //test_2_4();
    //test_2_5();
    //test_2_6();
    //test_2_7();
    //test_2_8();
    //test_2_9();
    //test_2_10();
    //test_2_11();
    //test_2_12();
    //test_2_13();
    //test_2_14();
    //test_2_15();
    //test_2_16();
    //test_2_17();
    //test_2_18();
    //test_2_19();
    //test_2_20();

    //test_2_21();
    //test_2_22();
    ////test_2_23();
    //test_2_24();

    test_2_25();

    return 0;
}

}

