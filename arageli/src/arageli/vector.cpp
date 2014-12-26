/*****************************************************************************

    vector.cpp -- See description in file vector.hpp.

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
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_VECTOR)

#include <sstream>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <numeric>

#include "vector.hpp"
#include "_utility.hpp"


namespace Arageli
{


template <typename T, bool REFCNT>
void vector<T, REFCNT>::assign_fromstr (const char* s)
{
    std::istringstream buf(s);
    // WARNING. It is valid if there are no virtual function.
    static_cast<std::istream&>(buf) >> *this;
    if(!buf && !buf.eof())
        throw incorrect_string(s);
}


template <typename T, bool REFCNT>
bool vector<T, REFCNT>::is_null () const
{
    for(size_type i = 0; i < size(); ++i)
        if(!Arageli::is_null(rep(i)))
            return false;
    return true;
}


template <typename T, bool REFCNT>
bool vector<T, REFCNT>::is_unit () const
{
    if(is_empty())
        return false;
    for(size_type i = 0; i < size(); ++i)
        if(!Arageli::is_unit(rep(i)))
            return false;
    return true;
}


template <typename T, bool REFCNT>
bool vector<T, REFCNT>::is_opposite_unit () const
{
    if(is_empty())
        return false;
    for(size_type i = 0; i < size(); ++i)
        if(!Arageli::is_opposite_unit(rep(i)))
            return false;
    return true;
}


template <typename T, bool REFCNT>
vector<T, REFCNT>& vector<T, REFCNT>::inverse ()
{
    unique();
    for(size_type i = 0; i < size(); ++i)
        Arageli::inverse(&rep(i));
    return *this;
}


template <typename T, bool REFCNT>
vector<T, REFCNT>& vector<T, REFCNT>::bitwise_not ()
{
    unique();
    for(size_type i = 0; i < size(); ++i)
        Arageli::assign_bitwise_not(rep(i));
    return *this;
}

template <typename T, bool REFCNT>
vector<T, REFCNT>& vector<T, REFCNT>::logical_not ()
{
    unique();
    for(size_type i = 0; i < size(); ++i)
        Arageli::assign_logical_not(rep(i));
    return *this;
}

template <typename T1, bool REFCNT1>
vector<T1, REFCNT1>& vector<T1, REFCNT1>::opposite ()
{
    if(is_empty())
        return *this;
    unique();

    Rep& repr1 = rep();
    for(size_type i = 0; i < size(); ++i)
        Arageli::opposite(&repr1[i]);

    return *this;
}


template <typename T, bool REFCNT>
void vector<T, REFCNT>::do_pack ()
{
    ARAGELI_ASSERT_1(size() < capacity());

    unique();
    Rep trep = rep();
    trep.swap(rep());
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4018)
#endif

template <typename T, bool REFCNT>
template <typename SV, typename V>
V& vector<T, REFCNT>::copy_subvector (const SV& sv, V& res) const
{
    size_type sz = sv.size();
    res.resize(sz);
    const_iterator thisvec = begin();
    typename V::iterator iv = res.begin();

    for
    (
        typename SV::const_iterator i = sv.begin(), svend = sv.end();
        i != svend;
        ++i, ++iv
    )
    {
        ARAGELI_ASSERT_0(*i >= 0 && *i < size());
        *iv = thisvec[*i];
    }

    return res;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

template <typename T, bool REFCNT>
template <typename SV>
void vector<T, REFCNT>::erase_subvector (const SV& sv)
{
    // WARNING! Slow implementation!!!
    // O(sv.size() * log(sv.size()) + this->size())

    //std::cout << "\n*** sv = " << sv << " ***\n";

    if(sv.is_empty())return;

    SV svt = sv;    // copy because need sorting and uniquelization
    typedef typename SV::iterator SViter;
    std::sort(svt.begin(), svt.end());

    SViter
        svti = svt.begin(),
        svtend = std::unique(svti, svt.end());

    size_type curerased = *svti;

    //if(*(svtend-1) >= size())
    //{
    //    std::cout << "\nsv = " << sv << ", *(svtend-1) = " << *(svtend-1) << ", size() = " << size();
    //}

    ARAGELI_ASSERT_0(*(svtend-1) < size());
    ARAGELI_ASSERT_0(curerased < size());

    iterator
        thisdst = begin() + curerased,
        thissrc = thisdst + 1,
        thisend = end();

    for(;;)
    {
        ++svti;

        if(svti != svtend && *svti == curerased+1)
        {
            ++curerased;
            ++svti;
            ARAGELI_ASSERT_0(thissrc != thisend);
            ++thissrc;
        }

        ARAGELI_ASSERT_1(thissrc != thisend || svti == svtend);

        iterator tend =        // end of current copied part of vector
            svti == svtend ?
            thisend :
            (thissrc + (*svti - curerased - 1));

        thisdst = std::copy(thissrc, tend, thisdst);

        if(tend == thisend)
            break;

        thissrc = tend + 1;
        ARAGELI_ASSERT_1(svti != svtend);
        curerased = *svti;
    }

    ARAGELI_ASSERT_1(svti == svtend);
    ARAGELI_ASSERT_1(thisdst + (svtend - svt.begin()) == thisend);
    erase(thisdst, thisend);
}




template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T1_factory
>
T1 dotprod
(
    const vector<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b,
    const T1_factory& t1fctr
)
{
    ARAGELI_ASSERT_0(a.size() == b.size());

    T1 res = a.is_empty() ? t1fctr.null() : t1fctr.null(a[0]);
    typename vector<T1, REFCNT1>::size_type size = a.size();

    for(std::size_t i = 0; i < size; ++i)
        res += a[i]*b[i];

    return res;
}


template <typename T, bool REFCNT, typename T2, typename T2_factory>
T2& product (const vector<T, REFCNT>& x, T2& res, const T2_factory& t2fctr)
{
    if(x.is_empty())
        return res = t2fctr.unit();
    res = x[0];
    for(std::size_t i = 1; i < x.size(); ++i)
        res *= x[i];
    return res;
}


template <typename T, bool REFCNT, typename T2, typename T2_factory>
T2& sum (const vector<T, REFCNT>& x, T2& res, const T2_factory& t2fctr)
{
    if(x.is_empty())
        return res = t2fctr.null();
    res = x[0];
    for(std::size_t i = 1; i < x.size(); ++i)
        res += x[i];
    return res;
}


template <typename In, typename T, bool REFCNT>
In& input_list
(
    In& in,
    vector<T, REFCNT>& x,
    const char* first_bracket,
    const char* second_bracket,
    const char* separator,
    const char* range
)
{
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(first_bracket));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(second_bracket));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(separator));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(range));

    if(_Internal::is_bad_read_literal(in, first_bracket))
        return in;

    // (second_bracket == "") is special case
    // in this case vector must be terminated by the some invalid character

    if(*second_bracket && _Internal::read_literal(in, second_bracket))
    {
        // empty vector
        x.resize(0);
        return in;
    }

    std::list<T> buf;    // a temporary buffer for values

    T t;

    _Internal::turn_off_comma_as_separator<In> _tocas(in);
    if
    (
        separator && separator[0] == ',' ||    // WARNING! Explicit ','
        second_bracket && second_bracket[0] == ','    // WARNING! Explicit ','
    )
        _tocas.activate();

    for(;;)
    {
        in >> t;

        if(_Internal::is_bad_input(in))
            return in;
        else if(_Internal::read_literal(in, separator))
        { // "t,"
            buf.push_back(t);
            continue;
        }
        else if(_Internal::read_literal(in, range))
        { // "t:"
            T t1;
            in >> t1;

            if(_Internal::is_bad_input(in))
                return in;
            else if(_Internal::read_literal(in, separator))
            { // "t:t1,"
                generate_range_helper(t, t1, std::back_inserter(buf));
                continue;
            }
            else if(_Internal::read_literal(in, range))
            { // "t:t1:"
                T t2;
                in >> t2;

                if(_Internal::is_bad_input(in))
                    return in;
                else if(_Internal::read_literal(in, separator))
                { // "t:t1:t2,"
                    generate_range_helper(t, t1, t2, std::back_inserter(buf));
                    continue;
                }
                else if
                (
                    *second_bracket == 0 ||
                    _Internal::read_literal(in, second_bracket)
                )
                { // "t:t1:t2)"
                    generate_range_helper(t, t1, t2, std::back_inserter(buf));
                    break;
                }
            }
            else if(*second_bracket == 0 || _Internal::read_literal(in, second_bracket))
            { // "t:t1)"
                generate_range_helper(t, t1, std::back_inserter(buf));
                break;
            }
        }
        else if(*second_bracket == 0 || _Internal::read_literal(in, second_bracket))
        { // "t)"
            buf.push_back(t);
            break;
        }

        in.clear(std::ios_base::badbit);
        return in;
    }

    // WARNING! Inefficient!
    //std::copy(buf.begin(), buf.end(), std::ostream_iterator<T>(std::cerr, ", "));
    x.resize(buf.size());
    std::copy(buf.begin(), buf.end(), x.begin());
    return in;
}


template <typename T, bool REFCNT>
std::ostream& output_aligned
(
    std::ostream& out,
    const vector<T, REFCNT>& x,
    const char* left_col,
    const char* right_col
)
{
    // WARNING! Some text parameters are computet many times
    // but only once is necessary.

    if(x.is_empty())
    {
        out << left_col << right_col << '\n';
        return out;
    }

    // The buffer with text representations of the corresponding elements
    // of the vector `x'.
    std::vector<std::string> buf(x.size());

    // The heights and widths in symbols of the text representations.
    std::vector<std::size_t> hs(x.size()), ws(x.size());

    // The maximum width and the maximum height (will be computed).
    std::size_t maxw = 0, maxh = 0;


    // Step 1. Preparation.
    // Get the text representation for each element of the vector,
    // precomute its width and height, compute the maximum ones.

    for(std::size_t i = 0; i < x.size(); ++i)
    {
        std::ostringstream strbuf;
        strbuf.copyfmt(out);

        // the text representation is aligned too
        output_aligned(static_cast<std::ostream&>(strbuf), x[i]);

        buf[i] = strbuf.str();
        std::string& txt = buf[i];

        // Delete terminator '\n' if exists.
        if(!txt.empty() && txt[txt.length() - 1] == '\n')
            txt.erase(txt.length() - 1);

        ws[i] = _Internal::text_width(txt);
        maxw = std::max(maxw, ws[i]);
        hs[i] = _Internal::text_hight(txt);
        maxh = std::max(maxh, hs[i]);
    }

    const std::string blankline =
        left_col + std::string(maxw, ' ') + right_col + '\n';


    // Step 2. Output.
    // Go over all elements of `x' and print them doing required vertical
    // and horizontal spacing.

    for(std::size_t i = 0; i < x.size(); ++i)
    {
        // Additional vertical space in 1 line between elements
        // if the height of at least one line is greater than 1.
        if(i != 0 && maxh > 1)
            out << blankline;

        const std::size_t leftmargin = (maxw - ws[i])/2;

        const std::string leftmarginline =
            left_col + std::string(leftmargin, ' ');

        out << leftmarginline;

        // current horizontal offset from the begin of the text block
        std::size_t cur = 0;

        for(std::size_t j = 0; j < buf[i].length(); ++j)
            if(buf[i][j] == '\n')
            {
                // the end of the current line and the begin of the new one
                out
                    << std::string(maxw - (leftmargin + cur), ' ')
                    << right_col << '\n'
                    << leftmarginline;
                cur = 0;
            }
            else
            {
                out << buf[i][j];
                ++cur;
            }

        out << std::string(maxw - (leftmargin + cur), ' ') << right_col << '\n';
    }

    return out;
}


template <typename T, bool REFCNT>
std::istream& input_polynom_internal
(std::istream& in, vector<T, REFCNT>& x)
{
    char ch = 0;
    in >> ch;
    if(!in && !in.eof() || ch != '+' && ch != '-')
        return in;
    vector<T, REFCNT> res;
    in >> res;
    if(!in && !in.eof())
        return in;
    if(ch == '-')
        res.opposite();
    x = res;
    return in;
}


template <typename T, bool REFCNT>
template <typename Stream>
Stream& io_binary<vector<T, REFCNT> >::input_stream (Stream& in, vector<T, REFCNT>& x)
{
    typename vector<T, REFCNT>::size_type size;
    if(!input_binary_stream(in, size))
        return in;  // fail without losing an old value
    x.resize(size);

    // this can lose an old value without new one have been loaded successfully
    if(size)
        input_binary_stream(in, &*x.begin(), size);

    return in;
}


#define ARAGELI_VECTOR_MATH_FUNCS1_IMPL(NAME)    \
    template <typename T, bool REFCNT>    \
    vector<T, true> NAME    \
    (const vector<T, REFCNT>& x)    \
    {    \
        std::size_t size = x.size();    \
        vector<T, true> res(size);    \
    \
        typename vector<T, REFCNT>::const_iterator    \
            xbeg = x.begin();    \
        typename vector<T, true>::iterator    \
            resbeg = res.begin();    \
    \
        for(std::size_t i = 0; i < size; ++i)    \
            resbeg[i] = NAME(xbeg[i]);    \
    \
        return res;    \
    }


ARAGELI_VECTOR_MATH_FUNCS1_IMPL(sin)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(cos)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(tan)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(sinh)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(cosh)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(tanh)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(asin)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(acos)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(atan)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(abs)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(exp)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(floor)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(ceil)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(log)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(log10)
ARAGELI_VECTOR_MATH_FUNCS1_IMPL(sqrt)


#undef ARAGELI_VECTOR_MATH_FUNCS1_IMPL


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
vector<T1, REFCNT1> pow
(
    const vector<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b
)
{
    ARAGELI_ASSERT_0(a.size() == b.size());
    std::size_t size = a.size();
    vector<T1, REFCNT1> res(size);
    for(std::size_t i = 0; i < size; ++i)
        res[i] = pow(a[i], b[i]);

    return res;
}


template
<
    typename T,
    bool REFCNT,
    typename P
>
vector<T, REFCNT> pow
(const vector<T, REFCNT>& a, const P& b)
{
    std::size_t size = a.size();
    vector<T, REFCNT> res(size);
    for(std::size_t i = 0; i < size; ++i)
        res[i] = pow(a[i], b);
}

} // namespace Arageli


#if ARAGELI_DEBUG_LEVEL > 3

#include "frwrddecl.hpp"
#include "big_int.hpp"
#include "rational.hpp"

namespace Arageli
{

template class vector<int, false>;
template class vector<int, true>;
template class vector<unsigned int, false>;
template class vector<unsigned int, true>;
template class vector<bool, false>;
template class vector<bool, true>;
template class vector<char, false>;
template class vector<char, true>;
template class vector<big_int, false>;
template class vector<big_int, true>;
template class vector<rational<>, false>;
template class vector<rational<>, true>;

} // namespace Arageli

#endif // #if ARAGELI_DEBUG_LEVEL > 3


#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
