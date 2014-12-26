/*****************************************************************************

    type_opers.cpp -- File description see in type_opers.hpp.

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
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TYPE_OPERS)

#include "type_opers.hpp"


namespace Arageli
{

template <typename T1, typename T2>
const false_type equal_types<T1, T2>::value = false_type();

template <typename T>
const true_type equal_types<T, T>::value = true_type();

//template <typename T1, typename T2>
//const false_type is_subcategory_of<T1, T2>::value = false_type();

} // namespace Arageli


#else


#include "type_opers.hpp"

namespace Arageli
{

const false_type bool_type<false>::value = false_type();
const true_type bool_type<true>::value = true_type();

}


#endif
