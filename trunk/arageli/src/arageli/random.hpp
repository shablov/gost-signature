/*****************************************************************************

    random.hpp -- file for the random generation Arageli objects.

    Now this file isn't a part of the Arageli library.

    Copyright (C) 2005 Kamaev Andrey

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

#ifndef _ARAGELI_random_hpp_
#define _ARAGELI_random_hpp_

#include <cstdlib>

#include "big_int.hpp"
#include "rational.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "sparse_polynom.hpp"

namespace Arageli
{

    struct rand_config
    {
        enum
        {
            integer_min = -100,
            integer_max = 100,
            degree_max = 10,
            vector_len_max = 10,
            matrix_dim_max = 6
        };
    };

namespace _Internal
{
    template<bool cond, class _then, class _else>
        struct IF {typedef _then ret;};
    template<class _then, class _else>
        struct IF<false,_then,_else> {typedef _else ret;};

    template<typename T> struct IS_TYPE_BASE {enum{ret = false};};

    template<> struct IS_TYPE_BASE<signed char> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<unsigned char> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<signed short> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<unsigned short> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<signed int> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<unsigned int> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<signed long> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<unsigned long> {enum{ret = true};};
    #if defined(ARAGELI_LONG_LONG_SUPPORT)
    template<> struct IS_TYPE_BASE<signed long long> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<unsigned long long> {enum{ret = true};};
    #endif // ARAGELI_LONG_LONG_SUPPORT
    #if defined(ARAGELI_INT64_SUPPORT)
    template<> struct IS_TYPE_BASE<signed __int64> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<unsigned __int64> {enum{ret = true};};
    #endif // ARAGELI_INT64_SUPPORT
    template<> struct IS_TYPE_BASE<float> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<double> {enum{ret = true};};
    template<> struct IS_TYPE_BASE<long double> {enum{ret = true};};

    template<> struct IS_TYPE_BASE<big_int> {enum{ret = true};};


    template<typename T, class Conf>
    struct rand_nolimited
    {
        static T rand(void)
        {
            return T();
        }
    };
    template<typename T, int min, int max>
    struct rand_limited
    {
        static T rand(void)
        {
            return T();
        }
    };
#define RAND (std::rand())
#define SPECIALIZE_RAND(type,gen_alg,lim_gen_alg)    \
    template<class Conf>    \
    struct rand_nolimited<type,Conf>    \
    {    \
        static type rand(void)    \
        {    \
            return (type)(gen_alg);    \
        }    \
    };    \
    template<int min, int max>    \
    struct rand_limited<type,min,max>    \
    {    \
        static type rand(void)    \
        {    \
            return (type)(lim_gen_alg);    \
        }    \
    }

    SPECIALIZE_RAND(signed int,(Conf::integer_min + RAND % (Conf::integer_max - Conf::integer_min)),(min + RAND % (max - min)));
    SPECIALIZE_RAND(unsigned int,(RAND % Conf::integer_max),(min + RAND % (max - min)));
    SPECIALIZE_RAND(signed char,(Conf::integer_min + RAND % (Conf::integer_max - Conf::integer_min)),(min + RAND % (max - min)));
    SPECIALIZE_RAND(unsigned char,(RAND % Conf::integer_max),(min + RAND % (max - min)));
    SPECIALIZE_RAND(signed short,(Conf::integer_min + RAND % (Conf::integer_max - Conf::integer_min)),(min + RAND % (max - min)));
    SPECIALIZE_RAND(unsigned short,(RAND % Conf::integer_max),(min + RAND % (max - min)));
    SPECIALIZE_RAND(signed long,(Conf::integer_min + RAND % (Conf::integer_max - Conf::integer_min)),(min + RAND % (max - min)));
    SPECIALIZE_RAND(unsigned long,(RAND % Conf::integer_max),(min + RAND % (max - min)));
    #if defined(ARAGELI_LONG_LONG_SUPPORT)
    SPECIALIZE_RAND(signed long long,(Conf::integer_min + RAND % (Conf::integer_max - Conf::integer_min)),(min + RAND % (max - min)));
    SPECIALIZE_RAND(unsigned long long,(RAND % Conf::integer_max),(min + RAND % (max - min)));
    #endif // ARAGELI_LONG_LONG_SUPPORT
    #if defined(ARAGELI_INT64_SUPPORT)
    SPECIALIZE_RAND(signed __int64,(Conf::integer_min + RAND % (Conf::integer_max - Conf::integer_min)),(min + RAND % (max - min)));
    SPECIALIZE_RAND(unsigned __int64,(RAND % Conf::integer_max),(min + RAND % (max - min)));
    #endif // ARAGELI_INT64_SUPPORT
    SPECIALIZE_RAND(float,(RAND / (1. + RAND)),(RAND / (1. + RAND)));
    SPECIALIZE_RAND(double,(RAND / (1. + RAND)),(RAND / (1. + RAND)));
    SPECIALIZE_RAND(long double,(RAND / (1. + RAND)),(RAND / (1. + RAND)));

    SPECIALIZE_RAND(big_int,(big_int::random_in_range(big_int(Conf::integer_max - Conf::integer_min)) + Conf::integer_min),
        (big_int::random_in_range(big_int(max - min)) + min));

    template<typename T, class Conf>
    struct rand_nolimited<rational<T>,Conf>
    {
        typedef typename IF<IS_TYPE_BASE<T>::ret,rand_limited<T,Conf::integer_min,Conf::integer_max>,rand_nolimited<T,Conf> >::ret r_type;
        static rational<T> rand(void)
        {
            T den;
            do
            {
                den = r_type::rand();
            }while(is_null(den));
            return rational<T>(r_type::rand(),den);
        }
    };

    template<typename F, typename I, class Conf>
    struct rand_nolimited<monom<F, I>,Conf>
    {
        typedef typename IF<IS_TYPE_BASE<F>::ret,rand_limited<F,Conf::integer_min,Conf::integer_max>,rand_nolimited<F,Conf> >::ret F_type;
        typedef typename IF<IS_TYPE_BASE<I>::ret,rand_limited<I,0,Conf::degree_max>,rand_nolimited<I,Conf> >::ret I_type;
        static monom<F, I> rand(void)
        {
            return monom<F, I>(F_type::rand(),I_type::rand());
        }
    };

    template<typename F, typename I, bool REFCNT, class Conf>
    struct rand_nolimited<sparse_polynom<F, I, REFCNT>,Conf>{
        static sparse_polynom<F, I, REFCNT> rand(void)
        {
            sparse_polynom<F, I, REFCNT> res;
            for (int i = 0; i < Conf::degree_max; ++i)
            {
                monom<F,I> mon = rand_nolimited<monom<F, I>,Conf>::rand();
                bool fl = true;
                for(typename sparse_polynom<F, I, REFCNT>::degree_iterator i = res.degrees_begin(), j = res.degrees_end(); i != j && fl; ++i)
                    if(*i == mon.degree())
                        fl = false;
                if(fl)
                    res += mon;
            }
            return res;
        }
    };

    template<typename T, bool REFCNT, class Conf>
    struct rand_nolimited<matrix<T, REFCNT>,Conf>
    {
        typedef typename IF<IS_TYPE_BASE<T>::ret,rand_limited<T,Conf::integer_min,Conf::integer_max>,rand_nolimited<T,Conf> >::ret r_type;
        static matrix<T, REFCNT> rand(void)
        {
            matrix<T, REFCNT> res
            (
                rand_limited<typename matrix<T, REFCNT>::size_type,1,Conf::matrix_dim_max>::rand(),
                rand_limited<typename matrix<T, REFCNT>::size_type,1,Conf::matrix_dim_max>::rand()
            );

            for (typename matrix<T, REFCNT>::size_type i = 0; i < res.nrows(); i++)
                for (typename matrix<T, REFCNT>::size_type j = 0; j < res.ncols(); j++)
                    res.el(i,j) = r_type::rand();
            return res;
        }
    };

    template<typename T, bool REFCNT, class Conf>
    struct rand_nolimited<vector<T, REFCNT>,Conf>
    {
        typedef typename IF<IS_TYPE_BASE<T>::ret,rand_limited<T,Conf::integer_min,Conf::integer_max>,rand_nolimited<T,Conf> >::ret r_type;
        static vector<T, REFCNT> rand(void)
        {
            vector<T, REFCNT> res(rand_limited<typename vector<T, REFCNT>::size_type,1,Conf::vector_len_max>::rand());
            for (typename vector<T, REFCNT>::size_type i = 0; i < res.size(); i++)
                    res.el(i) = r_type::rand();
            return res;
        }
    };

}//_Internal

template<typename T, class Conf = rand_config>
struct oldrnd
{
    static T rand()
    {
        return _Internal::rand_nolimited<T,Conf>::rand();
    }
};

}


#endif //_ARAGELI_random_hpp_
