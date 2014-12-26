/*****************************************************************************

    sparse_multipolynom.hpp

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
    \file sparse_multipolynom.hpp
    \brief Classes to allow computing with multivariate polynomials.

    This file contains a definition of multimonom, sparse_multipolynom
    template classes and related procedures to represent the concept of
    a multivariate polynomial.

    This is a sparse representation; only monoms with non-zero coefficients
    are held.

    In this file the terms monom and multimonom are interchangeable used
    if there is no other instructions.

    Classes in this file allow us for computing with both statically fixed
    and mutable in runtime the number of variables. By the default all
    things are inteded for computing with dynamically set the number of
    variables but providing an appropriate type for the multidegree storage
    you can allow computing with the statically fixed number.

    WARNIGN! All represented in this file do not have mixed computation
    compatibility.
*/


#ifndef _ARAGELI_sparse_multipolynom_hpp_
#define _ARAGELI_sparse_multipolynom_hpp_

#include "config.hpp"

#include <list>
#include <algorithm>
#include <functional>

#include "frwrddecl.hpp"
#include "factory.hpp"
#include "gcd.hpp"
#include "functional.hpp"
#include "vector.hpp"
#include "sparse_polynom.hpp"


namespace Arageli
{


/// Compare functors for multimonomial degrees.
template <typename MD1, typename MD2>
struct cmp_multidegree_lex : public std::binary_function<MD1, MD2, int>
{
    int operator() (MD1 a, MD2 b) const
    {
        if(a.size() > b.size())
            b.resize(a.size(), null(a.front()));
        else if(a.size() < b.size())
            a.resize(b.size(), null(b.front()));

        return cmp(a, b);
    }
};


/// Monom with multiple variables.
/** This class represents a monom with multiple variables.
    The representation is a pair: coefficient of type F and degree which
    stored as a vector of type MD. Class MD may not have a method to retrive
    its size (i.e. the number of variables in monom) and the user should
    hold the number of variables somewhere else.

    Note that the zero monom is not only the value of monom() but
    any x for which x.is_null() is satisfied. So if we should test
    monom x for zero equality we should check x.is_null() rather than
    x == monom().

    @param F type of the coefficient
    @param D type of the degree for each variable; it's an integer;
        by the default --- int
    @param MD type of the multiple degree for whole monom; it's a vector
        of integers; by the default --- Arageli::vector<D>. */
template <typename F, typename D, typename MD>
class multimonom
{
    template <typename F1, typename D1, typename MD1>
    friend class multimonom;

public:

    typedef F coef_type;    ///< Coefficient type.
    typedef D degree_type;    ///< Degree type.
    typedef MD multidegree_type;    ///< Multiple degree type.

    /// Type of indeces over variables.
    typedef typename MD::size_type size_type;


    /// Zero monom.
    /** Creates monom with zero coefficient and zero degree. */
    multimonom () :
        coef_m(null<coef_type>()),
        multidegree_m(null<multidegree_type>())
    {}

    /// Monom with zero degree --- just a constant of type F.
    /** Creates monom with zero degree and given coefficient.
        @param c initial value for coefficient. */
    multimonom (const F& c) :
        coef_m(c),
        multidegree_m(null<multidegree_type>())
    {}

    /// Creates monom from coefficient value and multidegree value.
    /** @param c coefficient value
        @param d multiple degree value */
    multimonom (const F& c, const MD& md) :
        coef_m(c),
        multidegree_m(md)
    {}

    /// Conversion from multimonom with any other actual template arguments.
    template <typename F1, typename D1, typename MD1>
    multimonom (const multimonom<F1, D1, MD1>& x) :
        coef_m(x.coef_m),
        multidegree_m(x.multidegree_m)
    {}

    /// Initialization from a string representation of monom.
    /** Rules for the argument string is the same as for operator>>. */
    multimonom (const char* s);    // WARNING! It isn't implemented.

    /// Conversion from multimonom with any other actual template arguments.
    template <typename F1, typename D1, typename MD1>
    multimonom& operator= (const multimonom<F1, D1, MD1>& x)
    {
        coef_m = x.coef_m; multidegree_m = x.multidegree_m;
        return *this;
    }

    /// Initialization from a string representation of monom.
    /** Rules for the argument string is the same as for operator>>. */
    multimonom& operator= (const char* s)
    {
        return *this = multimonom(s);
    }

    /// Determines if monom is constant.
    /** Returns true if the degree is zero value. */
    bool is_const () const
    {
        return Arageli::is_null(multidegree_m);
    }

    /// Determines if monom is zero monom.
    /** Returns true if the coefficient is zero value.
        The result does not depend on the degree value. */
    bool is_null () const
    {
        return Arageli::is_null(coef_m);
    }

    /// Determines if monom is unit.
    /** Returns ture if the coefficient is unit and
        the degree is zero value. */
    bool is_unit () const
    {
        return
            Arageli::is_unit(coef_m) &&
            Arageli::is_null(multidegree_m);
    }

    /// Determines if monom is minus unit.
    /** Returns true if the coefficient is minus unit and
        the degree is zero value */
    bool is_opposite_unit () const
    {
        return
            Arageli::is_opposite_unit(coef_m) &&
            Arageli::is_null(multidegree_m);
    }

    /// Returns true if monom is just one variable with coefficient 1.
    /** Returns true if coefficient = 1 and only one element of the multidegree
        is equal 1 and others are equal 0. This methrod requires MD has
        begin/end members.

        WARNING! It isn't implemented yet. */
    bool is_x () const;

    /// Number of variables in monom.
    /** Nevertheless some of them can have degree 0. */
    size_type nvars () const
    {
        return multidegree_m.size();
    }

    /// Expand or reduce the multimonom variable set.
    void renvars (size_type n)
    {
        multidegree_m.resize(n, null<D>());
    }

    /// Access to the degree of variable i. Read only.
    /** The same as operator[]. */
    const degree_type& degree (size_type i) const
    {
        return multidegree_m[i];
    }

    /// Access to the degree of variable i.
    /** The same as operator[]. */
    degree_type& degree (size_type i)
    {
        return multidegree_m[i];
    }

    /// Access to the degree of variable i. Read only.
    /** The same as degree(i). */
    const degree_type& operator[] (size_type i) const
    {
        return multidegree_m[i];
    }

    /// Access to the degree of variable i.
    /** The same as degree(i). */
    degree_type& operator[] (size_type i)
    {
        return multidegree_m[i];
    }

    /// Access to the coefficient. Read only.
    const coef_type& coef () const
    {
        return coef_m;
    }

    /// Access to the coefficient.
    coef_type& coef ()
    {
        return coef_m;
    }

    /// Access to the multiple degree. Read only.
    const multidegree_type& multidegree () const
    {
        return multidegree_m;
    }

    /// Access to the multiple degree.
    multidegree_type& multidegree ()
    {
        return multidegree_m;
    }

    /// Calculates the scalar degree as a sum of all multiple degrees.
    degree_type degree () const
    {
        return sum(multidegree_m);
    }

    /// Determines if monoms are correct for addition.
    /** Returns true if this monom can add with the argument,
        i.e. if they have the same degrees. */
    template <typename MM1>
    bool can_add (const MM1& x) const
    {
        return
            !cmp_multidegree_lex
            <
                multidegree_type,
                typename MM1::multidegree_type
            >()(multidegree_m, x.multidegree());
    }

    /// Inverts the coefficient to opposite value.
    multimonom& opposite ()
    {
        Arageli::opposite(&coef_m);
        return *this;
    }

    /// Returns a monom with opposite coefficient.
    multimonom operator- () const
    {
        return multimonom(Arageli::opposite(coef_m), multidegree_m);
    }

    /// Returns a reference to this monom. For symmetry with operator- only.
    const multimonom& operator+ () const
    {
        return *this;
    }

    /// Prefix increment of the coefficient. Only for zero degree.
    multimonom& operator++ ()
    {
        ARAGELI_ASSERT_0(is_const());
        ++coef_m;
        return *this;
    }

    /// Postfix increment of the coefficient. Only for zero degree.
    multimonom operator++ (int)
    {
        multimonom t = *this;
        operator++();
        return t;
    }

    /// Prefix decrement of the coefficient. Only for zero degree.
    multimonom& operator-- ()
    {
        ARAGELI_ASSERT_0(is_const());
        --coef_m;
        return *this;
    }

    /// Postfix decrement of the coefficient. Only for zero degree.
    multimonom operator-- (int)
    {
        multimonom t = *this;
        operator--();
        return t;
    }

    /// Adds constant to this monom.
    /** Adds the argument x to this monom. Returns reference to this monom.
        Requirement: this->is_const(). */
    template <typename F1>
    multimonom& operator+= (const F1& x)
    {
        ARAGELI_ASSERT_0(is_const());
        coef_m += x;
        return *this;
    }

    /// Subtracts constant from this monom.
    /** Subtracts the argument x to this monom.
        Returns reference to this monom.
        Requirement: this->is_const(). */
    template <typename F1>
    multimonom& operator-= (const F1& x)
    {
        ARAGELI_ASSERT_0(is_const());
        coef_m -= x;
        return *this;
    }

    /// Multilies this monom by constant.
    /** Multilies this monom by the argument x.
        Returns reference to this monom. */
    template <typename F1>
    multimonom& operator*= (const F1& x)
    {
        coef_m *= x;
        return *this;
    }

    /// Divides this monom by constant.
    /** Divides this monom by the argument x.
        Returns reference to this monom.
        Requirement: x is not zero. */
    template <typename F1>
    multimonom& operator/= (const F1& x)
    {
        ARAGELI_ASSERT_0(!Arageli::is_null(x));
        coef_m /= x;
        return *this;
    }

    /// Computes remainder from division this monom by constant.
    /** Divides this monom by the argument x and assign the result to this
        monom object. Returns reference to this monom.
        Requirement: x is not zero. */
    template <typename F1>
    multimonom& operator%= (const F1& x)
    {
        ARAGELI_ASSERT_0(!Arageli::is_null(x));
        coef_m %= x;
        return *this;
    }

    /// Adds two monoms.
    /** Adds the argument x to this monom. Returns reference to this monom.
        Requirement: this->can_add(x). */
    template <typename F1, typename D1, typename MD1>
    multimonom& operator+= (const multimonom<F1, D1, MD1>& x)
    {
        ARAGELI_ASSERT_0(can_add(x));
        coef_m += x.coef_m;
        return *this;
    }

    /// Subtracts two monoms.
    /** Subtracts the argument x to this multimonom.
        Returns reference to this multimonom.
        Requirement: this->can_add(x). */
    template <typename F1, typename D1, typename MD1>
    multimonom& operator-= (const multimonom<F1, D1, MD1>& x)
    {
        ARAGELI_ASSERT_0(can_add(x));
        coef_m -= x.coef_m;
        return *this;
    }

    /// Multiplies two monoms.
    /** Multiplies this multimonom by the argument x.
        Returns reference to this multimonom. */
    template <typename F1, typename D1, typename MD1>
    multimonom& operator*= (const multimonom<F1, D1, MD1>& x)
    {
        coef_m *= x.coef_m;

        if(multidegree_m.size() > x.multidegree_m.size())
        {
            for(size_type i = 0; i < x.multidegree_m.size(); ++i)
                multidegree_m[i] += x.multidegree_m.size(); // WARNING! IS IT CORRECT?
        }
        else if(multidegree_m.size() < x.multidegree_m.size())
            multidegree_m.resize(x.multidegree_m.size());

        multidegree_m += x.multidegree_m;
        return *this;
    }

    /// Divides two monoms.
    /** Divides this monom by the argument x.
        This method can leave the monom with zero coefficient but
        with non-zero degree.
        Returns reference to this monom. */
    template <typename F1, typename D1, typename MD1>
    multimonom& operator/= (const multimonom<F1, D1, MD1>& x)
    {
        ARAGELI_ASSERT_0(!Arageli::is_null(x.coef_m));
        //ARAGELI_ASSERT_0(nvars() == x.nvars());

        if(multidegree_m.size() < x.multidegree_m.size())
            multidegree_m.resize
            (
                x.multidegree_m.size(),
                null(x.multidegree_m.front())
            );

        size_type nv = x.nvars();

        for(size_type i = 0; i < nv; ++i)
            if(multidegree_m[i] >= x.multidegree_m[i])
                multidegree_m[i] -= x.multidegree_m[i];
            else
            {
                multidegree_m = null(multidegree_m);
                coef_m = null(coef_m);
                return *this;
            }

        // the following division can produce coef_m == 0 but
        // the multiple degree can have non-zero values
        coef_m /= x.coef_m;

        return *this;
    }

    /// Computes remainder from division this monom by another monom.
    /** Divides this monom by the argument x and assign the result to this
        monom object. Returns reference to this monom.
        Requirement: x is not zero. */
    template <typename F1, typename D1, typename MD1>
    multimonom& operator%= (const multimonom<F1, D1, MD1>& x)
    {
        //ARAGELI_ASSERT_0(nvars() == x.nvars());

        if(multidegree_m.size() < x.multidegree_m.size())
            multidegree_m.resize
            (
                x.multidegree_m.size(),
                null(x.multidegree_m.front())
            );

        size_type nv = x.nvars();
        for(size_type i = 0; i < nv; ++i)
            if(multidegree_m[i] < x.multidegree_m[i])
                return *this;

        // the following division can produce coef_m == 0 but
        // the multiple degree can have non-zero values
        coef_m %= x.coef_m;

        return *this;
    }

    /// Swaps two monom values.
    template <typename F1, typename D1, typename MD1>
    void swap (multimonom<F1, D1, MD1>& x)
    {
        std::swap(coef_m, x.coef_m);
        std::swap(multidegree_m, x.multidegree_m);
    }


private:

    F coef_m;
    MD multidegree_m;
};


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename F2,
    typename D2,
    typename MD2
>
inline multimonom<F1, D1, MD1> operator+
(
    multimonom<F1, D1, MD1> a,
    const multimonom<F2, D2, MD2>& b
)
{
    return a += b;
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename F2,
    typename D2,
    typename MD2
>
inline multimonom<F1, D1, MD1> operator-
(
    multimonom<F1, D1, MD1> a,
    const multimonom<F2, D2, MD2>& b
)
{
    return a -= b;
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename F2,
    typename D2,
    typename MD2
>
inline multimonom<F1, D1, MD1> operator*
(
    multimonom<F1, D1, MD1> a,
    const multimonom<F2, D2, MD2>& b
)
{
    return a *= b;
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename F2,
    typename D2,
    typename MD2
>
inline multimonom<F1, D1, MD1> operator/
(
    multimonom<F1, D1, MD1> a,
    const multimonom<F2, D2, MD2>& b
)
{
    return a /= b;
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename F2,
    typename D2,
    typename MD2
>
inline multimonom<F1, D1, MD1> operator%
(
    multimonom<F1, D1, MD1> a,
    const multimonom<F2, D2, MD2>& b
)
{
    return a %= b;
}


template <typename P1, typename P2>
P1 gcd_mm_by_mm (const P1& a, const P2& b)
{
    ARAGELI_ASSERT_0(a.nvars() == b.nvars());

    typedef typename P1::size_type size_type;
    typedef typename P1::multidegree_type MD1;
    size_type nv = a.nvars();
    P1 res(gcd(a.coef(), b.coef()), MD1(nv));

    for(size_type i = 0; i < nv; ++i)
        res[i] = std::min(a[i], b[i]);

    return res;
}


template <typename P1, typename P2>
P1 lcm_mm_by_mm (const P1& a, const P2& b)
{
    ARAGELI_ASSERT_0(a.nvars() == b.nvars());

    typedef typename P1::size_type size_type;
    size_type nv = a.nvars();
    typedef typename P1::multidegree_type MD1;
    P1 res(lcm(a.coef(), b.coef()), MD1(nv));

    for(size_type i = 0; i < nv; ++i)
        res[i] = std::max(a[i], b[i]);

    return res;
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename F2,
    typename D2,
    typename MD2
>
inline multimonom<F1, D1, MD1> gcd
(
    const multimonom<F1, D1, MD1>& a,
    const multimonom<F2, D2, MD2>& b
)
{
    return gcd_mm_by_mm(a, b);
}


template <typename F1, typename D1, typename MD1>
inline multimonom<F1, D1, MD1> gcd
(
    const multimonom<F1, D1, MD1>& a,
    const multimonom<F1, D1, MD1>& b
)
{
    return gcd_mm_by_mm(a, b);
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename F2,
    typename D2,
    typename MD2
>
inline multimonom<F1, D1, MD1> lcm
(
    const multimonom<F1, D1, MD1>& a,
    const multimonom<F2, D2, MD2>& b
)
{
    return lcm_mm_by_mm(a, b);
}


template <typename F1, typename D1, typename MD1>
inline multimonom<F1, D1, MD1> lcm
(
    const multimonom<F1, D1, MD1>& a,
    const multimonom<F1, D1, MD1>& b
)
{
    return lcm_mm_by_mm(a, b);
}


template <typename F, typename D, typename MD>
inline bool is_null (const multimonom<F, D, MD>& x)
{
    return x.is_null();
}


template
<
    typename M1, typename M2 = M1,
    typename CmpF = gfunc::cmp,    // WARNING! gfunc::cmp
    typename CmpMD = cmp_multidegree_lex
    <
        typename M1::multidegree_type,
        typename M2::multidegree_type
    >
>
class combined_cmp_multimonom :
    public std::binary_function<M1, M2, int>
{
    CmpF cmpf_m;
    CmpMD cmpmd_m;

public:

    combined_cmp_multimonom
    (
        const CmpF& cmpf = CmpF(),
        const CmpMD& cmpmd = CmpMD()
    ) :
        cmpf_m(cmpf),
        cmpmd_m(cmpmd)
    {}

    int operator() (const M1& m1, const M2& m2) const
    {
        if(int dres = cmpmd_m(m1.multidegree(), m2.multidegree()))
            return dres;
        else
            return cmpf_m(m1.coef(), m2.coef());
    }
};


/// Standard cmp for two multivariate monomials.
template
<
    typename F1,
    typename D1,
    typename MD1,
    typename F2,
    typename D2,
    typename MD2
>
int cmp
(
    const multimonom<F1, D1, MD1>& a,
    const multimonom<F2, D2, MD2>& b
)
{
    combined_cmp_multimonom
    <
        multimonom<F1, D1, MD1>,
        multimonom<F2, D2, MD2>
    >
        tcmp;

    return tcmp(a, b);
}


#define _ARAGELI_SPARSE_MULTIMONOM_CMP(OPER)    \
    template    \
    <    \
        typename F1,    \
        typename D1,    \
        typename MD1,    \
        typename F2,    \
        typename D2,    \
        typename MD2    \
    >    \
    inline bool operator OPER    \
    (    \
        const multimonom<F1, D1, MD1>& a,    \
        const multimonom<F2, D2, MD2>& b    \
    )    \
    {    \
        return cmp(a, b) OPER 0;    \
    }

/// @name Standard comparison operators that based on cmp function.
// @{

_ARAGELI_SPARSE_MULTIMONOM_CMP(==)
_ARAGELI_SPARSE_MULTIMONOM_CMP(!=)
_ARAGELI_SPARSE_MULTIMONOM_CMP(<=)
_ARAGELI_SPARSE_MULTIMONOM_CMP(>=)
_ARAGELI_SPARSE_MULTIMONOM_CMP(<)
_ARAGELI_SPARSE_MULTIMONOM_CMP(>)

// @}


// Multimonom input/output procedures use the same default
// notation as monom class from sparse_polynom.hpp.

extern const char* monom_input_list_first_bracket_default;
extern const char* monom_output_list_first_bracket_default;
extern const char* monom_input_list_second_bracket_default;
extern const char* monom_output_list_second_bracket_default;
extern const char* monom_input_list_separator_default;
extern const char* monom_output_list_separator_default;
extern const char* monom_input_var_mul_default;
extern const char* monom_output_var_mul_default;
extern const char* monom_input_var_var_default;
extern const char* monom_output_var_var_default;
extern const char* monom_input_var_pow_default;
extern const char* monom_output_var_pow_default;


/// Outputs multimonom in the notation with formal variable.
template
<
    typename Ch,
    typename ChT,
    typename F,
    typename D,
    typename MD
>
inline std::basic_ostream<Ch, ChT>& output_list
(
    std::basic_ostream<Ch, ChT>& out,
    const multimonom<F, D, MD>& x,
    const char* first_bracket = monom_output_list_first_bracket_default,
    const char* second_bracket = monom_output_list_second_bracket_default,
    const char* separator = monom_output_list_separator_default
)
{
    return
        out
            << first_bracket << x.coef()
            << separator
            << x.multidegree() << second_bracket;
}


/// Outputs multimonom in the notation with formal variable.
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
    bool first_a = true,
    const char* var = monom_output_var_var_default,
    const char* mul = monom_output_var_mul_default,
    const char* pow = monom_output_var_pow_default
);


/// Inputs multimonom in the list notation.
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
    const char* first_bracket = monom_input_list_first_bracket_default,
    const char* second_bracket = monom_input_list_second_bracket_default,
    const char* separator = monom_input_list_separator_default
);


/// Inputs multimonom in the notation with formal variable.
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
    const char* var = monom_input_var_var_default,
    const char* mul = monom_input_var_mul_default,
    const char* pow = monom_input_var_pow_default
);


/// Inputs multimonom in the notation with formal variable.
/** This version of the function input_var for multimonom
    don't give a factory object; this function uses the default
    factory.

    For more information on arguments see another version of the
    input_var function for multimonom. */
template
<
    typename Ch,
    typename ChT,
    typename F,
    typename D,
    typename MD
>
std::basic_istream<Ch, ChT>& input_var
(
    std::basic_istream<Ch, ChT>& in,
    multimonom<F, D, MD>& x,
    bool first_a = true,
    const char* var = monom_input_var_var_default,
    const char* mul = monom_input_var_mul_default,
    const char* pow = monom_input_var_pow_default
)
{
    return input_var
    (
        in,
        x,
        first_a,
        factory<F>(),
        monom_input_var_var_default,
        monom_input_var_mul_default,
        monom_input_var_pow_default
    );
}


// ***************************************************************************

/////////////////////////////////////
///////////// WARNING! //////////////
//// RELOCATE TO A SEPARATE FILE ////
/////////////////////////////////////


/// Addition of two multivariate polynomials.
/** The result is stored in p3 and returned a reference to it. */
template <typename P1, typename P2, typename P3>
P3& plus_smp_by_smp (const P1& p1, const P2& p2, P3& p3);

/// Subtraction of two multivariate polynomials.
/** The result is stored in p3 and returned a reference to it. */
template <typename P1, typename P2, typename P3>
inline P3& minus_smp_by_smp (const P1& p1, const P2& p2, P3& p3)
{
    return plus_smp_by_smp(p1, -p2, p3);    // WARNING! Temporary implementation.
}

/// Multiplication of two multivariate polynomials.
/** The result is stored in p3 and returned a reference to it. */
template <typename P1, typename P2, typename P3>
P3& multiplies_smp_by_smp (const P1& p1, const P2& p2, P3& p3);

/// Performs division of two sparse multivariate polynomials.
/**    @param p1 the dividend
    @param p2 the divisor
    @param q the resulting quotient
    @param r the resulting remainder    */
template <typename P1, typename P2, typename Q, typename R>
void divide_smp_by_smp (const P1& p1, const P2& p2, Q& q, R& r);

/// Quotient of the division of two multivariate polynomials.
/** The result is stored in p3 and returned a reference to it. */
template <typename P1, typename P2, typename P3>
inline P3& divides_smp_by_smp (const P1& p1, const P2& p2, P3& p3)
{
    P3 r;
    divide_smp_by_smp(p1, p2, p3, r);
    return p3;
}

/// Remainder of the division of two multivariate polynomials.
/** The result is stored in p3 and returned a reference to it. */
template <typename P1, typename P2, typename P3>
inline P3& modulus_smp_by_smp (const P1& p1, const P2& p2, P3& p3)
{
    P3 q;
    divide_smp_by_smp(p1, p2, q, p3);
    return p3;
}


/// Addition of two multivariate polynomials.
template <typename P1, typename P2>
inline P1 plus_smp_by_smp (const P1& p1, const P2& p2)
{
    P1 res;
    return plus_smp_by_smp(p1, p2, res);
}

/// Subtraction of two multivariate polynomials.
template <typename P1, typename P2>
inline P1 minus_smp_by_smp (const P1& p1, const P2& p2)
{
    P1 res;
    return minus_smp_by_smp(p1, p2, res);
}

/// Multiplication of two multivariate polynomials.
template <typename P1, typename P2>
inline P1 multiplies_smp_by_smp (const P1& p1, const P2& p2)
{
    P1 res;
    return multiplies_smp_by_smp(p1, p2, res);
}

/// Quotient of the division of two multivariate polynomials.
template <typename P1, typename P2>
inline P1 divides_smp_by_smp (const P1& p1, const P2& p2)
{
    P1 res;
    return divides_smp_by_smp(p1, p2, res);
}

/// Remainder of the division of two multivariate polynomials.
template <typename P1, typename P2>
inline P1 modulus_smp_by_smp (const P1& p1, const P2& p2)
{
    P1 res;
    return modulus_smp_by_smp(p1, p2, res);
}

/// Compares two sparse multivariate polynomials with the particular cmp.
template <typename P1, typename P2, typename CmpM>
int cmp_smp_by_smp (const P1& p1, const P2& p2, CmpM cmpm);

/// Compares two sparse multivariate polynomials with standard cmp.
template <typename P1, typename P2>
inline int cmp_smp_by_smp (const P1& p1, const P2& p2)
{
    return cmp_smp_by_smp(p1, p2, gfunc::cmp());    // WARNING! gfunc::cmp
}


/////////////////////////////////////


// ***************************************************************************


/// Holds some configuration properties for sparse_multipolynom class.
/** This class incupsulates the following concepts: the multidegree order,
    turn on/off reference counting. We declare that the coefficients
    are being compared by the standard cmp. */
template <typename F, typename D, typename MD, bool REFCNT>
struct sparse_multipolynom_config_default
{
    static const bool refcounting = REFCNT;

    /// Type for comparison "less than" object for multidegrees.
    typedef func::less_by_cmp<MD, MD, cmp_multidegree_lex<MD, MD> >
        multidegree_less_type;

    /// Type for comparison "cmp" object for multidegrees.
    typedef cmp_multidegree_lex<MD, MD> multidegree_cmp_type;

    /// Test if the sequence [begin, end) of multimonoms is valid.
    /** The correctness of the sequence of multimonoms means that
        they are well ordered and there are no zero monoms. */
    template <typename T>
    static bool is_normal (T begin, T end)
    {
        // WARNING! THE SAME IMPLEMENTATION AS FOR sparse_polynom.
        // TODO: Make common function.

        if(begin != end)
        {
            T i = begin;
            for(++i; i != end; ++i)
                if(i->nvars() != begin->nvars())
                    return false;
        }

        return std::adjacent_find
        (
            begin,
            end,
            std::not2
            (
                func::make_less_by_cmp
                (
                    combined_cmp_multimonom
                    <
                        multimonom<F, D, MD>,
                        multimonom<F, D, MD>,
                        gfunc::cmp,
                        multidegree_cmp_type
                    >
                    (
                        gfunc::cmp(),
                        multidegree_cmp()
                    )
                )
            )
        ) == end
        &&
        (begin == end || !begin->is_null());
    }

    /// Normalizes the sequence [begin, end) of multimonoms.
    /** Gathers terms, eliminates zero monoms (with the help of cont.erase
        function) and sort them in correct order. After calling this function,
        the call of function is_normal(begin, end) returns true. */
    template <typename T, typename C>
    static void normalize (T begin, T end, C cont)
    {
        // WARNING! THE SAME IMPLEMENTATION AS FOR sparse_polynom.
        // TODO: Make common function.

        // First make all multidegrees the same size.

        std::size_t maxsize = 0;
        for(T i = begin; i != end; ++i)
            if(i->nvars() > maxsize)
                maxsize = i->nvars();

        for(T i = begin; i != end; ++i)
            i->renvars(maxsize);    // WARNING! EXPLICIT 0.

        // Second remove all zero monoms and sort residuary ones.

        cont.remove_if(func::is_null<multimonom<F, D, MD> >());
        cont.sort
        (
            func::make_less_by_cmp
            (
                combined_cmp_multimonom
                <
                    multimonom<F, D, MD>,
                    multimonom<F, D, MD>,
                    gfunc::cmp,
                    multidegree_cmp_type
                >
                (gfunc::cmp(), multidegree_cmp())
            )
        );

        // Collect terms.

        for(T i = begin;;)
        {
            i = std::adjacent_find
                (
                    i,
                    end,
                    std::not2
                    (
                        combined_cmp_multimonom
                        <
                            multimonom<F, D, MD>,
                            multimonom<F, D, MD>,
                            gfunc::cmp,
                            multidegree_cmp_type
                        >
                        (gfunc::cmp(), multidegree_cmp())
                    )
                );

            if(i == end)break;

            T j = i;
            ++j;
            ARAGELI_ASSERT_1(j != end);
            ARAGELI_ASSERT_1(!Arageli::is_null(*i));
            ARAGELI_ASSERT_1(!Arageli::is_null(*j));

            if((*j += *i).is_null())
                i = cont.erase(i);
            i = cont.erase(i);
        }
    }

    /// Compare two multiple degrees.
    template <typename MD1, typename MD2>
    static int cmp (const MD1& md1, const MD2& md2);    // WARNING! It isn't implemented.

    /// Calculates the degree of the normal sequence [begin, end) of monoms.
    /** The degree of the sequence is the maximum from all scalar degrees
        of monoms of the sequence. If the sequence is empty, the function
        returns -1. */
    template <typename T>
    static D degree_normal (T begin, T end)
    {
        // WARNING! Only for purelex order.

        if(begin == end)
            return -1;
        return (--end)->degree();
    }

    /// Comparison object for multidegrees.
    static multidegree_less_type multidegree_less ()
    {
        return multidegree_less_type();
    }

    /// Comparison object for multidegrees.
    static multidegree_cmp_type multidegree_cmp ()
    {
        return multidegree_cmp_type();
    }
};


// ***************************************************************************


/// Polynomial with multiple variables with sparse representaion.
/** The polynomial is represented as a list of multimonoms.

    WARNING! This class is very incomplete. */
template <typename F, typename D, typename MD, typename Config>
class sparse_multipolynom
{
    template <typename F1, typename D1, typename MD1, typename Config1>
    friend class sparse_multipolynom;

public:

    /// Coefficient type.
    typedef F coef_type;

    /// Degree type.
    typedef D degree_type;

    /// Multiple degree type.
    typedef MD multidegree_type;

    /// Cofigurator type.
    typedef Config config_type;

    typedef typename Config::multidegree_less_type multidegree_less_type;
    typedef typename Config::multidegree_cmp_type multidegree_cmp_type;

    /// Multivariate monom type that is used to building the polynomial.
    typedef multimonom<F, D, MD> monom;

    /// Reference counter property. True iff the counter is turned on.
    static const bool refcounting = config_type::refcounting;

private:

    typedef std::list<monom> Rep;

public:

    /// Type for variables indexing and the number of monoms representing.
    typedef typename Rep::size_type size_type;


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Constructors.

    //@{

    /// Zero polynomial. It's an empty set of monoms.
    sparse_multipolynom ()
    {}


    /// Creates polynomial as a constant.
    /** If x is zero, the result is the same as for sparse_multipolynom(). */
    sparse_multipolynom (const F& x)
    {
        if(!Arageli::is_null(x))
            rep().push_back(x);
    }


    /// Creates polynomial as one monom(x, md).
    /**    If x is zero, the result is the same as for sparse_multipolynom(). */
    sparse_multipolynom (const F& x, const MD& md)
    {
        if(!Arageli::is_null(x))
            rep().push_back(monom(x, md));
    }


    /// Creates polynomial as one monom.
    /**    If x is zero monom, the result is the same as for
        sparse_multipolynom(). */
    template <typename F1, typename D1, typename MD1>
    sparse_multipolynom (const Arageli::multimonom<F1, D1, MD1>& x)
    {
        if(!x.is_null())rep().push_back(x);
    }


    /// Creates a copy of the object of type sparse_multipolynom.
    /** The number of monoms can be reduced during this transformation bcause
        the conversion from F1 to F can produce zero coefficients from
        non-zeros. */
    template <typename F1, typename D1, typename MD1, typename Config1>
    sparse_multipolynom (const sparse_multipolynom<F1, D1, MD1, Config1>& x)
    {
        rep().assign(x.rep().begin(), x.rep().end());
        normalize();
    }


    /// Initializes from some sequence [first, last) of multimonoms.
    template <typename In>
    sparse_multipolynom (In first, In last, const any_monom_seq_t&)
    {
        rep().assign(first, last);
        normalize();
    }


    /// Initializes by its string notation.
    /**    Input rules are the same as for operator>> with default behaviour. */
    sparse_multipolynom (const char* str);

    //@}

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    /// Reinitializes by 0.
    void assign ()
    {
        if(unique_clear())
            rep().clear();
    }


    /// Comparison "less than" object for multidegrees.
    multidegree_less_type multidegree_less () const
    {
        return config_m.multidegree_less();
    }


    /// Comparison "cmp" object for multidegrees.
    multidegree_cmp_type multidegree_cmp () const
    {
        return config_m.multidegree_cmp();
    }


    /// Returns true if this is zeroth polynomial.
    /** I.e. returns true if polynomial does not containes any monoms. */
    bool is_null () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return rep().empty();
    }


    /// Returns true if polynomial has only absolute term that equal to 1.
    bool is_unit () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return rep().size() == 1 && rep().front().is_unit();
    }


    /// Returns true if polynomial has only absolute term that equal to -1.
    bool is_opposite_unit () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return rep().size() == 1 && rep().front().is_opposite_unit();
    }


    /// Returns true if polynomial have no monoms with positive degree of x.
    bool is_const () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return rep().empty() || rep().size() == 1 && rep().front().is_const();
    }


    /// Returns true if polynomial has normal form.
    /** Normal form of polynomial has ascending sorted (by degrees in the
        meadning of the configurator) monom sequence and all monoms have
        different degrees and non-zero coefficients.  */
    bool is_normal () const
    {
        return config_m.is_normal(rep().begin(), rep().end());
    }


    /// Returns non-const reference to leading monom.
    /** Requirement: !is_null(). */
    monom& leading_monom ()
    {
        ARAGELI_ASSERT_0(is_normal());
        ARAGELI_ASSERT_0(!rep().empty());
        store.unique();
        return rep().back();
    }


    /// Returns const reference to leading monom.
    /** Requirement: !is_null(). */
    const monom& leading_monom () const
    {
        ARAGELI_ASSERT_0(is_normal());
        ARAGELI_ASSERT_0(!rep().empty());
        return rep().back();
    }


    /// Returns a copy of leading monom.
    /** For zeroth polynomial returns zero monom (i.e. monom()). */
    monom leading_monom_cpy () const
    {
        ARAGELI_ASSERT_0(is_normal());

        return
            rep().empty() ?
            monom() :
            leading_monom();
    }


    /// Returns degree of polynomial (the greatest scalar degree value).
    /** For zeroth polynomial returns -1. */
    degree_type degree () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return config_m.degree_normal(rep().begin(), rep().end());
    }


    /// Returns number of monoms in polynomial.
    /** Returns 0 if and only if the polynomial is zeroth. */
    size_type size () const
    {
        return rep().size();
    }


    /// Returns non-const reference to coefficient of leading monom.
    /** Requirement: !is_null(). */
    coef_type& leading_coef ()
    {
        return leading_monom().coef();
    }


    /// Returns const reference to coefficient of leading monom.
    /** Requirement: !is_null(). */
    const coef_type& leading_coef () const
    {
        return leading_monom().coef();
    }


    /// Returns a copy of coefficient of leading monom.
    /** For zeroth polynomial returns zero from dafault factory. */
    coef_type leading_coef_cpy () const
    {
        ARAGELI_ASSERT_0(is_normal());

        return
            rep().empty() ?
            null<coef_type>() :
            leading_monom().coef();
    }


    /// Returns non-const reference to multidegree of leading monom.
    /** Requirement: !is_null(). */
    multidegree_type& leading_multidegree ()
    {
        return leading_monom().multidegree();
    }


    /// Returns const reference to multidegree of leading monom.
    /** Requirement: !is_null(). */
    const multidegree_type& leading_multidegree () const
    {
        return leading_monom().multidegree();
    }


    /// Makes an individual representation for this object.
    /**    If ‘refcounting’ is false then this is empty operation.
        Otherwise it makes so that this sparse_multipolynom descriptor
        gets individual copy of the internal representation.
        Thus after this operation reference counter value
        is equal to one exactly.
        Returns true if the internal representation object is still the same
        and false if new copy has been made. */
    bool unique ()
    {
        return store.unique();
    }


    /// Makes an individual or a clean individual representation for this object.
    /**    If ‘refcounting’ is false this is empty operation.
        Otherwise if reference counter value is one (only this descriptor
        refers to) then immediate returns true.
        Else create for this descriptor new clean internal representation
        object (zeroth polynomial) and return false. */
    bool unique_clear ()
    {
        return store.unique_clear();
    }


    /// Make all monoms having the same nvars value.
    void normalize_nvars ()
    {
        // WARNING! This is a part of config_m.normalize()

        size_type maxsize = 0;
        for(typename Rep::iterator i = rep().begin(); i != rep().end(); ++i)
            if(i->nvars() > maxsize)
                maxsize = i->nvars();

        for(typename Rep::iterator i = rep().begin(); i != rep().end(); ++i)
            i->renvars(maxsize);
    }


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Iterators.
    // @{

    /// This iterator represents polynomial as a sequence of monoms. Non-constant form.
    typedef typename Rep::iterator monom_iterator;

    /// This iterator represents polynomial as a sequence of monoms. Constant form.
    typedef typename Rep::const_iterator monom_const_iterator;

    // @}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Access via iterators.

    // @{

    /// Returns iterator on the begin of a monom sequence.
    monom_iterator monoms_begin ()
    {
        unique();
        return rep().begin();
    }

    /// Returns iterator on the begin of a monom sequence.
    monom_const_iterator monoms_begin () const
    {
        return rep().begin();
    }

    /// Returns iterator on the end of a monom sequence.
    monom_iterator monoms_end ()
    {
        unique();
        return rep().end();
    }

    /// Returns iterator on the end of a monom sequence.
    monom_const_iterator monoms_end () const
    {
        return rep().end();
    }

    // @}


    /// Inserts the multimonom mm after all existing monoms.
    template <typename MM1>
    void push_back (const MM1& mm)
    {
        unique();
        rep().push_back(mm);
    }

    monom_iterator erase (monom_iterator i)
    {
        unique();
        return rep().erase(i);
    }

    /// Polynomial normalization according to the configurator.
    void normalize ()
    {
        if(rep().empty())
            return;
        unique();
        config_m.normalize(rep().begin(), rep().end(), rep());
    }

    /// Flips the sign for each monom. Returns *this.
    sparse_multipolynom& opposite ();

    /// Just *this. For the symmetry with operator-.
    const sparse_multipolynom& operator+ () const
    {
        return *this;
    }

    /// Returns a copy of this polynomial with flipped signs for each monom.
    sparse_multipolynom operator- () const
    {
        sparse_multipolynom t = *this;
        t.opposite();
        return t;
    }

    sparse_multipolynom& operator++ ()
    {
        return *this += sparse_multipolynom(unit<F>());
    }

    sparse_multipolynom operator++ (int)
    {
        sparse_multipolynom t = *this;
        operator++();
        return t;
    }

    sparse_multipolynom& operator-- ()
    {
        return *this -= sparse_multipolynom(unit<F>());
    }

    sparse_multipolynom operator-- (int)
    {
        sparse_multipolynom t = *this;
        operator--();
        return t;
    }

    template <typename F1, typename D1, typename MD1, typename Config1>
    sparse_multipolynom& operator+=
    (const sparse_multipolynom<F1, D1, MD1, Config1>& x)
    {
        ARAGELI_ASSERT_0(is_normal());
        ARAGELI_ASSERT_0(x.is_normal());
        return *this = plus_smp_by_smp(*this, x);
    }

    template <typename F1, typename D1, typename MD1, typename Config1>
    sparse_multipolynom& operator-=
    (const sparse_multipolynom<F1, D1, MD1, Config1>& x)
    {
        ARAGELI_ASSERT_0(is_normal());
        ARAGELI_ASSERT_0(x.is_normal());
        return *this = minus_smp_by_smp(*this, x);
    }

    template <typename F1, typename D1, typename MD1, typename Config1>
    sparse_multipolynom& operator*=
    (const sparse_multipolynom<F1, D1, MD1, Config1>& x)
    {
        ARAGELI_ASSERT_0(is_normal());
        ARAGELI_ASSERT_0(x.is_normal());
        return *this = multiplies_smp_by_smp(*this, x);
    }

    template <typename F1, typename D1, typename MD1, typename Config1>
    sparse_multipolynom& operator/=
    (const sparse_multipolynom<F1, D1, MD1, Config1>& x)
    {
        ARAGELI_ASSERT_0(is_normal());
        ARAGELI_ASSERT_0(x.is_normal());
        return *this = divides_smp_by_smp(*this, x);
    }

    template <typename F1, typename D1, typename MD1, typename Config1>
    sparse_multipolynom& operator%=
    (const sparse_multipolynom<F1, D1, MD1, Config1>& x)
    {
        ARAGELI_ASSERT_0(is_normal());
        ARAGELI_ASSERT_0(x.is_normal());
        return *this = modulus_smp_by_smp(*this, x);
    }


private:

    Rep& rep ()
    {
        return store.value();
    }

    const Rep& rep () const
    {
        return store.value();
    }

    refcntr_proxy<Rep, refcounting> store;
    config_type config_m;    // WARNING! Should we define this in `store'.

};


template <typename F, typename D, typename MD, typename Config>
struct factory<sparse_multipolynom<F, D, MD, Config> >
{
    static const bool is_specialized = true;

    static const sparse_multipolynom<F, D, MD, Config>& unit ()
    {
        static const sparse_multipolynom<F, D, MD, Config>
            unit_s(Arageli::unit<F>());
        return unit_s;
    }

    static sparse_multipolynom<F, D, MD, Config> unit
    (const sparse_multipolynom<F, D, MD, Config>& x)
    {
        if(x.is_null())
            return unit();
        else
            return sparse_multipolynom<F, D, MD, Config>
                (Arageli::unit(x.leading_coef()));
    }

    static const sparse_multipolynom<F, D, MD, Config>& opposite_unit ()
    {
        static const sparse_multipolynom<F, D, MD, Config> opposite_unit_s =
            Arageli::opposite_unit<F>();
        return opposite_unit_s;
    }

    static sparse_multipolynom<F, D, MD, Config> opposite_unit
    (const sparse_multipolynom<F, D, MD, Config>& x)
    {
        if(x.is_null())
            return opposite_unit();
        else
            return sparse_multipolynom<F, D, MD, Config>
                (Arageli::opposite_unit(x.leading_coef()));
    }

    static const sparse_multipolynom<F, D, MD, Config>& null ()
    {
        static const sparse_multipolynom<F, D, MD, Config> null_s;
        return null_s;
    }

    static sparse_multipolynom<F, D, MD, Config> null
    (const sparse_multipolynom<F, D, MD, Config>& x)
    {
        return null();
    }

};

template <typename F, typename D, typename MD, typename Config>
inline bool is_unit (const sparse_multipolynom<F, D, MD, Config>& x)
{
    return x.is_unit();
}

template <typename F, typename D, typename MD, typename Config>
inline bool is_opposite_unit (const sparse_multipolynom<F, D, MD, Config>& x)
{
    return x.is_opposite_unit();
}

template <typename F, typename D, typename MD, typename Config>
inline bool is_null (const sparse_multipolynom<F, D, MD, Config>& x)
{
    return x.is_null();
}

template <typename F, typename D, typename MD, typename Config>
inline sparse_multipolynom<F, D, MD, Config> opposite
(const sparse_multipolynom<F, D, MD, Config>& x)
{
    return -x;
}

template <typename F, typename D, typename MD, typename Config>
inline sparse_multipolynom<F, D, MD, Config>& opposite
(const sparse_multipolynom<F, D, MD, Config>& x, sparse_multipolynom<F, D, MD, Config>* y)
{
    return (*y = x).opposite();
}

template <typename F, typename D, typename MD, typename Config>
inline sparse_multipolynom<F, D, MD, Config>& opposite
(sparse_multipolynom<F, D, MD, Config>* x)
{
    return x->opposite();
}

template <typename F, typename D, typename MD, typename Config>
inline sparse_multipolynom<F, D, MD, Config> inverse (const sparse_multipolynom<F, D, MD, Config>& x)
{
    ARAGELI_ASSERT_0(x.is_const());
    return inverse(x.leading_coef());
}

template <typename F, typename D, typename MD, typename Config>
inline sparse_multipolynom<F, D, MD, Config>& inverse (const sparse_multipolynom<F, D, MD, Config>& x, sparse_multipolynom<F, D, MD, Config>* y)
{
    ARAGELI_ASSERT_0(x.is_const());
    return *y = inverse(x->leading_coef());
}

template <typename F, typename D, typename MD, typename Config>
inline sparse_multipolynom<F, D, MD, Config>& inverse (sparse_multipolynom<F, D, MD, Config>* x)
{
    ARAGELI_ASSERT_0(x->is_const());
    inverse(&x->leading_coef());
    return *x;
}


template <typename F, typename D, typename MD, typename Config>
inline std::ostream& output_polynom_first (std::ostream& out, const sparse_multipolynom<F, D, MD, Config>& x)
{
    return out << '(' << x << ')';
}

template <typename F, typename D, typename MD, typename Config>
inline std::ostream& output_polynom_internal
(std::ostream& out, const sparse_multipolynom<F, D, MD, Config>& x)
{
    return out << "+(" << x << ')';
}

template <typename F, typename D, typename MD, typename Config>
inline std::ostream& output_pow (std::ostream& out, const sparse_multipolynom<F, D, MD, Config>& x)
{
    return out << '(' << x << ')';
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename Config1,
    typename F2,
    typename D2,
    typename MD2,
    typename Config2
>
inline sparse_multipolynom<F1, D1, MD1, Config1> operator+
(
    const sparse_multipolynom<F1, D1, MD1, Config1>& a,
    const sparse_multipolynom<F2, D2, MD2, Config2>& b
)
{
    return plus_smp_by_smp(a, b);
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename Config1,
    typename F2,
    typename D2,
    typename MD2,
    typename Config2
>
inline sparse_multipolynom<F1, D1, MD1, Config1> operator-
(
    const sparse_multipolynom<F1, D1, MD1, Config1>& a,
    const sparse_multipolynom<F2, D2, MD2, Config2>& b
)
{
    return minus_smp_by_smp(a, b);
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename Config1,
    typename F2,
    typename D2,
    typename MD2,
    typename Config2
>
inline sparse_multipolynom<F1, D1, MD1, Config1> operator*
(
    const sparse_multipolynom<F1, D1, MD1, Config1>& a,
    const sparse_multipolynom<F2, D2, MD2, Config2>& b
)
{
    return multiplies_smp_by_smp(a, b);
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename Config1,
    typename F2,
    typename D2,
    typename MD2,
    typename Config2
>
inline sparse_multipolynom<F1, D1, MD1, Config1> operator/
(
    const sparse_multipolynom<F1, D1, MD1, Config1>& a,
    const sparse_multipolynom<F2, D2, MD2, Config2>& b
)
{
    return divides_smp_by_smp(a, b);
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename Config1,
    typename F2,
    typename D2,
    typename MD2,
    typename Config2
>
inline sparse_multipolynom<F1, D1, MD1, Config1> operator%
(
    const sparse_multipolynom<F1, D1, MD1, Config1>& a,
    const sparse_multipolynom<F2, D2, MD2, Config2>& b
)
{
    return modulus_smp_by_smp(a, b);
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename Config1,
    typename F2,
    typename D2,
    typename MD2,
    typename Config2
>
inline int cmp
(
    const sparse_multipolynom<F1, D1, MD1, Config1>& a,
    const sparse_multipolynom<F2, D2, MD2, Config2>& b
)
{
    return cmp_smp_by_smp(a, b);
}


#define _ARAGELI_SPARSE_MULTIPOLYNOMIAL_CMP(OPER)    \
    template    \
    <    \
        typename F1,    \
        typename D1,    \
        typename MD1,    \
        typename Config1,    \
        typename F2,    \
        typename D2,    \
        typename MD2,    \
        typename Config2    \
    >    \
    inline bool operator OPER    \
    (    \
        const sparse_multipolynom<F1, D1, MD1, Config1>& a,    \
        const sparse_multipolynom<F2, D2, MD2, Config2>& b    \
    )    \
    {    \
        return cmp(a, b) OPER 0;    \
    }

/// @name Standard comparison operators that based on cmp function.
// @{

_ARAGELI_SPARSE_MULTIPOLYNOMIAL_CMP(==)
_ARAGELI_SPARSE_MULTIPOLYNOMIAL_CMP(!=)
_ARAGELI_SPARSE_MULTIPOLYNOMIAL_CMP(<=)
_ARAGELI_SPARSE_MULTIPOLYNOMIAL_CMP(>=)
_ARAGELI_SPARSE_MULTIPOLYNOMIAL_CMP(<)
_ARAGELI_SPARSE_MULTIPOLYNOMIAL_CMP(>)

// @}

template
<
    typename F1,
    typename D1,
    typename MD1,
    typename Config1,
    typename F2,
    typename D2,
    typename MD2,
    typename Config2,
    typename F3,
    typename D3,
    typename MD3,
    typename Config3,
    typename F4,
    typename D4,
    typename MD4,
    typename Config4
>
inline void divide
(
    const sparse_multipolynom<F1, D1, MD1, Config1>& p1,
    const sparse_multipolynom<F2, D2, MD2, Config2>& p2,
    sparse_multipolynom<F3, D3, MD3, Config3>& q,
    sparse_multipolynom<F4, D4, MD4, Config4>& r
)
{
    divide_smp_by_smp(p1, p2, q, r);
}


// WARNING! TEMPORARY IMPLEMENTATION
template <typename P>
void smp_output_list
(
    std::ostream& out,
    const P& x,
    monoms_order mo = mo_inc,
    const char* first_bracket = monom_output_list_first_bracket_default,
    const char* second_bracket = monom_output_list_second_bracket_default,
    const char* separator = monom_output_list_separator_default
)
{
    switch(mo)
    {
        case mo_inc:
            polynom_output_list
            (
                out,
                _Internal::make_reverse_iterator(x.monoms_end()),
                _Internal::make_reverse_iterator(x.monoms_begin()),
                first_bracket,
                second_bracket,
                separator
            );
        case mo_dec:
            polynom_output_list
            (
                out,
                x.monoms_begin(),
                x.monoms_end(),
                first_bracket,
                second_bracket,
                separator
            );
    }
}


template <typename F1, typename D1, typename MD1, typename Config1>
inline void output_list
(
    std::ostream& out,
    const sparse_multipolynom<F1, D1, MD1, Config1>& p,
    monoms_order mo = mo_inc,
    const char* first_bracket = monom_output_list_first_bracket_default,
    const char* second_bracket = monom_output_list_second_bracket_default,
    const char* separator = monom_output_list_separator_default
)
{
    smp_output_list(out, p, mo_inc, first_bracket, second_bracket, separator);
}


template
<
    typename F1,
    typename D1,
    typename MD1,
    typename Config1,
    typename Ch,
    typename ChT
>
std::basic_ostream<Ch, ChT>& output_var
(
    std::basic_ostream<Ch, ChT>& out,
    const sparse_multipolynom<F1, D1, MD1, Config1>& x,
    monoms_order mo = mo_inc,
    const char* var = monom_output_var_var_default,
    const char* mul = monom_output_var_mul_default,
    const char* pow = monom_output_var_pow_default
)
{
    ARAGELI_ASSERT_0(mo == mo_inc || mo == mo_dec);

    switch(mo)
    {
        case mo_inc:
            return polynom_output_var
            (
                out,
                _Internal::make_reverse_iterator(x.monoms_end()),
                _Internal::make_reverse_iterator(x.monoms_begin()),
                mo,
                var,
                mul,
                pow
            );
        case mo_dec:
            return polynom_output_var
            (
                out,
                x.monoms_begin(),
                x.monoms_end(),
                mo,
                var,
                mul,
                pow
            );
        default:
            return out;
    }
}


template <typename F1, typename D1, typename MD1, typename Config1>
inline std::ostream& operator<<
(
    std::ostream& out,
    const sparse_multipolynom<F1, D1, MD1, Config1>& p
)
{
    output_var(out, p);
    return out;
}


template
<
    typename Ch,
    typename ChT,
    typename F,
    typename D,
    typename MD,
    typename Config1
>
std::basic_istream<Ch, ChT>& input_list
(
    std::basic_istream<Ch, ChT>& in,
    sparse_multipolynom<F, D, MD, Config1>& x,
    const char* fb = monom_input_list_first_bracket_default,
    const char* sb = monom_input_list_second_bracket_default,
    const char* sep = monom_input_list_separator_default
)
{
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(fb));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(sb));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(sep));
    ARAGELI_ASSERT_0(*sep);

    if(!_Internal::read_literal(in, fb))
        return in;
    typedef sparse_multipolynom<F, D, MD, Config1> P;
    P tmp;
    typename P::monom m;
    if(!input_list(in, m))
        return in;
    tmp += m;

    while(_Internal::read_literal(in, sep))
    {
        if(!input_list(in, m))
            return in;
        tmp += m;
    }

    if(!_Internal::read_literal(in, sb))
        return in;
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
    typename Config1,
    typename Coef_factory
>
std::basic_istream<Ch, ChT>& input_var
(
    std::basic_istream<Ch, ChT>& in,
    sparse_multipolynom<F, D, MD, Config1>& x,
    const Coef_factory& fctr,
    const char* var = monom_input_var_var_default,
    const char* mul = monom_input_var_mul_default,
    const char* pow = monom_input_var_pow_default
)
{
    // WARNING! It's similar to the function for sparse_polynom.

    typedef sparse_multipolynom<F, D, MD, Config1> P;
    P res;

    char ch = 0;
    in >> ch;
    bool brackets = false;
    if(ch == '(')
        brackets = true;
    else in.putback(ch);

    typedef typename P::monom monom;
    monom m;
    input_var(in, m, true, fctr, var, mul, pow);

    if(!in)
    {
        if(in.eof())
        {
            if(brackets)
                in.clear(std::ios_base::badbit);
            else x = m;
        }

        return in;
    }

    ch = in.get();

    if(in.eof())
    {
        if(brackets)
            in.clear(std::ios_base::badbit);
        else
        {
            x = m;
            in.clear();
        }

        return in;
    }

    in.clear();
    in.putback(ch);

    if(ch == '\n')
    {
        if(brackets)
            in.clear(std::ios_base::badbit);
        else
        {
            x = m;
            in.clear();
        }
        return in;
    }

    res += P(m);   // WARNING! TEMPORARY CONVERSION

    for(;;)
    {
        m = monom();
        input_var(in, m, false, fctr, var, mul, pow);

        if(!in)
        {
            if(in.eof())res += P(m);
            break;
        }

        ch = in.get();

        if(in.eof())
        {
            if(brackets)
            {
                in.clear(std::ios_base::badbit);
                return in;
            }

            res += P(m);
            break;
        }

        in.clear();
        in.putback(ch);

        if(ch == '\n')
        {
            if(brackets)
            {
                in.clear(std::ios_base::badbit);
                return in;
            }

            res += P(m);
            break;
        }

        res += P(m);
    }

    in.clear();
    if(brackets && !_Internal::read_literal(in, ")"))
        in.clear(std::ios_base::badbit);
    else
    {
        x = res;
        in.clear();
    }

    return in;
}



template
<
    typename Ch,
    typename ChT,
    typename F,
    typename D,
    typename MD,
    typename Config1
>
inline std::basic_istream<Ch, ChT>& input_var
(
    std::basic_istream<Ch, ChT>& in,
    sparse_multipolynom<F, D, MD, Config1>& x,
    const char* var = monom_input_var_var_default,
    const char* mul = monom_input_var_mul_default,
    const char* pow = monom_input_var_pow_default
)
{
    return input_var(in, x, factory<F>(), var, mul, pow);
}


template <typename F1, typename D1, typename MD1, typename Config1>
inline std::istream& operator>>
(
    std::istream& in,
    sparse_multipolynom<F1, D1, MD1, Config1>& p
)
{
    input_var(in, p);
    return in;
}


} // namesapce Arageli

namespace std
{

template <typename F1, typename D1, typename MD1, typename Config1>
inline
Arageli::sparse_multipolynom<F1, D1, MD1, Config1>
abs (const Arageli::sparse_multipolynom<F1, D1, MD1, Config1>& x)
{
    return x;
}

}


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_sparse_multipolynom
    #include "sparse_multipolynom.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_sparse_multipolynom
#endif

#endif    // #ifndef _ARAGELI_sparse_multipolynom_hpp_
