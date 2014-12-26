/*****************************************************************************

    basefuncs.hpp

    This file is a part of the Arageli library.

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
    \file basefuncs.hpp
    \brief Additional fuctions that represent each basic operation.

    Additional fuctions that represent each basic operation,
    like addition and multiplication, as function that get the result
    through one of parameter. And other functionality.
*/


#ifndef _ARAGELI_basefunc_hpp_
#define _ARAGELI_basefunc_hpp_

#include "config.hpp"

namespace Arageli
{

// The following functions are defined efficiently only for builtin types.

// Unary functions that don't alternate their argument.

#define ARAGELI_BASEFUNC_PREFIX_UNARY(NAME, OPER)    \
    template <typename T, typename Res>    \
    inline Res& NAME (const T& x, Res& res)    \
    {    \
        return res = OPER x;    \
    }


ARAGELI_BASEFUNC_PREFIX_UNARY(indirection, *)
ARAGELI_BASEFUNC_PREFIX_UNARY(address, &)
ARAGELI_BASEFUNC_PREFIX_UNARY(unary_plus, +)
ARAGELI_BASEFUNC_PREFIX_UNARY(unary_minus, -)
ARAGELI_BASEFUNC_PREFIX_UNARY(logical_not, !)
ARAGELI_BASEFUNC_PREFIX_UNARY(bitwise_not, ~)

// Unary functions that alternate their argument.

template <typename T, typename Res>
inline Res& prefix_increment (T& x, Res& res)
{
    res = ++x;
}

template <typename T, typename Res>
inline Res& prefix_decrement (T& x, Res& res)
{
    res = --x;
}

template <typename T, typename Res>
inline Res& postfix_increment (T& x, Res& res)
{
    res = x++;
}

template <typename T, typename Res>
inline Res& postfix_decrement (T& x, Res& res)
{
    res = x--;
}

// Binary functions that don't alternate their arguments.

#define ARAGELI_BASEFUNC_BINARY(NAME, OPER)    \
    template <typename T1, typename T2, typename Res>    \
    inline Res& NAME (const T1& x, const T2& y, Res& res)    \
    {    \
        return res = x OPER y;    \
    }

ARAGELI_BASEFUNC_BINARY(plus, +);
ARAGELI_BASEFUNC_BINARY(minus, -);
ARAGELI_BASEFUNC_BINARY(multiplies, *);
ARAGELI_BASEFUNC_BINARY(divides, /);
ARAGELI_BASEFUNC_BINARY(modulus, %);
ARAGELI_BASEFUNC_BINARY(logical_or, ||);
ARAGELI_BASEFUNC_BINARY(logical_and, &&);
ARAGELI_BASEFUNC_BINARY(equal_to, ==);
ARAGELI_BASEFUNC_BINARY(not_equal_to, !=);
ARAGELI_BASEFUNC_BINARY(greater, >);
ARAGELI_BASEFUNC_BINARY(less, <);
ARAGELI_BASEFUNC_BINARY(greater_equal, >=);
ARAGELI_BASEFUNC_BINARY(less_equal, <=);
ARAGELI_BASEFUNC_BINARY(bitwise_or, |);
ARAGELI_BASEFUNC_BINARY(bitwise_and, &);
ARAGELI_BASEFUNC_BINARY(bitwise_xor, ^);
ARAGELI_BASEFUNC_BINARY(shift_left, <<);
ARAGELI_BASEFUNC_BINARY(shift_right, >>);


// Binary functions that alternate their arguments.

#define ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(NAME, OPER)    \
    template <typename T1, typename T2>    \
    inline T1& right_assign_##NAME (T1& x, const T2& y)    \
    {    \
        return x OPER y;    \
    }    \
    \
    template <typename T1, typename T2>    \
    inline T1& assign_##NAME (T1& x, const T2& y)    \
    {    \
        return left_assign(x, y);    \
    }


ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(plus, +=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(minus, -=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(multiplies, *=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(divides, /=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(modulus, %=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(bitwise_or, |=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(bitwise_and, &=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(bitwise_xor, ^=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(shift_left, <<=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NATIVE(shift_right, >>=);


#define ARAGELI_BASEFUNC_RIGHT_ASSIGN_NOTNATIVE(NAME, OPER)    \
    template <typename T1, typename T2>    \
    inline T1& right_assign_##NAME (T1& x, const T2& y)    \
    {    \
        return x = x OPER y;    \
    }    \
    \
    template <typename T1, typename T2>    \
    inline T1& assign_##NAME (T1& x, const T2& y)    \
    {    \
        return left_assign(x, y);    \
    }

ARAGELI_BASEFUNC_RIGHT_ASSIGN_NOTNATIVE(logical_or, ||);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NOTNATIVE(logical_and, &&);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NOTNATIVE(equal_to, ==);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NOTNATIVE(not_equal_to, !=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NOTNATIVE(greater, >);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NOTNATIVE(less, <);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NOTNATIVE(greater_equal, >=);
ARAGELI_BASEFUNC_RIGHT_ASSIGN_NOTNATIVE(less_equal, <=);


#define ARAGELI_BASEFUNC_LEFT_ASSIGN(NAME, OPER)    \
    template <typename T1, typename T2>    \
    inline T1& left_assign_##NAME (const T1& x, T2& y)    \
    {    \
        return y = x OPER y;    \
    }

ARAGELI_BASEFUNC_LEFT_ASSIGN(plus, +);
ARAGELI_BASEFUNC_LEFT_ASSIGN(minus, -);
ARAGELI_BASEFUNC_LEFT_ASSIGN(multiplies, *);
ARAGELI_BASEFUNC_LEFT_ASSIGN(divides, /);
ARAGELI_BASEFUNC_LEFT_ASSIGN(modulus, %);
ARAGELI_BASEFUNC_LEFT_ASSIGN(logical_or, ||);
ARAGELI_BASEFUNC_LEFT_ASSIGN(logical_and, &&);
ARAGELI_BASEFUNC_LEFT_ASSIGN(equal_to, ==);
ARAGELI_BASEFUNC_LEFT_ASSIGN(not_equal_to, !=);
ARAGELI_BASEFUNC_LEFT_ASSIGN(greater, >);
ARAGELI_BASEFUNC_LEFT_ASSIGN(less, <);
ARAGELI_BASEFUNC_LEFT_ASSIGN(greater_equal, >=);
ARAGELI_BASEFUNC_LEFT_ASSIGN(less_equal, <=);
ARAGELI_BASEFUNC_LEFT_ASSIGN(bitwise_or, |);
ARAGELI_BASEFUNC_LEFT_ASSIGN(bitwise_and, &);
ARAGELI_BASEFUNC_LEFT_ASSIGN(bitwise_xor, ^);
ARAGELI_BASEFUNC_LEFT_ASSIGN(shift_left, <<);
ARAGELI_BASEFUNC_LEFT_ASSIGN(shift_right, >>);

#define ARAGELI_BASEFUNC_UNARY_ASSIGN(NAME, OPER)    \
    template <typename T>    \
    inline T& assign_##NAME (T& x)    \
    {    \
        return x = OPER x;    \
    }

ARAGELI_BASEFUNC_UNARY_ASSIGN(indirection, *)
ARAGELI_BASEFUNC_UNARY_ASSIGN(address, &)
ARAGELI_BASEFUNC_UNARY_ASSIGN(unary_plus, +)
ARAGELI_BASEFUNC_UNARY_ASSIGN(unary_minus, -)
ARAGELI_BASEFUNC_UNARY_ASSIGN(logical_not, !)
ARAGELI_BASEFUNC_UNARY_ASSIGN(bitwise_not, ~)

}


//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_BASEFUNC
//    #include "basefunc.cpp"
//    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_BASEFUNC
//#endif


#endif  //  #ifndef _ARAGELI_basefunc_hpp_
