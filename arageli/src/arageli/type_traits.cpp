/*****************************************************************************

    type_traits.cpp -- File description see in type_traits.hpp.

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

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TYPE_TRAITS)

#include "type_traits.hpp"


namespace Arageli
{

template <typename T>
const type_category::type
type_traits_default<T>::category_value = category_type();

template <typename T>
const typename auto_type_category_by_numeric_limits<T>::value_type
type_traits<T>::category_value = category_type();

template <typename T>
const type_category::complex
type_traits<std::complex<T> >::category_value = type_category::complex();

template <typename Iter, typename Val>
const type_category::iterator
type_traits_iterator<Iter, Val>::category_value =
    type_category::iterator();

template <typename Str, typename Char>
const type_category::string
type_traits_string<Str, Char>::category_value =
    type_category::string();

template <typename Str>
const type_category::iostream
type_traits_iostream<Str>::category_value =
    type_category::iostream();


} // namespace Arageli


#endif
