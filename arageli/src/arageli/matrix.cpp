/*****************************************************************************

    matrix.cpp -- See description in file matrix.hpp.

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
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_MATRIX)

#include <cstddef>
#include <list>
#include <sstream>

#include "matrix.hpp"
#include "vector.hpp"
//#include "gauss.hpp"
#include "_utility.hpp"


namespace Arageli
{

// See definition of this fuction template at "gauss.cpp".
template <typename T, bool REFCNT>
matrix<T, REFCNT> inverse (const matrix<T, REFCNT>& A);


template <typename T, bool REFCNT>
void matrix<T, REFCNT>::assign_fromstr (const char* s)
{
    std::istringstream buf(s);
    static_cast<std::istream&>(buf) >> *this;
    if(!buf && !buf.eof())
        throw incorrect_string(s);
}


template <typename T, bool REFCNT>
template <typename Val>
void matrix<T, REFCNT>::assign_fromval
(size_type rows_a, size_type cols_a, const Val& val)
{
    // WARNING! TEMPORARY IMPLEMENTATION!
    resize(rows_a, cols_a);
    std::fill(begin(), end(), val);
}


template <typename T, bool REFCNT>
template <typename Seq>
void matrix<T, REFCNT>::assign_fromseq
(size_type rows_a, size_type cols_a, Seq seq)
{
    // WARNING! TEMPORARY IMPLEMENTATION!
    resize(rows_a, cols_a);
    for(iterator i = begin(); i != end(); ++i)
        *i = *seq++;
}


template <typename T, bool REFCNT>
template <typename Val>
void matrix<T, REFCNT>::assign_diag_fromval
(size_type rows_a, size_type cols_a, const Val& val)
{
    // WARNING! TEMPORARY IMPLEMENTATION!
    assign_fromsize(rows_a, cols_a);
    size_type minsize = std::min(rows_a, cols_a);
    for(size_type i = 0; i < minsize; ++i)
        el(i, i) = val;
}


template <typename T, bool REFCNT>
template <typename Seq>
void matrix<T, REFCNT>::assign_diag_fromseq
(size_type rows_a, size_type cols_a, Seq seq)
{
    // WARNING! TEMPORARY IMPLEMENTATION!
    assign_fromsize(rows_a, cols_a);
    size_type minsize = std::min(rows_a, cols_a);
    for(size_type i = 0; i < minsize; ++i)
        el(i, i) = *seq++;
}


template <typename T, bool REFCNT>
bool matrix<T, REFCNT>::is_null () const
{
    for(size_type i = 0; i < size(); ++i)
        if(!Arageli::is_null(mem(i)))
            return false;
    return true;
}


template <typename T, bool REFCNT>
bool matrix<T, REFCNT>::is_unit () const
{
    if(is_empty())
        return false;
    if(!is_square())
        return false;
    if(!Arageli::is_unit(el(0, 0)))
        return false;

    for(size_type i = 1; i < size();)
    {
        for(size_type j = 0; j < ncols(); ++j, ++i)
            if(!Arageli::is_null(mem(i)))
                return false;
        if(!Arageli::is_unit(mem(i++)))
            return false;
    }

    return true;
}


template <typename T, bool REFCNT>
bool matrix<T, REFCNT>::is_opposite_unit () const
{
    if(is_empty())
        return false;
    if(!is_square())
        return false;
    if(!Arageli::is_opposite_unit(el(0, 0)))
        return false;

    for(size_type i = 1; i < size();)
    {
        for(size_type j = 0; j < ncols(); ++j, ++i)
            if(!Arageli::is_null(mem(i)))
                return false;
        if(!Arageli::is_opposite_unit(mem(i++)))
            return false;
    }

    return true;
}


template <typename T, bool REFCNT>
matrix<T, REFCNT>& matrix<T, REFCNT>::inverse ()
{
    return *this = Arageli::inverse(*this);
}


template <typename T, bool REFCNT>
matrix<T, REFCNT>& matrix<T, REFCNT>::each_inverse ()
{
    for(size_type i = 0; i < size(); ++i)
        Arageli::inverse(&mem(i));
    return *this;
}


#define _ARAGELI_MATRIX_OPERATOR_ASSIGN_1(OPER, MNEM)    \
    template <typename T1, bool REFCNT1>    \
    template <typename T2>    \
    matrix<T1, REFCNT1>& matrix<T1, REFCNT1>::MNEM##_scalar    \
    (const T2& x)    \
    {    \
        if(is_empty())    \
            return *this;    \
        /*unique();*/    \
    \
        iterator i = begin(), iend = end();    \
        for(; i < iend; ++i)    \
            *i OPER x;    \
    \
        return *this;    \
    }    \
    \
    template <typename T1, bool REFCNT1>    \
    template <typename T2>    \
    matrix<T1, REFCNT1>& matrix<T1, REFCNT1>::each_##MNEM##_matrix    \
    (const T2& x)    \
    {    \
        ARAGELI_ASSERT_0(x.nrows() == nrows());    \
        ARAGELI_ASSERT_0(x.ncols() == ncols());    \
        if(is_empty())    \
            return *this;    \
        /*unique();*/    \
    \
        iterator i1 = begin(), i1end = end();    \
        typename T2::const_iterator i2 = x.begin();    \
        for(; i1 < i1end; ++i1, ++i2)    \
            *i1 OPER *i2;    \
    \
        return *this;    \
    }


_ARAGELI_MATRIX_OPERATOR_ASSIGN_1(+=, add)
_ARAGELI_MATRIX_OPERATOR_ASSIGN_1(-=, sub)
_ARAGELI_MATRIX_OPERATOR_ASSIGN_1(*=, mul)
_ARAGELI_MATRIX_OPERATOR_ASSIGN_1(/=, div);
_ARAGELI_MATRIX_OPERATOR_ASSIGN_1(%=, mod);
_ARAGELI_MATRIX_OPERATOR_ASSIGN_1(&=, bitand)
_ARAGELI_MATRIX_OPERATOR_ASSIGN_1(|=, bitor)
_ARAGELI_MATRIX_OPERATOR_ASSIGN_1(^=, bitxor)
_ARAGELI_MATRIX_OPERATOR_ASSIGN_1(<<=, shl)
_ARAGELI_MATRIX_OPERATOR_ASSIGN_1(>>=, shr)

#undef _ARAGELI_MATRIX_OPERATOR_ASSIGN_1


template <typename T1, bool REFCNT1>
template <typename T2>
matrix<T1, REFCNT1>& matrix<T1, REFCNT1>::mul_matrix
(const T2& right)
{
    ARAGELI_ASSERT_0(ncols() == right.nrows());

    if(is_empty() && ncols() == right.ncols())
        return *this;

    if
    (
        reinterpret_cast<const void*>(this) !=
        reinterpret_cast<const void*>(&right)
    )
    {
        // If left and right agruments are NOT the same object.

        matrix left;
        swap(left);
        assign_fromsize(left.nrows(), right.ncols());

        // WARNING. This is slow implementation.

        for(size_type i = 0; i < nrows(); ++i)
            for(size_type j = 0; j < ncols(); ++j)
                for(size_type k = 0; k < left.ncols(); ++k)
                    el(i, j) += left(i, k) * right(k, j);
    }
    else
    {
        // If left and right agruments are the same object.

        ARAGELI_ASSERT_1(is_square());
        matrix arg(nrows()/*::null(), square_matrix*/);
        swap(arg);

        for(size_type i = 0; i < nrows(); ++i)
            for(size_type j = 0; j < nrows(); ++j)
                for(size_type k = 0; k < nrows(); ++k)
                    el(i, j) += arg(i, k) * arg(k, j);
    }

    return *this;
}


template <typename T1, bool REFCNT1>
template <typename T2>
matrix<T1, REFCNT1>& matrix<T1, REFCNT1>::div_matrix
(const T2& right)
{
    try
    {
        *this *= Arageli::inverse(right);
    }
    catch(const matrix_is_singular&)
    {
        throw division_by_zero();
    }
    return *this;
}


template <typename T1, bool REFCNT1>
matrix<T1, REFCNT1>& matrix<T1, REFCNT1>::opposite ()
{
    if(is_empty())
        return *this;
    unique();

    for(size_type i = 0; i < size(); ++i)
        Arageli::opposite(&mem(i));

    return *this;
}


template <typename T1, bool REFCNT1>
void matrix<T1, REFCNT1>::transpose ()
{
    // a temporary matrix with 'transposed' sizes
    matrix<T1, true> t(ncols(), nrows(), fromsize);

    for(size_type i = 0; i < nrows(); ++i)
        for(size_type j = 0; j < ncols(); ++j)
            t(j, i) = el(i, j);

    swap(t);
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename T, bool REFCNT>
void matrix<T, REFCNT>::insert_row (size_type pos, const T& val)
{
    ARAGELI_ASSERT_0(pos <= nrows());

    unique();
    mem().insert(begin() + pos*ncols(), ncols(), val);
    ++rep().rows;
}


template <typename T, bool REFCNT>
template <typename T1, bool REFCNT1>
void matrix<T, REFCNT>::insert_row
(size_type pos, const vector<T1, REFCNT1>& vals)
{
    ARAGELI_ASSERT_0(vals.size() >= ncols());
    ARAGELI_ASSERT_0(pos <= nrows());

    unique();
    if(ncols() == 0 && nrows() == 0)
        resize(0, vals.size());
    mem().insert(begin() + pos*ncols(), vals.begin(), vals.begin() + ncols());
    ++rep().rows;
}


template <typename T, bool REFCNT>
template <typename In>
void matrix<T, REFCNT>::insert_row (size_type pos, In first)
{
    ARAGELI_ASSERT_0(pos <= nrows());

    // WARNING! If In is input stream iterator then
    // the following expression is incorrect.

    In last = std::advance(first, ncols());

    unique();
    mem().insert(begin() + pos*ncols(), first, last);
    ++rep().rows;
}


template <typename T, bool REFCNT>
void matrix<T, REFCNT>::insert_rows
(size_type pos, size_type n, const T& val)
{
    ARAGELI_ASSERT_0(pos <= nrows());

    // WARNING. This is slow implementation.

    for(size_type i = pos; i < pos + n; ++i)
        insert_row(pos, val);
}


template <typename T, bool REFCNT>
template <typename T1, bool REFCNT1>
void matrix<T, REFCNT>::insert_rows
(size_type pos, size_type n, const vector<T1, REFCNT1>& vals)
{
    ARAGELI_ASSERT_0(pos <= nrows());

    // WARNING. This is slow implementation.

    for(size_type i = pos; i < pos + n; ++i)
        insert_row(pos, vals);
}



template <typename T, bool REFCNT>
template <typename In>
void matrix<T, REFCNT>::insert_rows
(size_type pos, size_type n, In first)
{
    ARAGELI_ASSERT_0(pos <= nrows());

    // WARNING. This is slow implementation.

    for(size_type i = pos; i < pos + n; ++i)
        insert_row(pos, first);
}


template <typename T, bool REFCNT>
void matrix<T, REFCNT>::erase_row (size_type pos)
{
    ARAGELI_ASSERT_0(pos < nrows());

    unique();
    mem().erase(begin() + pos*ncols(), begin() + (pos + 1)*ncols());
    --rep().rows;
}


template <typename T, bool REFCNT>
void matrix<T, REFCNT>::erase_rows (size_type pos, size_type n)
{
    ARAGELI_ASSERT_0(pos < nrows());
    ARAGELI_ASSERT_0(pos + n <= nrows());

    unique();
    mem().erase(begin() + pos*ncols(), begin() + (pos + n)*ncols());
    rep().rows -= n;
}

template <typename T, bool REFCNT>
void matrix<T, REFCNT>::swap_rows (size_type xpos, size_type ypos)
{
    ARAGELI_ASSERT_0(xpos < nrows());
    ARAGELI_ASSERT_0(ypos < nrows());

    if(xpos == ypos)return;

    std::swap_ranges
    (
        begin() + xpos*ncols(),
        begin() + (xpos + 1)*ncols(),
        begin() + ypos*ncols()
    );
}


template <typename T, bool REFCNT>
template <typename T1>
void matrix<T, REFCNT>::mult_row (size_type i, const T1& x)
{
    ARAGELI_ASSERT_0(i < nrows());

    unique();
    for(size_type j = i*ncols(); j < (i+1)*ncols(); ++j)
        mem(j) *= x;
}


template <typename T, bool REFCNT>
template <typename T1>
void matrix<T, REFCNT>::div_row (size_type i, const T1& x)
{
    ARAGELI_ASSERT_0(i < nrows());

    unique();
    size_type endpos = (i+1)*ncols();
    for(size_type j = i*ncols(); j < endpos; ++j)
        mem(j) /= x;
}


template <typename T, bool REFCNT>
template <typename T1>
void matrix<T, REFCNT>::mod_row (size_type i, const T1& x)
{
    ARAGELI_ASSERT_0(i < nrows());

    unique();
    size_type endpos = (i+1)*ncols();
    for(size_type j = i*ncols(); j < endpos; ++j)
        mem(j) = mod(mem(j), x);
}


// row[dstpos] += row[srcpos]
template <typename T, bool REFCNT>
void matrix<T, REFCNT>::add_rows (size_type dstpos, size_type srcpos)
{
    ARAGELI_ASSERT_0(dstpos < nrows());
    ARAGELI_ASSERT_0(srcpos < nrows());
    ARAGELI_ASSERT_0(srcpos != dstpos);

    unique();
    size_type endpos = (srcpos+1)*ncols();
    for(size_type i = srcpos*ncols(), j = dstpos*ncols(); i < endpos; ++i, ++j)
        mem(j) += mem(i);
}


// row[dstpos] -= row[srcpos]
template <typename T, bool REFCNT>
void matrix<T, REFCNT>::sub_rows (size_type dstpos, size_type srcpos)
{
    ARAGELI_ASSERT_0(dstpos < nrows());
    ARAGELI_ASSERT_0(srcpos < nrows());
    ARAGELI_ASSERT_0(srcpos != dstpos);

    unique();
    size_type endpos = (srcpos+1)*ncols();
    for(size_type i = srcpos*ncols(), j = dstpos*ncols(); i < endpos; ++i, ++j)
        mem(j) -= mem(i);
}


// row[dstpos] += y*row[srcpos]
template <typename T, bool REFCNT>
template <typename T2>
void matrix<T, REFCNT>::addmult_rows
(size_type dstpos, size_type srcpos, const T2& y)
{
    ARAGELI_ASSERT_0(dstpos < nrows());
    ARAGELI_ASSERT_0(srcpos < nrows());
    ARAGELI_ASSERT_0(dstpos != srcpos);

    unique();
    size_type endpos = (srcpos+1)*ncols();
    for(size_type i = srcpos*ncols(), j = dstpos*ncols(); i < endpos; ++i, ++j)
        mem(j) += y*mem(i);
}


// row[dstpos] = x*row[dstpos] + y*row[srcpos]
template <typename T, bool REFCNT>
template <typename T1, typename T2>
void matrix<T, REFCNT>::addmult_rows
(
    size_type dstpos,
    const T1& x,
    size_type srcpos,
    const T2& y
)
{
    ARAGELI_ASSERT_0(dstpos < nrows());
    ARAGELI_ASSERT_0(srcpos < nrows());
    ARAGELI_ASSERT_0(dstpos != srcpos);

    unique();
    size_type endpos = (srcpos+1)*ncols();
    for(size_type i = srcpos*ncols(), j = dstpos*ncols(); i < endpos; ++i, ++j)
        (mem(j) *= x) += y*mem(i);
}


template <typename T, bool REFCNT>
template <typename T1, typename V, typename T2>
void matrix<T, REFCNT>::addmult_rows
(
    size_type dstpos,
    const T1& x,
    const V& srcvec,
    const T2& y
)
{
    ARAGELI_ASSERT_0(dstpos < nrows());
    ARAGELI_ASSERT_0(srcvec.size() == ncols());

    unique();
    size_type endpos = ncols();
    for(size_type i = 0, j = dstpos*ncols(); i < endpos; ++i, ++j)
        (mem(j) *= x) += y*srcvec[i];
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename T, bool REFCNT>
void matrix<T, REFCNT>::insert_col (size_type pos, const T& val)
{
    ARAGELI_ASSERT_0(pos <= ncols());

    unique();
    size_type oldsize = size();

    if(oldsize == 0)
    {
        rep().cols++;
        return;
    }

    mem().resize(oldsize + nrows());

    // reverse copying of the data

    iterator src = mem().begin() + oldsize;    // at the old end of the storage
    iterator dst = mem().end();
    ARAGELI_ASSERT_1(dst == src + nrows());
    iterator end = dst - (ncols() - pos);

    // copying of the last piece (the end of the last row)
    while(dst != end)
        *--dst = *--src;

    ARAGELI_ASSERT_1(src != mem().begin() || oldsize == 1);
    ARAGELI_ASSERT_1(src <= dst);
    *(--dst) = val;
    ARAGELI_ASSERT_1(dst - src == nrows() - 1);

    size_type newncols = ncols() + 1;
    iterator endend = mem().begin() + pos + 1;

    for(;;)
    {
        if(end == endend)break;
        end -= newncols;
        while(dst != end)
            *--dst = *--src;
        *(--dst) = val;
    }

    ARAGELI_ASSERT_1(dst == src);
    ARAGELI_ASSERT_1(dst == mem().begin() + pos);

    rep().cols++;
}


template <typename T, bool REFCNT>
template <typename T1, bool REFCNT1>
void matrix<T, REFCNT>::insert_col
(size_type pos, const vector<T1, REFCNT1>& vals)
{
    ARAGELI_ASSERT_0(pos <= ncols());
    ARAGELI_ASSERT_0(vals.size() >= nrows());

    unique();
    if(ncols() == 0 && nrows() == 0)resize(vals.size(), 0);
    size_type oldsize = size();

    if(oldsize == 0)
    {
        rep().cols++;
        return;    // ERROR
    }

    mem().resize(oldsize + nrows());

    // reverse copying of the data

    iterator src = mem().begin() + oldsize;    // at the old end of the storage
    iterator dst = mem().end();
    ARAGELI_ASSERT_1(dst == src + nrows());
    iterator end = dst - (ncols() - pos);

    // copying of the last piece (the end of the last row)
    while(dst != end)
        *--dst = *--src;

    //ARAGELI_ASSERT_1(src != mem().begin());
    ARAGELI_ASSERT_1(src <= dst);
    typename vector<T1, REFCNT1>::const_iterator pval = vals.begin() + nrows();
    *(--dst) = *(--pval);
    ARAGELI_ASSERT_1(dst - src == nrows() - 1);

    size_type newncols = ncols() + 1;
    iterator endend = mem().begin() + pos + 1;

    for(;;)
    {
        if(end == endend)break;
        end -= newncols;
        while(dst != end)
            *--dst = *--src;
        *(--dst) = *(--pval);
    }

    ARAGELI_ASSERT_1(dst == src);
    ARAGELI_ASSERT_1(dst == mem().begin() + pos);
    ARAGELI_ASSERT_1(pval == vals.begin());

    rep().cols++;
}


template <typename T, bool REFCNT>
template <typename In>
void matrix<T, REFCNT>::insert_col (size_type pos, In first)
{
    ARAGELI_ASSERT_0(pos <= ncols());

    unique();
    size_type oldsize = size();

    if(oldsize == 0)
    {
        rep().cols++;
        return;
    }

    mem().resize(oldsize + nrows());

    // reverse copying of the data

    iterator src = mem().begin() + oldsize;    // at the old end of the storage
    iterator dst = mem().end();
    ARAGELI_ASSERT_1(dst == src + nrows());
    iterator end = dst - (ncols() - pos);

    // copying of the last piece (the end of the last row)
    while(dst != end)
        *--dst = *--src;

    ARAGELI_ASSERT_1(src != mem().begin());
    ARAGELI_ASSERT_1(src <= dst);
    //*(--dst) = *(--pval);
    --dst;
    ARAGELI_ASSERT_1(dst - src == nrows() - 1);

    size_type newncols = ncols() + 1;
    iterator endend = mem().begin() + pos + 1;

    for(;;)
    {
        if(end == endend)break;
        end -= newncols;
        while(dst != end)
            *--dst = *--src;
        //*(--dst) = *(--pval);
        --dst;
    }

    ARAGELI_ASSERT_1(dst == src);
    ARAGELI_ASSERT_1(dst == mem().begin() + pos);
    //ARAGELI_ASSERT_1(pval == vals.begin());

    rep().cols++;

    end = mem().end() - (newncols - pos) + 1;
    for(dst = begin() + pos; dst < end; dst += newncols)
        *dst = *first++;
}


template <typename T, bool REFCNT>
void matrix<T, REFCNT>::insert_cols
(size_type pos, size_type n, const T& val)
{
    ARAGELI_ASSERT_0(pos <= ncols());

    // WARNING. This is slow implementation.

    for(size_type i = pos; i < pos + n; ++i)
        insert_col(pos, val);
}


template <typename T, bool REFCNT>
template <typename T1, bool REFCNT1>
void matrix<T, REFCNT>::insert_cols
(size_type pos, size_type n, const vector<T1, REFCNT1>& vals)
{
    ARAGELI_ASSERT_0(pos <= ncols());

    // WARNING. This is slow implementation.

    for(size_type i = pos; i < pos + n; ++i)
        insert_col(pos, vals);
}



template <typename T, bool REFCNT>
template <typename In>
void matrix<T, REFCNT>::insert_cols
(size_type pos, size_type n, In first)
{
    ARAGELI_ASSERT_0(pos <= ncols());

    // WARNING. This is slow implementation.

    for(size_type i = pos; i < pos + n; ++i)
        insert_col(pos, first);
}


template <typename T, bool REFCNT>
void matrix<T, REFCNT>::erase_col (size_type pos)
{
    ARAGELI_ASSERT_0(pos < ncols());

    unique();
    size_type oldsize = size();

    if(oldsize == 0)
    {
        rep().cols--;
        return;
    }

    iterator dst = mem().begin() + pos;
    iterator src = dst + 1;
    size_type newncols = ncols() - 1;
    iterator endend = mem().end() - (ncols() - pos) + 1;

    while(src != endend)
    {
        iterator end = src + newncols;
        while(src != end)
            *dst++ = *src++;
        ++src;    // jump over deleted element in the current row
    }

    iterator end = mem().end();
    while(src != end)
        *dst++ = *src++;

    mem().resize(oldsize - nrows());
    rep().cols--;
}


template <typename T, bool REFCNT>
void matrix<T, REFCNT>::erase_cols (size_type pos, size_type n)
{
    ARAGELI_ASSERT_0(pos < ncols());
    ARAGELI_ASSERT_0(pos + n <= ncols());

    unique();
    size_type oldsize = size();

    if(oldsize == 0)
    {
        rep().cols-=n;
        return;
    }

    iterator dst = mem().begin() + pos;
    iterator src = dst + n;
    size_type newncols = ncols() - n;
    iterator endend = mem().end() - (newncols - pos);

    while(src != endend)
    {
        iterator end = src + newncols;
        while(src != end)
            *dst++ = *src++;
        src += n;    // jump over deleted elements in the current row
    }

    iterator end = mem().end();
    while(src != end)
        *dst++ = *src++;

    mem().resize(oldsize - n*nrows());
    rep().cols -= n;
}

template <typename T, bool REFCNT>
void matrix<T, REFCNT>::swap_cols (size_type xpos, size_type ypos)
{
    ARAGELI_ASSERT_0(xpos < ncols());
    ARAGELI_ASSERT_0(ypos < ncols());

    if(xpos == ypos || is_empty())
        return;

    unique();

    iterator p1 = mem().begin() + xpos, p2 = mem().begin() + ypos;
    size_type nc = ncols();
    iterator end = mem().end() - (nc - xpos);

    for(;;)
    {
        std::iter_swap(p1, p2);
        if(p1 == end)break;
        p1 += nc;
        p2 += nc;
    }

    ARAGELI_ASSERT_1(mem().end() - (nc - ypos) == p2);
}


template <typename T, bool REFCNT>
template <typename T1>
void matrix<T, REFCNT>::mult_col (size_type i, const T1& x)
{
    ARAGELI_ASSERT_0(i < ncols());

    if(is_empty())return;

    unique();

    iterator p = mem().begin() + i;
    size_type nc = ncols();
    iterator end = mem().end() - (nc - i);

    for(;;)
    {
        *p *= x;
        if(p == end)break;
        p += nc;
    }
}


template <typename T, bool REFCNT>
template <typename T1>
void matrix<T, REFCNT>::div_col (size_type i, const T1& x)
{
    ARAGELI_ASSERT_0(i < ncols());

    if(is_empty())return;

    unique();

    iterator p = mem().begin() + i;
    size_type nc = ncols();
    iterator end = mem().end() - (nc - i);

    for(;;)
    {
        *p /= x;
        if(p == end)break;
        p += nc;
    }
}



template <typename T, bool REFCNT>
template <typename T1>
void matrix<T, REFCNT>::mod_col (size_type i, const T1& x)
{
    ARAGELI_ASSERT_0(i < ncols());

    if(is_empty())return;

    unique();

    iterator p = mem().begin() + i;
    size_type nc = ncols();
    iterator end = mem().end() - (nc - i);

    for(;;)
    {
        *p = mod(*p, x);
        if(p == end)break;
        p += nc;
    }
}



// col[dstpos] += col[srcpos]
template <typename T, bool REFCNT>
void matrix<T, REFCNT>::add_cols (size_type dstpos, size_type srcpos)
{
    ARAGELI_ASSERT_0(dstpos < ncols());
    ARAGELI_ASSERT_0(srcpos < ncols());
    ARAGELI_ASSERT_0(srcpos != dstpos);

    if(is_empty())return;

    unique();

    iterator p1 = mem().begin() + dstpos, p2 = mem().begin() + srcpos;
    size_type nc = ncols();
    iterator end = mem().end() - (nc - dstpos);

    for(;;)
    {
        *p1 += *p2;
        if(p1 == end)break;
        p1 += nc; p2 += nc;
    }

    ARAGELI_ASSERT_1(mem().end() - (nc - srcpos) == p2);
}


// col[dstpos] -= col[srcpos]
template <typename T, bool REFCNT>
void matrix<T, REFCNT>::sub_cols (size_type dstpos, size_type srcpos)
{
    ARAGELI_ASSERT_0(dstpos < ncols());
    ARAGELI_ASSERT_0(srcpos < ncols());
    ARAGELI_ASSERT_0(srcpos != dstpos);

    if(is_empty())return;

    unique();

    iterator p1 = mem().begin() + dstpos, p2 = mem().begin() + srcpos;
    size_type nc = ncols();
    iterator end = mem().end() - (nc - dstpos);

    for(;;)
    {
        *p1 -= *p2;
        if(p1 == end)break;
        p1 += nc; p2 += nc;
    }

    ARAGELI_ASSERT_1(mem().end() - (nc - srcpos) == p2);
}


// col[dstpos] += y*col[srcpos]
template <typename T, bool REFCNT>
template <typename T2>
void matrix<T, REFCNT>::addmult_cols
(size_type dstpos, size_type srcpos, const T2& y)
{
    ARAGELI_ASSERT_0(dstpos < ncols());
    ARAGELI_ASSERT_0(srcpos < ncols());
    ARAGELI_ASSERT_0(srcpos != dstpos);

    if(is_empty())return;

    unique();

    iterator p1 = mem().begin() + dstpos, p2 = mem().begin() + srcpos;
    size_type nc = ncols();
    iterator end = mem().end() - (nc - dstpos);

    for(;;)
    {
        *p1 += y*(*p2);
        if(p1 == end)break;
        p1 += nc; p2 += nc;
    }

    ARAGELI_ASSERT_1(mem().end() - (nc - srcpos) == p2);
}


// col[dstpos] = x*col[dstpos] + y*col[srcpos]
template <typename T, bool REFCNT>
template <typename T1, typename T2>
void matrix<T, REFCNT>::addmult_cols
(
    size_type dstpos,
    const T1& x,
    size_type srcpos,
    const T2& y
)
{
    ARAGELI_ASSERT_0(dstpos < ncols());
    ARAGELI_ASSERT_0(srcpos < ncols());
    ARAGELI_ASSERT_0(srcpos != dstpos);

    if(is_empty())return;

    unique();

    iterator p1 = mem().begin() + dstpos, p2 = mem().begin() + srcpos;
    size_type nc = ncols();
    iterator end = mem().end() - (nc - dstpos);

    for(;;)
    {
        *p1 = x*(*p1) + y*(*p2);
        if(p1 == end)break;
        p1 += nc; p2 += nc;
    }

    ARAGELI_ASSERT_1(mem().end() - (nc - srcpos) == p2);
}


template <typename T, bool REFCNT>
template <typename T1, typename V, typename T2>
void matrix<T, REFCNT>::addmult_cols
(
    size_type dstpos,
    const T1& x,
    const V& srcvec,
    const T2& y
)
{
    ARAGELI_ASSERT_0(dstpos < ncols());
    ARAGELI_ASSERT_0(srcvec.size() == nrows());

    if(is_empty())return;

    unique();

    iterator p1 = mem().begin() + dstpos;
    typename V::const_iterator p2 = srcvec.begin();
    size_type nc = ncols();
    iterator end = mem().end() - (nc - dstpos);

    for(;;)
    {
        *p1 = x*(*p1) + y*(*p2);
        if(p1 == end)break;
        p1 += nc; ++p2;
    }

    ARAGELI_ASSERT_1(p2 == srcvec.end());
}


template <typename T, bool REFCNT>
template <typename V>
V& matrix<T, REFCNT>::copy_col
(size_type i, V& res) const
{
    ARAGELI_ASSERT_0(i < ncols());
    res.resize(nrows());
    for(size_type j = 0; j < nrows(); ++j)
        res[j] = el(j, i);
    return res;
}


template <typename T, bool REFCNT>
template <typename SV, typename M>
M& matrix<T, REFCNT>::copy_rows (const SV& sv, M& res) const
{
    size_type nc = ncols();
    res.assign_fromsize(sv.size(), nc);

    const_iterator data = begin();
    typename M::iterator resi = res.begin();
    typename SV::const_iterator svi = sv.begin(), svend = sv.end();

    for(; svi != svend; ++svi)
    {
        size_type r = *svi;
        ARAGELI_ASSERT_0(r < nrows());
        resi = std::copy(data + r*nc, data + r*nc + nc, resi);
    }

    return res;
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4018)
#endif

template <typename T, bool REFCNT>
template <typename SV>
void matrix<T, REFCNT>::erase_rows (const SV& sv)
{
    if(sv.is_empty())return;

    SV svt = sv;    // copy because need sorting and uniquelization
    typedef typename SV::iterator SViter;
    std::sort(svt.begin(), svt.end());

    SViter
        svti = svt.begin(),
        svtend = std::unique(svti, svt.end());

    size_type curerased = *svti, nc = ncols();

    ARAGELI_ASSERT_0(*(svtend-1) < size());
    ARAGELI_ASSERT_0(curerased < size());

    iterator
        thisdst = begin() + curerased*nc,
        thissrc = thisdst + nc,
        thisend = end();

    for(;;)
    {
        ++svti;

        if(svti != svtend && *svti == curerased+1)
        {
            ++curerased;
            ++svti;
            ARAGELI_ASSERT_0(thissrc != thisend);
            thissrc += nc;
        }

        ARAGELI_ASSERT_1(thissrc != thisend || svti == svtend);

        iterator tend =        // end of current copied part of the vector
            svti == svtend ?
            thisend :
            (thissrc + (*svti - curerased - 1)*nc);

        thisdst = std::copy(thissrc, tend, thisdst);

        if(tend == thisend)
            break;

        thissrc = tend + nc;
        ARAGELI_ASSERT_1(svti != svtend);
        curerased = *svti;
    }

    ARAGELI_ASSERT_1(svti == svtend);
    ARAGELI_ASSERT_1(thisdst + (svtend - svt.begin())*nc == thisend);
    mem().erase(thisdst, thisend);
    ARAGELI_ASSERT_1(mem().size()%nc == 0);
    rep().rows = mem().size()/nc;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

template <typename T, bool REFCNT>
template <typename SV, typename M>
M& matrix<T, REFCNT>::copy_cols (const SV& sv, M& res) const
{
    // WARNING!!! SLOW IMPLEMENTATION!!!

    res.assign_fromsize(nrows(), sv.size());
    typename SV::const_iterator
        svi = sv.begin(),
        svend = sv.end();
    size_type nr = nrows();
    typename M::size_type i = 0;

    for(; svi != svend; ++svi, ++i)
    {
        size_type c = *svi;
        for(size_type r = 0; r < nr; ++r)
            res(r, i) = el(r, c);
    }

    return res;
}


template <typename T, bool REFCNT>
template <typename SV>
void matrix<T, REFCNT>::erase_cols (const SV& sv)
{
    // WARNING!!! SLOW IMPLEMENTATION!!!

    if(sv.is_empty())return;

    SV svt = sv;    // copy because need sorting and uniquelization
    std::sort(svt.begin(), svt.end(), std::greater<typename SV::value_type>());
    svt.erase(std::unique(svt.begin(), svt.end()), svt.end());

    for(typename SV::const_iterator i = svt.begin(); i < svt.end(); ++i)
        erase_col(*i);
}


template <typename T, bool REFCNT>
template <typename RS, typename CS, typename M>
M& matrix<T, REFCNT>::copy_submatrix (const RS& rs, const CS& cs, M& res) const
{
    // WARNING!!! SLOW IMPLEMENTATION!!!

    res.assign_fromsize(rs.size(), cs.size());

    for(size_type i = 0; i < rs.size(); ++i)
        for(size_type j = 0; j < cs.size(); ++j)
            res(i, j) = el(rs[i], cs[j]);

    return res;
}


template <typename T, bool REFCNT>
template <typename RS, typename CS>
void matrix<T, REFCNT>::erase_submatrix (const RS& rs, const CS& cs)
{
    erase_rows(rs);
    erase_cols(cs);
}


template <typename T, bool REFCNT>
void matrix<T, REFCNT>::do_pack ()
{
    ARAGELI_ASSERT_1(size() < capacity());

    unique();
    Rep trep = rep();
    trep.swap(rep());
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
vector<T2, REFCNT2> operator*
(
    const matrix<T1, REFCNT1>& m,
    const vector<T2, REFCNT2>& x
)
{
    ARAGELI_ASSERT_0(m.ncols() == x.size());

    vector<T2, REFCNT2> res(m.nrows());

    for(std::size_t i = 0; i < m.nrows(); ++i)
    {
        typename vector<T2, REFCNT2>::reference cur = res[i];

        for(std::size_t j = 0; j < m.ncols(); ++j)
            cur += m(i, j)*x[j];
    }

    return res;
}


template
<
    typename T2,
    bool REFCNT2,
    typename T1,
    bool REFCNT1
>
vector<T2, REFCNT2> operator*
(
    const vector<T2, REFCNT2>& x,
    const matrix<T1, REFCNT1>& m
)
{
    ARAGELI_ASSERT_0(m.nrows() == x.size());

    vector<T2, REFCNT2> res(m.ncols());

    for(std::size_t i = 0; i < m.ncols(); ++i)
    {
        typename vector<T2, REFCNT2>::reference cur = res[i];

        for(std::size_t j = 0; j < m.nrows(); ++j)
            cur += m(j, i)*x[j];
    }

    return res;
}


template <typename T, bool REFCNT>
std::ostream& output_list
(
    std::ostream& out,
    const matrix<T, REFCNT>& x,
    const char* first_bracket,
    const char* second_bracket,
    const char* row_separator,
    const char* first_row_bracket,
    const char* second_row_bracket,
    const char* col_separator
)
{
    out << first_bracket;
    typedef typename matrix<T, REFCNT>::size_type size_type;

    if(!x.is_empty())
    {
        ARAGELI_ASSERT_0(x.ncols() && x.nrows());

        out << first_row_bracket << x(0, 0);
        for(size_type j = 1; j < x.ncols(); ++j)
            out << col_separator << x(0, j);
        out << second_row_bracket;

        for(size_type i = 1; i < x.nrows(); ++i)
        {
            out << row_separator << first_row_bracket << x(i, 0);
            for(size_type j = 1; j < x.ncols(); ++j)
                out << col_separator << x(i, j);
            out << second_row_bracket;
        }
    }

    out << second_bracket;
    return out;
}


template <typename T, bool REFCNT>
std::istream& input_list
(
    std::istream& in,
    matrix<T, REFCNT>& x,
    const char* first_bracket,
    const char* second_bracket,
    const char* row_separator,
    const char* first_row_bracket,
    const char* second_row_bracket,
    const char* col_separator
)
{
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(first_bracket));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(second_bracket));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(row_separator));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(first_row_bracket));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(second_row_bracket));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(col_separator));

    if(!_Internal::read_literal(in, first_bracket))
    {
        in.clear(std::ios_base::failbit);
        return in;
    }

    if(*second_bracket && _Internal::read_literal(in, second_bracket))
    { // empty matrix
        x.assign_fromsize(0, 0);
        return in;
    }

    typedef std::list<vector<T, true> > Buf;    // temporary buffer for rows
    Buf buf;
    std::size_t cols;

    do
    {
        vector<T, false> tmp;

        input_list
        (
            in,
            tmp,
            first_row_bracket,
            second_row_bracket,
            col_separator
        );

        if(!in)
        {
            in.clear(std::ios_base::badbit);
            return in;
        }

        if(buf.empty())
            cols = tmp.size();
        else if(cols != tmp.size())
        {
            in.clear(std::ios_base::badbit);
            return in;
        }

        buf.push_back(tmp);

    }while(_Internal::read_literal(in, row_separator));


    if(!_Internal::read_literal(in, second_bracket))
    {
        in.clear(std::ios_base::badbit);
        return in;
    }

    x.assign_fromsize(buf.size(), cols);
    typename matrix<T, REFCNT>::iterator j = x.begin();

    for(typename Buf::iterator i = buf.begin(); i != buf.end(); ++i)
    {
        ARAGELI_ASSERT_1(i->size() == cols);
        j = std::copy(i->begin(), i->end(), j);
    }

    ARAGELI_ASSERT_1(j == x.end());

    return in;
}


template <typename T, bool REFCNT>
std::ostream& output_aligned
(
    std::ostream& out,
    const matrix<T, REFCNT>& x,
    const char* left_col,
    const char* right_col,
    const char* inter_col
)
{
    if(x.is_empty())
    {
        out << left_col << right_col << '\n';
        return out;
    }

    std::vector<std::string> buf(x.size());
    std::vector<std::size_t> maxlens(x.ncols());
    std::size_t ii = 0;    // index for buf

    for(std::size_t j = 0; j < x.ncols(); ++j)
    {
        std::size_t maxlen = 0;

        for(std::size_t i = 0; i < x.nrows(); ++i, ++ii)
        {
            std::ostringstream strbuf;
            strbuf.copyfmt(out);
            strbuf << x(i, j);
            buf[ii] = strbuf.str();

            if(buf[ii].length() > maxlen)
                maxlen = buf[ii].length();
        }

        maxlens[j] = maxlen;
    }



    for(std::size_t i = 0; i < x.nrows(); ++i)
    {
        out << left_col;

        std::string& str = buf[i];
        std::size_t numspace = maxlens[0] - str.length();

        out
            << std::string(numspace/2, ' ')
            << str << std::string(numspace - numspace/2, ' ');

        for(std::size_t j = 1; j < x.ncols(); ++j)
        {
            std::string& str = buf[j*x.nrows() + i];
            std::size_t numspace = maxlens[j] - str.length();

            out
                << inter_col << std::string(numspace/2, ' ')
                << str << std::string(numspace - numspace/2, ' ');
        }

        out << right_col << '\n';
    }

    return out;
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
std::ostream& output_aligned_ver_pair
(
    std::ostream& out,
    const matrix<T1, REFCNT1>& m1,
    const matrix<T2, REFCNT2>& m2,
    const char* left_col,
    const char* right_col,
    const char* inter_col,
    const char* hsplitter
)
{
    ARAGELI_ASSERT_0(m1.ncols() == m2.ncols());

    if(m1.is_empty() && m2.is_empty())
    {
        out << left_col << right_col << '\n';
        return out;
    }

    std::vector<std::string> buf(m1.size() + m2.size());
    std::vector<std::size_t> maxlens(m1.ncols());
    std::size_t ii = 0;    // index for buf

    for(std::size_t j = 0; j < m1.ncols(); ++j)
    {
        std::size_t maxlen = 0;

        for(std::size_t i = 0; i < m1.nrows(); ++i, ++ii)
        {
            std::string& curbuf = buf[ii];

            std::ostringstream strbuf;
            strbuf << m1(i, j);
            curbuf = strbuf.str();

            std::size_t curlen = curbuf.length();
            if(curlen > maxlen)
                maxlen = curlen;
        }

        for(std::size_t i = 0; i < m2.nrows(); ++i, ++ii)
        {
            std::string& curbuf = buf[ii];

            std::ostringstream strbuf;
            strbuf << m2(i, j);
            curbuf = strbuf.str();

            std::size_t curlen = curbuf.length();
            if(curlen > maxlen)
                maxlen = curlen;
        }

        maxlens[j] = maxlen;
    }


    for(std::size_t i = 0; i < m1.nrows(); ++i)
    {
        out << left_col;

        std::string& str = buf[i];
        std::size_t numspace = maxlens[0] - str.length();

        out
            << std::string(numspace/2, ' ')
            << str << std::string(numspace - numspace/2, ' ');

        for(std::size_t j = 1; j < m1.ncols(); ++j)
        {
            std::string& str = buf[j*(m1.nrows() + m2.nrows()) + i];
            std::size_t numspace = maxlens[j] - str.length();

            out
                << inter_col << std::string(numspace/2, ' ')
                << str << std::string(numspace - numspace/2, ' ');
        }

        out << right_col << '\n';
    }

    ARAGELI_ASSERT_1(m1.ncols());

    std::size_t width = (m1.ncols() - 1)*std::strlen(inter_col);
    width = std::accumulate(maxlens.begin(), maxlens.end(), width);
    std::size_t splitter_len = std::strlen(hsplitter);
    std::size_t insplit = width/splitter_len;

    out << left_col;

    for(std::size_t i = 0; i < insplit; ++i)
        out << hsplitter;

    insplit = width%splitter_len;

    for(std::size_t i = 0; i < insplit; ++i)
        out << hsplitter[i];

    out << right_col << '\n';

    for(std::size_t i = 0; i < m2.nrows(); ++i)
    {
        out << left_col;

        std::string& str = buf[i +  + m1.nrows()];
        std::size_t numspace = maxlens[0] - str.length();

        out
            << std::string(numspace/2, ' ')
            << str << std::string(numspace - numspace/2, ' ');

        for(std::size_t j = 1; j < m2.ncols(); ++j)
        {
            std::string& str = buf[j*(m1.nrows() + m2.nrows()) + i + m1.nrows()];
            std::size_t numspace = maxlens[j] - str.length();

            out
                << inter_col << std::string(numspace/2, ' ')
                << str << std::string(numspace - numspace/2, ' ');
        }

        out << right_col << '\n';
    }

    return out;
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
std::ostream& output_aligned_hor_pair
(
    std::ostream& out,
    const matrix<T1, REFCNT1>& m1,
    const matrix<T2, REFCNT2>& m2,
    const char* left_col,
    const char* right_col,
    const char* inter_col,
    const char* vsplitter
)
{
    ARAGELI_ASSERT_0(m1.nrows() == m2.nrows());

    if(m1.is_empty() && m2.is_empty())
    {
        out << left_col << right_col << '\n';
        return out;
    }

    std::vector<std::string> buf1(m1.size());
    std::vector<std::size_t> maxlens1(m1.ncols());
    std::size_t ii = 0;    // index for buf

    for(std::size_t j = 0; j < m1.ncols(); ++j)
    {
        std::size_t maxlen = 0;

        for(std::size_t i = 0; i < m1.nrows(); ++i, ++ii)
        {
            std::ostringstream strbuf;
            strbuf << m1(i, j);
            buf1[ii] = strbuf.str();

            if(buf1[ii].length() > maxlen)
                maxlen = buf1[ii].length();
        }

        maxlens1[j] = maxlen;
    }

    std::vector<std::string> buf2(m2.size());
    std::vector<std::size_t> maxlens2(m1.ncols());
    ii = 0;    // index for buf

    for(std::size_t j = 0; j < m2.ncols(); ++j)
    {
        std::size_t maxlen = 0;

        for(std::size_t i = 0; i < m2.nrows(); ++i, ++ii)
        {
            std::ostringstream strbuf;
            strbuf << m2(i, j);
            buf2[ii] = strbuf.str();

            if(buf2[ii].length() > maxlen)
                maxlen = buf2[ii].length();
        }

        maxlens2[j] = maxlen;
    }


    for(std::size_t i = 0; i < m1.nrows(); ++i)
    {
        out << left_col;

        {
            std::string& str = buf1[i];
            std::size_t numspace = maxlens1[0] - str.length();

            out
                << std::string(numspace/2, ' ')
                << str << std::string(numspace - numspace/2, ' ');

            for(std::size_t j = 1; j < m1.ncols(); ++j)
            {
                std::string& str = buf1[j*m1.nrows() + i];
                std::size_t numspace = maxlens1[j] - str.length();

                out
                    << inter_col << std::string(numspace/2, ' ')
                    << str << std::string(numspace - numspace/2, ' ');
            }
        }

        out << vsplitter;

        {
            std::string& str = buf2[i];
            std::size_t numspace = maxlens2[0] - str.length();

            out
                << std::string(numspace/2, ' ')
                << str << std::string(numspace - numspace/2, ' ');

            for(std::size_t j = 1; j < m2.ncols(); ++j)
            {
                std::string& str = buf2[j*m2.nrows() + i];
                std::size_t numspace = maxlens2[j] - str.length();

                out
                    << inter_col << std::string(numspace/2, ' ')
                    << str << std::string(numspace - numspace/2, ' ');
            }
        }

        out << right_col << '\n';
    }

    return out;
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3
>
std::ostream& output_aligned_corner_triplet_br
(
    std::ostream& out,
    const matrix<T1, REFCNT1>& m1,
    const matrix<T2, REFCNT2>& m2,
    const matrix<T3, REFCNT3>& m3,
    const char* left_col,
    const char* right_col,
    const char* inter_col,
    const char* vsplitter,
    const char* hsplitter
)
{
    /*
        ||         m1  ||
        ||        ---- ||
        ||  m3  |  m2  ||
    */

    ARAGELI_ASSERT_0(m1.ncols() == m2.ncols());
    ARAGELI_ASSERT_0(m3.nrows() == m2.nrows());

    if(m1.is_empty() && m2.is_empty())
    {
        out << left_col << right_col;
        return out;
    }

    std::stringstream strbuf;

    output_aligned_ver_pair(strbuf, m1, m2, "", "", inter_col, hsplitter);
    std::string m12str = strbuf.str();
    strbuf.str("");

    output_aligned(strbuf, m3, "", "", inter_col);
    std::string m3str = strbuf.str();
    strbuf.str("");

    std::size_t width3 = m3str.find('\n');
    ARAGELI_ASSERT_1(width3 < m3str.length());

    std::string tlspace(width3 + std::strlen(vsplitter), ' ');

    std::size_t cur12 = 0, cur3 = 0;

    for(std::size_t i = 0; i < m1.nrows() + 1; ++i)
    {
        std::size_t end12 = m12str.find('\n', cur12);
        ARAGELI_ASSERT_1(end12 < m12str.length());

        out
            << left_col << tlspace
            << m12str.substr(cur12, end12 - cur12)
            << right_col << '\n';

        cur12 = end12 + 1;
    }

    for(std::size_t i = 0; i < m2.nrows(); ++i)
    {
        std::size_t end3 = m3str.find('\n', cur3);
        ARAGELI_ASSERT_1(end3 < m3str.length());

        std::size_t end12 = m12str.find('\n', cur12);
        ARAGELI_ASSERT_1(end12 < m12str.length());

        out
            << left_col
            << m3str.substr(cur3, end3 - cur3)
            << vsplitter
            << m12str.substr(cur12, end12 - cur12)
            << right_col << '\n';

        cur12 = end12 + 1;
        cur3 = end3 + 1;
    }

    return out;
}


template <typename T, bool REFCNT>
std::istream& input_polynom_internal
(std::istream& in, matrix<T, REFCNT>& x)
{
    char ch = 0;
    in >> ch;
    if(!in && !in.eof() || ch != '+' && ch != '-')
        return in;
    matrix<T, REFCNT> res;
    input_list(in, res);
    if(!in && !in.eof())
        return in;
    if(ch == '-')
        res.opposite();
    x = res;
    return in;
}


template <typename T, bool REFCNT>
template <typename Stream>
Stream& io_binary<matrix<T, REFCNT> >::input_stream (Stream& in, matrix<T, REFCNT>& x)
{
    typename matrix<T, REFCNT>::size_type nrows, ncols;
    if(!input_binary_stream(in, nrows) || !input_binary_stream(in, ncols))
        return in;  // fail without losing an old value
    x.resize(nrows, ncols);

    // this can lose an old value without new one have been loaded successfully
    if(x.size())
        input_binary_stream(in, &*x.begin(), x.size());

    return in;
}


} // namespace Arageli


#else

#include "matrix.hpp"


namespace Arageli
{

const char* matrix_output_list_first_bracket_default = "(";
const char* matrix_output_list_second_bracket_default = ")";
const char* matrix_output_list_row_separator_default = ", ";
const char* matrix_output_list_first_row_bracket_default = "(";
const char* matrix_output_list_second_row_bracket_default = ")";
const char* matrix_output_list_col_separator_default = ", ";
const char* matrix_input_list_first_bracket_default = "(";
const char* matrix_input_list_second_bracket_default = ")";
const char* matrix_input_list_row_separator_default = ",";
const char* matrix_input_list_first_row_bracket_default = "(";
const char* matrix_input_list_second_row_bracket_default = ")";
const char* matrix_input_list_col_separator_default = ",";
const char* matrix_output_aligned_left_col_default = "||";
const char* matrix_output_aligned_right_col_default = "||";
const char* matrix_output_aligned_inter_col_default = " ";

}


#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
