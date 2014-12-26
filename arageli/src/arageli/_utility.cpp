/*****************************************************************************

    _utility.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2005--2010 Sergey S. Lyalin

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
    \file _utility.cpp
    \brief The _utility.hpp file stuff implementation
        (an internal file of the library).
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE__UTILITY)

#include "_utility.hpp"

namespace Arageli
{
namespace _Internal
{

template <typename A, typename B, typename Store_A, typename Store_B>
void swap_help_1
(
    A& a,
    B& b,
    Store_A& sa,
    Store_B& sb,
    false_type
)
{
    A t;
    a.swap(t);
    a = b;
    b = t;
}


template <typename In_a, typename In_b, typename Cmp>
int aggregate_cmp (In_a ai, In_a aend, In_b bi, In_b bend, Cmp cmp)
{
    for(; ai != aend && bi != bend; ++ai, ++bi)
    {
        int lres = cmp(*ai, *bi);
        if(lres < 0)
            return -1;
        else if(lres > 0)
            return +1;
    }

    if(ai == aend)
        if(bi == bend)
            return 0;
        else
            return -1;
    else
    {
        ARAGELI_ASSERT_1(bi == bend);
        return +1;
    }
}

}
}


#else


#include <cctype>
#include "_utility.hpp"


namespace Arageli
{
namespace _Internal
{


bool is_not_contains_spaces (const char* s)
{
    while(*s)
        if(std::isspace(*s++))
            return false;
    return true;
}


bool read_literal (std::istream& in, const char* s)
{
    const char* s_beg = s;
    if(!*s)
        return true;
    if(!in)
        return false;

    char ch = 0;
    do
    {
        ch = in.get();
    }while(in && std::isspace(ch) && ch != '\n');

    if(!in)
        return false;
    if(*s != ch)
    {
        in.putback(ch);
        return false;
    }

    auto_stream_state ass(in, in.flags() & ~std::istream::skipws);

    while(*++s)
    {
        ch = in.get();
        if(!in)
        {
            in.clear();
            do
            {
                in.putback(*--s);
            }while(s != s_beg);
            return false;
        }
        if(*s != ch)
        {
            in.clear();
            in.putback(*s);
            do
            {
                in.putback(*--s);
            }while(s != s_beg);
            return false;
        }
    }

    return true;
}

}
}

#endif
