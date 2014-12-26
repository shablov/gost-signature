/*****************************************************************************

    test/test1part4.cpp

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


enum simplex_method_result { SMR_OK, SMR_INCOMPATIBLE, SMR_INFINITE };


template <typename T, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& my_output_latex
(
    std::basic_ostream<Ch, ChT>& out, const T& x,
    bool mathmode = false
)
{ return out << x; }


template <typename T, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& my_output_latex
(
    std::basic_ostream<Ch, ChT>& out,
    const rational<T>& x,
    bool mathmode = false,
    size_t msaf = 5
)
{
    typedef std::basic_string<Ch, ChT> S;

    std::basic_ostringstream<Ch, ChT> buf;
    my_output_latex(buf, x.numerator(), false);
    S snumer = buf.str();

    if(x.is_integer())
    {
        if(mathmode)
            out << "$" << snumer << "$";
        else
            out << snumer;

        return out;
    }

    buf.str(S());
    my_output_latex(buf, x.denominator(), false);
    S sdenom = buf.str();

    if(mathmode)out << "$";
    if(snumer.length() + sdenom.length() <= msaf)
        out << "{" << snumer << "}/{" << sdenom << "}";
    else
        out << "\\frac{" << snumer << "}{" << sdenom << '}';
    if(mathmode)out << "$";
    return out;
}


template <typename T, bool REFCNT, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& my_output_latex
(
    std::basic_ostream<Ch, ChT>& out,
    const matrix<T, REFCNT>& x,
    bool mathmode = false,
    size_t msaf = 5
)
{
    typedef std::basic_string<Ch, ChT> S;

    //std::basic_ostringstream<Ch, ChT> buf;
    //buf << x.numerator();

    out << "\\begin{pmatrix}";

    for(size_t i = 0; i < x.nrows(); ++i)
    {
        if(i != 0)out << " \\\\ ";
        for(size_t j = 0; j < x.ncols(); ++j)
        {
            if(j != 0)out << " & ";
            my_output_latex(out, x(i, j), false);
        }
    }

    out << "\\end{pmatrix}";
    return out;
}


template <typename T, bool REFCNT, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_simplex_table_latex
(
    std::basic_ostream<Ch, ChT>& out,
    const matrix<T, REFCNT>& x,
    bool mathmode = false,
    size_t pivot_row = std::numeric_limits<size_t>::max(),
    size_t pivot_col = std::numeric_limits<size_t>::max()
)
{
    typedef std::basic_string<Ch, ChT> S;

    //std::basic_ostringstream<Ch, ChT> buf;
    //buf << x.numerator();

    out << " \\left( \\begin{tabular}{c|" << S(x.ncols()-1, 'c') << "c}";

    for(size_t i = 0; i < x.nrows(); ++i)
    {
        if(i != 0)out << " \\\\ ";
        if(i == 1)out << "\\hline ";
        for(size_t j = 0; j < x.ncols(); ++j)
        {
            if(j != 0)out << " & ";
            bool pivoting = (i == pivot_row && j == pivot_col);
            if(pivoting)out << "\\fbox{";
            my_output_latex(out, x(i, j), true);
            if(pivoting)out << "}";
        }
    }

    out << "\\end{tabular} \\right) ";
    return out;
}


template <typename T, bool REFCNT, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_basis_latex
(
    std::basic_ostream<Ch, ChT>& out,
    const vector<T, REFCNT>& x
)
{
    out << "\n$$\n {\\cal B}=\\left<";
    output_list(out, x, "", "");
    out << "\\right>\n$$";
    return out;
}


template
<
    typename Tc, bool REFCNTc,    // vector c
    typename Ta, bool REFCNTa,    // matrix a
    typename Tb, bool REFCNTb,    // vector b
    typename T,
    typename Tx, bool REFCNTx        // vector x
>
simplex_method_result maximize_simplex_method_l
(
    const vector<Tc, REFCNTc>& c,
    const matrix<Ta, REFCNTa>& a,
    const vector<Tb, REFCNTb>& b,
    T& res,
    vector<Tx, REFCNTx>& x    // may be the same as c
)
{
    ARAGELI_ASSERT_0(c.size() > 0);
    ARAGELI_ASSERT_0(c.size() == a.ncols());
    ARAGELI_ASSERT_0(a.nrows() == b.size());

    std::cout << "Решаем ЗЛП, заданную в канонической форме:";
    std::cout <<
        "\n$$"
        "\\max(cx)$$ $$"
        "\\begin{cases}"
        "Ax=b \\\\ x\\ge 0"
        "\\end{cases}"
        "$$\n";
    std::cout << "где\n$$";
    output_list(std::cout << "c=", c);
    std::cout << "\n$$\n$$\n";
    my_output_latex(std::cout << "A=", a);
    std::cout << "\n$$\n$$\n";
    output_list(std::cout << "b=", b);
    std::cout << "^T\n$$\n";
    std::cout << std::flush;

    // Stage 0.  Building of the first allowable plan.

    typedef matrix<T, false> ST;    // a simplex table
    typedef typename ST::size_type size_type;
    size_type n = a.ncols(), m = a.nrows();
    ST st = a;
    st.insert_col(0, b);
    st.insert_cols(1, m, factory<T>::null());
    st.insert_row(0, factory<T>::null());
    std::fill_n(st.begin() + 1, m, factory<T>::unit());

    for(size_type i = 1; i <= m; ++i)
    {
        st(i, i) = factory<T>::unit();
        st.sub_rows(0, i);
    }

    vector<size_type> basis(m);
    for(size_type i = 1; i <= m; ++i)
        basis[i-1] = i;

    std::cout << "Строим начальную допустимую базу и начальную симплекс-таблицу методом"
        " искусственного базиса.\\par";
    //std::cout << "\n$$\n";
    //output_simplex_table_latex(std::cout, st);
    //std::cout << "\n$$\n";
    //output_basis_latex(std::cout, basis);

    for(;;)
    {
        size_type s = std::find_if
        (
            st.begin() + 1, st.begin() + st.ncols(),
            func::is_negative<T>()
        ) - st.begin();

        if(s == st.ncols())break;

        vector<vector<T, false>, false> norm_rows(m);
        vector<size_type, false> positive_rows(m);
        size_type npr = 0;    // number of positive rows

        for(size_type i = 1; i <= m; ++i)
        {
            const T& cur = st(i, s);
            if(is_positive(cur))
            {
                positive_rows[npr] = i;
                vector<T, false>& cv = norm_rows[npr];
                cv.resize(st.ncols());
                for(size_type j = 0; j < st.ncols(); ++j)
                    cv[j] = st(i, j)/cur;
                ++npr;
            }
        }

        ARAGELI_ASSERT_1(npr > 0);

        size_type r = positive_rows
        [
            min_element (norm_rows.begin(), norm_rows.begin() + npr) -
                norm_rows.begin()
        ];

        std::cout << "\n$$\n";
        output_simplex_table_latex(std::cout, st, false, r, s);
        std::cout << "\n$$\n";
        output_basis_latex(std::cout, basis);

        //std::cout << "Направляющий элемент: $(r = " << r << ", s = " << s << ")$.";

        ARAGELI_ASSERT_1(is_positive(st(r, s)));

        T t = st(r, s);
        st.div_row(r, t);

        for(size_type i = 0; i <= m; ++i)
        {
            if(i == r)continue;
            st.addmult_rows(i, r, -st(i, s));
        }

        basis[r - 1] = s;

    }

    std::cout << "\n$$\n";
    output_simplex_table_latex(std::cout, st, false);
    std::cout << "\n$$\n";
    output_basis_latex(std::cout, basis);

    if(is_negative(st(0, 0)))return SMR_INCOMPATIBLE;
    std::list<size_type> delrows;

    for(size_type r = 1; r <= m; ++r)
    {
        size_type s = basis[r-1];
        ARAGELI_ASSERT_1(s > 0);
        if(s > m)continue;
        ARAGELI_ASSERT_1(is_null(st(r, 0)));

        size_type j = std::find_if
        (
            st.begin() + st.ncols()*r + 1 + m, st.begin() + st.ncols()*(r+1),
            func::sign<T>()
        ) - (st.begin() + st.ncols()*r);

        if(j == st.ncols())
        {
            delrows.push_back(r);
            continue;
        }

        ARAGELI_ASSERT_1(j > m && j < st.ncols());

        T t = st(r, j);
        st.div_row(r, t);
        for(size_type i = 0; i <= m; ++i)
        {
            if(i == r)continue;
            st.addmult_rows(i, r, -st(i, j));
        }

        basis[r-1] = j;
    }

    for
    (
        typename std::list<size_type>::reverse_iterator i = delrows.rbegin();
        i != delrows.rend();
        ++i
    )
    {
        st.erase_row(*i);
        basis.erase(*i-1);
    }

    basis -= m;

    st.erase_cols(1, m);

    m -= delrows.size();


    std::cout << "Симплекс таблица после первой фазы:\n$$\n";
    output_simplex_table_latex(std::cout, st, false);
    std::cout << "\n$$\n";

    for(size_type i = 1; i <= n; ++i)
        st(0, i) = -c[i-1];

    std::cout << "Начинаем вторую фазу симплекс метода.  Допишем вектор $c$:\n$$\n";
    output_simplex_table_latex(std::cout, st, false);
    std::cout << "\n$$\n";

    for(size_type i = 1; i <= m; ++i)
        st.addmult_rows(0, i, -st(0, basis[i - 1]));

    std::cout << "Вторая фаза.\n";
    //output_simplex_table_latex(std::cout, st);
    //std::cout << "\n$$\n";
    //output_basis_latex(std::cout, basis);

    for(;;)
    {
        size_type s = std::find_if
        (
            st.begin() + 1, st.begin() + st.ncols(),
            func::is_negative<T>()
        ) - st.begin();

        if(s == st.ncols())break;

        vector<vector<T, false>, false> norm_rows(m);
        vector<size_type, false> positive_rows(m);
        size_type npr = 0;    // number of positive rows

        for(size_type i = 1; i <= m; ++i)
        {
            const T& cur = st(i, s);
            if(is_positive(cur))
            {
                positive_rows[npr] = i;
                vector<T, false>& cv = norm_rows[npr];
                cv.resize(st.ncols());
                for(size_type j = 0; j < st.ncols(); ++j)
                    cv[j] = st(i, j)/cur;
                ++npr;
            }
        }

        if(npr == 0)return SMR_INFINITE;

        size_type r = positive_rows
        [
            min_element (norm_rows.begin(), norm_rows.begin() + npr) -
                norm_rows.begin()
        ];

        std::cout << "\n$$\n";
        output_simplex_table_latex(std::cout, st, false, r, s);
        std::cout << "\n$$\n";
        output_basis_latex(std::cout, basis);

        ARAGELI_ASSERT_1(is_positive(st(r, s)));

        T t = st(r, s);
        st.div_row(r, t);

        for(size_type i = 0; i <= m; ++i)
        {
            if(i == r)continue;
            st.addmult_rows(i, r, -st(i, s));
        }

        basis[r - 1] = s;

    }

    std::cout << "\n$$\n";
    output_simplex_table_latex(std::cout, st, false);
    std::cout << "\n$$\n";
    output_basis_latex(std::cout, basis);

    res = st(0, 0);

    ARAGELI_ASSERT_0(x.size() >= n);
    x = 0;
    for(size_type i = 0; i < m; ++i)
        x[basis[i]-1] = st(i+1, 0);

    output_list(std::cout << "\n\n{\\bf Ответ:} оптимальный вектор $x=", x);
    std::cout << "^T $, ";
    std::cout << "оптимальное значение:~$" << res << "$.";

    return SMR_OK;
}


void test1_66 ()
{
    typedef rational<> T;
    //vector<T> c = "(0, 2, -1, 1)";
    //matrix<T> a = "((2, 1, 1, 0), (1, 2, 0, 1))";
    //vector<T> b = "(6, 6)";
    vector<T> c;
    matrix<T> a;
    vector<T> b;
    vector<T> x;
    T res;
    const char* status[] = {"OK", "INCOMPATIBLE", "INFINITE"};


    //c = "(-1, -1, 0, -5)";
    //a = "((1, 1, -1, 3), (1, -2, 3, -1), (5, -4, 7, 3))";
    //b = "(1, 1, 5)";
    //x.resize(c.size());
    //
    //std::cout
    //    << "\nStatus = "
    //    << status[maximize_simplex_method_l(c, a, b, res, x)] << std::endl
    //    << "++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    //
    //c = "(0, 2, -1, 1)";
    //a = "((2, 1, 1, 0), (1, 2, 0, 1))";
    //b = "(6, 6)";
    //x.resize(c.size());
    //
    //std::cout
    //    << "\nStatus = "
    //    << status[maximize_simplex_method_l(c, a, b, res, x)] << std::endl
    //    << "++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    //
    //c = "(2, 1, 0, 0, 0)";
    //a = "((3, 4, 1, 0, 0), (3, 1, 0, 1, 0), (1, 0, 0, 0, 1))";
    //b = "(32, 17, 5)";
    //x.resize(c.size());
    //
    //std::cout
    //    << "\nStatus = "
    //    << status[maximize_simplex_method_l(c, a, b, res, x)] << std::endl
    //    << "++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    //
    //c = "(0, 0, 0, 0, 200, 175, -1100, -2)";
    //a =
    //    "((0, 1, 0, 0, -3, -5/4, 7, 1/50),"
    //    " (1, 0, 0, 0, 1/3, 1/6, -1, -1/150),"
    //    " (0, 0, 1, 0, 72/5, -25/4, 175/2, 1/4),"
    //    " (0, 0, 0, 1, 0, 0, 0, 1))";
    //b = "(0, 0, 0, 1)";
    //x.resize(c.size());
    //
    //std::cout
    //    << "\nStatus = "
    //    << status[maximize_simplex_method_l(c, a, b, res, x)] << std::endl
    //    << "++++++++++++++++++++++++++++++++++++++++++++++++++\n";

    c = "(4, 4, -1, -4, -4)";
    a =
        "((5, -2, -3, 5, 0),"
        " (-2, -2, 1, 3, -3),"
        " (5, 2, 0, -3, 4))";
    b = "(5, 1, 4)";
    x.resize(c.size());

    std::cout
        << "\nStatus = "
        << status[maximize_simplex_method_l(c, a, b, res, x)] << std::endl
        << "++++++++++++++++++++++++++++++++++++++++++++++++++\n";

    c = "(-4, -4, 1, 4, 4)";
    a =
        "((5, -2, -3, 5, 0),"
        " (-2, -2, 1, 3, -3),"
        " (5, 2, 0, -3, 4))";
    b = "(5, 1, 4)";
    x.resize(c.size());

    std::cout
        << "\nStatus = "
        << status[maximize_simplex_method_l(c, a, b, res, x)] << std::endl
        << "++++++++++++++++++++++++++++++++++++++++++++++++++\n";

    //c = "(-5, +5, -1, +1, -4, +4, 0, 0, 0, 0, 0)";
    //a =
    //    "((+5, -5, -2, +2, +5, -5, -1, 0,  0,  0,  0),"
    //    " (-2, +2, -2, +2, +2, -2, 0,  -1, 0,  0,  0),"
    //    " (+3, -3, -1, +1, 0,  0,  0,  0,  +1, 0,  0),"
    //    " (-5, +5, -3, +3, +3, -3, 0,  0,  0,  +1, 0),"
    //    " (0,  0,  +3, -3, -4, +4, 0,  0,  0,  0,  +1))";
    //b = "(4, 4, 1, 4, 4)";
    //x.resize(c.size());
    //
    //std::cout
    //    << "\nStatus = "
    //    << status[maximize_simplex_method_l(c, a, b, res, x)] << std::endl
    //    << "++++++++++++++++++++++++++++++++++++++++++++++++++\n";

}


void test1_67 ()
{
    typedef vector<double> V;
    V v;
    std::cin >> v;
    //std::istringstream s;
    //s >> v;
}


void test1_68 ()
{
    typedef monom<double> T;
    T x;

    std::cin >> iomanip::monom_input_list() >> x;
    std::cout << iomanip::monom_output_list() << x << std::endl;
    std::cin >> iomanip::monom_input_var() >> x;
    std::cout << iomanip::monom_output_var(true, "variable", " multiply by ", " in poser of ") << x << '\n';
    std::cout << iomanip::monom_output_aligned() << x;
}


void test1_69 ()
{
    typedef sparse_polynom<double> T;
    T a = "x-2", b = "x^2+5";
    std::cout << (a < b);
}


void test1_70 ()
{
    typedef int T;
    my_output_latex(std::cout << "\n", rational<T>(0));
    my_output_latex(std::cout << "\n", rational<T>(1));
    my_output_latex(std::cout << "\n", rational<T>(1, 13));
    my_output_latex(std::cout << "\n", rational<T>(234, 11));
    my_output_latex(std::cout << "\n", rational<T>(234, 111));
    my_output_latex(std::cout << "\n", rational<T>(23243243, 1141));

    std::cout << "\n";

    matrix<rational<> > m = "((1, 2), (91827/132987687624, 23/2), (1/234, -23223/5))";
    my_output_latex(std::cout << "\n", m);
}


void test1_71 ()
{
    iomanip::vector_input_list vil;
    iomanip::vector_output_aligned voa("|| ", " ||");

    vector<sparse_polynom<rational<> > > v;
    std::cin >> vil >> v;
    std::cout << '\n' << voa << v;
}


void test1_72 ()
{
    matrix<sparse_polynom<int> > mp = "((-x^32-12*x^3+7,12), (x-1,x^2-2*x+1))";
    sparse_polynom<int> p = "-x^32-12*x^3+7";

    std::cout
        << iomanip::matrix_output_aligned() << mp << '\n'
        << iomanip::sparse_polynom_output_aligned() << p;
}


void test1_73 ()
{
    matrix<int> mm = "((1, 2),(3, 4))";
    output_aligned(std::cout << "\n\n", mm);

    {
        matrix<int> m = mm;
        m.insert_col(0, 5);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<int> m = mm;
        m.insert_col(1, 5);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<int> m = mm;
        m.insert_col(2, 5);
        output_aligned(std::cout << "\n\n", m);
    }

    vector<int> v = "(5, 6)";

    {
        matrix<int> m = mm;
        m.insert_col(0, v);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<int> m = mm;
        m.insert_col(1, v);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<int> m = mm;
        m.insert_col(2, v);
        output_aligned(std::cout << "\n\n", m);
    }

    {
        matrix<int> m = mm;
        m.insert_col(0, v.begin());
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<int> m = mm;
        m.insert_col(1, v.begin());
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<int> m = mm;
        m.insert_col(2, v.begin());
        output_aligned(std::cout << "\n\n", m);
    }

    {
        matrix<int> m = mm;
        m.erase_col(0);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<int> m = mm;
        m.erase_col(1);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<int> m = mm;
        m.erase_cols(0, 2);
        output_aligned(std::cout << "\n\n", m);
    }

    {
        matrix<int> m = mm;
        m.swap_cols(0, 1);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<int> m = mm;
        m.swap_cols(1, 0);
        output_aligned(std::cout << "\n\n", m);
    }

    {
        matrix<int> m = mm;
        m.mult_col(0, 5);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<int> m = mm;
        m.mult_col(1, 5);
        output_aligned(std::cout << "\n\n", m);
    }

    {
        matrix<rational<int> > m = mm;
        m.div_col(0, 5);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<rational<int> > m = mm;
        m.div_col(1, 5);
        output_aligned(std::cout << "\n\n", m);
    }

    {
        matrix<rational<int> > m = mm;
        m.add_cols(0, 0);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<rational<int> > m = mm;
        m.add_cols(0, 1);
        output_aligned(std::cout << "\n\n", m);
    }
    {
        matrix<rational<int> > m = mm;
        m.add_cols(1, 0);
        output_aligned(std::cout << "\n\n", m);
    }

}

void test1_74 ()
{
    try { big_int(""); } catch(incorrect_string) { std::cout << "All is right!\n"; }
    try { big_int("-"); } catch(incorrect_string) { std::cout << "All is right!\n"; }
    try { big_int("d"); } catch(incorrect_string) { std::cout << "All is right!\n"; }
    try { matrix<big_int>("(234, 234, )"); } catch(incorrect_string) { std::cout << "All is right!\n"; }
    try { sparse_polynom<rational<> >("x^2-1/x"); } catch(incorrect_string) { std::cout << "All is right!\n"; }
}


void test1_75 ()
{
    //unsigned long long a = 92233719531022718101ul;
    //big_int a = "92233719531022718101";
    //std::cout << "\na = " << a << " is prime: " << is_prime(a);
    //std::cout << "\nfactorization is " << partial_factorize_division(a, 1000);
    //a = "9233719531022718101";
    //std::cout << "\na = " << a << " is prime: " << is_prime(a);
    //std::cout << "\nfactorization is " << partial_factorize_division(a, 1000);
    //a = "9223719531022718101";
    //std::cout << "\na = " << a << " is prime: " << is_prime(a);
    //std::cout << "\nfactorization is " << partial_factorize_division(a, 1000);

    //typedef unsigned long long T;

    //T
    //    b1 = next_prime(4100032442u), b2 = next_prime(4102032442u), c = b1*b2;


    //std::cout << b1 << "*" << b2 << " = " << c;
    //
    //vector<T> res;

    //factorize_division(c, res);
    //std::cout << "\nfactorization is " << res;
    //std::cout << "\nfactorization is valid: " << (product(res) == c);

    //a = "9233719531022718101";
    //factorize_division(a, res);
    //std::cout << "\nfactorization is " << res;
    //std::cout << "\nfactorization is valid: " << (product(res) == a);

    //a = "9223719531022718101";
    //factorize_division(a, res);
    //std::cout << "\nfactorization is " << res;
    //std::cout << "\nfactorization is valid: " << (product(res) == a);

    //big_int d = "92233719531022718101";
    //vector<big_int> res2; big_int rest;
    //partial_factorize_division(d, res2, big_int(1000000), rest);
    //std::cout << "\npartial factorization of " << d << " is " << res2 << ", rest is " << rest;

    std::cout << factorize_division(big_int("92233719531022718101"));
    //std::cout << factorize_division(16818466434869209727u);
}


void test1_76 ()
{
    sparse_polynom<int> p;
    p == 0;
}


// Tests for Arageli::vector  ////////////////////////////////////////////////

void test_vector (std::ostream& report);    // See implementation below.


template <typename CL, typename T, bool REFCNT>
void test_static_declarations (std::ostream& report)
{
    ARAGELI_ASSERT_ALWAYS((equal_types<typename CL::value_type, T>::bvalue));
    ARAGELI_ASSERT_ALWAYS((equal_types<typename CL::const_reference, const T&>::bvalue));
    ARAGELI_ASSERT_ALWAYS(CL::refcounting == REFCNT);
}


template <typename V>
void test_vector_construct (std::ostream&)
{
    V a;
    ARAGELI_ASSERT_ALWAYS(a.size() == 0);

    for(size_t i = 0; i < 100; ++i)
    {
        V b(i);
        ARAGELI_ASSERT_ALWAYS(b.size() == i);
    }

    V c(1), d(2);

    ARAGELI_ASSERT_ALWAYS(c.size() == 1);
    ARAGELI_ASSERT_ALWAYS(d.size() == 2);
    a = c;
    ARAGELI_ASSERT_ALWAYS(a.size() == 1);
    ARAGELI_ASSERT_ALWAYS(a == c);
    c = d;
    ARAGELI_ASSERT_ALWAYS(a.size() == 1);
    ARAGELI_ASSERT_ALWAYS(c.size() == 2);
    ARAGELI_ASSERT_ALWAYS(c == d);

    V e(c), f(a);

    ARAGELI_ASSERT_ALWAYS(e.size() == 2);
    ARAGELI_ASSERT_ALWAYS(f.size() == 1);
    ARAGELI_ASSERT_ALWAYS(e == c);
    ARAGELI_ASSERT_ALWAYS(f == a);
}


template <typename V1, typename V2>
void test_vector_conversion (std::ostream&)
{
    V1 v11(3); V2 v21(2);
    v11 = v21;

    ARAGELI_ASSERT_ALWAYS(v21.size() == 2);
    ARAGELI_ASSERT_ALWAYS(v11.size() == v21.size());

    v21 = v11;

    ARAGELI_ASSERT_ALWAYS(v21.size() == 2);
    ARAGELI_ASSERT_ALWAYS(v21.size() == v11.size());

    V1 v12(v21); V2 v22(v11);
}


template <typename V>
void test_vector_assignment (std::ostream&)
{

}


void test_vector (std::ostream& report)
{
    test_static_declarations
        <vector<int, true>, int, true>
        (report);
    test_static_declarations
        <vector<int, false>, int, false>
        (report);
    test_static_declarations
        <vector<big_int, true>, big_int, true>
        (report);
    test_static_declarations
        <vector<big_int, false>, big_int, false>
        (report);

    test_vector_construct<vector<int, true> >(report);
    test_vector_construct<vector<int, false> >(report);
    test_vector_construct<vector<big_int, true> >(report);
    test_vector_construct<vector<big_int, false> >(report);
}


}
