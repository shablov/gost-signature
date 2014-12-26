/*****************************************************************************

    sparse_multipolynom.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2007 Sergey S. Lyalin

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

/**
    \file sparse_multipolynom.cpp
    \brief The sparse_multipolynom.hpp file stuff implementation.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_sparse_multipolynom)

// REFERENCE ADDITIONAL HEADERS HERE

#include <iostream>    // WARNING! FOR DEBUGGING
#include <sstream>

#include "_utility.hpp"
#include "sparse_multipolynom.hpp"


namespace Arageli
{


template
<
    typename Ch,
    typename ChT,
    typename F,
    typename D,
    typename MD
>
std::basic_ostream<Ch, ChT>& output_var
(
    std::basic_ostream<Ch, ChT>& out,
    const multimonom<F, D, MD>& x,
    bool first,
    const char* var,
    const char* mul,
    const char* pow
)
{
    if(x.is_const())
        if(first)
            output_polynom_first(out, x.coef());
        else
            output_polynom_internal(out, x.coef());
    else
    {
        if(is_opposite_unit(x.coef()))
            out << '-';
        else if(is_unit(x.coef()))
        {
            if(!first)
                out << '+';
        }
        else if(first)
            output_polynom_first(out, x.coef()) << mul;
        else
            output_polynom_internal(out, x.coef()) << mul;

        // We always output the multidegree in contrast to the function
        // for monom class.

        out << var << pow;
        output_pow(out, x.multidegree());
    }

    return out;
}


template
<
    typename Ch,
    typename ChT,
    typename F,
    typename D,
    typename MD
>
std::basic_istream<Ch, ChT>& input_list
(
    std::basic_istream<Ch, ChT>& in,
    multimonom<F, D, MD>& x,
    const char* fb,
    const char* sb,
    const char* sep
)
{
    // WARNING! There are not many things that differ from the
    // function for monom class. May be a separate common function is
    // better idea for these two functions?

    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(fb));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(sb));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(sep));

    multimonom<F, D, MD> tmp;    // differs from monom

    if
    (
        !(
            !_Internal::read_literal(in, fb)  ||
            !input_polynom_first(in, tmp.coef())  ||
            !_Internal::read_literal(in, sep)  ||
            !input_pow(in, tmp.multidegree())  ||    // differs from monom
            !_Internal::read_literal(in, sb)
        )
    )
        x = tmp;

    return in;
}


template
<
    typename Ch,
    typename ChT,
    typename F,
    typename D,
    typename MD,
    typename Factory_coef
>
std::basic_istream<Ch, ChT>& input_var
(
    std::basic_istream<Ch, ChT>& in,
    multimonom<F, D, MD>& x,
    bool first_a,
    const Factory_coef& fctr,
    const char* var,
    const char* mul,
    const char* pow
)
{
    // WARNING! There are not many things that differ from the
    // function for monom class. May be a separate common function is
    // better idea for these two functions?

    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(var));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(mul));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(pow));

    multimonom<F, D, MD> res;
    char ch, ch1 = 0;

    in >> ch1;
    if(!in)
        return in;

    if(ch1 != '-' && ch1 != '+')
    {
        in.putback(ch1);

        if(!first_a)
        {
            in.clear(std::ios_base::failbit);
            return in;
        }
    }

    if(_Internal::read_literal(in, var))
    {
        if(ch1 == '-')
            res.coef() = fctr.opposite_unit(x.coef());
        else
            res.coef() = fctr.unit(x.coef());

        ch = in.get();
        in.clear();
        in.putback(ch);

        // If the formal variable is used, we must provide the multidegree.

        if(ch != '\n' && _Internal::read_literal(in, pow))
        {
            input_pow(in, res.multidegree());
            if(!in && !in.eof())
            {
                ARAGELI_ASSERT_1(!in.good());
                return in;
            }
        }
        else
        {
            in.clear(std::ios_base::badbit);
            return in;
        }

        x = res;
        in.clear();
        return in;
    }

    //std::cerr << "\n" << in.good();
    if(ch1 == '-' || ch1 == '+')
        in.putback(ch1);
    in.clear();
    //std::cerr << "\n" << in.good();

    if(first_a)
        input_polynom_first(in, res.coef());
    else
        input_polynom_internal(in, res.coef());

    if(!in)
    {
        if(!in.eof())
        {
            ARAGELI_ASSERT_1(!in.good());
            return in;
        }
    }
    else
    {
        ch = in.get();
        in.putback(ch);
        in.clear();

        if(!in.eof() && ch != '\n')
        {
            if(_Internal::read_literal(in, mul))
            {
                if(!_Internal::read_literal(in, var))
                {
                    in.clear(std::ios_base::badbit);
                    return in;
                }

                ch = in.get();
                in.clear();
                in.putback(ch);

                // If the formal variable is used, we must provide
                // the multidegree.

                // WARNING! The same block as above.

                if(ch != '\n' && _Internal::read_literal(in, pow))
                {
                    input_pow(in, res.multidegree());
                    if(!in && !in.eof())
                    {
                        ARAGELI_ASSERT_1(!in.good());
                        return in;
                    }
                }
                else
                {
                    in.clear(std::ios_base::badbit);
                    return in;
                }
            }
        }
    }

    x = res;

    in.clear();
    return in;
}


template <typename F, typename D, typename MD, typename Config>
sparse_multipolynom<F, D, MD, Config>&
sparse_multipolynom<F, D, MD, Config>::opposite ()
{
    // WARNING! The implementation of this function is the same
    // as for univariate polynomials and also is invariant relatively
    // any sparse_multipolynom interfaced type.
    // TODO: Relocate the stuff to a separate, more general function.

    for(monom_iterator i = monoms_begin(), e = monoms_end(); i != e; ++i)
        i->opposite();

    return *this;
}



/////////////////////////////////////
///////////// WARNING! //////////////
/// RELOCATE TO THE SEPARATE FILE ///
/////////////////////////////////////


template <typename P1, typename P2, typename P3>
P3& plus_smp_by_smp (const P1& p1, const P2& p2, P3& p3)
{
    // WARNING! MONOMIAL ORDERS FOR ALL p1, p2 AND p3 MUST BE THE SAME.

    //std::cerr << "\nADD " << p1 << " + " << p2;

    ARAGELI_ASSERT_0(p1.is_normal());
    ARAGELI_ASSERT_0(p2.is_normal());

    ARAGELI_ASSERT_0(&p1 != &p3 && &p2 != &p3);    // WARNING! TEMPORARY.

    if(p1.is_null())
        return p3 = p2;
    if(p2.is_null())
        return p3 = p1;

    typename P1::monom_const_iterator
        p1i = p1.monoms_begin(), p1e = p1.monoms_end();
    typename P2::monom_const_iterator
        p2i = p2.monoms_begin(), p2e = p2.monoms_end();
    typename P3::multidegree_cmp_type
        mdcmp = p3.multidegree_cmp();

    p3.assign();

    while(p1i != p1e || p2i != p2e)
    {
        if(p1i == p1e)
            p3.push_back(*p2i++);
        else if(p2i == p2e)
            p3.push_back(*p1i++);
        else switch(mdcmp(p1i->multidegree(), p2i->multidegree()))
        {
            case -1:
            {
                p3.push_back(*p1i++);
                break;
            }
            case 0:
            {
                ARAGELI_ASSERT_1(p1i->can_add(*p2i));
                typename P3::monom m = (*p1i++) + (*p2i++);
                if(!m.is_null())p3.push_back(m);
                break;
            }
            case 1:
            {
                p3.push_back(*p2i++);
                break;
            }
        }
    }

    p3.normalize_nvars();   // WARNING! It can be done in the main loop above.
    ARAGELI_ASSERT_1(p3.is_normal());

    //std::cerr << "\nADD " << p1 << " + " << p2 << " = " << p3;
    return p3;
}


template <typename P1, typename P2, typename P3>
P3& multiplies_smp_by_smp (const P1& p1, const P2& p2, P3& p3)
{
    // WARNING! MONOMIAL ORDERS FOR ALL p1, p2 AND p3 MUST BE THE SAME.

    //std::cerr << "\nMUL " << p1 << " * " << p2;

    ARAGELI_ASSERT_0(p1.is_normal());
    ARAGELI_ASSERT_0(p2.is_normal());

    ARAGELI_ASSERT_0(&p1 != &p3 && &p2 != &p3);    // WARNING! TEMPORARY.

    if(p1.is_null() || p2.is_null())
    {
        p3.assign();
        return p3;
    }

    if(p1.is_unit())
        return p3 = p2;
    if(p2.is_unit())
        return p3 = p1;
    if(p1.is_opposite_unit())
        return (p3 = p2).opposite();
    if(p2.is_opposite_unit())
        return (p3 = p1).opposite();

    // WARNING! TEMPORARY SLOW IMPLEMENTATION.
    // For each monomial in p1 and whole p2 we do multiplication
    // and call plus_smp_by_smp to accumulate the result.

    p3.assign();

    for
    (
        typename P1::monom_const_iterator i1 =
            p1.monoms_begin(), e1 = p1.monoms_end();
        i1 != e1;
        ++i1
    )
    {
        P2 tp2 = p2;

        // Monomial-polynomial multiplication.
        for
        (
            typename P2::monom_iterator i2 =
                tp2.monoms_begin(), e2 = tp2.monoms_end();
            i2 != e2;
        )
            if((*i2 *= *i1).is_null())
                i2 = tp2.erase(i2);
            else
                ++i2;

        p3 = plus_smp_by_smp(p3, tp2);
    }

    ARAGELI_ASSERT_1(p3.is_normal());
    //std::cerr << "\nMUL " << p1 << " * " << p2 << " = " << p3;
    return p3;
}


template <typename P1, typename P2, typename Q, typename R>
void divide_smp_by_smp (const P1& p1, const P2& p2, Q& q, R& r)
{
    // WARNING! MONOMIAL ORDERS FOR ALL p1, p2 AND p3 MUST BE THE SAME.

    ARAGELI_TRACE_BEGIN_AUTO
    (
        tracing_common
            << tracing_indent() << "divide_smp_by_smp: p1 = "
            << p1 << ", p2 = " << p2 << '\n',
        0
    );

    ARAGELI_ASSERT_0(p1.is_normal());
    ARAGELI_ASSERT_0(p2.is_normal());

    ARAGELI_ASSERT_0(&p2 != &q);    // WARNING! TEMPORARY.
    ARAGELI_ASSERT_0(&p2 != &r);    // WARNING! TEMPORARY.

    ARAGELI_ASSERT_0(!p2.is_null());

    r = p1;
    q.assign();

    typedef typename P2::monom P2M;
    const P2M& lmp2 = p2.leading_monom();

    while(!r.is_null() && is_divisible(r.leading_monom(), lmp2))
    {
        ARAGELI_TRACE_BEGIN_AUTO
        (
            tracing_common
                << tracing_indent() << "divide_smp_by_smp.iteration: q = "
                << q << ", r = " << r << '\n',
            0
        );

        P2 s = r.leading_monom()/lmp2;
        r -= p2*s;
        q += s;
    }

    //ARAGELI_ASSERT_2(p1 == p2*q + r);  WARNING!!! TEMPORARY DISABLED!
    //std::cerr << "\nDIV(" << p1 << ", " << p2 << ", " << q << ", " << r << ")";
}


template <typename P1, typename P2, typename CmpM>
int cmp_smp_by_smp (const P1& p1, const P2& p2, CmpM cmpm)
{
    // WARNING! THE SAME IMPLEMENTATION AS FOR sparse_polynom.
    // TODO: Make common function to compare.

    ARAGELI_ASSERT_0(p1.is_normal());
    ARAGELI_ASSERT_0(p2.is_normal());

    typedef
        std::reverse_iterator<typename P1::monom_const_iterator> RI_a;
    typedef
        std::reverse_iterator<typename P2::monom_const_iterator> RI_b;

    int res = _Internal::aggregate_cmp
    (
        RI_a(p1.monoms_end()),
        RI_a(p1.monoms_begin()),
        RI_b(p2.monoms_end()),
        RI_b(p2.monoms_begin()),
        cmpm
    );

    return res;
}



/////////////////////////////////////


template <typename F, typename D, typename MD, typename Config1>
sparse_multipolynom<F, D, MD, Config1>::sparse_multipolynom (const char* str)
{
    std::istringstream buf(str);
    // WARNING. It's correct if and only if sparse_multipolynom has no virtual functions.
    static_cast<std::istream&>(buf) >> *this;
    if(!buf && !buf.eof())
        throw incorrect_string(str);
    //ARAGELI_ASSERT_0(buf.good() || buf.eof());
}


// WARNING! TEMPORARY HERE.
template <typename F, typename D, typename MD, typename Config, typename T_factory>
sparse_multipolynom<F, D, MD, Config> euclid
(
    sparse_multipolynom<F, D, MD, Config> a,
    sparse_multipolynom<F, D, MD, Config> b,
    const T_factory& tfctr
)
{
    ARAGELI_TRACE_BEGIN_AUTO
    (
        tracing_common << tracing_indent() << "euclid<sparse_multipolynom>: a = "
        << a << ", b = " << b << '\n', 0
    );

    if(is_negative(a))
        opposite(&a);
    if(is_negative(b))
        opposite(&b);

    if(is_null(a))
        if(is_null(b))
            return tfctr.unit(a);
        else
            return _Internal::euclid_norm(b);
    if(is_null(b))
        return _Internal::euclid_norm(a);

    // WARNING! TEMPORARY!

    if(a == b)
        return a;
    else
        return tfctr.unit();
}


}


#if ARAGELI_DEBUG_LEVEL > 3

#include "big_int.hpp"
#include "rational.hpp"

namespace Arageli
{

template class sparse_multipolynom<int>;
template class sparse_multipolynom<big_int>;
template class sparse_multipolynom<rational<> >;

template sparse_multipolynom<int>& plus_smp_by_smp
(
    const sparse_multipolynom<int>& p1,
    const sparse_multipolynom<int>& p2,
    sparse_multipolynom<int>& p3
);

template sparse_multipolynom<big_int>& plus_smp_by_smp
(
    const sparse_multipolynom<big_int>& p1,
    const sparse_multipolynom<big_int>& p2,
    sparse_multipolynom<big_int>& p3
);

template sparse_multipolynom<rational<> >& plus_smp_by_smp
(
    const sparse_multipolynom<rational<> >& p1,
    const sparse_multipolynom<rational<> >& p2,
    sparse_multipolynom<rational<> >& p3
);

template sparse_multipolynom<int>& multiplies_smp_by_smp
(
    const sparse_multipolynom<int>& p1,
    const sparse_multipolynom<int>& p2,
    sparse_multipolynom<int>& p3
);

template sparse_multipolynom<big_int>& multiplies_smp_by_smp
(
    const sparse_multipolynom<big_int>& p1,
    const sparse_multipolynom<big_int>& p2,
    sparse_multipolynom<big_int>& p3
);

template sparse_multipolynom<rational<> >& multiplies_smp_by_smp
(
    const sparse_multipolynom<rational<> >& p1,
    const sparse_multipolynom<rational<> >& p2,
    sparse_multipolynom<rational<> >& p3
);

template void divide_smp_by_smp
(
    const sparse_multipolynom<int>& p1,
    const sparse_multipolynom<int>& p2,
    sparse_multipolynom<int>& q,
    sparse_multipolynom<int>& r
);

template void divide_smp_by_smp
(
    const sparse_multipolynom<big_int>& p1,
    const sparse_multipolynom<big_int>& p2,
    sparse_multipolynom<big_int>& q,
    sparse_multipolynom<big_int>& r
);

template void divide_smp_by_smp
(
    const sparse_multipolynom<rational<> >& p1,
    const sparse_multipolynom<rational<> >& p2,
    sparse_multipolynom<rational<> >& q,
    sparse_multipolynom<rational<> >& r
);

template int cmp
(
    const sparse_multipolynom<int>& a,
    const sparse_multipolynom<int>& b
);

template int cmp
(
    const sparse_multipolynom<big_int>& a,
    const sparse_multipolynom<big_int>& b
);

template int cmp
(
    const sparse_multipolynom<rational<> >& a,
    const sparse_multipolynom<rational<> >& b
);


}

#endif


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
