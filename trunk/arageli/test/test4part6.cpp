/*****************************************************************************

    test/test4part6.cpp

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


void test4_36 ()
{
    matrix<sparse_polynom<rational<big_int> > > a = "((1, 0), (0, 13*x-7))", b, p, q;
    size_t rank;
    sparse_polynom<rational<big_int> > det;
    smith(a, b, p, q, rank, det);
    cout << "b = " << b;
    cout << "It is valid: " << (b == p*a*q);
}


void test4_37 ()
{
    typedef sparse_polynom<rational<> > P;

    {
        P a = "17*x-23", b = "6*x-23", u, v;
        cout << "\neuclid_bezout(a, b, u, v) = " << euclid_bezout(a, b, u, v);
        cout << "\na*u + b*v = " << a*u + b*v;
    }
    {
        P a = "17*x", b = "6*x", u, v;
        cout << "\neuclid_bezout(a, b, u, v) = " << euclid_bezout(a, b, u, v);
        cout << "\na*u + b*v = " << a*u + b*v;
    }
    {
        P a = "16*x^2-4", b = "4*x+2", u, v;
        cout << "\neuclid_bezout(a, b, u, v) = " << euclid_bezout(a, b, u, v);
        cout << "\na*u + b*v = " << a*u + b*v;
    }
}


void test4_38 ()
{
    // Working in ${\bf Q}/(x^2 + 1){\bf Q}$

    typedef residue<sparse_polynom<rational<> > > T;

    T a = "((1+x) (mod (x^2+1)))";
    T b = "((1-x) (mod (x^2+1)))";

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "a / b = " << a / b << endl << endl;
}


void test4_39 ()
{
    typedef big_int T;
    matrix<T> a = "((1, 2), (3, 4))";
    vector<T> b = "(5, 6)";

    {
        typedef int TT;
        matrix<TT> aa = a, gx;
        vector<TT> bb = b, offset;
        solve_linsys_result status = solve_linsys_integer(aa, bb, offset, gx);
        output_aligned(cout << "aa =\n", aa);
        cout << "bb = " << b;
        cout << "\noffset = " << offset;
        output_aligned(cout << "\ngx =\n", gx);
        cout << "status = " << slr_name(status) << "\n";
    }

    {
        typedef big_int TT;
        matrix<TT> aa = a, gx;
        vector<TT> bb = b, offset;
        solve_linsys_result status = solve_linsys_integer(aa, bb, offset, gx);
        output_aligned(cout << "aa =\n", aa);
        cout << "bb = " << b;
        cout << "\noffset = " << offset;
        output_aligned(cout << "\ngx =\n", gx);
        cout << "status = " << slr_name(status) << "\n";
    }

    {
        typedef rational<int> TT;
        matrix<TT> aa = a, gx;
        vector<TT> bb = b, offset;
        solve_linsys_result status = solve_linsys_rational(aa, bb, offset, gx);
        output_aligned(cout << "aa =\n", aa);
        cout << "bb = " << b;
        cout << "\noffset = " << offset;
        output_aligned(cout << "\ngx =\n", gx);
        cout << "status = " << slr_name(status) << "\n";
    }

    {
        typedef rational<big_int> TT;
        matrix<TT> aa = a, gx;
        vector<TT> bb = b, offset;
        solve_linsys_result status = solve_linsys_rational(aa, bb, offset, gx);
        output_aligned(cout << "aa =\n", aa);
        cout << "bb = " << b;
        cout << "\noffset = " << offset;
        output_aligned(cout << "\ngx =\n", gx);
        cout << "status = " << slr_name(status) << "\n";
    }

}


void test4_40 ()
{
    cout
        << "double(0) = " << double(0)
        << "\nbig_float(0) = " << big_float(0) << std::endl;
}


namespace Arageli
{

namespace ctrl
{
    // Класс контролёра, который ничего не делает.
    // Нужен для того, что бы пользователь мог вызывать функцию так,
    // как будтно она неконтролируемая.
    struct sqrt_approx_idler
    {
        class abort : public ctrl::abort {};

        // Вызывается в начале работы функции; принимает аргументы.
        template <typename T1, typename T2>
        void preamble (const T1& x, const T2& tolerance) const {}

        // Вызывается на каждой итерации извлечения корня;
        // принимает текущее и следующее приближения.
        template <typename T1>
        void iter
        (
            const T1& cur,    // текущее приближение a[n]
            const T1& next,   // следующее приближение a[n+1]
            const T1& curtol  // |a[n+1] - a[n]|
        ) const {}

        // Вызывается в конце работы функции; принимает результат.
        template <typename T1>
        void conclusion (const T1& res) const {}
    };
}

// Контролируемая функция извлечения квадратного корня.
template <typename T1, typename T2, typename Ctrler>
T1 sqrt_approx (const T1& x, const T2& tolerance, Ctrler ctrler)
{
    ctrler.preamble(x, tolerance);

    if(is_null(x))
    {
        ctrler.conclusion(x);
        return x;
    }

    T1 next = x, cur, curtol;

    do
    {
        cur = next;
        next = (cur + x/cur)/2;
        curtol = abs(next - cur);
        ctrler.iter(cur, next, curtol);
    }while(curtol > tolerance);

    ctrler.conclusion(next);
    return next;
}

// Вызов функции без контроля.
template <typename T1, typename T2>
inline T1 sqrt_approx (const T1& x, const T2& tolerance)
{ return sqrt_approx(x, tolerance, ctrl::sqrt_approx_idler()); }

namespace ctrl
{
    // Контролёр для sqrt_approx_slog выводящий промежуточные
    // результаты в поток в простом текстовом формате
    template <typename Out>
    class sqrt_approx_slog : public sqrt_approx_idler
    {
        Out& out;
        std::size_t curiter;

    public:

        sqrt_approx_slog (Out& out_a) : out(out_a), curiter(1) {}

        template <typename T1, typename T2>
        void preamble (const T1& x, const T2& tolerance) const
        {
            out << "Находим приближение к квадратному корню из "
                << x << '\n'
                << "константа, контролирующая точность: "
                << tolerance << '\n';
        }

        template <typename T1>
        void iter
        (
            const T1& cur,
            const T1& next,
            const T1& curtol
        )
        {
            out << "a[" << curiter++ << "] = " << cur
                << ", отклонение: " << curtol << '\n';
        }

        template <typename T1>
        void conclusion (const T1& res) const
        {
            out << "Найдено приближение a[" << curiter << "] = "
                << res << '\n'
                << "всего итераций: " << curiter-1 << '\n';
        }
    };

    // Для удобства: make-функция.
    template <typename Out>
    inline sqrt_approx_slog<Out> make_sqrt_approx_slog (Out& out)
    { return sqrt_approx_slog<Out>(out); }
}
} // namespace Arageli


void test4_41 ()
{
    std::cout
        << std::setprecision(8) << sqrt_approx(5.0, 1.0/100000) << '\n'
        << sqrt_approx(rational<>(5), rational<>(1, 100000));

    std::cout << "\n****************\n";

    sqrt_approx
    (
        5.0, 1.0/100000,
        ctrl::make_sqrt_approx_slog(std::cout)
    );

    std::cout << '\n';

    sqrt_approx
    (
        rational<>(5), rational<>(1, 100000),
        ctrl::make_sqrt_approx_slog(std::cout)
    );
}


void test4_42 ()
{
    {
        typedef big_int T;

        for(int i = 1; i <= 10; ++i)
        {
            cout << "i = " << i << "\n" << std::endl;
            typedef set::ipositive<T> S;
            S s(i);
            rnd::equiprob<S> rnd(s);
            std::map<int, int> hist;
            for(int j = 0; j < 100*i; ++j)
                ++hist[rnd()];

            for(std::map<int, int>::iterator j = hist.begin(); j != hist.end(); ++j)
                cout << "[" << j->first << "] = " << j->second << "\n";

            cout << "\n";
        }
    }
    {
        typedef big_int T;

        for(int i = 0; i <= 10; ++i)
        {
            cout << "i = " << i << "\n" << std::endl;
            typedef set::inonnegative<T> S;
            S s(i);
            rnd::equiprob<S> rnd(s);
            std::map<int, int> hist;
            for(int j = 0; j < 100*(i+1); ++j)
                ++hist[rnd()];

            for(std::map<int, int>::iterator j = hist.begin(); j != hist.end(); ++j)
                cout << "[" << j->first << "] = " << j->second << "\n";

            cout << "\n";
        }
    }
    {
        typedef unsigned T;

        for(int i = 1; i <= 10; ++i)
        {
            cout << "i = " << i << "\n" << std::endl;
            typedef set::ipositive<T> S;
            S s(i);
            rnd::equiprob<S> rnd(s);
            std::map<int, int> hist;
            for(int j = 0; j < 100*i; ++j)
                ++hist[rnd()];

            for(std::map<int, int>::iterator j = hist.begin(); j != hist.end(); ++j)
                cout << "[" << j->first << "] = " << j->second << "\n";

            cout << "\n";
        }
    }
    {
        typedef unsigned T;

        for(int i = 0; i <= 10; ++i)
        {
            cout << "i = " << i << "\n" << std::endl;
            typedef set::inonnegative<T> S;
            S s(i);
            rnd::equiprob<S> rnd(s);
            std::map<int, int> hist;
            for(int j = 0; j < 100*(i+1); ++j)
                ++hist[rnd()];

            for(std::map<int, int>::iterator j = hist.begin(); j != hist.end(); ++j)
                cout << "[" << j->first << "] = " << j->second << "\n";

            cout << "\n";
        }
    }
    {
        typedef rational<int> T;

        for(int i = 1; i <= 10; ++i)
        {
            cout << "i = " << i;
            typedef set::grid1<T> S;
            S s(-1, 1, T(3, i));
            cout << "\nstep = " << s.step() << std::endl;
            rnd::equiprob<S> rnd(s);
            std::map<T, int> hist;
            for(int j = 0; j < 100*i; ++j)
                ++hist[rnd()];

            for(std::map<T, int>::iterator j = hist.begin(); j != hist.end(); ++j)
                cout << "[" << j->first << "] = " << j->second << "\n";

            cout << "\n";
        }
    }
}


void test4_43 ()
{
    {
        typedef big_int T;

        for(int i = 0; i <= 10; ++i)
        {
            cout << "i = " << i << "\n" << std::endl;
            typedef set::ipositive<T> S;
            S s(i);
            typedef enumer::ordered<S> E;
            for(E j(enumer::begin, s); !j.is_end(); ++j)
                cout << *j << ", ";
            cout << "\n\n";
        }
    }
    {
        typedef big_int T;

        for(int i = 0; i <= 10; ++i)
        {
            cout << "i = " << i << "\n" << std::endl;
            typedef set::inonnegative<T> S;
            S s(i);
            typedef enumer::ordered<S> E;
            for(E j(enumer::begin, s); !j.is_end(); ++j)
                cout << *j << ", ";
            cout << "\n\n";
        }
    }
    {
        typedef unsigned T;

        for(int i = 0; i <= 10; ++i)
        {
            cout << "i = " << i << "\n" << std::endl;
            typedef set::ipositive<T> S;
            S s(i);
            typedef enumer::ordered<S> E;
            for(E j(enumer::begin, s); !j.is_end(); ++j)
                cout << *j << ", ";
            cout << "\n\n";
        }
    }
    {
        typedef unsigned T;

        for(int i = 0; i <= 10; ++i)
        {
            cout << "i = " << i << "\n" << std::endl;
            typedef set::inonnegative<T> S;
            S s(i);
            typedef enumer::ordered<S> E;
            for(E j(enumer::begin, s); !j.is_end(); ++j)
                cout << *j << ", ";
            cout << "\n\n";
        }
    }
    {
        typedef rational<int> T;

        for(int i = 1; i <= 10; ++i)
        {
            cout << "i = " << i;
            typedef set::grid1<T> S;
            S s(-1, 1, T(3, i));
            cout << "\nstep = " << s.step() << std::endl;
            typedef enumer::ordered<S> E;
            for(E j(enumer::begin, s); !j.is_end(); ++j)
                cout << *j << ", ";
            cout << "\n\n";
        }
    }
    {
        typedef rational<big_int> T;

        for(int i = 1; i <= 10; ++i)
        {
            cout << "i = " << i;
            typedef set::grid1<T> S;
            S s(-1, 1, T(3, i));
            cout << "\nstep = " << s.step() << std::endl;
            typedef enumer::ordered<S> E;
            for(E j(enumer::begin, s); !j.is_end(); ++j)
                cout << *j << ", ";
            cout << "\n\n";
        }
    }
    {
        typedef rational<big_int> T;

        for(int i = 1; i <= 10; ++i)
        {
            cout << "i = " << i;
            typedef set::grid1<T> S;
            S s(-1, 1, T(3, i));
            cout << "\nstep = " << s.step() << std::endl;
            typedef enumer::ordered<S> E;
            for(E j(enumer::begin, s); j != E(enumer::end, s); ++j)
                cout << *j << ", ";
            cout << "\n\n";
        }
    }
}
