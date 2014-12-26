/*****************************************************************************

    io.cpp -- See description in file io.hpp.

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
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_IO)

#include <limits>
#include <iomanip>
#include <algorithm>

#include "io.hpp"
#include "exception.hpp"
#include "cmp.hpp"


namespace Arageli
{


template <typename T, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_polynom_internal_default
(std::basic_ostream<Ch, ChT>& out, const T& x)
{
    std::ios_base::fmtflags old_opts = out.flags();

    try
    {
        out << std::showpos << x;
    }
    catch(...)
    {
        out.flags(old_opts);
        throw;
    }

    out.flags(old_opts);
    return out;
}


template <typename T, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_pow_default
(std::basic_ostream<Ch, ChT>& out, const T& x)
{
    // ПРЕДУПРЕЖДЕНИЕ. Только для тех T, для которых определён is_negative.

    if(is_negative(x))
        out << "(" << x << ")";
    else
        out << x;
    return out;
}


template <typename T, typename Ch, typename ChT>
std::basic_istream<Ch, ChT>& input_polynom_internal_default
(std::basic_istream<Ch, ChT>& in, T& x)
{
    char ch;
    in >> ch;
    if(!in)
        return in;

    // WARNING. The following implementation is restricted.

    bool minus = false;

    if(ch == '-')
    {// aligne minus sign to the next non-space characters
        char ch1;
        in >> ch1;    // pass spaces and read next character
        if(!in)
            return in;
        in.putback(ch1);

        if(ch1 == '(')
            minus = true;
        else
            in.putback(ch);
    }
    else if(ch != '+')
    {
        in.putback(ch);
        in.clear(std::ios_base::failbit);
    }

    T res;
    in >> res;
    if(!in && !in.eof())
        return in;
    if(minus)
        opposite(&res);

    x = res;
    return in;
}


template <typename T, typename Ch, typename ChT>
std::basic_istream<Ch, ChT>& input_polynom_first_default
(std::basic_istream<Ch, ChT>& in, T& x)
{
    char ch;
    in >> ch;
    if(!in)
        return in;

    // WARNING. The following implementation is restricted.

    bool minus = false;

    if(ch == '-')
    {// aligne minus sign to the next non-space characters
        char ch1;
        in >> ch1;    // pass spaces and read next character
        if(!in)
            return in;
        in.putback(ch1);

        if(ch1 == '(')
            minus = true;
        else
            in.putback(ch);
    }
    else if(ch != '+')
        in.putback(ch);

    T res;
    in >> res;
    if(!in && !in.eof())
        return in;
    if(minus)
        opposite(&res);

    x = res;
    return in;
}


template <typename T, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_polynom_internal_default
(std::basic_ostream<Ch, ChT>& out, const std::complex<T>& x)
{
    std::ios_base::fmtflags old_opts = out.flags();

    try
    {
        out << std::noshowpos << '+' << x;
    }
    catch(...)
    {
        out.flags(old_opts);
        throw;
    }

    out.flags(old_opts);
    return out;
}


template <typename T>
template <typename Stream>
Stream& io_binary_base<T>::output_stream
(
    Stream& out,
    const T* x,
    std::size_t n
)
{
    ARAGELI_ASSERT_1(std::numeric_limits<std::size_t>::max()/sizeof(T) >= n);
    for(std::size_t i = 0; i < n; ++i)
        output_binary_stream(out, x[i]);
    return out;
}


template <typename T>
template <typename Stream>
Stream& io_binary_base<T>::input_stream
(
    Stream& in,
    T* x,
    std::size_t n
)
{
    ARAGELI_ASSERT_1(std::numeric_limits<std::size_t>::max()/sizeof(T) >= n);
    for(std::size_t i = 0; i < n; ++i)
        input_binary_stream(in, x[i]);
    return in;
}


template <typename T>
std::size_t io_binary_base<T>::calc (const T* x, std::size_t n)
{
    ARAGELI_DEBUG_EXEC_1
    (
        const std::size_t resmax = std::numeric_limits<std::size_t>::max()
    );
    ARAGELI_ASSERT_1(resmax/sizeof(T) >= n);

    std::size_t res = 0;

    for(std::size_t i = 0; i < n; ++i)
    {
        std::size_t i_calc = calc_binary(x[i]);
        ARAGELI_ASSERT_1(res <= resmax - i_calc);
        res += i_calc;
    }
    return res;
}


template <typename T>
char* io_binary_base<T>::output_mem
(
    char* out,
    const T* x,
    std::size_t n
)
{
    ARAGELI_ASSERT_1(std::numeric_limits<std::size_t>::max()/sizeof(T) >= n);
    for(std::size_t i = 0; i < n; ++i)
        out = output_binary_mem(out, x[i]);
    return out;
}


template <typename T>
const char* io_binary_base<T>::input_mem
(
    const char* in,
    T* x,
    std::size_t n
)
{
    ARAGELI_ASSERT_1(std::numeric_limits<std::size_t>::max()/sizeof(T) >= n);
    for(std::size_t i = 0; i < n; ++i)
        in = input_binary_mem(in, x[i]);
    return in;
}


} // namespace Arageli


#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
