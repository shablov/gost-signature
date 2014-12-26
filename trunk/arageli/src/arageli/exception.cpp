/*****************************************************************************

    exception.cpp -- See declarations in exception.hpp.

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
    \file exception.cpp
    \brief The exception.hpp file stuff implementation.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_exception)

// REFERENCE ADDITIONAL HEADERS HERE


namespace Arageli
{

// PLACE ALL TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


#include "exception.hpp"


namespace Arageli
{


void exception::add_location
(
    const std::string& src,
    std::size_t line,
    const std::string& desc
)
{
    std::ostringstream buf;

    buf
        << "Location:"
        << "\n\tSource: " << src
        << "\n\tLine: " << line;

    if(!desc.empty())
        buf << "\n\tDescription: " << desc;

    add_description(buf.str());
}


std::string exception::msg () const
{
    std::string res = std::string("Arageli exception \"") + typeid(*this).name() + "\"";
    for
    (
        descriptions_type::const_iterator i = descriptions_m.begin();
        i != descriptions_m.end();
        ++i
    )
        (res += "\n") += *i;

    return res;
}


void assert_failed::output (std::ostream& out) const
{
    out
        << "Arageli assertion failed."
        << "\n\tSource: " << source()
        << "\n\tLine: " << line ()
        << "\n\tExpression: " << expr()
        << "\nAdditional information:\n"
        << exception::msg();
}


#ifdef ARAGELI_TRACING

std::ostream& tracing_common = std::cerr;
std::size_t tracing_deep = 0;

#endif


}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
