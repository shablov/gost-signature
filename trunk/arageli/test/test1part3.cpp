/*****************************************************************************

    test/test1part3.cpp

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


template <typename T>
struct If_in_interval_0_1
{
    bool operator() (const T& x) const
    {
        return x > 0 && x < 1;
    }
};


void test1_34 ()
{
    const int n = 10;
    typedef vector<rational<int> > Nums;
    Nums nums(n*n);
    int i = 0;

    for(int num = 1; num <= n; ++num)
        for(int den = 1; den <= n; ++den)
            nums[i++] = Nums::value_type(num, den);

    std::sort(nums);

    nums.erase
    (
        std::partition
        (
            nums.begin(), std::unique(nums),
            If_in_interval_0_1<Nums::value_type>()
        ),
        nums.end()
    );

    vector<double> b = nums;

    output_aligned(std::cout, nums);
    output_aligned(std::cout, b);

    std::cout << std::endl;
}


void test1_35 ()
{
    matrix<big_int> a = "((1, 2, 3), (4, 5, 6))";
    matrix<rational<big_int> > b = "((10, 9), (8, 7), (6, 5), (4, 3))";

    output_aligned(std::cout << "\na = \n", a);
    output_aligned(std::cout << "\nb = \n", b);
    std::cout << "\nSwapping...";
    std::swap(a, b);
    output_aligned(std::cout << "\na = \n", a);
    output_aligned(std::cout << "\nb = \n", b);
    std::cout << "\nTraspose...";
    a.transpose(); b.transpose();
    output_aligned(std::cout << "\na = \n", a);
    output_aligned(std::cout << "\nb = \n", b);

    std::cout << std::endl;
}


void test1_36 ()
{
    matrix<rational<> > a = "((1, 2, 3), (4, 5, 6), (7, 8, 9))", b = a;

    output_aligned(std::cout << "\na = \n", a);

    a.mult_row(0, 2);
    output_aligned(std::cout << "\na = \n", a);

    a.div_row(2, 3);
    output_aligned(std::cout << "\na = \n", a);

    a.add_rows(0, 1);
    output_aligned(std::cout << "\na = \n", a);

    a.sub_rows(2, 1);
    output_aligned(std::cout << "\na = \n", a);

    a.addmult_rows(1, 0, 10);
    output_aligned(std::cout << "\na = \n", a);

    a.addmult_rows(0, 0, 1);
    output_aligned(std::cout << "\na = \n", a);

    std::swap(a, b);

    std::cout << "\n+++++++++++++++++++++++++\n";

    a.transpose();

    a.mult_col(0, 2);
    output_aligned(std::cout << "\na = \n", a);

    a.div_col(2, 3);
    output_aligned(std::cout << "\na = \n", a);

    a.add_cols(0, 1);
    output_aligned(std::cout << "\na = \n", a);

    a.sub_cols(2, 1);
    output_aligned(std::cout << "\na = \n", a);

    a.addmult_cols(1, 0, 10);
    output_aligned(std::cout << "\na = \n", a);

    a.addmult_cols(0, 0, 1);
    output_aligned(std::cout << "\na = \n", a);

    a.transpose();

    output_aligned(std::cout << "\na = \n", a);

    std::cout << std::endl;
}


//template <typename P> struct Dergee_expansion
//{
//    big_int operator() (const P& x) const
//    { return x.is_null() ? -1 : x.degree(); }
//};


template <typename P> struct Less_degree : std::binary_function<P, P, bool>
{
    bool operator() (const P& a, const P& b) const
    {
        if(a.is_null())return false;
        if(b.is_null())return !a.is_null();
        return a.degree() < b.degree();
    }
};


template <typename P>
matrix<P> canonical_matrix (matrix<P> a)
{
    ARAGELI_ASSERT_0(a.is_square());

    if(a.is_empty())return a;

    if(a.size() == 1)
    {
        P& item = a(0, 0);
        if(!item.is_null())a /= item.leading_coef();
        return a;
    }

    ARAGELI_ASSERT_1(a.size() > 1);

    typedef typename matrix<P>::iterator Iter;
    typedef typename matrix<P>::size_type Size;
    typedef typename P::coef_type Coef;

    Size size = a.ncols();

    output_aligned(std::cerr << "\na from canonical_matrix = \n", a, "|| ", " ||");

    for(Size cur = 0; cur < size - 1; ++cur)
    {
        Iter p = std::min_element
        (
            a.begin() + cur*(size + 1), a.end(),
            Less_degree<P>()
        );

        if(p->is_null())return a;

        Size
            ii = (p - a.begin())/size,
            jj = (p - a.begin())%size;

        std::cerr << "\n(ii, jj) = (" << ii << ", " << jj << ")\n";
        output_aligned(std::cerr << "\na from canonical_matrix = \n", a, "|| ", " ||");

        a.swap_rows(cur, ii);
        a.swap_cols(cur, jj);

        output_aligned(std::cerr << "\na from canonical_matrix = \n", a, "|| ", " ||");

        p = a.begin() + cur*(size + 1);    // it is a(cur, cur)

        ARAGELI_ASSERT_1(!p->is_null());

        Coef lc = p->leading_coef();
        a.div_row(cur, lc);
        ARAGELI_ASSERT_1(is_unit(p->leading_coef()));

        output_aligned(std::cerr << "\na from canonical_matrix = \n", a, "|| ", " ||");

        for(Size j = cur + 1; j < size; ++j)
        {
            std::cerr << "\na(cur, j) % *p  =  a(" << cur << ", " << j << ") % " << *p << "  =  " << a(cur, j) << " % " << *p << "  =  " << a(cur, j) % *p << "\n";

            ARAGELI_ASSERT_1(is_null(a(cur, j) % *p));
            a.addmult_cols(j, cur, -(a(cur, j) / *p));
            ARAGELI_ASSERT_1(is_null(a(cur, j)));
        }

        for(Size i = cur + 1; i < size; ++i)
        {
            std::cerr << "\na(i, cur) % *p  =  a(" << i << ", " << cur << ") % " << *p << "  =  " << a(i, cur) << " % " << *p << "  =  " << a(i, cur) % *p << "\n";

            ARAGELI_ASSERT_1(is_null(a(i, cur) % *p));
            a.addmult_rows(i, cur, -(a(i, cur) / *p));
            ARAGELI_ASSERT_1(is_null(a(i, cur)));
        }
    }

    output_aligned(std::cerr << "\na from canonical_matrix = \n", a, "|| ", " ||");

    Iter p = a.end() - 1;
    if(!p->is_null())*p /= p->leading_coef();

    return a;
}


void test1_37 ()
{
    typedef matrix<sparse_polynom<rational<> > > M;

    M a = "((x^3-x, 2*x^2), (x^2+5*x, 3*x))";
    output_aligned(std::cout << "\na = \n", a);
    std::cout << "\ndet(a) = " << det_int(a) << "\n";
    M ca = canonical_matrix(a);
    output_aligned(std::cout << "\nca = \n", ca);
    std::cout << "\ndet(ca) = " << det_int(ca) << "\n";

    M b = "((x, x^3+5), (x^2-x-4, x^4-x^3-4*x^2+5*x-5))";
    output_aligned(std::cout << "\nb = \n", b);
    std::cout << "\ndet(b) = " << det_int(b) << "\n";
    M cb = canonical_matrix(b);
    output_aligned(std::cout << "\ncb = \n", cb);
    std::cout << "\ndet(cb) = " << det_int(cb) << "\n";

    std::cout << std::endl;
}


void test1_38 ()
{
    matrix<sparse_polynom<rational<> > >
        a = "((x, x^2, x^3), (1, 2*x, 3*x^2), (0, 2, 6*x))";

    output_aligned(std::cout << "\na = \n", a);
    std::cout << "\ndet(a) = " << det_int(a);

    std::cout << std::endl;
}


void test1_39 ()
{
    vector<int> a(10, next_prime(100000)), c;
    big_int b;
    factorize(product(a, b), c);
    std::cout << a << "\n" << b << "\n" << c;
    std::cout << "\nIt is valid: " << (a == c);

    std::cout << std::endl;
}


void test1_40 ()
{
    std::cout << is_prime_division(next_prime(big_int("1234567890")));

    std::cout << std::endl;
}


template <typename T, typename B>
vector<T> power_iterations (const matrix<B>& a, const T& c, size_t numiters)
{
    assert(a.is_square());
    assert(!a.is_empty());

    output_aligned(std::cout << "\na = \n", a);

    size_t n = a.nrows();

    vector<T> p(n);

    for(size_t i = 0; i < n; ++i)
    {
        T nnz = std::count
        (
            a.begin() + i*n, a.begin() + (i+1)*n,    // elements in i-th row
            true
        );

        if(nnz)p[i] = T(1)/nnz;
    }

    output_aligned(std::cout << "\np = \n", p);

    matrix<T> e(n, 1, 1);
    matrix<T> vt(n, 1, T(1)/T(n));
    matrix<T> d(n, 1, fromsize);
    vt.transpose();

    for(size_t i = 0; i < n; ++i)
        if(p[i] == 0)d(i, 0) = 1;

    output_aligned(std::cout << "\ne = \n", e);
    output_aligned(std::cout << "\nvt = \n", vt);
    output_aligned(std::cout << "\nd = \n", d);

    matrix<T> pppt(n);

    for(size_t i = 0; i < n; ++i)
        for(size_t j = 0; j < n; ++j)
            if(a(i, j))pppt(i, j) = p[i];

    output_aligned(std::cout << "\npp = \n", pppt);

    output_aligned(std::cout << "\nd*vt = \n", d*vt);
    output_aligned(std::cout << "\nppp + d*vt = \n", pppt + d*vt);
    output_aligned(std::cout << "\nc*(ppp + d*vt) = \n", matrix<T>(n, c, diag)*(pppt + d*vt));
    output_aligned(std::cout << "\n(1 - c)*e*vt = \n", matrix<T>(n, 1- c, diag)*e*vt);

    pppt = matrix<T>(n, c, diag)*(pppt + d*vt) + matrix<T>(n, 1 - c, diag)*e*vt;

    output_aligned(std::cout << "\nppp = \n", pppt);
    pppt.transpose();
    output_aligned(std::cout << "\npppt = \n", pppt);

    vector<T> pr = vector<T>(vt.size(), vt.begin());
    output_aligned(std::cout << "\ninitial vector = \n", pr);

    for(size_t i = 0; i < numiters; ++i)
    {
        vector<T> y = pppt*pr;
        std::cout << "\ni = " << i
            << ", delta = " << sum(std::abs(pr - y))
            << "\nfdelta = " << double(sum(std::abs(pr - y)));
        pr = y;
    }

    return pr;
}


template <typename B>
void input_sparse_matrix_from_text_file (std::istream& in, matrix<B>& res)
{
    size_t numrows, numcols, numnzs;
    in >> numrows >> numcols >> numnzs;
    if(!in)return;

    res.assign_fromsize(numrows, numcols);

    size_t nzi = 0;
    for(size_t i = 0; i < numrows; ++i)
    {
        size_t lnlen;
        in >> lnlen;
        if(!in)return;

        assert(nzi + lnlen <= numnzs);

        for(size_t j = 0; j < lnlen; ++j)
        {
            size_t t;
            in >> t;
            if(!in)return;
            res(i, t) = 1;
            ++nzi;
        }
    }

    assert(nzi == numnzs);
}


void test1_41 ()
{
    //matrix<bool> a =
    //    "((1, 1, 0),"
    //    " (0, 1, 1),"
    //    " (1, 0, 1))";

    matrix<bool> a;
    std::ifstream file("../../webgraph/webgraph/lz77_test.in.matrix.txt");
    input_sparse_matrix_from_text_file(file, a);

    vector<rational<> > pr = power_iterations(a, rational<>(85, 100), 100);
    output_aligned(std::cout << "\n\nPageRank = \n", pr);
    output_aligned(std::cout << "\n\nPageRank = \n", vector<double>(pr));

    std::cout << std::endl;
}


void test1_42 ()
{
    sparse_polynom<rational<big_int> > p = "2*x^12-x+23-12*x^2+x^7";
    std::cout << "\np = " << p << "\ndiff(p) = " << diff(p);
    std::cout << "\ngcd(p, diff(p)) = " << gcd(p, diff(p));

    std::cout << std::endl;
}


//void test1_43 ()
//{
//    vector<matrix<double> > v =
//        "(((1, 2), (4, 7)), ((1, 2), (2, 0.4)), ((0, 0), (1, 60)), ((1, 2), (3, 4)), ((0, 0), (-1, 0), (0, 2)))";
//
//    for(size_t i = 0; i < v.size(); ++i)
//    {
//        std::cout << "\nv[" << i << "] =\n";
//        output_aligned(std::cout, v[i]);
//        if(v[i].is_square())std::cout << "\ndet = " << det(v[i]);
//        std::cout << "\nrank = " << rank(v[i]) << std::endl;
//    }
//}


void test1_44 ()
{
    matrix<double>
        m1 = "((1, 2, 3), (4234.222, 5, 6))",
        m2 = "((7, 8, 9000.1))";

    output_aligned_ver_pair(std::cout, m1, m2);

    m1.transpose();
    m2.transpose();

    output_aligned_hor_pair(std::cout << '\n', m1, m2);

    matrix<int> m3 = "((111), (2222), (3333333))";

    output_aligned_corner_triplet_br(std::cout << '\n', m3, m2, m1);
}


//void test1_45 ()
//{
//    matrix<rational<> > m = "((1))";
//    std::cout << det(m) << " " << det_int(matrix<double>(m));
//}

void test1_46 ()
{
    matrix<sparse_polynom<int> > a = "((x, 1, 0), (0, x, 1), (0, 0, x))";
    output_aligned(std::cout, a, "|| ", " ||", " ");
}


void test1_47 ()
{
    typedef rational<sparse_polynom<rational<> > > RF;
    RF
        a1 = "x",
        a2 = "0",
        a3 = "1",
        a4 = "1/3",
        a5 = "x-2*x^4",
        a6 = "x/(x^6-12*x+5)",
        a7 = "(x-2)/(x^2-4*x+4)";

    std::cout
        << "a1 = " << a1 << std::endl
        << "a2 = " << a2 << std::endl
        << "a3 = " << a3 << std::endl
        << "a4 = " << a4 << std::endl
        << "a5 = " << a5 << std::endl
        << "a6 = " << a6 << std::endl
        << "a7 = " << a7 << std::endl;

    std::cout << "a1 + a4 = " << a1 + a4 << std::endl;
    std::cout << "a1 * a4 = " << a1 * a4 << std::endl;
    std::cout << "a1 / a4 = " << a1 / a4 << std::endl;
    std::cout << "a5 / a6 = " << a5 / a6 << std::endl;
    std::cout << "a5 * a6 = " << a5 * a6 << std::endl;
    std::cout << "a6 * x^6-12*x+5 = " << a6 * "x^6-12*x+5" << std::endl;
    //std::cout << "a1 + a4" << a1 + a4 << std::endl;
    //std::cout << "a1 + a4" << a1 + a4 << std::endl;
    //std::cout << "a1 + a4" << a1 + a4 << std::endl;

}


void test1_48 ()
{
    Arageli::big_int a, b, c;
    std::cin >> a >> b;
    c = (a+1)*(b+1);
    std::cout << c << "\n";
}


void test1_49 ()
{
    typedef sparse_polynom<rational<> > P;
    std::cout << gcd(P("1"), P("2")) << '\n';
    std::cout << gcd(P("0"), P("2")) << '\n';
    std::cout << gcd(P("1"), P("0")) << '\n';
    std::cout << gcd(P("x"), P("2")) << '\n';
    std::cout << gcd(P("x^2"), P("2")) << '\n';
    std::cout << gcd(P("x^2"), P("x")) << '\n';
    std::cout << gcd(P("x"), P("x^2")) << '\n';
    std::cout << gcd(P("x^12-x+1"), P("x^12-x+1")) << '\n';
    std::cout << gcd(P("x^2+2*x+1"), P("x+1")) << '\n';
    //std::cut << gcd(P("1"), P("2")) << '\n';
    //std::cut << gcd(P("1"), P("2")) << '\n';
}


void test1_50 ()
{
    typedef sparse_polynom<sparse_polynom<sparse_polynom<rational<> > > > P3v;
    P3v p1 = "(x+(x-(x)))";
    std::cout << p1 << "(x+(x-(x)))^5 = " << std::pow(p1, 5) << '\n';
    P3v q, r;
    divide(std::pow(p1, 5) + P3v("((x))"), std::pow(p1, 3), q, r);
    std::cout << "q = " << q << "\nr = " << r;
}


void test1_51 ()
{
    typedef sparse_polynom<sparse_polynom<rational<> > > P2v;
    P2v p = "((x)-x^2)";    // (x - y^2)
    p *= p;    // (x - y^2)^2 = x^2 - 2*x*y^2 + y^4
    std::cout << p << '\n';
    p = p.subs(P2v("((x))"));
    std::cout << p;
}


//void test1_52 ()
//{
//    typedef sparse_polynom<sparse_polynom<rational<> > > P2v;
//    typedef rational<P2v> RP2v;
//    typedef matrix<RP2v> MRP2v;
//
//    MRP2v a =
//        "((  ((x)) , (((0)))  ),"
//        "(  (((0))), (((x)))  ))";
//
//    //a(0, 0) = RP2v("((x))");
//    //a(1, 1) = RP2v("(((x)))");
//
//    output_aligned(std::cout << "\na = \n", a);
//    std::cout << "\ndet(a) = " << det(a);
//    output_aligned(std::cout << "\ninverse(a) = \n", inverse(a));
//}


void test1_53 ()
{
    sparse_polynom<int> a, b;
    for(;;)
        std::cin >> a >> b, std::cout << cmp(a, b) << '\n';
}


//void test1_54 ()
//{
//    typedef
//        sparse_polynom<
//        sparse_polynom<
//        sparse_polynom<
//        sparse_polynom<
//        sparse_polynom<
//        sparse_polynom<
//        rational<> > > > > > > P6v;
//
//    typedef rational<P6v> RP6v;
//    typedef matrix<RP6v> MRP6v;
//    typedef vector<RP6v> VRP6v;
//
//    MRP6v a =
//        "((   ((x))  ,   (((x)))   ),"
//        "(  ((((x)))), (((((x))))) ))";
//
//    VRP6v b = "( ((((((x)))))), (((((((x))))))) )";
//
//    output_aligned(std::cout << "\na = \n", a);
//    output_aligned(std::cout << "\nb = \n", b);
//    MRP6v inv_a = inverse(a);
//    output_aligned(std::cout << "\ninverse of a = \n", inv_a);
//    VRP6v res1 = inv_a * b;
//    output_aligned(std::cout << "\nres1 = \n", res1);
//    VRP6v res2 = solve_linsys(a, b);
//    output_aligned(std::cout << "\nres2 = \n", res2);
//    std::cout << "\nsolution is equal: " << std::boolalpha << (res1 == res2);
//    std::cout << "\nthe first solution is valid: " << (a*res1 == b);
//    std::cout << "\nthe second solution is valid: " << (a*res2 == b);
//}


void test1_55 ()
{
    for(int t = 0; t < 10; ++t)
    {
        double ct = 0;

        for(int s = 0; s < 10; ++s)
        {
            rational<big_int>
                a = rational<>
                    (
                        big_int::random_with_length_or_less((t+1)*32),
                        big_int::random_with_length_or_less((t+1)*32)
                    ),
                b = rational<>
                    (
                        big_int::random_with_length_or_less((t+1)*32),
                        big_int::random_with_length_or_less((t+1)*32)
                    );

            Timing tm;

            for(int i = 0; i < 10000; ++i)
                a + b;

            ct += tm.time();
        }

        std::cout << "\nt = " << t + 1 << "\t time = " << ct;
    }
}


void test1_56 ()
{
    // ************ WARNING! Please uncomment this. ***************

    //typedef std::complex<rational<big_int> > CRBI;
    //
    //CRBI
    //    a("2039875902357", "29287349872394"),
    //    b("1111", "9824987948721984729234");

    //std::cout
    //    << "\na = " << a << "\nb = " << b
    //    << "\na + b = " << a+b << "\na*b = " << a*b
    //    << "\na/b = " << a/b;

    //sparse_polynom<CRBI> aa = "x^3+(0,1)", bb = "((0,1)*x^5+2*x)";
    //aa.leading_coef() = a;
    //bb -= sparse_polynom<CRBI>::monom(b, 4);

    //std::cout << "\naa = " << aa << "\nbb = " << bb;
    //std::cout << "\naa + bb = " << aa + bb << "\naa*bb = " << aa*bb;

}


void test1_57 ()
{
    sparse_polynom<rational<> > a = "1/7+12*x^23-23/9873*x+x^2";
    std::cout << is_primitive(a);
}


void test1_58 ()
{
    sparse_polynom<rational<> > a = "1/7+12*x^23-23/9873*x+x^2";
    std::cout << is_primitive(a);
}


void test1_59 ()
{
    big_int a = "35278925";
    //big_int a = 10;
    sparse_polynom<rational<> > p;
    for(size_t i = 0; i < a.length(); ++i)
        if(a.bit(i))p += sparse_polynom<rational<> >::monom(1, i);

    //std::cout << "\np = " << p << "\ndiff p = " << diff(sparse_polynom<bool>(p)) <<
    //    "\ngcd = " << gcd(p, sparse_polynom<rational<> >(diff(sparse_polynom<bool>(p))));
}


void test1_60 ()
{
    unsigned long long a = next_prime(487198273);
    unsigned long long b = next_prime(722347161);
    unsigned long long ab = a*b;
    std::cout << "time";
    std::cin.get();
    std::cout << ab << " = " << factorize(ab);
}


void test1_61 ()
{
    // правильные остатки
    std::cout
        << '\n' << (-1) % 4
        << '\n' << (-2) % 4
        << '\n' << (-5) % 4
        << '\n' << big_int(-1) % big_int(4)
        << '\n' << big_int(-2) % big_int(4)
        << '\n' << big_int(-5) % big_int(4);
}


void test1_62 ()
{
    sparse_polynom<int> a = 5;
    //sparse_polynom_input_var(std::cin, a, "x", "");
    std::cout << a << std::endl;
}


void test1_63 ()
{
    std::cout
        << "\nJ(1209478239875929287349875936, 52349124378234343) = "
            << jacobi(big_int("1209478239875929287349875936"), big_int("52349124378234343"))
        << "\nJ(136, 53) = " << jacobi(136, 53)
        << "\nJ(1, 99) = " << jacobi(1, 99);
}

void test1_64 ()
{
    typedef big_int T;

    for(T n = 2; n < 100; n = next_prime(n))
        for(T a = 1; a < n; ++a)
        {
            T r = inverse_mod(a, n);
            std::cout
                << "\n" << a << "^(-1) (mod " << n << ") = "
                << r;

            if(r*a % n != 1)
                std::cerr << "\n!!! ERROR !!!: a = " << a << ", n = " << n;
        }
}


void test1_65 ()
{
    typedef big_int T;
    T end = big_int("100000000000000000000001000");

    for(T i = "100000000000000000000000000"; i < end; ++i)
    {
        std::cout << "\nsqrt(" << i << ") = " << std::sqrt(i);
    }
}


}
