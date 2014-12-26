/*****************************************************************************

    mixcomp.hpp

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

/** \file mixcomp.hpp
    \brief Base and support for mixed computations.

    This file contains base system for mixed computation. Use this to integrate
    new type to the library.
*/


#ifndef _ARAGELI_mixcomp_hpp_
#define _ARAGELI_mixcomp_hpp_

#include "config.hpp"

#include "basefuncs.hpp"


//****************************************************************************


namespace Arageli
{


// TODO: Need a default implementation of Gate Slot template class system.

/// Gate slot for mixed computations.
/** Each correct specialization of this structure must includes the following.
    The first is type function_traits that would be used as a base for
    function traits builded in Mix Gate.  The second is static function
    (named just 'function') that would be used as entry point for the function
    which is described by the declaration of binary_function_gate_slot. */
template <typename Tag, typename Owner, typename T1, typename T2>
struct binary_function_gate_slot
{
    /*
    struct function_traits :
        public binary_function_traits<T1, T2, Owner, false, false, false>
    {};
    */
};


/*
// Default implementation of gate slots for some builtin operator.
#define ARAGELI_BINARY_FUNCTION_GATE_SLOT(MNEM)    \
    template <typename Owner, typename T1, typename T2>    \
    struct binary_function_gate_slot    \
        <function_tag::MNEM, Owner, T1, T2> {};

ARAGELI_BINARY_FUNCTION_GATE_SLOT(plus);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(minus);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(multiplies);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(divides);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(modulus);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(logical_or);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(logical_and);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(equal_to);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(not_equal_to);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(greater);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(less);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(greater_equal);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(less_equal);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(bitwise_or);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(bitwise_and);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(bitwise_xor);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(shift_left);
ARAGELI_BINARY_FUNCTION_GATE_SLOT(shift_right);
*/

/// Implements correct gate for compiler to undestand binary functions in mixed computations.
/** This macro need previously defined following macros
    ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_PARAMS
    ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE
    ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_PARAMS
    ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE
    ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS
    ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE
    ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OWNER_COMMA
    ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA

    */
#define ARAGELI_BINARY_FUNCTION_MIXGATE(FUNC, OPER)    \
    template    \
    <    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_PARAMS    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS    \
    >    \
    struct binary_function_traits    \
    <    \
        function_tag::FUNC,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE    \
    > :    \
        public binary_function_gate_slot    \
        <    \
            function_tag::FUNC,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE    \
        >::function_traits    \
    {};    \
    \
    template    \
    <    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_PARAMS    \
    >    \
    struct binary_function_traits    \
    <    \
        function_tag::FUNC,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE    \
    > :    \
        public binary_function_gate_slot    \
        <    \
            function_tag::FUNC,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE    \
        >::function_traits    \
    {};    \
    \
    template    \
    <    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_PARAMS    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS    \
    >    \
    inline typename binary_function_traits    \
    <    \
        function_tag::FUNC,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE    \
    >::result_type    \
    OPER    \
    (    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE a,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE b    \
    )    \
    {    \
        return binary_function_gate_slot    \
        <    \
            function_tag::FUNC,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE    \
        >::function(a, b);    \
    }    \
    \
    template    \
    <    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_PARAMS    \
    >    \
    inline typename binary_function_traits    \
    <    \
        function_tag::FUNC,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE    \
    >::result_type    \
    OPER    \
    (    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE a,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE b    \
    )    \
    {    \
        return binary_function_gate_slot    \
        <    \
            function_tag::FUNC,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE    \
        >::function(a, b);    \
    }


#define ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(FUNC, OPER)    \
    template    \
    <    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_PARAMS    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OWNER_COMMA    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_PARAMS    \
    >    \
    struct binary_function_traits    \
    <    \
        function_tag::FUNC,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE    \
    > :    \
        public binary_function_gate_slot    \
        <    \
            function_tag::FUNC,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE    \
        >::function_traits    \
    {};    \
    \
    template    \
    <    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_PARAMS    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OWNER_COMMA    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_PARAMS    \
    >    \
    inline typename binary_function_traits    \
    <    \
        function_tag::FUNC,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE    \
    >::result_type    \
    OPER    \
    (    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE a,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE b    \
    )    \
    {    \
        return binary_function_gate_slot    \
        <    \
            function_tag::FUNC,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE,    \
            ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE    \
        >::function(a, b);    \
    }


#define ARAGELI_BINARY_COMPARISON_MIXGATE(FUNC, OPER, OP)    \
    template    \
    <    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_PARAMS    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS    \
    >    \
    inline bool OPER    \
    (    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE a,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE b    \
    )    \
    {    \
        return (cmp(a, b) OP 0);    \
    }    \
    \
    template    \
    <    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_PARAMS    \
    >    \
    inline bool OPER    \
    (    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE a,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE b    \
    )    \
    {    \
        return (cmp(a, b) OP 0);    \
    }


#define ARAGELI_BINARY_COMPARISON_SELF_MIXGATE(FUNC, OPER, OP)    \
    template    \
    <    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_PARAMS    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OWNER_COMMA    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_PARAMS    \
    >    \
    inline bool OPER    \
    (    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE a,    \
        ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE b    \
    )    \
    {    \
        return (cmp(a, b) OP 0);    \
    }


#define ARAGELI_ALL_BINARY_ARITHM_FUNCTIONS_MIXGATE    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(plus, operator+)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(minus, operator-)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(multiplies, operator*)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(divides, operator/)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(modulus, operator%)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(bitwise_or, operator|)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(bitwise_and, operator&)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(bitwise_xor, operator^)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(shift_left, operator<<)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(shift_right, operator>>)

#define ARAGELI_ALL_BINARY_LOGIC_FUNCTIONS_MIXGATE    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(logical_or, operator||)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(logical_and, operator&&)

#define ARAGELI_ALL_BINARY_CMP_MIXGATE    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(cmp, cmp)    \
    ARAGELI_BINARY_COMPARISON_MIXGATE(equal_to, operator==, ==)    \
    ARAGELI_BINARY_COMPARISON_MIXGATE(not_equal_to, operator!=, !=)    \
    ARAGELI_BINARY_COMPARISON_MIXGATE(greater, operator>, >)    \
    ARAGELI_BINARY_COMPARISON_MIXGATE(less, operator<, <)    \
    ARAGELI_BINARY_COMPARISON_MIXGATE(greater_equal, operator>=, >=)    \
    ARAGELI_BINARY_COMPARISON_MIXGATE(less_equal, operator<=, <=)

#define ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(NAME)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE(NAME, NAME)

#define ARAGELI_ALL_EACH_ALL_CMP_FUNCTIONS_MIXGATE    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(each_equal_to)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(each_not_equal_to)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(each_greater)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(each_less)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(each_greater_equal)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(each_less_equal)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(all_equal_to)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(all_not_equal_to)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(all_greater)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(all_less)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(all_greater_equal)    \
    ARAGELI_BINARY_FUNCTION_MIXGATE_SAMENAME(all_less_equal)

#define ARAGELI_ALL_BINARY_ARITHM_FUNCTIONS_SELF_MIXGATE    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(plus, operator+)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(minus, operator-)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(multiplies, operator*)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(divides, operator/)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(modulus, operator%)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(bitwise_or, operator|)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(bitwise_and, operator&)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(bitwise_xor, operator^)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(shift_left, operator<<)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(shift_right, operator>>)

#define ARAGELI_ALL_BINARY_LOGIC_FUNCTIONS_SELF_MIXGATE    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(logical_or, operator||)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(logical_and, operator&&)

#define ARAGELI_ALL_BINARY_CMP_SELF_MIXGATE    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(cmp, cmp)    \
    ARAGELI_BINARY_COMPARISON_SELF_MIXGATE(equal_to, operator==, ==)    \
    ARAGELI_BINARY_COMPARISON_SELF_MIXGATE(not_equal_to, operator!=, !=)    \
    ARAGELI_BINARY_COMPARISON_SELF_MIXGATE(greater, operator>, >)    \
    ARAGELI_BINARY_COMPARISON_SELF_MIXGATE(less, operator<, <)    \
    ARAGELI_BINARY_COMPARISON_SELF_MIXGATE(greater_equal, operator>=, >=)    \
    ARAGELI_BINARY_COMPARISON_SELF_MIXGATE(less_equal, operator<=, <=)

#define ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(NAME)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE(NAME, NAME)

#define ARAGELI_ALL_EACH_ALL_CMP_FUNCTIONS_SELF_MIXGATE    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(each_equal_to)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(each_not_equal_to)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(each_greater)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(each_less)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(each_greater_equal)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(each_less_equal)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(all_equal_to)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(all_not_equal_to)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(all_greater)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(all_less)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(all_greater_equal)    \
    ARAGELI_BINARY_FUNCTION_SELF_MIXGATE_SAMENAME(all_less_equal)


} // namespace Arageli


#if 0
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_MIXCOMP
    #include "mixcomp.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_MIXCOMP
#endif
#endif

#endif  //  #ifndef _ARAGELI_mixcomp_hpp_
