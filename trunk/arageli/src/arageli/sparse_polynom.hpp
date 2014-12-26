/*****************************************************************************

    sparse_polynom.hpp

    This file is a part of Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
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

/**
    \file
    Sparse polinomial and related classes and operations definition.
    This file contains following definitions for following class templates:
    monom, sparse_polynom and the partialy specialization of type_traits
    template for monom and sparse_polynom. Generaly sparse polynomial is
    a list of monoms with appropriate type for coefficient and degree.
*/


#ifndef _ARAGELI_sparse_polynom_hpp_
#define _ARAGELI_sparse_polynom_hpp_

#include "config.hpp"

#include <iostream>
#include <list>
#include <iomanip>
#include <algorithm>
#include <cmath>

#include "frwrddecl.hpp"

#include "type_traits.hpp"
#include "config.hpp"
#include "exception.hpp"
#include "refcntr.hpp"
#include "iteradapt.hpp"
#include "type_opers.hpp"
#include "factory.hpp"
#include "cmp.hpp"
#include "io.hpp"
#include "powerest.hpp"
#include "big_int.hpp"
#include "rational.hpp"
#include "_utility.hpp"
#include "polyalg.hpp"
//#include "polynom.hpp"

#include "std_import.hpp"

//****************************************************************************

namespace Arageli
{

template <typename T> class rational;


/// Monom template definition.
/** The monom is a pair of the coefficient and the degree.

    Note that the zero monom is not only the value of monom() but
    any x for which x.is_null() is satisfied. So if we should test
    monom x for zero equality we should check x.is_null() rather than
    x == monom().

    @param F type of a coefficient
    @param I type of a degree
*/
template
<
    typename F,
    typename I
>
class monom
{
    template <typename F1, typename I1>
    friend class monom;

public:

    typedef F coef_type;    ///< Coefficient type.
    typedef I degree_type;    ///< Degree type.

    /// Zero monom.
    /** Creates monom with zero coefficient and zero degree. */
    monom () :
        coef_m(factory<coef_type>::null()),
        degree_m(factory<degree_type>::null())
    {}

    /// Monom with zero degree (i.e. a constant of type F).
    /** Creates monom with zero degree and given coefficient.
        @param c the coefficient. */
    monom (const F& c) :
        coef_m(c),
        degree_m(factory<degree_type>::null())
    {}

    /// Creates monom of common form.
    /** @param c the coefficient
        @param d degree */
    monom (const F& c, const I& d) :
        coef_m(c),
        degree_m(d)
    {}

    /// Initialization with conversion from monom with any other actual template arguments.
    template <typename F1, typename I1>
    monom (const monom<F1, I1>& x) :
        coef_m(x.coef_m),
        degree_m(x.degree_m)
    {}

    /// Initialization from a string representation of monom.
    /** Rule for the argument string is the same as for operator>>. */
    monom (const char* s);

    template <typename F1, typename I1>
    monom& operator= (const monom<F1, I1>& x)
    {
        coef_m = x.coef_m; degree_m = x.degree_m;
        return *this;
    }

    monom& operator= (const char* s)
    {
        return *this = monom(s);
    }

    /// Determines if monom is constant.
    /** Returns true if the degree is zero value. */
    bool is_const () const
    {
        return Arageli::is_null(degree_m);
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
            Arageli::is_null(degree_m);
    }

    /// Determines if monom is minus unit.
    /** Returns true if the coefficient is minus unit and
        the degree is zero value */
    bool is_opposite_unit () const
    {
        return
            Arageli::is_opposite_unit(coef_m) &&
            Arageli::is_null(degree_m);
    }


    /// Returns true if monom is "x".
    /** Returns true if degree = 1 and coefficient = 1. */
    bool is_x () const
    {
        return
            Arageli::is_unit(coef_m) &&
            Arageli::is_unit(degree_m);
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

    /// Access to the degree. Read only.
    const degree_type& degree () const
    {
        return degree_m;
    }

    /// Access to the degree.
    degree_type& degree ()
    {
        return degree_m;
    }

    /// Determines if monoms are correct for addition.
    /** Returns true if this monom can add with the argument,
        i.e. if they have the same degrees. */
    template <typename F1, typename I1>
    bool can_add (const monom<F1, I1>& x) const
    {
        return degree_m == x.degree_m;
    }

    /// Inverts the coefficient to opposite value.
    monom& opposite ()
    {
        Arageli::opposite(&coef_m);
        return *this;
    }

    /// Returns a monom with opposite coefficient.
    monom operator- () const
    {
        return monom(Arageli::opposite(coef_m), degree_m);
    }

    /// Returns a reference on this monom. For symmetry with operator- only.
    const monom& operator+ () const
    {
        return *this;
    }

    monom& operator++ ()
    {
        ARAGELI_ASSERT_0(is_const());
        ++coef_m;
        return *this;
    }

    monom operator++ (int)
    {
        monom t = *this;
        operator++();
        return t;
    }

    monom& operator-- ()
    {
        ARAGELI_ASSERT_0(is_const());
        --coef_m;
        return *this;
    }

    monom operator-- (int)
    {
        monom t = *this;
        operator--();
        return t;
    }

    /// Adds constant to this monom.
    /** Adds the argument x to this monom. Returns reference to this monom.
        Requirement: this->is_const(). */
    template <typename F1>
    monom& operator+= (const F1& x)
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
    monom& operator-= (const F1& x)
    {
        ARAGELI_ASSERT_0(is_const());
        coef_m -= x;
        return *this;
    }

    /// Multiplies this monom by constant.
    /** Multiplies this monom by the argument x.
        Returns reference to this monom. */
    template <typename F1>
    monom& operator*= (const F1& x)
    {
        coef_m *= x;
        return *this;
    }

    /// Divides this monom by constant.
    /** Divides this monom by the argument x.
        Returns reference to this monom.
        Requirement: x is not zero. */
    template <typename F1>
    monom& operator/= (const F1& x)
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
    monom& operator%= (const F1& x)
    {
        ARAGELI_ASSERT_0(!Arageli::is_null(x));
        coef_m %= x;
        return *this;
    }

    /// Adds two monoms.
    /** Adds the argument x to this monom. Returns reference to this monom.
        Requirement: this->can_add(x). */
    template <typename F1, typename I1>
    monom& operator+= (const monom<F1, I1>& x)
    {
        ARAGELI_ASSERT_0(can_add(x));
        coef_m += x.coef_m;
        return *this;
    }

    /// Subtracts two monoms.
    /** Subtracts the argument x to this monom.
        Returns reference to this monom.
        Requirement: this->can_add(x). */
    template <typename F1, typename I1>
    monom& operator-= (const monom<F1, I1>& x)
    {
        ARAGELI_ASSERT_0(can_add(x));
        coef_m -= x.coef_m;
        return *this;
    }

    /// Multilies two monoms.
    /** Multilies this monom by the argument x.
        Returns reference to this monom. */
    template <typename F1, typename I1>
    monom& operator*= (const monom<F1, I1>& x)
    {
        coef_m *= x.coef_m;
        degree_m += x.degree_m;
        return *this;
    }

    /// Divides two monoms.
    /** Divides this monom by the argument x.
        This method can leave the monom with zero coefficient but
        with non-zero degree.
        Returns reference to this monom. */
    template <typename F1, typename I1>
    monom& operator/= (const monom<F1, I1>& x)
    {
        ARAGELI_ASSERT_0(!Arageli::is_null(x.coef_m));

        if(degree_m >= x.degree_m)
        {
            // the following division can produce coef_m == 0 but
            // the degree can have non-zero value
            coef_m /= x.coef_m;
            degree_m -= x.degree_m;
        }
        else
        {
            coef_m = factory<coef_type>::null();
            degree_m = null<degree_type>();
        }

        return *this;
    }

    /// Computes remainder from division this monom by another monom.
    /** Divides this monom by the argument x and assign the reminder to this
        monom object. Returns reference to this monom.
        This method can leave the monom with zero coefficient but
        with non-zero degree.
        Requirement: x is not zero. */
    template <typename F1, typename I1>
    monom& operator%= (const monom<F1, I1>& x)
    {
        ARAGELI_ASSERT_0(!Arageli::is_null(x.coef_m));
        if(degree_m >= x.degree_m)
            coef_m %= x.coef_m;
        return *this;
    }

    /// Swaps two monom values.
    template <typename F1, typename I1>
    void swap (monom<F1, I1>& x)
    {
        std::swap(coef_m, x.coef_m);
        std::swap(degree_m, x.degree_m);
    }

private:

    coef_type coef_m;
    degree_type degree_m;

};


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/// Specialization of io_binary for monom.
template <typename F, typename I>
class io_binary<monom<F, I> > :
    public io_binary_base<monom<F, I> >
{
public:

    using io_binary_base<monom<F, I> >::output_stream;
    using io_binary_base<monom<F, I> >::input_stream;
    using io_binary_base<monom<F, I> >::calc;
    using io_binary_base<monom<F, I> >::input_mem;
    using io_binary_base<monom<F, I> >::output_mem;

    /// Stores monom object state to a binary stream. Seft-delimeted binary serialization.
    /** This functions uses the following format:
            COEFFICIENT DEGREE
        where COEFFICIENT is a coefficient of a given monom and DEGREE is
        the degree of the monom.
        All output is in The Simple Binary format. */
    template <typename Stream>
    static inline Stream& output_stream (Stream& out, const monom<F, I>& x)
    {
        output_binary_stream(out, x.coef());
        output_binary_stream(out, x.degree());
        return out;
    }


    /// Loads monom object state from a binary stream. Compatible with output_stream.
    /** See output_stream(stream, monom) function for detailes on the format.
        If the function fails to read some of state components, an old value of x
        may be lost. All depends on input_binary_stream function for F and I.
        So, do not relay on the value of x when a given stream is not in a good state
        after call returns.

        The function takes input in The Simple Binary format.
    */
    template <typename Stream>
    static inline Stream& input_stream (Stream& in, monom<F, I>& x)
    {
        input_binary_stream(in, x.coef());
        input_binary_stream(in, x.degree());
        return in;
    }


    /// Calculates the number of chars required to store a given monom object in The Simple Binary form.
    /** This function calculates precise number of chars that will emit
        any function outputs in The Simple Binary format for one monom object,
        for example, output_binary_mem function. */
    static inline std::size_t calc (const monom<F, I>& x)
    {
        return calc_binary(x.coef()) + calc_binary(x.degree());
    }
};


template <typename F, typename I>
struct type_traits<monom<F, I> > :
    public type_traits_default<monom<F, I> >
{
    static const bool is_specialized =
        type_traits<F>::is_specialized &&
        type_traits<I>::is_specialized;

    static const bool is_integer_number = false;
    static const bool is_polynom = false;
    static const bool is_real_number = false;
    static const bool is_rational_number = false;
    static const bool is_complex_number = false;
    static const bool is_ring = false;
    static const bool is_field = false;

    static const bool is_finite =
        type_traits<F>::is_finite &&
        type_traits<I>::is_finite;

    static const bool is_additive_group = type_traits<F>::is_ring;
    static const bool is_multiplicative_group = false;

    static const bool has_zero_divisor =
        type_traits<F>::has_zero_divisor &&
        type_traits<I>::has_null;

    static const bool is_integer_modulo_ring = false;
    static const bool is_matrix = false;
    static const bool is_vector = false;

    static const bool has_commutative_multiplication =
        type_traits<F>::has_commutative_multiplication &&
        type_traits<I>::has_commutative_addition;

    static const bool has_commutative_addition =
        type_traits<F>::has_commutative_addition;

    static const bool has_null =
        type_traits<F>::has_null &&
        type_traits<I>::has_null;

    static const bool has_unit =
        type_traits<F>::has_unit &&
        type_traits<I>::has_null;

    static const bool has_opposite_unit =
        type_traits<F>::has_opposite_unit &&
        type_traits<I>::has_null;

    /// True iff type is composite type consists another elements.
    static const bool is_aggregate = true;

    /// Type of each element.
    typedef F element_type;

};




//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename F, typename I>
struct factory<monom<F, I> >
{
    static const bool is_specialized = true;

    static const monom<F, I>& unit ()
    {
        static const monom<F, I> unit_s(Arageli::unit<F>());
        return unit_s;
    }

    static monom<F, I> unit (const monom<F, I>& x)
    {
        return monom<F, I>(Arageli::unit(x.coef()));
    }

    static const monom<F, I>& opposite_unit ()
    {
        static const monom<F, I> opposite_unit_s(Arageli::opposite_unit<F>());
        return opposite_unit_s;
    }

    static monom<F, I> opposite_unit (const monom<F, I>& x)
    {
        return monom<F, I>(opposite_unit(x.coef()));
    }

    static const monom<F, I>& null ()
    {
        static const monom<F, I> null_s(Arageli::null<F>());
        return null_s;
    }

    static monom<F, I> null (const monom<F, I>& x)
    {
        return monom<F, I>(Arageli::null(x.coef()));
    }

};


template <typename F, typename I>
inline bool is_unit (const monom<F, I>& x)
{
    return x.is_unit();
}

template <typename F, typename I>
inline bool is_opposite_unit (const monom<F, I>& x)
{
    return x.is_opposite_unit();
}

template <typename F, typename I>
inline bool is_null (const monom<F, I>& x)
{
    return x.is_null();
}

template <typename F, typename I>
inline monom<F, I> opposite (const monom<F, I>& x)
{
    return -x;
}

template <typename F, typename I>
inline monom<F, I>& opposite (const monom<F, I>& x, monom<F, I>* y)
{
    return (*y = x).opposite();
}

template <typename F, typename I>
inline monom<F, I>& opposite (monom<F, I>* x)
{
    return x->opposite();
}

template <typename F, typename I>
inline monom<F, I> inverse (const monom<F, I>& x)
{
    ARAGELI_ASSERT_0(x.is_const());
    return inverse(x.coef());
}

template <typename F, typename I>
inline monom<F, I>& inverse (const monom<F, I>& x, monom<F, I>* y)
{
    ARAGELI_ASSERT_0(x.is_const());
    inverse(x.coef(), &y->coef());
    y->degree() = x.degree();
    return *y;
}

template <typename F, typename I>
inline monom<F, I>& inverse (monom<F, I>* x)
{
    ARAGELI_ASSERT_0(x.is_const());
    inverse(&x->coef());
    return *x;
}

template <typename F, typename I>
inline std::ostream& output_polynom_first (std::ostream& out, const monom<F, I>& x)
{
    return out << '(' << x << ')';
}

template <typename F, typename I>
inline std::ostream& output_polynom_internal
(std::ostream& out, const monom<F, I>& x)
{
    return out << "+(" << x << ')';
}

template <typename F, typename I>
inline std::ostream& output_pow (std::ostream& out, const monom<F, I>& x)
{
    return out << '(' << x << ')';
}

template <typename F, typename I>
std::istream& input_polynom_first (std::istream& in, monom<F, I>& x);

template <typename F, typename I>
std::istream& input_polynom_internal
(std::istream& in, monom<F, I>& x);

template <typename F, typename I>
inline std::istream& input_pow (std::istream& in, monom<F, I>& x)
{
    return input_polynom_first(in, x);
}



/// Lexicographical comparision of two monoms.
/** Treats each monom as pair (sign of coefficient, degree, coefficient)
    The function delegates real comparision of the degree and
    the coefficient values to appropriate cmp function.
    If particular cmp function for degree and coefficient work properly then
    the function returns
    -   0 if a == b,
    -  -1 if a < b,
    -  +1 if a > b. */
template <typename F1, typename I1, typename F2, typename I2>
inline int cmp
(
    const monom<F1, I1>& m1,
    const monom<F2, I2>& m2
)
{
    if(int dres = cmp(m1.degree(), m2.degree()))
        return dres;
    else
        return cmp(m1.coef(), m2.coef());
}


template <typename F1, typename I1, typename F2>
inline int cmp (const monom<F1, I1>& m1, const F2& b)
{
    return cmp(m1, monom<F2, I1>(b));
}


template <typename F1, typename F2, typename I2>
inline int cmp (const F1& a, const monom<F2, I2>& m2)
{
    return cmp(monom<F1, I2>(a), m2);
}


/// Determines the sign of monom, i.e. sign of the its coefficient.
/** Factory_coef must have a method to produce null element of F type. */
template <typename F, typename I, typename Factory_coef>
inline int sign (const monom<F, I>& x)
{
    return sign(x.coef());
}


#define _ARAGELI_MONOM_CMP(OPER)    \
    template <typename F1, typename I1, typename F2, typename I2>    \
    inline bool operator OPER    \
    (const monom<F1, I1>& a, const monom<F2, I2>& b)    \
    {    \
        return cmp(a, b) OPER 0;    \
    }    \
    \
    template <typename F1, typename I1, typename F2>    \
    inline bool operator OPER    \
    (    \
        const monom<F1, I1>& a,    \
        const F2& b    \
    )    \
    {    \
        return cmp(a, b) OPER 0;    \
    }    \
    \
    template <typename F1, typename F2, typename I2>    \
    inline bool operator OPER    \
    (    \
        const F1& a,    \
        const monom<F2, I2>& b    \
    )    \
    {    \
        return cmp(a, b) OPER 0;    \
    }


/// @name Standard comparision operators that based on cmp function.
// @{

_ARAGELI_MONOM_CMP(==)
_ARAGELI_MONOM_CMP(!=)
_ARAGELI_MONOM_CMP(<=)
_ARAGELI_MONOM_CMP(>=)
_ARAGELI_MONOM_CMP(<)
_ARAGELI_MONOM_CMP(>)

// @}


#undef _ARAGELI_MONOM_CMP


/// Binary predicate for two monoms for degrees comparision.
/** It is true if degree of the first monom is less then degree of the second monom. */
template <typename M1, typename M2 = M1>
struct monom_degree_less : std::binary_function<M1, M2, bool>
{
    bool operator() (const M1& x, const M2& y) const
    {
        return x.degree() < y.degree();
    }
};


/// Binary predicate for two monoms for equality degrees determination
/**    It is true if degree of the first monom is equal to degree of the second monom. */
template <typename M1, typename M2 = M1>
struct monom_degree_equal : std::binary_function<M1, M2, bool>
{
    bool operator() (const M1& x, const M2& y) const
    {
        return x.degree() == y.degree();
    }
};


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

/// Simple outputting of monom.
/** Выводит моном в простом виде "(коэф, степ)", где
    символы разделители могут задаваться пользователем.    */
template
<
    typename Ch,
    typename ChT,
    typename F,
    typename I
>
inline std::basic_ostream<Ch, ChT>& output_list
(
    std::basic_ostream<Ch, ChT>& out,
    const monom<F, I>& x,
    const char* first_bracket = monom_output_list_first_bracket_default,
    const char* second_bracket = monom_output_list_second_bracket_default,
    const char* separator = monom_output_list_separator_default
)
{
    return
        out
            << first_bracket << x.coef()
            << separator
            << x.degree() << second_bracket;
}


/// Выводит моном в привычном виде, с указанием формальной переменной.
template
<
    typename Ch,
    typename ChT,
    typename F,
    typename I
>
std::basic_ostream<Ch, ChT>& output_var
(
    std::basic_ostream<Ch, ChT>& out,
    const monom<F, I>& x,
    bool first_a = true,
    const char* var = monom_output_var_var_default,
    const char* mul = monom_output_var_mul_default,
    const char* pow = monom_output_var_pow_default
);

/// Simple intputting for monom.
template
<
    typename Ch,
    typename ChT,
    typename F,
    typename I
>
std::basic_istream<Ch, ChT>& input_list
(
    std::basic_istream<Ch, ChT>& in,
    monom<F, I>& x,
    const char* first_bracket = monom_input_list_first_bracket_default,
    const char* second_bracket = monom_input_list_second_bracket_default,
    const char* separator = monom_input_list_separator_default
);

/// Inputting with variable symbol for monom.
template
<
    typename Ch,
    typename ChT,
    typename F,
    typename I,
    typename Factory_coef
>
std::basic_istream<Ch, ChT>& input_var
(
    std::basic_istream<Ch, ChT>& in,
    monom<F, I>& x,
    bool first_a,
    const Factory_coef& fctr,
    const char* var = monom_input_var_var_default,
    const char* mul = monom_input_var_mul_default,
    const char* pow = monom_input_var_pow_default
);


/// Inputting with variable symbol for monom.
template
<
    typename Ch,
    typename ChT,
    typename F,
    typename I
>
std::basic_istream<Ch, ChT>& input_var
(
    std::basic_istream<Ch, ChT>& in,
    monom<F, I>& x,
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


/// Oputputs monom in aligned form.  (Restricted implementation.)
/** The aligned form for monom includes two line of output:
    on the bottom line coefficient and variable symbol are placed and
    on the top line degree is placed.
    WARNING.  Temporary this form is equal to output_var.    */
template
<
    typename Ch,
    typename ChT,
    typename F,
    typename I
>
inline std::basic_ostream<Ch, ChT>& output_aligned
(
    std::basic_ostream<Ch, ChT>& out,
    const monom<F, I>& x,
    bool first_a = true,
    const char* var = monom_output_var_var_default,    // WARNING! Don't forgot to change to aligned
    const char* mul = monom_output_var_mul_default, // WARNING! Don't forgot to change to aligned
    const char* pow = monom_output_var_pow_default    // WARNING! Don't forgot to change to aligned
)
{
    return output_var(out, x, first_a, var, mul, pow);
}


/// Стандартный способ вывода монома: совпадает с output_var.
template
<
    typename F,
    typename I,
    typename Ch,
    typename ChT
>
inline std::basic_ostream<Ch, ChT>& operator<<
(std::basic_ostream<Ch, ChT>& out, const monom<F, I>& x)
{
    return output_var(out, x);
}


/// Стандартный способ ввода монома: совпадает с input_var.
template
<
    typename F,
    typename I,
    typename Ch,
    typename ChT
>
inline std::basic_istream<Ch, ChT>& operator>>
(std::basic_istream<Ch, ChT>& in, monom<F, I>& x)
{
    return input_var(in, x);
}


// Дополнительные операторы для мономов; сочетание со скаляром
#define _ARAGELI_MONOM_BINOPER(AOPER, BOPER)    \
    template    \
    <    \
        typename F1,    \
        typename I1,    \
        typename F2,    \
        typename I2    \
    >    \
    inline monom<F1, I1> operator BOPER    \
    (const monom<F1, I1>& x, const monom<F2, I2>& y)    \
    {    \
        monom<F1, I1> t(x);    \
        return t AOPER y;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename I1,    \
        typename F2    \
    >    \
    inline monom<F1, I1> operator BOPER    \
    (const monom<F1, I1>& x, const F2& y)    \
    {    \
        monom<F1, I1> t(x);    \
        return t AOPER y;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename F2,    \
        typename I2    \
    >    \
    inline monom<F2, I2> operator BOPER    \
    (const F1& x, const monom<F2, I2>& y)    \
    {    \
        monom<F2, I2> t(x);    \
        return t AOPER y;    \
    }


/// @name Дополнительные операторы для мономов; сочетание со скаляром.
// @{

_ARAGELI_MONOM_BINOPER(+=, +)
_ARAGELI_MONOM_BINOPER(-=, -)
_ARAGELI_MONOM_BINOPER(*=, *)
_ARAGELI_MONOM_BINOPER(/=, /)
_ARAGELI_MONOM_BINOPER(%=, %)

// @}

#undef _ARAGELI_POLYNOM_MONOM_BINOPER


/// Унарный функтор: по моному возвращает ссылку на его коэффициент.
template <typename M>
struct coef_extractor :
    std::unary_function<M, typename M::coef_type>
{
    typename M::coef_type& operator() (M& x) const
    {
        return x.coef();
    }

    const typename M::coef_type& operator() (const M& x) const
    {
        return x.coef();
    }
};


/// Унарный функтор: по моному возвращает ссылку на его степень.
template <typename M>
struct degree_extractor :
    std::unary_function<M, typename M::degree_type>
{
    typename M::degree_type& operator() (M& x) const
    {
        return x.degree();
    }

    const typename M::degree_type& operator() (const M& x) const
    {
        return x.degree();
    }
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

///    @name Sorted and nonsorted helper objects.
/*    'norm_monom_seq_t' и 'any_monom_seq_t' служат для указания
    сортирована или не не сортирована некоторая последовательность мономов.
    Используются при задании последовательностей мономов для полинома. */

// @{


/// Type for helper object norm_monom_seq.
/** See norm_monom_seq. */
struct norm_monom_seq_t {};

/// Helper object for choosing method that gives sorted monom sequence.
/** See constructors and relative methods of sparse_polynom
    template class. */
static const norm_monom_seq_t norm_monom_seq = norm_monom_seq_t();

/// Type for helper object any_monom_seq.
/** See any_monom_seq. */
struct any_monom_seq_t {};

/// Helper object for choosing method that gives unsorted monom sequence.
/** See constructors and relative methods of sparse_polynom
    template class. */
static const any_monom_seq_t any_monom_seq = any_monom_seq_t();


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/// Template class for representing of a sparse polynomials.
/**    This class contains a sequence of monoms sorted by growing of degree.
    Sequence does not contain monoms with zero degree.
    @param F a type of coefficiens
    @param I a type of degree; by default, it is signed
    @param REFCNT reference counter switch; by default, it is true */
template
<
    typename F,
    typename I,
    bool REFCNT
>
class sparse_polynom;    // Определение см. ниже.


/// Base for all sparse polynomials.
/** Для исключений и для того, что бы сделать сравнимыми
    указатели на два полинома с различными параметрами шаблона. */
struct sparse_polynom_base
{
    /// Exception of sparse_polynom.
    /**    Base for any other exception of sparse_polynom. */
    class exception :
        public virtual Arageli::exception
    {};

    /// Возникает при делении на нулевой полином.
    class division_by_zero :
        public exception
    {};

    /// Возникает при возвединии нулевого полинома в нулевую степень.
    class zero_in_zero_degree :
        public exception
    {};
};


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename F, typename I, bool REFCNT>
class sparse_polynom : public sparse_polynom_base
{
    template <typename F1, typename I1, bool REFCNT1>
    friend class sparse_polynom;

public:

    /// Coefficient type.
    typedef F coef_type;

    /// Degree type.
    typedef I degree_type;

    /// Monom type that is used to building the polynomial.
    typedef Arageli::monom<F, I> monom; //GCC 4.3 fix ( Arageli:: )

    /// Reference counter property. True iff the counter is turned on.
    static const bool refcounting = REFCNT;

    /// Defines a polynomial with different coefficient type.
    template <typename F1> struct other_coef
    {
        typedef sparse_polynom<F1, I, REFCNT> type;
    };

    /// Defines a polynomial with different coeffs type and degree type.
    template <typename F1, typename I1> struct other_coef_degree
    {
        typedef sparse_polynom<F1, I1, REFCNT> type;
    };

private:

    // 'Rep' -- представление полинома: последовательность мономов.
    // ПРЕДУПРЕЖДЕНИЕ. std::list временно, нужен свой односвязный список
    typedef std::list<monom> Rep;

public:

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Constructors.

    //@{

    /// Zero polynomial. It's empty set of monoms.
    sparse_polynom ()
    {}

    /// Полином состоящий из одного монома 'monom(x)',
    /** если 'x' -- нулевой элемент, то результат эквивалентен sparse_polynom() */
    sparse_polynom (const F& x)
    {
        if(!Arageli::is_null(x))
            rep().push_back(x);
    }

    /// Полином состоящий из одного монома monom(x, p)
    /**    Если 'x' равен нулю, то результат эквивалентен sparse_polynom() */
    sparse_polynom (const F& x, const I& p)
    {
        if(!Arageli::is_null(x))
            rep().push_back(monom(x, p));
    }


    /// Полином состоящий из одного монома x.
    /**    Если 'x' -- нулевой моном,
        то результат эквивалентен sparse_polynom(). */
    template <typename F1, typename I1>
    sparse_polynom (const Arageli::monom<F1, I1>& x)
    {
        if(!x.is_null())
            rep().push_back(x);
    }


    /// Creates a copy of the some object of type sparse_polynom.
    template <typename F1, typename I1, bool REFCNT1>
    sparse_polynom (const sparse_polynom<F1, I1, REFCNT1>& x)
    {
        rep().assign(x.rep().begin(), x.rep().end());
        normalize();    // WARNING. We only need zero monom elimination.
    }


    /// Инициализирует полином произвольной последовательностью мономов.
    template <typename In>
    sparse_polynom (In first, In last, const any_monom_seq_t&)
    {
        rep().assign(first, last);
        normalize();
    }


    /// Initialize polynomial from vector of coefficients.
    template <typename Vec>
    sparse_polynom (const Vec& vec, const fromvec_t&)
    {
        assign_fromvec(vec);
    }


    /// Инициализирует полином "нормализованной" последовательностью мономов.
    /** Последовательность отсортированна по возростанию степеней,
        не содержbn мономов с одинаковыми степенями и мономов с нулевыми
        коэффициентами.    */
    template <typename In>
    sparse_polynom (In first, In last, const norm_monom_seq_t&)
    {
        rep().assign(first, last);
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);
    }


    /// TEMPORARY not mixed conversion from dense polynomial.
    template <typename F1, typename Config1>
    sparse_polynom (const polynom<F1, Config1>& x)
    {
        typedef typename polynom<F1, Config1>::size_type size_type;
        for(size_type i = 0; i < x.size(); ++i)
            if(!Arageli::is_null(x[i]))
                rep().push_back(monom(x[i], i));
    }


    /// Initializes sparse_polynom by its string notation.
    /**    Input rules are the same as for operator>> with default behaviour. */
    sparse_polynom (const char* str);

    //@}

    /// Creates a copy of polynomial x.
    template <typename F1, typename I1, bool REFCNT1>
    sparse_polynom& operator= (const sparse_polynom<F1, I1, REFCNT1>& x)
    {
        ARAGELI_ASSERT_1
        (
            static_cast<sparse_polynom_base*>(this) !=
            static_cast<const sparse_polynom_base*>(&x)
        );

        store.unique_clear();
        rep().assign(x.rep().begin(), x.rep().end());
        normalize();    // WARNING. We only need zero monom elimination.

        return *this;
    }

    template <typename F1, typename I1>
    sparse_polynom& operator= (const Arageli::monom<F1, I1>& x)
    {
        return *this = sparse_polynom(x);
    }

    template <typename F1>
    sparse_polynom& operator= (const F1& x)
    {
        return *this = sparse_polynom(x);
    }

    sparse_polynom& operator= (const char* s)
    {
        return *this = sparse_polynom(s);
    }


    template <typename Vec>
    void assign_fromvec (const Vec& vec)
    {
        rep().clear();
        for(typename Vec::size_type i = 0; i < vec.size(); ++i)
            if(!Arageli::is_null(vec[i]))
                rep().push_back(monom(vec[i], i));
    }



    /// Returns true if this is zeroth polynomial.
    /** I.e. returns true if polynomial does not containes any monoms. */
    bool is_null () const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);
        return rep().empty();
    }


    /// Returns true if polynomial has only absolute term that equal to 1.
    bool is_unit () const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);
        return rep().size() == 1 && rep().front().is_unit();
    }


    /// Returns true if polynomial has only absolute term that equal to -1.
    bool is_opposite_unit () const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);
        return rep().size() == 1 && rep().front().is_opposite_unit();
    }


    /// Returns true if polynomial have no monoms with positive degree of x.
    bool is_const () const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);
        return rep().empty() || rep().size() == 1 && rep().front().is_const();
    }


    /// Returns true if polynomial is "x".
    /** Returns true if polynomial contains only one monom with degree = 1
        and coefficient = 1. */
    bool is_x () const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);
        return rep().size() == 1 && rep().front().is_x();
    }


    /// Returns true if polynomial has normal form.
    /** Normal form of polynomial has ascending sorted (by degrees) monom sequence
        and all monoms have different degrees and nonzero coefficients.  */
    bool is_normal () const;


    /// Returns non-const reference to monom with the greatest degree.
    /** Requirement: !is_null(). */
    monom& leading_monom ()
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);
        ARAGELI_ASSERT_0(!rep().empty());
        store.unique();
        return rep().back();
    }


    /// Returns const reference to monom with the greatest degree.
    /** Requirement: !is_null(). */
    const monom& leading_monom () const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);
        ARAGELI_ASSERT_0(!rep().empty());
        return rep().back();
    }


    /// Returns a copy of monom with the greatest degree.
    /** For zeroth polynomial returns zero monom. */
    monom leading_monom_cpy () const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);

        return
            rep().empty() ?
            monom() :
            leading_monom();
    }


    /// Returns degree of polynomial (greatest degree value).
    /** For zeroth polynomial returns -1 from factory. */
    template <typename Factory_degree>
    degree_type degree (const Factory_degree& fctr) const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);

        return
            rep().empty() ?
            fctr.opposite_unit() :
            leading_monom().degree();
    }

    /// Returns degree of polynomial (greatest degree value).
    /** For zeroth polynomial returns -1 from factory. */
    degree_type degree () const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);

        return
            rep().empty() ?
            factory<degree_type>::opposite_unit() :
            leading_monom().degree();
    }

    /// Returns number of monoms in polynomial.
    /** Returns 0 if and only if polynomial is zeroth. */
    typename Rep::size_type size () const
    {
        return rep().size();
    }

    /// Returns non-const reference to coefficient of monom with the greatest degree.
    /** Requirement: !is_null(). */
    coef_type& leading_coef ()
    {
        return leading_monom().coef();
    }

    /// Returns const reference to coefficient of monom with the greatest degree.
    /** Requirement: !is_null(). */
    const coef_type& leading_coef () const
    {
        return leading_monom().coef();
    }

    /// Returns a copy of coefficient of monom with the greatest degree.
    /** For zeroth polynomial returns zero from dafault factory. */
    coef_type leading_coef_cpy () const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);

        return
            rep().empty() ?
            factory<coef_type>::null() :
            leading_monom().coef();
    }

    /// Returns a copy of coefficient of monom with the greatest degree.
    /** For zeroth polynomial returns zero from factory. */
    template <typename Factory_coef>
    coef_type leading_coef_cpy (const Factory_coef& fctr) const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);

        return
            rep().empty() ?
            fctr.null() :
            leading_monom().coef();
    }

    /// Makes an individual representation for this object.
    /**    If ‘refcounting’ is false then this is empty operation.
        Otherwise it makes so that this sparse_polynom descriptor
        gets individual copy of internal representation.
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

    /// Opposites all coefficients of polynomial.
    /** For each coefficient x calls FT::opposite(&x).
        Returns reference to this polynomial. */
    sparse_polynom& opposite ();

    /// Returns opposite polynomial.
    sparse_polynom operator- () const
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);

        sparse_polynom t(*this);
        return t.opposite();
    }

    /// Returns reference to this sparse_polynom. For sameness only.
    const sparse_polynom& operator+ () const
    {
        return *this;
    }

    sparse_polynom& operator++ ()
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);

        unique();

        if(!rep().empty() && rep().front().is_const())
        {
            if(Arageli::is_null(++rep().front().coef()))
                rep().pop_front();
        }
        else
        {
            rep().push_front(unit<F>());
        }

        ARAGELI_ASSERT_EX1_1(is_normal(), *this);
        return *this;
    }

    sparse_polynom operator++ (int)
    {
        sparse_polynom t = *this;
        operator++();
        return t;
    }

    sparse_polynom& operator-- ()
    {
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);

        unique();

        if(!rep().empty() && rep().front().is_const())
        {
            if(Arageli::is_null(--rep().front().coef()))
                rep().pop_front();
        }
        else
        {
            rep().push_front(opposite_unit<F>());
        }

        ARAGELI_ASSERT_EX1_1(is_normal(), *this);
        return *this;
    }

    sparse_polynom operator-- (int)
    {
        sparse_polynom t = *this;
        operator--();
        return t;
    }


    template <typename T1> sparse_polynom& add_scalar (const T1& x);
    template <typename T1> sparse_polynom& sub_scalar (const T1& x);
    template <typename T1> sparse_polynom& mul_scalar (const T1& x);
    template <typename T1> sparse_polynom& div_scalar (const T1& x);
    template <typename T1> sparse_polynom& mod_scalar (const T1& x);
    template <typename T1> sparse_polynom& bitand_scalar (const T1& x);
    template <typename T1> sparse_polynom& bitor_scalar (const T1& x);
    template <typename T1> sparse_polynom& bitxor_scalar (const T1& x);
    template <typename T1> sparse_polynom& shl_scalar (const T1& x);
    template <typename T1> sparse_polynom& shr_scalar (const T1& x);

    template <typename T1> sparse_polynom& add_sparse_polynom (const T1& x);
    template <typename T1> sparse_polynom& sub_sparse_polynom (const T1& x);
    template <typename T1> sparse_polynom& mul_sparse_polynom (const T1& x);
    template <typename T1> sparse_polynom& div_sparse_polynom (const T1& x);
    template <typename T1> sparse_polynom& mod_sparse_polynom (const T1& x);

    /*
    template <typename T1> sparse_polynom& bitand_sparse_polynom (const T1& x);
    template <typename T1> sparse_polynom& bitor_sparse_polynom (const T1& x);
    template <typename T1> sparse_polynom& bitxor_sparse_polynom (const T1& x);
    template <typename T1> sparse_polynom& shl_sparse_polynom (const T1& x);
    template <typename T1> sparse_polynom& shr_sparse_polynom (const T1& x);
    */

    /// @name Standard mathematical operations with polynomial and scalar.
    /** Each function returns reference to this polynomial. */
    // @{

    template <typename F1> sparse_polynom&
    operator+= (const F1& x)
    {
        return add_scalar(x);
    }

    template <typename F1> sparse_polynom&
    operator-= (const F1& x)
    {
        return sub_scalar(x);
    }

    template <typename F1> sparse_polynom&
    operator*= (const F1& x)
    {
        return mul_scalar(x);
    }

    template <typename F1> sparse_polynom&
    operator/= (const F1& x)
    {
        return div_scalar(x);
    }

    template <typename F1> sparse_polynom&
    operator%= (const F1& x)
    {
        return mod_scalar(x);
    }

    template <typename F1> sparse_polynom&
    operator&= (const F1& x)
    {
        return bitand_scalar(x);
    }

    template <typename F1> sparse_polynom&
    operator|= (const F1& x)
    {
        return bitor_scalar(x);
    }

    template <typename F1> sparse_polynom&
    operator^= (const F1& x)
    {
        return bitxor_scalar(x);
    }

    template <typename F1> sparse_polynom&
    operator<<= (const F1& x)
    {
        return shl_scalar(x);
    }

    template <typename F1> sparse_polynom&
    operator>>= (const F1& x)
    {
        return shr_scalar(x);
    }

    // BEGIN OF TEMPORARY EXTENTION

    sparse_polynom& operator+= (const F& x)
    {
        return add_scalar(x);
    }

    sparse_polynom& operator-= (const F& x)
    {
        return sub_scalar(x);
    }

    sparse_polynom& operator*= (const F& x)
    {
        return mul_scalar(x);
    }

    sparse_polynom& operator/= (const F& x)
    {
        return div_scalar(x);
    }

    sparse_polynom& operator%= (const F& x)
    {
        return mod_scalar(x);
    }

    sparse_polynom& operator&= (const F& x)
    {
        return bitand_scalar(x);
    }

    sparse_polynom& operator|= (const F& x)
    {
        return bitor_scalar(x);
    }

    sparse_polynom& operator^= (const F& x)
    {
        return bitxor_scalar(x);
    }

    sparse_polynom& operator<<= (const F& x)
    {
        return shl_scalar(x);
    }

    sparse_polynom& operator>>= (const F& x)
    {
        return shr_scalar(x);
    }

    // END OF TEMPORARY EXTENTION

    // @}


    /// @name Standard mathematical operations with polynomial and monom.
    /** Each function returns reference to this polynomial. */
    // @{

    template <typename F1, typename I1>
    sparse_polynom& operator+= (const Arageli::monom<F1, I1>& x);

    template <typename F1, typename I1>
    sparse_polynom& operator-= (const Arageli::monom<F1, I1>& x);

    template <typename F1, typename I1>
    sparse_polynom& operator*= (const Arageli::monom<F1, I1>& x);

    template <typename F1, typename I1>
    sparse_polynom& operator/= (const Arageli::monom<F1, I1>& x)
    { return operator/=(sparse_polynom(x)); }

    template <typename F1, typename I1>
    sparse_polynom& operator%= (const Arageli::monom<F1, I1>& x)
    {
        return operator%=(sparse_polynom(x));    // WARNING!
    }

    // @}


    /// @name Standard mathematical operations with two polynomials.
    /** Each function returns reference to this polynomial. */
    // @{

    template <typename F1, typename I1, bool REFCNT1>
    sparse_polynom& operator+= (const sparse_polynom<F1, I1, REFCNT1>& x)
    {
        return add_sparse_polynom(x);
    }

    template <typename F1, typename I1, bool REFCNT1>
    sparse_polynom& operator-= (const sparse_polynom<F1, I1, REFCNT1>& x)
    {
        return sub_sparse_polynom(x);
    }

    template <typename F1, typename I1, bool REFCNT1>
    sparse_polynom& operator*= (const sparse_polynom<F1, I1, REFCNT1>& x)
    {
        return mul_sparse_polynom(x);
    }

    template <typename F1, typename I1, bool REFCNT1>
    sparse_polynom& operator/= (const sparse_polynom<F1, I1, REFCNT1>& x)
    {
        return div_sparse_polynom(x);
    }

    template <typename F1, typename I1, bool REFCNT1>
    sparse_polynom& operator%= (const sparse_polynom<F1, I1, REFCNT1>& x)
    {
        return mod_sparse_polynom(x);
    }

    // @}

    ///// Raise this polynomial to a power n.
    ///** Returns reference to this polynomial. */
    //template <typename I1, typename IT1>
    //inline sparse_polynom& pow (const I1& n, const IT1&);

    /// Raise this polynomial to a power n.
    /** Returns reference to this polynomial. */
    template <typename I1>
    sparse_polynom& pow (const I1& n)
    {
        return *this = power(*this, n);
    }


    /// @name Iterators.
    // @{

    /// This iterator represents polynomial as a sequence of monoms. Non-constant form.
    typedef typename Rep::iterator monom_iterator;

    /// This iterator represents polynomial as a sequence of monoms. Constant form.
    typedef typename Rep::const_iterator monom_const_iterator;

    /// This iterator represents a coefficients sequence of polynomial. Non-constant form.
    typedef apply_iterator<coef_type, monom_iterator, coef_extractor<monom> >
        coef_iterator;

    /// This iterator represents a coefficients sequence of polynomial. Constant form.
    typedef apply_iterator<const coef_type, monom_const_iterator, coef_extractor<monom> >
        coef_const_iterator;

    /// This iterator represents a degrees sequence of polynomial. Non-constant form.
    typedef apply_iterator<degree_type, monom_iterator, degree_extractor<monom> >
        degree_iterator;

    /// This iterator represents a degrees sequence of polynomial. Constant form.
    typedef apply_iterator<const degree_type, monom_const_iterator, degree_extractor<monom> >
        degree_const_iterator;

    // @}


    /// @name Access via iterators.
    /** В нормализованном полиноме мономы
        расположены в порядке возростания их степеней. Последовательности
        коэффициентов и степеней соответсвуют последовательности мономов.
        ПРЕДУПРЕЖДЕНИЕ! При вызове неконстантной функции всегда вызывается
        unique, но при использовании любого итератора эта функция никогда
        не вызывается. Поэтому при включенном счётчике ссылок не рекомендуется
        делать копий полинома если существует хотя бы один неконстантный
        итератор, т.к. изменение полинома через этот итератор повлечёт за собой
        изменение всех или некоторых его копий.    */

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

    /// Returns iterator on the begin of a coefficient sequence.
    coef_iterator coefs_begin ()
    {
        return coef_iterator(monoms_begin());
    }

    /// Returns iterator on the begin of a coefficient sequence.
    coef_const_iterator coefs_begin () const
    {
        return coef_const_iterator(monoms_begin());
    }

    /// Returns iterator on the end of a coefficient sequence.
    coef_iterator coefs_end ()
    {
        return coef_iterator(monoms_end());
    }

    /// Returns iterator on the end of a coefficient sequence.
    coef_const_iterator coefs_end () const
    {
        return coef_const_iterator(monoms_end());
    }

    /// Returns iterator on the begin of a degree sequence.
    degree_iterator degrees_begin ()
    {
        return degree_iterator(monoms_begin());
    }

    /// Returns iterator on the begin of a degree sequence.
    degree_const_iterator degrees_begin () const
    {
        return degree_const_iterator(monoms_begin());
    }

    /// Returns iterator on the end of a degree sequence.
    degree_iterator degrees_end ()
    {
        return degree_iterator(monoms_end());
    }

    /// Returns iterator on the end of a degree sequence.
    degree_const_iterator degrees_end () const
    {
        return degree_const_iterator(monoms_end());
    }

    // @}


    /// @name Inserting and erasing monoms.
    // @{

    /// Erases the monom by iterator pos.
    monom_iterator erase (monom_iterator pos)
    {
        unique();
        return rep().erase(pos);
    }

    /// Erases all monoms in iterator range [first, last).
    monom_iterator erase (monom_iterator first, monom_iterator last)
    {
        unique();
        return rep().erase(first, last);
    }


    /// Inserts the monom x before monom by pos in monom sequence.
    /** Note that is_normal property is maybe lost after the operation. */
    template <typename F1, typename I1>
    monom_iterator insert (monom_iterator pos, const Arageli::monom<F1, I1>& x)
    {
        unique();
        return monom_iterator(rep().insert(pos, x));
    }


    /// Inserts monoms in range (first, last] before monom by pos in monom sequence.
    /** Note that is_normal property is maybe lost after the operation. */
    template <typename Iter>
    void insert (monom_iterator pos, Iter first, Iter last)
    {
        unique();
        rep().insert(pos, first, last);
    }

    // @}


    /// @name Addition and subtraction of the some free monom sequence.
    /** The third argument set restrictions on monoms sequence
        in range [first, last). */
    // @{

    template <typename Iter>
    void add (Iter first, Iter last, const any_monom_seq_t&);

    template <typename Iter>
    void add (Iter first, Iter last, const norm_monom_seq_t&);

    template <typename Iter>
    void sub (Iter first, Iter last, const any_monom_seq_t&);

    template <typename Iter>
    void sub (Iter first, Iter last, const norm_monom_seq_t&);

    // @}


    /// @name Monoms moving without actually copying (if possible).
    /** Функции 'addsub' производят перенос мономов из одного полинома в другой
        (из 'x' в 'this'). Т.е. из 'x' они вычитаются, а к 'this' прибавляются.
        Если это возможно (зависит от соотношения пар типов (F, I) этих
        полиномов и значений), то мономы перемещаются без копирования.
        Входные последовательности могут быть как сортированными так
        и несортированными.    */

    // @{

    template <typename F1, typename I1, bool REFCNT1>
    void addsub
    (
        sparse_polynom<F1, I1, REFCNT1>& x,
        typename sparse_polynom<F1, I1, REFCNT1>::monom_iterator pos
    );

    template <bool REFCNT1>
    void addsub
    (
        sparse_polynom<F, I, REFCNT1>& x,
        typename sparse_polynom<F, I, REFCNT1>::monom_iterator pos
    );

    template <typename F1, typename I1, bool REFCNT1>
    void addsub
    (
        sparse_polynom<F1, I1, REFCNT1>& x,
        typename sparse_polynom<F1, I1, REFCNT1>::monom_iterator first,
        typename sparse_polynom<F1, I1, REFCNT1>::monom_iterator last,
        const any_monom_seq_t&
    );

    template <bool REFCNT1>
    void addsub
    (
        sparse_polynom<F, I, REFCNT1>& x,
        typename sparse_polynom<F, I, REFCNT1>::monom_iterator first,
        typename sparse_polynom<F, I, REFCNT1>::monom_iterator last,
        const any_monom_seq_t&
    );

    template <typename F1, typename I1, bool REFCNT1>
    void addsub
    (
        sparse_polynom<F1, I1, REFCNT1>& x,
        typename sparse_polynom<F1, I1, REFCNT1>::monom_iterator first,
        typename sparse_polynom<F1, I1, REFCNT1>::monom_iterator last,
        const norm_monom_seq_t&
    );

    template <bool REFCNT1>
    void addsub
    (
        sparse_polynom<F, I, REFCNT1>& x,
        typename sparse_polynom<F, I, REFCNT1>::monom_iterator first,
        typename sparse_polynom<F, I, REFCNT1>::monom_iterator last,
        const norm_monom_seq_t&
    );

    template <typename F1, typename I1, bool REFCNT1>
    void addsub
    (
        sparse_polynom<F1, I1, REFCNT1>& x,
        const any_monom_seq_t&
    );

    template <bool REFCNT1>
    void addsub
    (
        sparse_polynom<F, I, REFCNT1>& x,
        const any_monom_seq_t&
    );

    template <typename F1, typename I1, bool REFCNT1>
    void addsub
    (
        sparse_polynom<F1, I1, REFCNT1>& x,
        const norm_monom_seq_t&
    );

    template <bool REFCNT1>
    void addsub
    (
        sparse_polynom<F, I, REFCNT1>& x,
        const norm_monom_seq_t&
    );

    // @}


    /// Polynomial normalization.
    /** Приводит мономы в полиноме к нормальному виду: приводит подобные члены,
        удаляет нулевые мономы, сортирует их по возростанию степеней. */
    void normalize ();

    /// Swaps two polynomials without actually copying (if possible).
    template <typename F1, typename I1, bool REFCNT1>
    void swap (sparse_polynom<F1, I1, REFCNT1>& x)
    {
        _Internal::swap_help_1
        (
            *this,
            x, store,
            x.store,
            equal_types
            <
                Rep,
                typename sparse_polynom<F1, I1, REFCNT1>::Rep
            >::value
        );
    }


    /// Variable substitution.
    /** Computes the value of polynomial at point x. */
    template <typename F1, typename Coef_factory>
    F1 subs (const F1& x, const Coef_factory&) const;

    /// Variable substitution.
    /** Computes the value of polynomial at point x. */
    template <typename F1>
    inline F1 subs (const F1& x) const;

private:

    // быстрый доступ к представлению
    Rep& rep ()
    {
        return store.value();
    }

    const Rep& rep () const
    {
        return store.value();
    }

    template <typename F1, typename I1>
    typename Rep::iterator find_pos (const Arageli::monom<F1, I1>& x)
    {
        return std::find_if
        (
            rep().begin(), rep().end(),
            std::not1
            (
                std::bind2nd
                (
                    monom_degree_less<monom, Arageli::monom<F1, I1> >(),
                    x
                )
            )
        );
    }

    template <typename F1, typename I1>
    typename Rep::const_iterator find_pos (const Arageli::monom<F1, I1>& x) const
    {
        return std::find_if
        (
            rep().begin(), rep().end(),
            std::not1
            (
                std::bind2nd
                (
                    monom_degree_less<monom, Arageli::monom<F1, I1> >(),
                    x
                )
            )
        );
    }

    /*
    template
    <
        typename I1,
        bool REFCNT1,
        typename FT1,
        typename IT1,
        typename I2,
        bool REFCNT2,
        typename FT2,
        typename IT2,
        typename TT3,
        typename TT4
    >
    static void assign_div
    (
        sparse_polynom<rational<big_int>, I1, REFCNT1, FT1, IT1>& x,
        const sparse_polynom<rational<big_int, TT4>, I2, REFCNT2, FT2, IT2>& y
    );
    */

    template
    <
        typename F1,
        typename I1,
        bool REFCNT1,
        typename F2,
        typename I2,
        bool REFCNT2
    >
    static void assign_div
    (
        sparse_polynom<F1, I1, REFCNT1>& x,
        const sparse_polynom<F2, I2, REFCNT2>& y
    )
    {
        assign_div_common(x, y);
    }


    template <typename F1, typename I1, bool REFCNT1>
    static void assign_div_common
    (
        sparse_polynom& x,
        const sparse_polynom<F1, I1, REFCNT1>& y
    );


    refcntr_proxy<Rep, REFCNT> store;

};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/// Specialization of io_binary for sparse_polynom.
template <typename F, typename I, bool REFCNT>
class io_binary<sparse_polynom<F, I, REFCNT> > :
    public io_binary_base<sparse_polynom<F, I, REFCNT> >
{
public:

    using io_binary_base<sparse_polynom<F, I, REFCNT> >::output_stream;
    using io_binary_base<sparse_polynom<F, I, REFCNT> >::input_stream;
    using io_binary_base<sparse_polynom<F, I, REFCNT> >::calc;
    using io_binary_base<sparse_polynom<F, I, REFCNT> >::input_mem;
    using io_binary_base<sparse_polynom<F, I, REFCNT> >::output_mem;

    /// Stores polynomial object state to a binary stream. Seft-delimeted binary serialization.
    /** This functions uses the following format:
            SIZE MONOMS
        where SIZE is a number of monomial in a given polynomial and
        MONOMS is all monoms of the polynomial (if any).
        All output is in The Simple Binary format. */
    template <typename Stream>
    static Stream& output_stream
    (
        Stream& out,
        const sparse_polynom<F, I, REFCNT>& x
    );

    /// Loads polynomial object state from a binary stream. Compatible with output_stream.
    /** See output_stream(stream, sparse_polynom) function for detailes on the format.
        If the function fails to read some of state components, an old value of x
        may be lost. All depends on input_binary_stream function for F and I.
        So, do not relay on the value of x when a given stream is not in a good state
        after call returns.

        The function takes input in The Simple Binary format.
    */
    template <typename Stream>
    static Stream& input_stream
    (
        Stream& in,
        sparse_polynom<F, I, REFCNT>& x
    );


    /// Calculates the number of chars required to store a given polynomial object in The Simple Binary form.
    /** This function calculates precise number of chars that will emit
        any function outputs in The Simple Binary format for one polynomial object,
        for example, output_binary_mem function. */
    static std::size_t calc (const sparse_polynom<F, I, REFCNT>& x);

};


#if 0

/// Specialization of type_pair_traits for big_int and sparse_polynom.
template <typename TE2, typename TD2, bool REFCNTE2>
struct type_pair_traits<big_int, sparse_polynom<TE2, TD2, REFCNTE2> > :
    public type_pair_traits_for_unknown_and_polynom
        <big_int, sparse_polynom<TE2, TD2, REFCNTE2> >
{};

#endif


/// Pefrorms differentiation of polynomial x by common symbolic rules.
template <typename F, typename I, bool REFCNT>
sparse_polynom<F, I, REFCNT> diff
(const sparse_polynom<F, I, REFCNT>& x);

/// Simple outputting as monom list.
/** Выводит полином в простом виде
    "(моном_1, моном_2, ..., моном_size)", где многоточие не является элементом
    синтаксиса и мономы выводятся с помощью output_list. Символы-
    разделители могут задаваться пользователем.
    Замечание. Если вы хотите изменить параметры по умолчанию для этой функции,
    то, не зубудьте сделать то же самое и для конструктора
    sparse_polynom_outputer_simple.    */
template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT
>
std::basic_ostream<Ch, ChT>& output_list
(
    std::basic_ostream<Ch, ChT>& out,
    const sparse_polynom<F, I, REFCNT>& x,
    monoms_order mo = mo_inc,
    const char* first_bracket = monom_output_list_first_bracket_default,
    const char* second_bracket = monom_output_list_second_bracket_default,
    const char* separator = monom_output_list_separator_default
);


/// Outputting of polynomial with variable symbol.
/**    Выводит полином в привычном виде, с указанием формальной переменной,
    для вывода монома используется output_var.
    Замечание. Если вы хотите изменить параметры по умолчанию для этой функции,
    то, не зубудьте сделать то же самое и для конструктора
    sparse_polynom_outputer_var.    */
template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT
>
std::basic_ostream<Ch, ChT>& output_var
(
    std::basic_ostream<Ch, ChT>& out,
    const sparse_polynom<F, I, REFCNT>& x,
    monoms_order mo = mo_inc,
    const char* var = monom_output_var_var_default,
    const char* mul = monom_output_var_mul_default,
    const char* pow = monom_output_var_pow_default
);


/// Simple inputting of a polynomial.
template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT
>
std::basic_istream<Ch, ChT>& input_list
(
    std::basic_istream<Ch, ChT>& in,
    sparse_polynom<F, I, REFCNT>& x,
    const char* first_bracket = monom_input_list_first_bracket_default,
    const char* second_bracket = monom_input_list_second_bracket_default,
    const char* separator = monom_input_list_separator_default
);


/// Inputting of a polynomial with variable symbol.
template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT,
    typename Coef_factory
>
std::basic_istream<Ch, ChT>& input_var
(
    std::basic_istream<Ch, ChT>& in,
    sparse_polynom<F, I, REFCNT>& x,
    const Coef_factory& fctr,
    const char* var = monom_input_var_var_default,
    const char* mul = monom_input_var_mul_default,
    const char* pow = monom_input_var_pow_default
);


/// Inputting of a polynomial with variable symbol.
template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT
>
inline std::basic_istream<Ch, ChT>& input_var
(
    std::basic_istream<Ch, ChT>& in,
    sparse_polynom<F, I, REFCNT>& x,
    const char* var = monom_input_var_var_default,
    const char* mul = monom_input_var_mul_default,
    const char* pow = monom_input_var_pow_default
)
{
    return input_var(in, x, factory<F>(), var, mul, pow);
}


/// Oputputs polynomial in aligned form.  (Restricted implementation.)
/** The aligned form for polynomial includes two line of output:
    on the bottom line coefficients and variables symbol are placed and
    on the top line degrees are placed.
    WARNING.  Temporary this form is equal to output_var.    */
template
<
    typename Ch,
    typename ChT,
    typename F,
    typename I,
    bool REFCNT
>
inline std::basic_ostream<Ch, ChT>& output_aligned
(
    std::basic_ostream<Ch, ChT>& out,
    const sparse_polynom<F, I, REFCNT>& x,
    monoms_order mo = mo_inc,
    const char* var = monom_output_var_var_default,
    const char* mul = monom_output_var_mul_default,
    const char* pow = monom_output_var_pow_default
)
{
    return output_var(out, x, mo, var, mul, pow);
}


/// Станартный вывод полинома. Совпадает с sparse_polynom_output_var.
template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT
>
std::basic_ostream<Ch, ChT>& operator<<
(
    std::basic_ostream<Ch, ChT>& out,
    const sparse_polynom<F, I, REFCNT>& x
)
{
    return output_var(out, x);
}


/// Станартный вывод полинома. Совпадает с sparse_polynom_input_var.
template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT
>
std::basic_istream<Ch, ChT>& operator>>
(
    std::basic_istream<Ch, ChT>& in,
    sparse_polynom<F, I, REFCNT>& x
)
{
    return input_var(in, x);
}


// Дополнительные операции для полиномов.
// Некоторые операции реализованы не эффективно.
#define _ARAGELI_POLYNOM_BINOPER(AOPER, BOPER)    \
    template    \
    <    \
        typename F1,    \
        typename I1,    \
        bool REFCNT1,    \
        typename F2,    \
        typename I2,    \
        bool REFCNT2    \
    >    \
    inline sparse_polynom<F1, I1, REFCNT1> operator BOPER    \
    (    \
        const sparse_polynom<F1, I1, REFCNT1>& x,    \
        const sparse_polynom<F2, I2, REFCNT2>& y    \
    )    \
    {    \
        sparse_polynom<F1, I1, REFCNT1> t(x);    \
        return t AOPER y;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename I1,    \
        bool REFCNT1,    \
        typename F2,    \
        typename I2    \
    >    \
    inline sparse_polynom<F1, I1, REFCNT1> operator BOPER    \
    (    \
        const sparse_polynom<F1, I1, REFCNT1>& x,    \
        const monom<F2, I2>& y    \
    )    \
    {    \
        sparse_polynom<F1, I1, REFCNT1> t(x);    \
        return t AOPER y;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename I1,    \
        typename F2,    \
        typename I2,    \
        bool REFCNT2    \
    >    \
    inline sparse_polynom<F2, I2, REFCNT2> operator BOPER    \
    (    \
        const monom<F1, I1>& x,    \
        const sparse_polynom<F2, I2, REFCNT2>& y    \
    )    \
    {    \
        sparse_polynom<F2, I2, REFCNT2> t(x);    \
        return t AOPER y;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename I1,    \
        bool REFCNT1,    \
        typename F2    \
    >    \
    inline sparse_polynom<F1, I1, REFCNT1> operator BOPER    \
    (    \
        const sparse_polynom<F1, I1, REFCNT1>& x,    \
        const F2& y    \
    )    \
    {    \
        sparse_polynom<F1, I1, REFCNT1> t(x);    \
        return t AOPER y;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename F2,    \
        typename I2,    \
        bool REFCNT2    \
    >    \
    inline sparse_polynom<F2, I2, REFCNT2> operator BOPER    \
    (    \
        const F1& x,    \
        const sparse_polynom<F2, I2, REFCNT2>& y    \
    )    \
    {    \
        sparse_polynom<F2, I2, REFCNT2> t(x);    \
        return t AOPER y;    \
    }    \
    /* BEGIN OF TEMPORARY EXTENTION */    \
    template <typename F1, typename I1, bool REFCNT1>    \
    inline sparse_polynom<rational<F1>, I1, REFCNT1> operator BOPER    \
    (    \
        const sparse_polynom<rational<F1>, I1, REFCNT1>& x,    \
        const rational<F1>& y    \
    )    \
    {    \
        sparse_polynom<rational<F1>, I1, REFCNT1> t(x);    \
        return t AOPER y;    \
    }    \
    \
    template <typename F1, typename I1, bool REFCNT1>    \
    inline sparse_polynom<rational<F1>, I1, REFCNT1> operator BOPER    \
    (    \
        const rational<F1>& x,    \
        const sparse_polynom<rational<F1>, I1, REFCNT1>& y    \
    )    \
    {    \
        sparse_polynom<rational<F1>, I1, REFCNT1> t(x);    \
        return t AOPER y;    \
    }
    /* END OF TEMPORARY EXTENTION */


/// @name Дополнительные операторы для мономов; сочетание со скаляром.
// @{

_ARAGELI_POLYNOM_BINOPER(+=, +)
_ARAGELI_POLYNOM_BINOPER(-=, -)
_ARAGELI_POLYNOM_BINOPER(*=, *)
_ARAGELI_POLYNOM_BINOPER(/=, /)
_ARAGELI_POLYNOM_BINOPER(%=, %)

// @}

#undef _ARAGELI_POLYNOM_BINOPER


/// Lexicographical comparision of two polynomials.
/**
    Regards polynomial as sequence of monoms that begins monom with greatest
    degree value and compares two sequences of monoms lexicograficaly.
    Returns
    -   0 if a == b,
    -  -1 if a < b,
    -  +1 if a > b.
*/
template
<
    typename F1,
    typename I1,
    bool REFCNT1,
    typename F2,
    typename I2,
    bool REFCNT2
>
inline int cmp
(
    const sparse_polynom<F1, I1, REFCNT1>& a,
    const sparse_polynom<F2, I2, REFCNT2>& b
)
{
    ARAGELI_ASSERT_EX1_0(a.is_normal(), a);
    ARAGELI_ASSERT_EX1_0(b.is_normal(), b);

    typedef
        std::reverse_iterator
        <
            typename sparse_polynom<F1, I1, REFCNT1>::monom_const_iterator
        >
            RI_a;
    typedef
        std::reverse_iterator
        <
            typename sparse_polynom<F2, I2, REFCNT2>::monom_const_iterator
        >
            RI_b;

    int res =
        _Internal::aggregate_cmp
        (
            RI_a(a.monoms_end()),
            RI_a(a.monoms_begin()),
            RI_b(b.monoms_end()),
            RI_b(b.monoms_begin())
        );

    return res;
}


template <typename F, typename I, bool REFCNT>
bool is_primitive (const sparse_polynom<F, I, REFCNT>& x)
{
    vector<F, false> cfs(x.size(), x.coefs_begin());
    return is_coprime(cfs);
}


#define _ARAGELI_SPARSE_POLYNOMIAL_CMP(OPER)    \
    template    \
    <    \
        typename F1,    \
        typename I1,    \
        bool REFCNT1,    \
        typename F2,    \
        typename I2,    \
        bool REFCNT2    \
    >    \
    inline bool operator OPER    \
    (    \
        const sparse_polynom<F1, I1, REFCNT1>& a,    \
        const sparse_polynom<F2, I2, REFCNT2>& b    \
    )    \
    {    \
        return cmp(a, b) OPER 0;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename I1,    \
        bool REFCNT1,    \
        typename F2,    \
        typename I2    \
    >    \
    inline bool operator OPER    \
    (    \
        const sparse_polynom<F1, I1, REFCNT1>& a,    \
        const monom<F2, I2>& b    \
    )    \
    {    \
        return cmp(a, sparse_polynom<F2, I2, false>(b)) OPER 0;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename I1,    \
        typename F2,    \
        typename I2,    \
        bool REFCNT2    \
    >    \
    inline bool operator OPER    \
    (    \
        const monom<F1, I1>& a,    \
        const sparse_polynom<F2, I2, REFCNT2>& b    \
    )    \
    {    \
        return cmp(sparse_polynom<F1, I1, false>(a), b) OPER 0;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename I1,    \
        bool REFCNT1,    \
        typename F2    \
    >    \
    inline bool operator OPER    \
    (    \
        const sparse_polynom<F1, I1, REFCNT1>& a,    \
        const F2& b    \
    )    \
    {    \
        return cmp(a, sparse_polynom<F2, I1, false>(b)) OPER 0;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename F2,    \
        typename I2,    \
        bool REFCNT2    \
    >    \
    inline bool operator OPER    \
    (    \
        const F1& a,    \
        const sparse_polynom<F2, I2, REFCNT2>& b    \
    )    \
    {    \
        return cmp(sparse_polynom<F1, I2, false>(a), b) OPER 0;    \
    }


/// @name Standard comparison operators that based on cmp function.
// @{

_ARAGELI_SPARSE_POLYNOMIAL_CMP(==)
_ARAGELI_SPARSE_POLYNOMIAL_CMP(!=)
_ARAGELI_SPARSE_POLYNOMIAL_CMP(<=)
_ARAGELI_SPARSE_POLYNOMIAL_CMP(>=)
_ARAGELI_SPARSE_POLYNOMIAL_CMP(<)
_ARAGELI_SPARSE_POLYNOMIAL_CMP(>)

// @}

#undef _ARAGELI_SPARSE_POLYNOMIAL_CMP


template <typename F, typename I, bool REFCNT>
struct factory<sparse_polynom<F, I, REFCNT> >
{
    static const bool is_specialized = true;

    static const sparse_polynom<F, I, REFCNT>& unit ()
    {
        static const sparse_polynom<F, I, REFCNT> unit_s(Arageli::unit<F>());
        return unit_s;
    }

    static sparse_polynom<F, I, REFCNT> unit (const sparse_polynom<F, I, REFCNT>& x)
    {
        if(x.is_null())
            return unit();
        else
            return sparse_polynom<F, I, REFCNT>
                (Arageli::unit(x.leading_coef()));
    }

    static const sparse_polynom<F, I, REFCNT>& opposite_unit ()
    {
        static const sparse_polynom<F, I, REFCNT>
            opposite_unit_s(Arageli::opposite_unit<F>());
        return opposite_unit_s;
    }

    static sparse_polynom<F, I, REFCNT> opposite_unit
    (const sparse_polynom<F, I, REFCNT>& x)
    {
        if(x.is_null())
            return opposite_unit();
        else
            return sparse_polynom<F, I, REFCNT>
                (Arageli::opposite_unit(x.leading_coef()));
    }

    static const sparse_polynom<F, I, REFCNT>& null ()
    {
        static const sparse_polynom<F, I, REFCNT> null_s;
        return null_s;
    }

    static sparse_polynom<F, I, REFCNT> null (const sparse_polynom<F, I, REFCNT>& x)
    {
        return null();
    }

};

template <typename F, typename I, bool REFCNT>
inline bool is_unit (const sparse_polynom<F, I, REFCNT>& x)
{
    return x.is_unit();
}

template <typename F, typename I, bool REFCNT>
inline bool is_opposite_unit (const sparse_polynom<F, I, REFCNT>& x)
{
    return x.is_opposite_unit();
}

template <typename F, typename I, bool REFCNT>
inline bool is_null (const sparse_polynom<F, I, REFCNT>& x)
{
    return x.is_null();
}

template <typename F, typename I, bool REFCNT>
inline sparse_polynom<F, I, REFCNT> opposite
(const sparse_polynom<F, I, REFCNT>& x)
{
    return -x;
}

template <typename F, typename I, bool REFCNT>
inline sparse_polynom<F, I, REFCNT>& opposite
(const sparse_polynom<F, I, REFCNT>& x, sparse_polynom<F, I, REFCNT>* y)
{
    return (*y = x).opposite();
}

template <typename F, typename I, bool REFCNT>
inline sparse_polynom<F, I, REFCNT>& opposite
(sparse_polynom<F, I, REFCNT>* x)
{
    return x->opposite();
}

template <typename F, typename I, bool REFCNT>
inline sparse_polynom<F, I, REFCNT> inverse (const sparse_polynom<F, I, REFCNT>& x)
{
    ARAGELI_ASSERT_0(x.is_const());
    return inverse(x.leading_coef());
}

template <typename F, typename I, bool REFCNT>
inline sparse_polynom<F, I, REFCNT>& inverse
(
    const sparse_polynom<F, I, REFCNT>& x,
    sparse_polynom<F, I, REFCNT>* y
)
{
    ARAGELI_ASSERT_0(x.is_const());
    return *y = inverse(x->leading_coef());
}

template <typename F, typename I, bool REFCNT>
inline sparse_polynom<F, I, REFCNT>& inverse (sparse_polynom<F, I, REFCNT>* x)
{
    ARAGELI_ASSERT_0(x->is_const());
    inverse(&x->leading_coef());
    return *x;
}

template <typename F, typename I, bool REFCNT>
inline std::ostream& output_polynom_first
(
    std::ostream& out,
    const sparse_polynom<F, I, REFCNT>& x
)
{
    return out << '(' << x << ')';
}

template <typename F, typename I, bool REFCNT>
inline std::ostream& output_polynom_internal
(std::ostream& out, const sparse_polynom<F, I, REFCNT>& x)
{
    return out << "+(" << x << ')';
}

template <typename F, typename I, bool REFCNT>
inline std::ostream& output_pow
(std::ostream& out, const sparse_polynom<F, I, REFCNT>& x)
{
    return out << '(' << x << ')';
}

template <typename F, typename I, bool REFCNT>
std::istream& input_polynom_first
(std::istream& in, sparse_polynom<F, I, REFCNT>& x);

template <typename F, typename I, bool REFCNT>
std::istream& input_polynom_internal
(std::istream& in, sparse_polynom<F, I, REFCNT>& x);

template <typename F, typename I, bool REFCNT>
inline std::istream& input_pow
(std::istream& in, sparse_polynom<F, I, REFCNT>& x)
{
    return input_polynom_first(in, x);
}


template <typename F, typename I, bool REFCNT>
struct type_traits<sparse_polynom<F, I, REFCNT> > :
    public type_traits_default<sparse_polynom<F, I, REFCNT> >
{
    static const bool is_specialized =
        type_traits<F>::is_specialized &&
        type_traits<I>::is_specialized;

    static const bool is_integer_number = false;
    static const bool is_polynom = true;
    static const bool is_real_number = false;
    static const bool is_rational_number = false;
    static const bool is_complex_number = false;
    static const bool is_ring = type_traits<F>::is_ring;
    static const bool is_field = false;

    static const bool is_finite =
        type_traits<F>::is_finite &&
        type_traits<I>::is_finite;

    static const bool is_additive_group = type_traits<F>::is_ring;
    static const bool is_multiplicative_group = false;

    static const bool has_zero_divisor =
        type_traits<F>::has_zero_divisor &&
        type_traits<I>::has_null;

    static const bool is_integer_modulo_ring = false;
    static const bool is_matrix = false;
    static const bool is_vector = false;

    static const bool has_commutative_multiplication =
        type_traits<F>::has_commutative_multiplication &&
        type_traits<I>::has_commutative_addition;

    static const bool has_commutative_addition =
        type_traits<F>::has_commutative_addition;

    static const bool has_null =
        type_traits<F>::has_null &&
        type_traits<I>::has_null;

    static const bool has_unit =
        type_traits<F>::has_unit &&
        type_traits<I>::has_null;

    static const bool has_opposite_unit =
        type_traits<F>::has_opposite_unit &&
        type_traits<I>::has_null;

    /// True iff type is composite type consists another elements.
    static const bool is_aggregate = true;

    /// Type of each element.
    typedef F element_type;

    typedef type_category::sparse_polynom category_type;
    static const category_type category_value;
};

template <typename F, typename I, bool REFCNT>
const type_category::sparse_polynom
type_traits<sparse_polynom<F, I, REFCNT> >::category_value =
    type_category::sparse_polynom();

template <typename F, typename I, bool REFCNT>
template <typename F1>
inline F1 sparse_polynom<F, I, REFCNT>::subs (const F1& x) const
{
    return subs(x, factory<F1>());
}


/// Specialization of euclid for sparse_polynom.
template <typename F, typename I, bool REFCNT>
sparse_polynom<F, I, REFCNT> euclid
(
    sparse_polynom<F, I, REFCNT> a,
    sparse_polynom<F, I, REFCNT> b
)
{
    //std::cout << "\neuclid: a = " << a << ", b = " << b << std::endl;

    if(is_null(a))
        return b;
    if(is_null(b))
        return a;

    while(!(a %= b).is_null())
        a.swap(b);

    return b;
}


/// Specialization of is_coprime for sparse_polynom.
template <typename F, typename I, bool REFCNT>
inline bool is_coprime
(
    const sparse_polynom<F, I, REFCNT>& a,
    const sparse_polynom<F, I, REFCNT>& b
)
{
    return gcd(a, b).is_const();
}


template
<
    typename F1,
    typename I1,
    bool REFCNT1,
    typename F2,
    typename I2,
    bool REFCNT2,
    typename Q,
    typename R
>
inline void prdivide
(
    const sparse_polynom<F1, I1, REFCNT1>& a,
    const sparse_polynom<F2, I2, REFCNT2>& b,
    Q& q,
    R& r
)
{
    divide(a, b, q, r);
}


template
<
    typename F1,
    typename I1,
    bool REFCNT1,
    typename I2
>
inline sparse_polynom<F1, I1, REFCNT1> pow
(const sparse_polynom<F1, I1, REFCNT1>& a, const I2& n)
{
    sparse_polynom<F1, I1, REFCNT1> t(a);
    return t.pow(n);
}


/// Absolute value for monom.
/** It is a monom with the same degree and abs(x.coef()) coefficient. */
template <typename F, typename I>
inline monom<F, I> abs
(const monom<F, I>& x)
{
    return x;
}

/// Absolute value for polynomial.
/** It is x if leading coefficient of x is positive
    and -x if leading coefficient is negative. */
template <typename F, typename I, bool REFCNT>
inline sparse_polynom<F, I, REFCNT> abs
(const sparse_polynom<F, I, REFCNT>& x)
{
    return x;
}

} // namespace Arageli


namespace std
{

template
<
    typename F1,
    typename I1,
    bool REFCNT1,
    typename I2
>
inline Arageli::sparse_polynom<F1, I1, REFCNT1> pow
(const Arageli::sparse_polynom<F1, I1, REFCNT1>& a, const I2& n)
{
    return Arageli::pow(a, n);
}


template
<
    typename F1,
    typename I1,
    typename F2,
    typename I2
>
inline void swap
(
    Arageli::monom<F1, I1>& a,
    Arageli::monom<F2, I2>& b
)
{
    a.swap(b);
}


template
<
    typename F1,
    typename I1,
    bool REFCNT1,
    typename F2,
    typename I2,
    bool REFCNT2
>
inline void swap
(
    Arageli::sparse_polynom<F1, I1, REFCNT1>& a,
    Arageli::sparse_polynom<F2, I2, REFCNT2>& b
)
{
    a.swap(b);
}


template <typename F, typename I>
inline void swap
(
    Arageli::monom<F, I>& a,
    Arageli::monom<F, I>& b
)
{
    a.swap(b);
}


template <typename F, typename I, bool REFCNT>
inline void swap
(
    Arageli::sparse_polynom<F, I, REFCNT>& a,
    Arageli::sparse_polynom<F, I, REFCNT>& b
)
{
    a.swap(b);
}


///    Absolute value for monom.
/**    It is a monom with the same degree and abs(x.coef()) coefficient. */
template <typename F, typename I>
inline Arageli::monom<F, I> abs
(const Arageli::monom<F, I>& x)
{
    return Arageli::abs(x);
}

/// Absolute value for polynomial.
/** It is x if leading coefficient of x is positive
    and -x if leading coefficient is negative. */
template <typename F, typename I, bool REFCNT>
inline Arageli::sparse_polynom<F, I, REFCNT> abs
(const Arageli::sparse_polynom<F, I, REFCNT>& x)
{
    return Arageli::abs(x);
}


} // namespace std


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SPARSE_POLYNOM
    #include "sparse_polynom.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SPARSE_POLYNOM
#endif


#endif  //  #ifndef _ARAGELI_sparse_polynom_hpp_
