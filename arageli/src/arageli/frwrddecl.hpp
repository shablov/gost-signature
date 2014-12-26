/*****************************************************************************

    frwrddecl.hpp

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

/** \file frwrddecl.hpp
    \brief Forward declarations for a lot of Arageli template classes.

    Here we can find declarations with the default parameters for many
    template classes of the library. See full definitions of the classes
    in the corresponding files.
*/


#ifndef _ARAGELI_frwrddecl_hpp_
#define _ARAGELI_frwrddecl_hpp_

#include "config.hpp"


namespace Arageli
{

class big_int;

template <typename T>
class rational;

template <typename T, bool REFCNT = true>
class vector;

template <typename T, bool REFCNT = true>
class matrix;

template <typename F, typename I = int>
class monom;

template <typename F, typename I = int, bool REFCNT = true>
class sparse_polynom;


template <typename T, typename M>
class cone_default_config;

template
<
    typename T = big_int,
    typename M = matrix<T>,
    typename CFG = cone_default_config<T, M>
>
class cone;


template
<
    typename Base,
    typename Index = vector<typename Base::size_type, true>
>
class indexed_subvector;


template
<
    typename Base,
    typename RowIndex = vector<typename Base::size_type, true>,
    typename ColIndex = RowIndex
>
class indexed_submatrix;


template <typename F, bool REFCNT = true>
struct polynom_config_default;

template <typename F, typename Config = polynom_config_default<F> >
class polynom;


template
<
    typename F,
    typename D = int,
    typename MD = vector<D>
>
class multimonom;


template
<
    typename F,
    typename D = int,
    typename MD = vector<D>,
    bool REFCNT = true
>
struct sparse_multipolynom_config_default;


template
<
    typename F,
    typename D = int,
    typename MD = vector<D>,
    typename Config = sparse_multipolynom_config_default<F, D, MD>
>
class sparse_multipolynom;


template <typename T, typename T_factory>
T euclid (T a, T b, const T_factory& tfctr);


template <typename T>
T euclid (const T& a, const T& b);


} // namespace Arageli


#endif  //  #ifndef _ARAGELI_frwrddecl_hpp_
