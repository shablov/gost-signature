/*****************************************************************************

    function_traits.hpp

    This file is part of the Arageli library.

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

/** \file function_traits.hpp
    \brief Additional information about functions and operators.

    This file contains some structures to represent additional
    information about functions and operators. That information likes that:
    argument types, result type, side effect, etc.
    That information is used by some parts of the library. Note that
    here is only general implementation of the structures; it is
    not specialized for concrete types and makes some assumptions for
    specialized types. If function for your types doesn't fit to this
    implementaion you need to implement own specialization of one or all
    forms of function_traits templates.

    Presented specializations fit to the built-in types such int, double,
    bool etc., but the applied rules differ from the standard. For instant,
    if we add signed char and signed short the result type will be signed
    short, not signed int as it is in the standard of C++. This exception
    omit the rule that all arithmetic integer type will be casted to int
    before performing operations. In our case it's useful when we are dealing
    with vectors and other aggregate types. In addition, this exception
    makes rules simpler and easier to implement.
*/


#ifndef _ARAGELI_function_traits_hpp_
#define _ARAGELI_function_traits_hpp_

#include "config.hpp"
#include "type_opers.hpp"
#include "type_traits.hpp"
#include "type_pair_traits.hpp"


//****************************************************************************


namespace Arageli
{

/// Holds additional information about unary function.
/** Use it to build your own function_traits. */
template
<
    typename TAG,
    typename ARG,
    typename RETTYPE,
    bool ALARG,
    bool SE
>
struct unary_function_traits_base
{
    static const bool is_specialized = true;
    typedef RETTYPE result_type;
    typedef ARG argument_type;
    typedef TAG tag;
    static const bool alternates_argument = ALARG;
    static const bool has_side_effect = SE;
};


template
<
    typename TAG,
    typename ARG1, typename ARG2,
    typename RETTYPE,
    bool ALARG1, bool ALARG2,
    bool SE
>
struct binary_function_traits_base
{
    static const bool is_specialized = true;
    typedef RETTYPE result_type;
    typedef ARG1 first_argument_type;
    typedef ARG2 second_argument_type;
    typedef TAG tag;
    static const bool alternates_first_argument = ALARG1;
    static const bool alternates_second_argument = ALARG2;
    static const bool has_side_effect = SE;
};


template
<
    typename TAG,
    typename ARG1, typename ARG2, typename ARG3,
    typename RETTYPE,
    bool ALARG1, bool ALARG2, bool ALARG3,
    bool SE
>
struct ternary_function_traits_base
{
    static const bool is_specialized = true;
    typedef RETTYPE result_type;
    typedef ARG1 first_argument_type;
    typedef ARG2 second_argument_type;
    typedef ARG3 third_argument_type;
    typedef TAG tag;
    static const bool alternates_first_argument = ALARG1;
    static const bool alternates_second_argument = ALARG2;
    static const bool alternates_third_argument = ALARG3;
    static const bool has_side_effect = SE;
};


/// Function categories.
namespace function_tag
{


class function {};

class unary :
    public virtual function
{};

class binary :
    public virtual function
{};

class ternary :
    public virtual function
{};

class arithmetic :
    public virtual function
{};

class logical :
    public virtual function
{};

class bitwise :
    public virtual function
{};

class compare :
    public virtual function
{};

class each_compare :
    public binary, public compare
{};

class all_compare :
    public binary, public compare
{};

// Concrete functions.

/// Tag for operator*(x).
class indirection :
    public unary
{};

/// Tag for operator&(x).
class address :
    public unary
{};

/// Tag for operator+(x).
class unary_plus :
    public unary,
    public arithmetic
{};

/// Tag for operator-(x).
class unary_minus :
    public unary,
    public arithmetic
{};

/// Tag for operator!(x).
class logical_not :
    public unary,
    public logical
{};

/// Tag for operator~(x).
class bitwise_not :
    public unary,
    public bitwise
{};

/// Tag for operator++(x).
class prefix_increment :
    public unary,
    public arithmetic
{};

/// Tag for operator--(x).
class prefix_decrement :
    public unary,
    public arithmetic
{};

/// Tag for operator++(x, int).
class postfix_increment :
    public unary,
    public arithmetic
{};

/// Tag for operator--(x, int).
class postfix_decrement :
    public unary,
    public arithmetic
{};

/// Tag for X::operator()().
class parentheses_0 :
    public unary
{};

/// Tag for X::operator()(y).
class parentheses_1 :
    public binary
{};

/// Tag for X::operator()(y, z).
class parentheses_2 :
    public ternary
{};

/// Tag for X::operator[](y).
class subscript :
    public binary
{};


// TODO: CLASSIFY THE FOLLOWING TAGS!

/// Tag for operator+(x, y).
class plus
{};

/// Tag for operator-(x, y).
class minus
{};

/// Tag for operator*(x, y).
class multiplies
{};

/// Tag for operator/(x, y).
class divides
{};

/// Tag for operator%(x, y).
class modulus
{};

/// Tag for operator||(x, y).
class logical_or
{};

/// Tag for operator&&(x, y).
class logical_and
{};

/// Tag for operator==(x, y).
class equal_to :
    public binary,
    public compare
{};

/// Tag for operator!=(x, y).
class not_equal_to :
    public binary,
    public compare
{};

/// Tag for operator>(x, y).
class greater :
    public binary,
    public compare
{};

/// Tag for operator<(x, y).
class less :
    public binary,
    public compare
{};

/// Tag for operator>=(x, y).
class greater_equal :
    public binary,
    public compare
{};

/// Tag for operator<=(x, y).
class less_equal :
    public binary,
    public compare
{};


/// Tag for operator|(x, y).
class bitwise_or
{};

/// Tag for operator&(x, y).
class bitwise_and
{};

/// Tag for operator^(x, y).
class bitwise_xor
{};


/// Tag for operator<<(x, y).
class shift_left
{};

/// Tag for operator>>(x, y).
class shift_right
{};


/// Tag for X::operator=(y).
class assign
{};

/// Tag for operator+=(x, y).
class assign_plus
{};

/// Tag for operator-=(x, y).
class assign_minus
{};

/// Tag for operator*=(x, y).
class assign_multiplies
{};

/// Tag for operator/=(x, y).
class assign_divides
{};

/// Tag for operator%=(x, y).
class assign_modulus
{};

/// Tag for operator|=(x, y).
class assign_bitwise_or
{};

/// Tag for operator&=(x, y).
class assign_bitwise_and
{};

/// Tag for operator^=(x, y).
class assign_bitwise_xor
{};

/// Tag for operator<<=(x, y).
class assign_shift_left
{};

/// Tag for operator>>=(x, y).
class assign_shift_right
{};

typedef assign_plus right_assign_plus;
typedef assign_minus right_assign_minus;
typedef assign_multiplies right_assign_multiplies;
typedef assign_divides right_assign_divides;
typedef assign_modulus right_assign_modulus;
typedef assign_bitwise_or right_assign_bitwise_or;
typedef assign_bitwise_and right_assign_bitwise_and;
typedef assign_bitwise_xor right_assign_bitwise_xor;
typedef assign_shift_left right_assign_shift_left;
typedef assign_shift_right right_assign_shift_right;


/// Tag for left_assign_plus(x, y).
class left_assign_plus
{};

/// Tag for left_assign_minus(x, y).
class left_assign_minus
{};

/// Tag for left_assign_multiplies(x, y).
class left_assign_multiplies
{};

/// Tag for left_assign_modulus(x, y).
class left_assign_modulus
{};

/// Tag for left_assign_bitwise_or(x, y).
class left_assign_bitwise_or
{};

/// Tag for left_assign_bitwise_and(x, y).
class left_assign_bitwise_and
{};

/// Tag for left_assign_bitwise_xor(x, y).
class left_assign_bitwise_xor
{};

/// Tag for left_assign_shift_left(x, y).
class left_assign_shift_left
{};

/// Tag for left_assign_shift_right(x, y).
class left_assign_shift_right
{};


/// Tag for cmp(x, y).
class cmp :
    public binary,
    public compare
{};

class each_cmp :
    public each_compare
{};

class each_equal_to :
    public each_compare
{};

class each_not_equal_to :
    public each_compare
{};

class each_greater :
    public each_compare
{};

class each_less :
    public each_compare
{};

class each_greater_equal :
    public each_compare
{};

class each_less_equal :
    public each_compare
{};

class all_equal_to :
    public all_compare
{};

class all_not_equal_to :
    public all_compare
{};

class all_greater :
    public all_compare
{};

class all_less :
    public all_compare
{};

class all_greater_equal :
    public all_compare
{};

class all_less_equal :
    public all_compare
{};


/// Makes function tag X by each_X or all_X tags.
template <typename T>
struct omit_elemwise;

#define ARAGELI_FUNCTION_TAG_AG2EL(NAME, PREFIX)    \
    template <>    \
    struct omit_elemwise<PREFIX##_##NAME>    \
    {    \
        typedef NAME type;    \
    };

ARAGELI_FUNCTION_TAG_AG2EL(cmp, each)
ARAGELI_FUNCTION_TAG_AG2EL(equal_to, each)
ARAGELI_FUNCTION_TAG_AG2EL(not_equal_to, each)
ARAGELI_FUNCTION_TAG_AG2EL(less, each)
ARAGELI_FUNCTION_TAG_AG2EL(greater, each)
ARAGELI_FUNCTION_TAG_AG2EL(less_equal, each)
ARAGELI_FUNCTION_TAG_AG2EL(greater_equal, each)
ARAGELI_FUNCTION_TAG_AG2EL(equal_to, all)
ARAGELI_FUNCTION_TAG_AG2EL(not_equal_to, all)
ARAGELI_FUNCTION_TAG_AG2EL(less, all)
ARAGELI_FUNCTION_TAG_AG2EL(greater, all)
ARAGELI_FUNCTION_TAG_AG2EL(less_equal, all)
ARAGELI_FUNCTION_TAG_AG2EL(greater_equal, all)


/// Extract unique parent of tag.
/** Now it is implemented only for each_X and all_X tags. */
template <typename T> struct category
{
    typedef function type;
};

#define ARAGELI_FUNCTION_TAG_CMPCAT(TAG, CATEGORY)    \
    template <>    \
    struct category<TAG>    \
    {    \
        typedef CATEGORY type;    \
    };

ARAGELI_FUNCTION_TAG_CMPCAT(each_equal_to, each_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(each_not_equal_to, each_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(each_greater, each_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(each_less, each_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(each_greater_equal, each_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(each_less_equal, each_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(all_equal_to, all_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(all_not_equal_to, all_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(all_greater, all_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(all_less, all_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(all_greater_equal, all_compare)
ARAGELI_FUNCTION_TAG_CMPCAT(all_less_equal, all_compare)


} // namespace function_tag


// WARING! Do we really need the default implementation for function_traits templates?

template <typename TAG, typename ARG>
struct unary_function_traits
{
    static const bool is_specialized = false;
    typedef void result_type;
    typedef ARG argument_type;
    typedef TAG tag;
    static const bool alternates_argument = false;
    static const bool has_side_effect = false;
};


template <typename TAG, typename ARG1, typename ARG2>
struct binary_function_traits
{
    static const bool is_specialized = false;
    typedef void result_type;
    typedef ARG1 first_argument_type;
    typedef ARG2 second_argument_type;
    typedef TAG tag;
    static const bool alternates_first_argument = false;
    static const bool alternates_second_argument = false;
    static const bool has_side_effect = false;
};


template <typename TAG, typename ARG1, typename ARG2, typename ARG3>
struct ternary_function_traits
{
    static const bool is_specialized = false;
    typedef void result_type;
    typedef ARG1 first_argument_type;
    typedef ARG2 second_argument_type;
    typedef ARG3 third_argument_type;
    typedef TAG tag;
    static const bool alternates_first_argument = false;
    static const bool alternates_second_argument = false;
    static const bool alternates_third_argument = false;
    static const bool has_side_effect = false;
};


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// WARNING! PROBLEM: In this implementaion we have argument types the same
// as type of paramenter of the template. Actually it is not true.
// For example unary_function_traits<prefix_increment, T>::argument_type is
// just 'T', but need 'T&' if T is not in the form 'P&' for some P.

#define ARAGELI_UNARY_FUNCTION_TRAITS(MNEM, RETTYPE, ALARG, SE)    \
    template <typename Arg>    \
    struct unary_function_traits<function_tag::MNEM, Arg> :    \
        public unary_function_traits_base    \
        <    \
            function_tag::MNEM,    \
            Arg,    \
            RETTYPE,    \
            ALARG,    \
            SE    \
        >    \
    {};

#define ARAGELI_BINARY_FUNCTION_TRAITS(MNEM, RETTYPE, ALARG1, ALARG2, SE)    \
    template <typename Arg1, typename Arg2>    \
    struct binary_function_traits<function_tag::MNEM, Arg1, Arg2> :    \
        public binary_function_traits_base    \
        <    \
            function_tag::MNEM,    \
            Arg1,    \
            Arg2,    \
            RETTYPE,    \
            ALARG1,    \
            ALARG2,    \
            SE    \
        >    \
    {};

#define ARAGELI_TERNARY_FUNCTION_TRAITS(MNEM, RETTYPE, ALARG1, ALARG2, ALARG3, SE)    \
    template <typename Arg1, typename Arg2, typename Arg3>    \
    struct ternary_function_traits<function_tag::MNEM, Arg1, Arg2, Arg3> :    \
        public ternary_function_traits_base    \
        <    \
            function_tag::MNEM,    \
            Arg1,    \
            Arg2,    \
            Arg3,    \
            RETTYPE,    \
            ALARG1,    \
            ALARG2,    \
            ALARG3,    \
            SE    \
        >    \
    {};

#define ARAGELI_EACH_CMP_FUNCTION_TRAITS(MNEM, ALARG1, ALARG2, SE)    \
    template <typename Arg1, typename Arg2>    \
    struct binary_function_traits<function_tag::MNEM, Arg1, Arg2> :    \
        public binary_function_traits_base    \
        <    \
            function_tag::MNEM,    \
            Arg1,    \
            Arg2,    \
            typename type_traits<Arg1>::    \
                template other_element_type_refcnt<bool, true>::type,    \
            ALARG1,    \
            ALARG2,    \
            SE    \
        >    \
    {};


namespace _Internal
{

template <typename T1, typename T2>
struct absorbent :
    public power_union_type<T1, T2>
{};

} // namespace _Internal


#define _ARAGELI_CLEARARG typename omit_ref<Arg>::type
#define _ARAGELI_CLEARARG1 typename omit_ref<Arg1>::type

#define _ARAGELI_ABSORBENT    \
    typename _Internal::absorbent    \
    <    \
        typename omit_ref<Arg1>::type,    \
        typename omit_ref<Arg2>::type    \
    >::type

#define ARAGELI_BINARY_FUNCTION_TRAITS_ARITH(NAME)    \
    ARAGELI_BINARY_FUNCTION_TRAITS    \
        (NAME, _ARAGELI_ABSORBENT, false, false, false);


ARAGELI_UNARY_FUNCTION_TRAITS
(
    indirection,
    typename omit_asterisk<_ARAGELI_CLEARARG>::type,
    false,
    false
);

ARAGELI_UNARY_FUNCTION_TRAITS(address, _ARAGELI_CLEARARG*, false, false);
ARAGELI_UNARY_FUNCTION_TRAITS(unary_plus, _ARAGELI_CLEARARG, false, false);
ARAGELI_UNARY_FUNCTION_TRAITS(unary_minus, _ARAGELI_CLEARARG, false, false);
ARAGELI_UNARY_FUNCTION_TRAITS(logical_not, bool, false, false);
ARAGELI_UNARY_FUNCTION_TRAITS(bitwise_not, _ARAGELI_CLEARARG, false, false);

ARAGELI_UNARY_FUNCTION_TRAITS
(
    prefix_increment,
    _ARAGELI_CLEARARG&,
    true,
    false
);

ARAGELI_UNARY_FUNCTION_TRAITS
(
    prefix_decrement,
    _ARAGELI_CLEARARG&,
    true,
    false
);

ARAGELI_UNARY_FUNCTION_TRAITS
(
    postfix_increment,
    _ARAGELI_CLEARARG,
    true,
    false
);

ARAGELI_UNARY_FUNCTION_TRAITS
(
    postfix_decrement,
    _ARAGELI_CLEARARG,
    true,
    false
);

ARAGELI_UNARY_FUNCTION_TRAITS
(
    parentheses_0, Arg,
    false,
    false
);

ARAGELI_BINARY_FUNCTION_TRAITS
(
    parentheses_1, Arg1,
    false,
    false,
    false
);

ARAGELI_TERNARY_FUNCTION_TRAITS
(
    parentheses_2, Arg1,
    false,
    false,
    false,
    false
);

ARAGELI_BINARY_FUNCTION_TRAITS
(
    subscript,
    typename array_element<Arg1>::type,
    false,
    false,
    false
);

ARAGELI_BINARY_FUNCTION_TRAITS_ARITH(plus);
ARAGELI_BINARY_FUNCTION_TRAITS_ARITH(minus);
ARAGELI_BINARY_FUNCTION_TRAITS_ARITH(multiplies);
ARAGELI_BINARY_FUNCTION_TRAITS_ARITH(divides);
ARAGELI_BINARY_FUNCTION_TRAITS_ARITH(modulus);
ARAGELI_BINARY_FUNCTION_TRAITS_ARITH(bitwise_or);
ARAGELI_BINARY_FUNCTION_TRAITS_ARITH(bitwise_and);
ARAGELI_BINARY_FUNCTION_TRAITS_ARITH(bitwise_xor);

ARAGELI_BINARY_FUNCTION_TRAITS
(
    shift_left,
    _ARAGELI_CLEARARG1,
    false,
    false,
    false
);

ARAGELI_BINARY_FUNCTION_TRAITS
(
    shift_right,
    _ARAGELI_CLEARARG1,
    false,
    false,
    false
);

ARAGELI_BINARY_FUNCTION_TRAITS(logical_or, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(logical_and, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(equal_to, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(not_equal_to, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(greater, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(less, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(greater_equal, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(less_equal, bool, false, false, false);

ARAGELI_BINARY_FUNCTION_TRAITS(all_equal_to, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(all_not_equal_to, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(all_greater, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(all_less, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(all_greater_equal, bool, false, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(all_less_equal, bool, false, false, false);

ARAGELI_EACH_CMP_FUNCTION_TRAITS(each_equal_to, false, false, false);
ARAGELI_EACH_CMP_FUNCTION_TRAITS(each_not_equal_to, false, false, false);
ARAGELI_EACH_CMP_FUNCTION_TRAITS(each_greater, false, false, false);
ARAGELI_EACH_CMP_FUNCTION_TRAITS(each_less, false, false, false);
ARAGELI_EACH_CMP_FUNCTION_TRAITS(each_greater_equal, false, false, false);
ARAGELI_EACH_CMP_FUNCTION_TRAITS(each_less_equal, false, false, false);


ARAGELI_BINARY_FUNCTION_TRAITS(assign, Arg1&, true, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(assign_plus, Arg1&, true, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(assign_minus, Arg1&, true, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(assign_multiplies, Arg1&, true, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(assign_divides, Arg1&, true, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(assign_modulus, Arg1&, true, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(assign_bitwise_or, Arg1&, true, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(assign_bitwise_and, Arg1&, true, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(assign_bitwise_xor, Arg1&, true, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(assign_shift_left, Arg1&, true, false, false);
ARAGELI_BINARY_FUNCTION_TRAITS(assign_shift_right, Arg1&, true, false, false);

ARAGELI_BINARY_FUNCTION_TRAITS(left_assign_plus, Arg2&, false, true, false);
ARAGELI_BINARY_FUNCTION_TRAITS(left_assign_minus, Arg2&, false, true, false);
ARAGELI_BINARY_FUNCTION_TRAITS(left_assign_multiplies, Arg2&, false, true, false);
ARAGELI_BINARY_FUNCTION_TRAITS(left_assign_modulus, Arg2&, false, true, false);
ARAGELI_BINARY_FUNCTION_TRAITS(left_assign_bitwise_or, Arg2&, false, true, false);
ARAGELI_BINARY_FUNCTION_TRAITS(left_assign_bitwise_and, Arg2&, false, true, false);
ARAGELI_BINARY_FUNCTION_TRAITS(left_assign_bitwise_xor, Arg2&, false, true, false);
ARAGELI_BINARY_FUNCTION_TRAITS(left_assign_shift_left, Arg2&, false, true, false);
ARAGELI_BINARY_FUNCTION_TRAITS(left_assign_shift_right, Arg2&, false, true, false);


#undef _ARAGELI_CLEARARG
#undef _ARAGELI_CLEARARG1
#undef _ARAGELI_ABSORBENT


} // namespace Arageli



//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_FUNCTION_TRAITS
//    #include "function_traits.cpp"
//    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_FUNCTION_TRAITS
//#endif


#endif  //  #ifndef _ARAGELI_function_traits_hpp_
