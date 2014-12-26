/*****************************************************************************

    vecalg.cpp -- See vecalh.hpp.

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

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_VECALG)

#include "vecalg.hpp"


namespace Arageli
{


template <typename Out, typename Vec>
Out& vec_output_list
(
    Out& out,
    const Vec& x,
    const char* first_bracket,
    const char* second_bracket,
    const char* separator
)
{
    out << first_bracket;

    if(!x.is_empty())
    {
        out << x[0];

        typedef typename Vec::size_type size_type;
        for(size_type i = 1; i < x.size(); ++i)
            out << separator << x[i];
    }

    out << second_bracket;
    return out;
}


}

#else

#include "vecalg.hpp"


namespace Arageli
{

const char* vector_output_list_first_bracket_default = "(";
const char* vector_output_list_second_bracket_default = ")";
const char* vector_output_list_separator_default = ", ";
const char* vector_input_list_first_bracket_default = "(";
const char* vector_input_list_second_bracket_default = ")";
const char* vector_input_list_separator_default = ",";
const char* vector_input_list_range_default = ":";
const char* vector_output_aligned_left_col_default = "|| ";
const char* vector_output_aligned_right_col_default = " ||";

}


#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
