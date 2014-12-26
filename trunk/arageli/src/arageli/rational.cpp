/*****************************************************************************

    rational.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh

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
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_RATIONAL)

#include "rational.hpp"


namespace Arageli
{


template <typename T>
rational<T>::rational (const char* s)
{
    std::istringstream buf(s);
    buf >> *this;
    if(!buf && !buf.eof())
        throw incorrect_string(s);
}


template <typename T, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_default
(
    std::basic_ostream<Ch, ChT>& s,
    const rational<T>& x,
    const Ch* oblique
)
{
    //ARAGELI_ASSERT_0(x.is_normal());

    if(s.flags() & std::ios_base::showpos)
        s << "+(";   // WARNING! Explicit symbol.

    output_polynom_first(s, x.numerator());
    if(!is_unit(x.denominator()))
    {
        s << oblique;

        if(s.flags() & std::ios_base::showpos)
        {
            s << std::noshowpos;
            output_polynom_first(s, x.denominator());
            s << std::showpos;
        }
        else
            output_polynom_first(s, x.denominator());
    }

    if(s.flags() & std::ios_base::showpos)
        s << ")";   // WARNING! Explicit symbol.

    return s;
}


template <typename T, typename Ch, typename ChT>
std::basic_istream<Ch, ChT>& input_list
(
    std::basic_istream<Ch, ChT>& in,
    rational<T>& x,
    const Ch* first_bracket,
    const Ch* second_bracket,
    const Ch* separator
)
{
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(first_bracket));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(second_bracket));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(separator));

    if(!_Internal::read_literal(in, first_bracket))
    {
        in.clear(std::ios_base::failbit);
        return in;
    }

    T numerator;
    in >> numerator;
    if(!in && !in.eof())
        return in;

    if(!_Internal::read_literal(in, separator))
    {
        in.clear(std::ios_base::failbit);
        return in;
    }

    T denominator;
    in >> denominator;
    if(!in && !in.eof())
        return in;

    if(!_Internal::read_literal(in, second_bracket))
    {
        in.clear(std::ios_base::badbit);
        return in;
    }

    x.numerator() = numerator;
    x.denominator() = denominator;
    x.normalize();

    return in;
}


template <typename T, typename Ch, typename ChT>
std::basic_istream<Ch, ChT>& input_default
(
    std::basic_istream<Ch, ChT>& s,
    rational<T>& x,
    const Ch* oblique
)
{
    T p, q;
    Ch ch = 0;
    bool brackets = false;

    s >> ch;
    if(ch == '(')    // WARNING!!!
        brackets = true;
    else
        s.putback(ch);

    s >> p;

    if(!s && !s.eof())
        return s;    // error

    if(s.eof())
    {
        if(brackets && !_Internal::read_literal(s, ")"))
            s.clear(std::ios_base::badbit);

        x = p;
        return s;
    }

    //ch = s.get();
    if(_Internal::read_literal(s, oblique))
    {
        s >> q;
        if(!s && !s.eof())
            return s;    // error
        x = p;
        x /= q;
    }
    else
    {
        //s.putback(ch);
        x = p;
    }

    s.clear();

    if(brackets && !_Internal::read_literal(s, ")"))    // WARNING!!!
        s.clear(std::ios_base::badbit);

    return s;
}


template <typename T1, typename T2>
rational<T1> operator+
(const rational<T1>& b, const rational<T2>& c)
{
    ARAGELI_ASSERT_0(b.is_normal() && c.is_normal());

    rational<T1> a;
    a.numerator() = b.numerator() * c.denominator() + c.numerator() * b.denominator();
    a.denominator() = b.denominator() * c.denominator();
    a.normalize();
    return a;
}


template <typename T1, typename T2>
rational<T1> operator-
(const rational<T1>& b, const rational<T2>& c)
{
    ARAGELI_ASSERT_0(b.is_normal() && c.is_normal());

    rational<T1> a;
    a.numerator() = b.numerator() * c.denominator() - c.numerator() * b.denominator();
    a.denominator() = b.denominator() * c.denominator();
    a.normalize();
    return a;
}


template <typename T1, typename T2>
rational<T1> operator*
(const rational<T1>& b, const rational<T2>& c)
{
    ARAGELI_ASSERT_0(b.is_normal() && c.is_normal());

    rational<T1> a;

    // WARNING! Solution with type conversion is temporal.

    T1 first_gcd  = gcd(b.numerator(), T1(c.denominator()));
    T1 second_gcd = gcd(T1(c.numerator()), b.denominator());
    a.numerator() = (b.numerator()/first_gcd) * (c.numerator()/second_gcd);
    a.denominator() = (b.denominator()/second_gcd) * (c.denominator()/first_gcd);

    if(is_negative(a.denominator()))
    {
        opposite(&a.numerator());
        opposite(&a.denominator());
    }

    ARAGELI_ASSERT_1(a.is_normal());

    //a.numerator() = b.numerator() * c.numerator();
    //a.denominator() = b.denominator() * c.denominator();
    //a.normalize();

    return a;
}


template <typename T1, typename T2>
rational<T1> operator/
(const rational<T1>& b, const rational<T2>& c)
{
    ARAGELI_ASSERT_0(b.is_normal() && c.is_normal());

    rational<T1> a;


    // WARNING! Solution with type conversion is temporal.

    T1 first_gcd  = gcd(b.numerator(), T1(c.numerator()));
    T1 second_gcd = gcd(T1(c.denominator()), b.denominator());
    a.numerator() = (b.numerator()/first_gcd) * (c.denominator()/second_gcd);
    a.denominator() = (b.denominator()/second_gcd) * (c.numerator()/first_gcd);

    if(is_negative(a.denominator()))
    {
        opposite(&a.numerator());
        opposite(&a.denominator());
    }

    //std::cout << "\np = " << a.numerator() << ", q = " << a.denominator() << std::endl;
    ARAGELI_ASSERT_1(a.is_normal());

    //a.numerator() = b.numerator() * c.denominator();
    //a.denominator() = b.denominator() * c.numerator();
    //a.normalize();

    return a;
}


template <typename T>
rational<T>& rational<T>::inverse ()
{
    ARAGELI_ASSERT_0(!Arageli::is_null(denominator()));
    std::swap(numerator(), denominator());

    if(is_negative(denominator()))
    {
        Arageli::opposite(&numerator());
        Arageli::opposite(&denominator());
    }

    return *this;
}


template <typename T>
int rational<T>::sign () const
{
    ARAGELI_ASSERT_0(is_normal());
    return Arageli::sign(numerator());
}


template <typename T>
void rational<T>::normalize ()
{
    T d = gcd(p, q);
    p /= d;
    q /= d;

    if(is_negative(q))
    {
        Arageli::opposite(&p);
        Arageli::opposite(&q);
    }
}


template <typename T>
bool rational<T>::is_normal () const
{
    return
        is_positive(denominator()) &&
        is_coprime(numerator(), denominator());
}



template <typename T>
T ifloor (const rational<T>& x)
{
    T result = x.numerator() / x.denominator();    // integer devision
    if(is_null(x.numerator() % x.denominator()))
        return result;
    else if(sign(x.numerator()) >= 0)
        return result;
    return result - 1;
}


template <typename T>
T iceil (const rational<T>& x)
{
    T result = x.numerator() / x.denominator();    // integer devision
    if(is_null(x.numerator() % x.denominator()))
        return result;
    else if(is_negative(x.numerator()))
        return result;
    return result + 1;
}



} // namespace Arageli

#else

#include "rational.hpp"

namespace Arageli
{

const char* rational_output_list_first_bracket_default = "(";
const char* rational_output_list_second_bracket_default = ")";
const char* rational_output_list_separator_default = ", ";
const char* rational_input_list_first_bracket_default = "(";
const char* rational_input_list_second_bracket_default = ")";
const char* rational_input_list_separator_default = ",";
const char* rational_output_default_oblique_default = "/";
const char* rational_input_default_oblique_default = "/";

}

#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
