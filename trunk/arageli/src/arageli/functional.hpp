/*****************************************************************************

    functional.hpp

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
    \file functional.hpp
    \brief Representats as structures a lot of standard Arageli functions.
*/


#ifndef _ARAGELI_functional_hpp_
#define _ARAGELI_functional_hpp_

#include "config.hpp"

#include <functional>
#include <vector>    // to make specializations for vector<bool>

#include "basefuncs.hpp"
#include "function_traits.hpp"
#include "factory.hpp"
#include "cmp.hpp"

// WARNING! Need more headers to include.

#include "std_import.hpp"
//#include "prime.hpp"
//#include "powerest.hpp"


namespace Arageli
{

//#define _ARAGELI_RIGHT_ASSIGN(MNEM, OPER)    \
//    template <typename T1, typename T2>    \
//    inline T2& left_assign_##MNEM (const T1& x, T2& y)    \
//    { return y = x OPER y; }
//
///// @name Operation with assignment to the right argument.
//// @{
//
//_ARAGELI_RIGHT_ASSIGN(plus, +);
//_ARAGELI_RIGHT_ASSIGN(minus, -);
//_ARAGELI_RIGHT_ASSIGN(multiplies, *);
//_ARAGELI_RIGHT_ASSIGN(divides, /);
//_ARAGELI_RIGHT_ASSIGN(modulus, %);
//_ARAGELI_RIGHT_ASSIGN(bitwise_or, |);
//_ARAGELI_RIGHT_ASSIGN(bitwise_and, &);
//_ARAGELI_RIGHT_ASSIGN(bitwise_xor, ^);
//_ARAGELI_RIGHT_ASSIGN(shift_left, <<);
//_ARAGELI_RIGHT_ASSIGN(shift_right, >>);
//
//// @}


///// @name Standard operators with one additional argument as result reference.
//// @{
//
//
//
//// @}

// These prototypes are here temporary.
template <typename T>
bool is_prime (const T& x);

//template <typename T>
//bool is_null (const T& x);


/// Arageli functions represented as template classes.
namespace func
{

#define _ARAGELI_FUNC_BUILDER_1(NAME, RES)    \
    template <typename T>    \
    struct NAME :    \
        public std::unary_function<T, RES>    \
    {    \
        RES operator() (const T& x) const    \
        {    \
            return Arageli::NAME(x);    \
        }    \
    \
        RES operator() (T& x) const    \
        {    \
            return Arageli::NAME(x);    \
        }    \
    }

_ARAGELI_FUNC_BUILDER_1(is_null, bool);
_ARAGELI_FUNC_BUILDER_1(is_unit, bool);
_ARAGELI_FUNC_BUILDER_1(is_opposite_unit, bool);
_ARAGELI_FUNC_BUILDER_1(is_negative, bool);
_ARAGELI_FUNC_BUILDER_1(is_positive, bool);
_ARAGELI_FUNC_BUILDER_1(sign, int);
_ARAGELI_FUNC_BUILDER_1(is_prime, bool);
_ARAGELI_FUNC_BUILDER_1(is_even, bool);
_ARAGELI_FUNC_BUILDER_1(is_odd, bool);
// and so on

#undef _ARAGELI_FUNC_BUILDER_1

#define _ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_1(NAME, OPER)    \
    template <typename Arg, typename Res = Arg>    \
    struct NAME :    \
        public std::unary_function<Arg, Res>    \
    {    \
        Res operator() (const Arg& x) const    \
        {    \
            return OPER x;    \
        }    \
    \
        Res operator() (Arg& x) const    \
        {    \
            return OPER x;    \
        }    \
    };

// Ha-ha! I cannot address std::vector<bool>::reference and obtain
// std::vector<bool>::pointer in my STL implementation. (Sergey Lyalin)
// This std::vector<bool> is total ... (not passed by the censor).
// Because that the following definition is so sophisticated.

// The default implementation conflicts with fake member types from vector<bool>.
// So we define this special macro for vector<bool>.

/*

#define _ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_VB(NAME, OPER, ARG, RES)    \
    template <>    \
    struct NAME<std::vector<bool>::ARG, std::vector<bool>::RES> :    \
        public std::unary_function    \
        <    \
            std::vector<bool>::ARG,    \
            std::vector<bool>::RES    \
        >    \
    {    \
        std::vector<bool>::RES operator() (std::vector<bool>::ARG x) const;    \
    };

*/

_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_1(indirection, *);
_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_1(address, &);
_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_1(unary_plus, +);
_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_1(unary_minus, -);
_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_1(logical_not, !);
_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_1(bitwise_not, ~);
_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_1(prefix_increment, ++);
_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_1(prefix_decrement, --);

template <>
struct prefix_increment<bool, bool&> :
    public std::unary_function<bool, bool&>
{
    bool& operator() (bool& x) const
    {
        return x = true;
    }

    std::vector<bool>::reference operator() (std::vector<bool>::reference x) const
    {
        return x = true;
    }
};


template <>
struct prefix_decrement<bool, bool&> :
    public std::unary_function<bool, bool&>
{
    bool& operator() (bool& x) const
    {
        return x = !x;
    }

    std::vector<bool>::reference operator() (std::vector<bool>::reference x) const
    {
        return x = !bool(x);
    }
};


/*

_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_VB
(address, &, reference, pointer)

_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_VB
(address, &, const_reference, const_pointer)

_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_VB
(prefix_increment, ++, reference, reference)

_ARAGELI_UNARY_OPERATOR_FUNC_BUILDER_VB
(prefix_decrement, --, reference, reference)

*/


template <typename Arg, typename Res = Arg>
struct postfix_increment :
    public std::unary_function<Arg, Res>
{
    Res operator() (const Arg& x) const
    {
        return x++;
    }

    Res operator() (Arg& x) const
    {
        return x++;
    }
};

template <typename Arg, typename Res = Arg>
struct postfix_decrement :
    public std::unary_function<Arg, Res>
{
    Res operator() (const Arg& x) const
    {
        return x--;
    }

    Res operator() (Arg& x) const
    {
        return x--;
    }
};


template <>
struct postfix_increment<bool, bool> :
    public std::unary_function<bool, bool>
{
    bool operator() (bool& x) const
    {
        bool t = x;
        x = true;
        return t;
    }

    bool operator() (std::vector<bool>::reference x) const
    {
        bool t = x;
        x = true;
        return t;
    }
};


template <>
struct postfix_decrement<bool, bool> :
    public std::unary_function<bool, bool>
{
    bool operator() (bool& x) const
    {
        bool t = x;
        x = !t;
        return t;
    }

    bool operator() (std::vector<bool>::reference x) const
    {
        bool t = x;
        x = !t;
        return t;
    }
};


#define _ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1(NAME, OPER)    \
    template    \
    <    \
        typename Arg1,    \
        typename Arg2 = Arg1,    \
        typename Res = Arg1    \
    >    \
    struct NAME :    \
        public std::binary_function<Arg1, Arg2, Res>    \
    {    \
        Res operator() (const Arg1& x, const Arg2& y) const    \
        {    \
            return OPER(x, y);    \
        }    \
    \
        Res operator() (const Arg1& x, Arg2& y) const    \
        {    \
            return OPER(x, y);    \
        }    \
    \
        Res operator() (Arg1& x, const Arg2& y) const    \
        {    \
            return OPER(x, y);    \
        }    \
    \
        Res operator() (Arg1& x, Arg2& y) const    \
        {    \
            return OPER(x, y);    \
        }    \
    };

#define _ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(NAME, OPER)    \
    template    \
    <    \
        typename Arg1,    \
        typename Arg2 = Arg1,    \
        typename Res = Arg1    \
    >    \
    struct NAME :    \
        public std::binary_function<Arg1, Arg2, Res>    \
    {    \
        Res operator() (const Arg1& x, const Arg2& y) const    \
        {    \
            return x OPER y;    \
        }    \
    \
        Res operator() (const Arg1& x, Arg2& y) const    \
        {    \
            return x OPER y;    \
        }    \
    \
        Res operator() (Arg1& x, const Arg2& y) const    \
        {    \
            return x OPER y;    \
        }    \
    \
        Res operator() (Arg1& x, Arg2& y) const    \
        {    \
            return x OPER y;    \
        }    \
    };

_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(plus, +);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(minus, -);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(multiplies, *);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(divides, /);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(modulus, %);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(logical_or, ||);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(logical_and, &&);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(equal_to, ==);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(not_equal_to, !=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(greater, >);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(less, <);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(greater_equal, >=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(less_equal, <=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(bitwise_or, |);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(bitwise_and, &);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(bitwise_xor, ^);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(shift_left, <<);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(shift_right, >>);


_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign, =);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign_plus, +=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign_minus, -=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign_multiplies, *=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign_divides, /=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign_modulus, %=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign_bitwise_or, |=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign_bitwise_and, &=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign_bitwise_xor, ^=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign_shift_left, <<=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(assign_shift_right, >>=);


template <typename ARG1, typename ARG2, typename CMP>
class less_by_cmp :
    public std::binary_function<ARG1, ARG2, bool>
{
    CMP cmp_m;

public:

    less_by_cmp (const CMP& cmp = CMP()) :
        cmp_m(cmp)
    {}

    bool operator() (const ARG1& a, const ARG2& b) const
    {
        return cmp_m(a, b) < 0;
    }
};


template <typename CMP>
inline less_by_cmp
<
    typename CMP::first_argument_type,
    typename CMP::second_argument_type,
    CMP
>
make_less_by_cmp (const CMP& cmp)
{
    return less_by_cmp
    <
        typename CMP::first_argument_type,
        typename CMP::second_argument_type,
        CMP
    >
        (cmp);
}

/*
template
<
    typename Arg1,
    typename Arg2 = Arg1,
    typename Res = Arg1
>
struct assign_shift_left :
    public std::binary_function<Arg1, Arg2, Res>
{
    Res operator() (const Arg1& x, const Arg2& y) const
    {
        return operator<<=(x, y);
    }

    Res operator() (const Arg1& x, Arg2& y) const
    {
        return operator<<=(x, y);
    }

    Res operator() (Arg1& x, const Arg2& y) const
    {
        return x <<= y;
    }

    Res operator() (Arg1& x, Arg2& y) const
    {
        return operator<<=(x, y);
    }
};


template
<
    typename Arg1,
    typename Arg2 = Arg1,
    typename Res = Arg1
>
struct assign_shift_right :
    public std::binary_function<Arg1, Arg2, Res>
{
    Res operator() (const Arg1& x, const Arg2& y) const
    {
        return operator>>=(x, y);
    }

    Res operator() (const Arg1& x, Arg2& y) const
    {
        return operator>>=(x, y);
    }

    Res operator() (Arg1& x, const Arg2& y) const
    {
        return operator>>=(x, y);
    }

    Res operator() (Arg1& x, Arg2& y) const
    {
        return operator>>=(x, y);
    }
};
*/

_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(right_assign_plus, +=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(right_assign_minus, -=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(right_assign_multiplies, *=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(right_assign_divides, /=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(right_assign_modulus, %=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(right_assign_bitwise_or, |=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(right_assign_bitwise_and, &=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(right_assign_bitwise_xor, ^=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(right_assign_shift_left, <<=);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_2(right_assign_shift_right, >>=);

_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1
    (left_assign_plus, Arageli::left_assign_plus);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1
    (left_assign_minus, Arageli::left_assign_minus);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1
    (left_assign_multiplies, Arageli::left_assign_multiplies);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1
    (left_assign_divides, Arageli::left_assign_divides);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1
    (left_assign_modulus, Arageli::left_assign_modulus);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1
    (left_assign_bitwise_or, Arageli::left_assign_bitwise_or);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1
    (left_assign_bitwise_and, Arageli::left_assign_bitwise_and);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1
    (left_assign_bitwise_xor, Arageli::left_assign_bitwise_xor);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1
    (left_assign_shift_left, Arageli::left_assign_shift_left);
_ARAGELI_BINARY_OPERATOR_FUNC_BUILDER_1
    (left_assign_shift_right, Arageli::left_assign_shift_right);


template <typename Arg, typename Res = Arg>
struct parentheses_0 :
    public std::unary_function<Arg, Res>
{
    Res operator() (const Arg& x) const
    {
        return x();
    }

    Res operator() (Arg& x) const
    {
        return x();
    }
};

template <typename Arg1, typename Arg2, typename Res = Arg2>
struct parentheses_1 : public std::binary_function<Arg1, Arg2, Res>
{
    Res operator() (const Arg1& x, const Arg2& y) const
    {
        return x(y);
    }

    Res operator() (const Arg1& x, Arg2& y) const
    {
        return x(y);
    }

    Res operator() (Arg1& x, const Arg2& y) const
    {
        return x(y);
    }

    Res operator() (Arg1& x, Arg2& y) const
    {
        return x(y);
    }
};

template <typename Arg1, typename Arg2, typename Res = Arg2>
struct subscript :
    public std::binary_function<Arg1, Arg2, Res>
{
    Res operator() (const Arg1& x, const Arg2& y) const
    {
        return x[y];
    }

    Res operator() (const Arg1& x, Arg2& y) const
    {
        return x[y];
    }

    Res operator() (Arg1& x, const Arg2& y) const
    {
        return x[y];
    }

    Res operator() (Arg1& x, Arg2& y) const
    {
        return x[y];
    }
};

template
<
    typename Func,
    typename Arg1 = typename Func::first_argument_type,
    typename Arg2 = typename Func::second_argument_type,
    typename Res = typename Func::result_type
>
struct swap_args :
    public std::binary_function<Arg1, Arg2, Res>
{
    Res operator() (const Arg1& x, const Arg2& y) const
    {
        return base(y, x);
    }

    Res operator() (const Arg1& x, Arg2& y) const
    {
        return base(y, x);
    }

    Res operator() (Arg1& x, const Arg2& y) const
    {
        return base(y, x);
    }

    Res operator() (Arg1& x, Arg2& y) const
    {
        return base(y, x);
    }

    swap_args (const Func& func_a) :
        base(func_a)
    {}

    Func base;
};

template <typename Func>
inline swap_args<Func> make_swap_args (const Func& func)
{
    return swap_args<Func>(func);
}

}


/// Arageli functions represented as classes with template members.
namespace gfunc
{

#define _ARAGELI_FUNC_BUILDER_1(NAME, RES)    \
    struct NAME    \
    {    \
        template <typename T>    \
        RES operator() (const T& x) const    \
        {    \
            return Arageli::NAME(x);    \
        }    \
    };

#define _ARAGELI_FUNC_BUILDER_2(NAME, RES)    \
    struct NAME    \
    {    \
        template <typename T1, typename T2>    \
        RES operator() (const T1& x, const T2& y) const    \
        {    \
            return Arageli::NAME(x, y);    \
        }    \
    };

_ARAGELI_FUNC_BUILDER_2(cmp, int)

// WARNING! TEMPORARY DEFINITION FOR max AND min
_ARAGELI_FUNC_BUILDER_2(max, T1)
_ARAGELI_FUNC_BUILDER_2(min, T1)


template <typename Tag> struct by_tag;

#define ARAGELI_FUNCTIONAL_PREFIX_NONMODIF_UNARY_GFUNC_BY_TAG(NAME, OPER)    \
    template<>    \
    struct by_tag<function_tag::NAME>    \
    {    \
        template <typename T1>    \
        typename unary_function_traits<function_tag::NAME, T1>::result_type    \
        operator() (const T1& x) const    \
        {    \
            return OPER x;    \
        }    \
    };

ARAGELI_FUNCTIONAL_PREFIX_NONMODIF_UNARY_GFUNC_BY_TAG(indirection, *)
ARAGELI_FUNCTIONAL_PREFIX_NONMODIF_UNARY_GFUNC_BY_TAG(address, &)
ARAGELI_FUNCTIONAL_PREFIX_NONMODIF_UNARY_GFUNC_BY_TAG(unary_plus, +)
ARAGELI_FUNCTIONAL_PREFIX_NONMODIF_UNARY_GFUNC_BY_TAG(unary_minus, -)
ARAGELI_FUNCTIONAL_PREFIX_NONMODIF_UNARY_GFUNC_BY_TAG(logical_not, !)
ARAGELI_FUNCTIONAL_PREFIX_NONMODIF_UNARY_GFUNC_BY_TAG(bitwise_not, ~)


#define ARAGELI_FUNCTIONAL_PREFIX_MODIF_UNARY_GFUNC_BY_TAG(NAME, OPER)    \
    template<>    \
    struct by_tag<function_tag::NAME>    \
    {    \
        template <typename T1>    \
        typename unary_function_traits<function_tag::NAME, T1>::result_type    \
        operator() (T1& x) const    \
        {    \
            return OPER x;    \
        }    \
    };


ARAGELI_FUNCTIONAL_PREFIX_MODIF_UNARY_GFUNC_BY_TAG(prefix_increment, ++)
ARAGELI_FUNCTIONAL_PREFIX_MODIF_UNARY_GFUNC_BY_TAG(prefix_decrement, --)


#define ARAGELI_FUNCTIONAL_POSTFIX_MODIF_UNARY_GFUNC_BY_TAG(NAME, OPER)    \
    template<>    \
    struct by_tag<function_tag::NAME>    \
    {    \
        template <typename T1>    \
        typename unary_function_traits<function_tag::NAME, T1>::result_type    \
        operator() (T1& x) const    \
        {    \
            return x OPER;    \
        }    \
    };


ARAGELI_FUNCTIONAL_POSTFIX_MODIF_UNARY_GFUNC_BY_TAG(postfix_increment, ++)
ARAGELI_FUNCTIONAL_POSTFIX_MODIF_UNARY_GFUNC_BY_TAG(postfix_decrement, --)


#define ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(NAME, OPER)    \
    template<>    \
    struct by_tag<function_tag::NAME>    \
    {    \
        template <typename T1, typename T2>    \
        typename binary_function_traits    \
        <    \
            function_tag::NAME,    \
            const T1&,    \
            const T2&    \
        >::result_type    \
        operator() (const T1& x, const T2& y) const    \
        {    \
            return x OPER y;    \
        }    \
    };

ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(plus, +);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(minus, -);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(multiplies, *);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(divides, /);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(modulus, %);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(logical_or, ||);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(logical_and, &&);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(equal_to, ==);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(not_equal_to, !=);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(greater, >);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(less, <);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(greater_equal, >=);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(less_equal, <=);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(bitwise_or, |);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(bitwise_and, &);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(bitwise_xor, ^);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(shift_left, <<);
ARAGELI_FUNCTIONAL_BINARY_GFUNC_BY_TAG(shift_right, >>);


}

}

#if 0
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_VECTOR
    #include "vector.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_VECTOR
#endif
#endif

#endif  //  #ifndef _ARAGELI_functional_hpp_

