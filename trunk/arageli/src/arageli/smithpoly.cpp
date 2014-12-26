/*****************************************************************************

    smithpoly.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2005 Andrey Somsikov

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

#if defined(ARAGELI_USE_COUNTERS)
#include "counter.hpp"
#endif

#include <cstddef>
#include <vector>

#include "vector.hpp"

//#define GCD_LOG

namespace std
{
template <class T>
std::ostream& operator << (std::ostream & s, std::vector<T> &v)
{
    std::size_t i = v.size();
    s << "(";
    do
    {
        i--;
        s << v[i] << ", ";
    } while(i != 0);
    s << ")";
    return s;
}
}

// The following block was commented by Sergey Lyalin.
/*
template <class T>
std::ostream& operator << (std::ostream & s, Arageli::vector<T> &v)
{
    std::size_t i = v.size();
    s << "(";
    do {
        i--;
        s << v[i] << ", ";
    } while(i != 0);
    s << ")";
    return s;
}
*/

namespace Arageli
{
#ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smithpoly
#if defined(ARAGELI_USE_COUNTERS)
    CCounter counter_big_int_sum_GC;
    CCounter counter_big_int_mul_GC;
    CCounter counter_big_int_div_GC;
#endif
#endif
};

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smithpoly)

#include "smithpoly.hpp"
#include "hermite.hpp"
#include "sparse_polynom.hpp"
#include <stdlib.h>
#include <iostream>

namespace Arageli
{

template <typename T>
T normalize(const T &value)
{
    if (0 == value.size())
        return value;
    T res;
    T::coef_type d = value.leading_monom().coef();
    for
    (
        T::monom_const_iterator it = value.monoms_begin();
        it != value.monoms_end();
        it++
    )
        res += *it / T::monom(d);
    return res;
}

template <typename P>
P gcd_gauss(P &f, P &g, P *u = NULL, P *v = NULL)
{
    typedef typename P::coef_type PT;

    #ifdef USE_POLYGCD_COUNTER
        counter_polygcd_count++;
        counter_polygcd_maxdeg += f.deg();
        counter_polygcd_maxdeg += g.deg();
    #endif

    #ifdef GCD_LOG
        std::cout << "START GCD COMPUTATION\n"
            << "f=" << f << "\n"
            << "g=" << g << "\n";
    #endif

    //    if (P(factory<PT>::null()) == f || P(factory<PT>::null()) == g)

    if
    (
        is_null(f.degree()) ||
        is_null(g.degree())
    )
    {
        if (is_null(f) && !is_null(g.degree()))
            return normalize(g);
        else if(is_null(g) && !is_null(f.degree()))
            return normalize(f);

        return P(factory<PT>::unit());
    }
        // should fill u, v;
    std::size_t i, j, k;
    std::size_t dSize = f.degree() + g.degree();
    matrix<PT> m(dSize, dSize);
    std::vector<PT> d(dSize);
    vector<std::size_t> uvColIndex(dSize);

    P::degree_type commonMult =
        std::min
        (
            f.monoms_begin()->degree(),
            g.monoms_begin()->degree()
        );
    /*
    commonMult = 0;
    while (is_null(f[commonMult]) && is_null(g[commonMult]))
        commonMult++;
    */

    for (i = 0; i < m.nrows(); i++)
        for (j = 0; j < m.ncols(); j++)
            m(i, j) = factory<PT>::null();

    P::monom_iterator it;

    it = f.monoms_begin();
    for (i = 0; i < f.degree()+1; i++)
    {
        P::coef_type coef;
        if (it->degree() == P::degree_type(i))
        {
            coef = it->coef();
            it++;
        }
        else
            coef = factory<P::coef_type>::null();
        for (j = 0; j < g.degree(); j++)
            m(i + j, j) = coef;
    }
    it = g.monoms_begin();
    for (i = 0; i < g.degree()+1; i++)
    {
        P::coef_type coef;
        if (it->degree() == P::degree_type(i))
        {
            coef = it->coef();
            it++;
        }
        else
            coef = factory<P::coef_type>::null();
        for (j = 0; j < f.degree(); j++)
            m(i + j, g.degree() + j) = coef;
    }

    #ifdef GCD_LOG
        std::cout << "m = \n";
        output_aligned(std::cout, m, "|| ", " ||", "  ");
        std::cout << "\n";
    #endif

    matrix<PT> mTmp = m;

    PT mMult = factory<PT>::unit();
    i = m.nrows() - 1; // row
    for (;;)
    {
        // find left most non zero element in row i
        for (j = 0; j < m.ncols(); j++)
            if (!is_null(m.el(i, j)))
                break;
        if (m.ncols() == j)
        {
            i++;
            // deg(d)=m.nrows()- i; coefficients of
            //the u(x) and v(x) can be found from linear system
            break;
        }
        uvColIndex[i] = j;
        // d = (0, ..., 0, d_i:=1, 0, ..., 0)

        #ifdef GCD_LOG
            std::cout << "leading column = " << j << "\n";
        #endif

        for (k = 0; k < i; k++)
        {
            PT tmp = factory<PT>::opposite_unit()*m.el(k, j);
            m.mult_row(k, m.el(i, j));
            m.addmult_rows(k, i, tmp);
            m.div_row(k, mMult);
            d[k] = tmp/mMult;
        }
        d[i] = factory<PT>::unit();
        for (k = i+1; k < m.nrows(); k++)
        {
            PT tmp = factory<PT>::opposite_unit()*m.el(k, j);
            m.mult_row(k, m.el(i, j));
            m.addmult_rows(k, i, tmp);
            m.div_row(k, mMult);
            d[k] = tmp/mMult;
        }

        #ifdef GCD_LOG
            std::cout << "m = \n";
            output_aligned(std::cout, m, "|| ", " ||", "  ");
            std::cout << "\n";
            std::cout << "d = " << d << "\n";
        #endif

        mMult = m.el(i, j);

        if (commonMult == i)
            return P(factory<PT>::unit()) * P(P::monom(factory<PT>::unit(), commonMult));
        //break;
        i--;
    }

    vector<PT> uv(dSize);
    for (j = 0; j < dSize; j++)
        uv[j] = factory<PT>::null();
    for (j = i; j < dSize; j++)
        uv[uvColIndex[j]] = d[j]/m.el(j, uvColIndex[j]);

    for (j = 0; j < dSize; j++)
    {
        PT val = factory<PT>::null();
        for (i = 0; i < dSize; i++)
            val += mTmp.el(j, i) * uv[i];
        d[j] = val; // !!! should be d = mTmp*uv
    }

    #ifdef GCD_LOG
        std::cout << "uvColIndex = " << uvColIndex << "\n";
        std::cout << "uv = " << uv << "\n";
        std::cout << "d = " << d << "\n";
    #endif

    P dNorm;
    for (i = 0; i < d.size(); i++)
        dNorm += P::monom(d[i], i);//= P(d, true);
    normalize(dNorm);

    #ifdef GCD_LOG
        std::cout << "dNorm = " << dNorm << "\n";
    #endif

    #if defined(ARAGELI_USE_COUNTERS)
        counter_big_int.suspend();
        counter_big_int_sum.suspend();
        counter_big_int_mul.suspend();
        counter_big_int_div.suspend();
    #endif

    if
    (
        f.degree() < dNorm.degree() ||
        g.degree() < dNorm.degree() ||
        P(factory<PT>::null()) != f % dNorm ||
        P(factory<PT>::null()) != g % dNorm
    )
    {

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
            std::cout << "gcd check fail:\n" <<
                "f=" << f << "\n" <<
                "g=" << g << "\n" <<
                "f % dNorm=" << f % dNorm << "\n" <<
                "g % dNorm=" << g % dNorm << "\n" <<
                "dNorm=" << dNorm << "\n";
        #endif

        throw "gcd check fail";
    }

    #if defined(ARAGELI_USE_COUNTERS)
        counter_big_int.resume();
        counter_big_int_sum.resume();
        counter_big_int_mul.resume();
        counter_big_int_div.resume();
    #endif

/*
    if (NULL != u)
    {
        vector<T> uData();
        u->initLe(uv.);
    }
*/
    if (0 == d.size())
        return P(factory<PT>::unit());

    P res;
    for (i = 0; i < d.size(); i++)
        res += P::monom(d[i], i);//= P(d, true);
    normalize(res);

    return res;
}

template <typename T>
vector<T> makeGoodConditioning(matrix<T>& b, std::size_t col)
{
    typedef matrix<T> M;
    typedef typename M::value_type MV;
    typedef typename M::size_type size_type;

    size_type i, j;
    vector<MV> res(b.ncols() - col);
    /*
    std::cout << "GC\n";
    output_aligned(std::cout, b, "|| ", " ||", "  ");
    std::cout << "\n";
    std::cout << "col = " << col << "\n";
    */
    for (j = col + 1; j < b.ncols(); j++)
    {
        MV gTarget = b.el(col, col);
        for (i = col+1; i < b.nrows(); i++)
        {
            //std::cout << "gTarget" << gTarget << "b" << b.el(i, col) << "\n";
            gTarget = gcd_gauss(gTarget, b.el(i, col));
        }
        MV g(gTarget);
        for (i = col; i < b.nrows(); i++)
        {
            //std::cout << "gTarget" << gTarget << "b" << b.el(i, j) << "\n";
            gTarget = gcd_gauss(gTarget, b.el(i, j));
        }
        MV a = factory<MV>::null();
        while (g != gTarget)
        {
            a+=factory<MV>::unit();
            b.addmult_cols(col, j, factory<MV>::unit());
            g = b.el(col, col);
            for (i = col+1; i < b.nrows(); i++)
                g = gcd_gauss(g, b.el(i, col));
            //std::cout << "gTarget" << gTarget << " g" << g << " b=\n";
            //output_aligned(std::cout, b, "|| ", " ||", "  ");
            //std::cout << "\n";
            //std::cout << "a = " << a << "\n";
        }
        res[j-1 - col] = a;
    }
    return res;
}

template <typename T>
matrix<T> smithPoly_rand(const matrix<T>& m)
{
    typedef matrix<T> M;
    typedef typename M::value_type MV;
    typedef typename M::size_type size_type;

    size_type step = 0;
    size_type i, j;
    bool nonZeroFound;
    M s(m);
    M u(m.nrows(), eye);
    M v(m.ncols(), eye);

    for (;;)
    {
        //        std::cout << step++ << " ";
        size_type d = 0;
        size_type mSize = s.nrows();
        for (i = 0; i < s.nrows(); i++)
            for(j = 0; j < s.ncols(); j++)
                // TO DO: use max function here
                if(d < s.el(i, j).degree())
                    d = s.el(i, j).degree();
        MV maxRand(100000);
        maxRand *= MV(2)*MV(d)*MV(mSize)*MV(mSize)*MV(mSize);

        M c(s.nrows(), eye);
        for (i = 0; i < c.nrows(); i++)
            for(j = i+1; j < c.ncols(); j++)
                c.el(i, j) = rand(maxRand);  // WARNING! Deprecated rand is used.

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
            std::cout << step << " preconditioning: \n";
            output_aligned(std::cout, c, "|| ", " ||", "  ");
        #endif

        s = s*c;
        s = hermite(s);

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
            std::cout << "after ref: \n";
            output_aligned(std::cout, s, "|| ", " ||", "  ");
        #endif

        for (i = 0; i < s.nrows(); i++)
        {
            MV::monom::coef_type tmp(s.el(i, i).leading_monom().coef());
            for(j = i; j < s.ncols(); j++)
                if (!is_null(s.el(i, j)))
                    s.el(i, j) /= tmp;
        }

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
            std::cout << "after ref normalized: \n";
            output_aligned(std::cout, s, "|| ", " ||", "  ");
        #endif

        s.transpose();
        s = hermite(s);
        s.transpose();

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
            std::cout << "after cef: \n";
            output_aligned(std::cout, s, "|| ", " ||", "  ");
        #endif

        step++;

        bool isInSmith = true;
        for (i = 0; i < s.nrows(); i++)
            for(j = 0; j < s.ncols(); j++)
                if (i != j && !is_null(s.el(i, j)))
                {
                    isInSmith = false;
                    break;
                }
        if (!isInSmith)
            continue;
        for (i = 1; i < s.nrows(); i++)
        {
            if (!is_null(s.el(i, i) % s.el(i-1, i-1)))
            {
                isInSmith = false;
                break;
            }
        }
        if (isInSmith)
            break;
    }

    return s;
}

template <typename T, bool REFCNT>
matrix<T, REFCNT> smithPoly_GC(const matrix<T, REFCNT>& m)
{
    typedef matrix<T, REFCNT> M;
    typedef typename M::value_type MV;
    typedef typename M::size_type size_type;

    size_type i, j;
    bool nonZeroFound;
    matrix<T, REFCNT> s(m);
    matrix<T, REFCNT> u(m.nrows(), eye);
    matrix<T, REFCNT> v(m.ncols(), eye);

    j = 0;
    for (;;)
    {
        /*
        std::cout << "value=\n";
        output_aligned(std::cout, s, "|| ", " ||", "  ");
        std::cout << "\nu=\n";
        output_aligned(std::cout, u, "|| ", " ||", "  ");
        std::cout << "\nv=\n";
        output_aligned(std::cout, v, "|| ", " ||", "  ");
        std::cout << "\n";
        */
        #if defined(ARAGELI_USE_COUNTERS)
            std::size_t sum = counter_big_int_sum.getSum();
            std::size_t mul = counter_big_int_mul.getSum();
            std::size_t div = counter_big_int_div.getSum();
        #endif

        vector<T> gc = makeGoodConditioning(s, j);

        #if defined(ARAGELI_USE_COUNTERS)
            counter_big_int_sum_GC += counter_big_int_sum.getSum() - sum;
            counter_big_int_mul_GC += counter_big_int_mul.getSum() - mul;
            counter_big_int_div_GC += counter_big_int_div.getSum() - div;
        #endif
        /*
        for (i = 0; i < j; i++)
            v.el(i, j) = factory<MV>::null();
        v.el(j, j) = factory<MV>::unit();
        */
        for (i = j+1; i < s.nrows(); i++)
            v.el(i, j) += gc[i-(j+1)];
        //std::cout << "gc " << j << ":\n";
        //output_aligned(std::cout, s, "|| ", " ||", "  ");
        //std::cout << "\n";

        size_type iPivot = j;
        T valPivot = s.el(j, j);
        for (i = j+1; i < s.nrows(); i++)
        {
            //std::cout << s.el(i, j) << "\n";
            if
            (
                (s.el(i, j).degree() < valPivot.degree() || is_null(valPivot)) &&
                !is_null(s.el(i, j))
            )
            {
                iPivot = i;
                valPivot = s.el(i, j);
            }
        }
        //std::cout << "pivot:" << iPivot << ", " << j << "=" << valPivot << "\n";
        s.swap_rows(j, iPivot);
        u.swap_rows(j, iPivot);

        nonZeroFound = false;
        for (i = j+1; i < s.nrows(); i++)
        {
            if (!is_null(s.el(i, j)))
            {
                T mult(factory<MV>::opposite_unit() * (s.el(i, j) / s.el(j, j)));
                //std::cout << "step" << i << "," << j << "*" << mult << ": \n";
                //output_aligned(std::cout, s, "|| ", " ||", "  ");
                //std::cout << "\n";
                s.addmult_rows(i, j, mult);
                u.addmult_rows(i, j, mult);
                nonZeroFound = true;
            }
        }
        if (nonZeroFound)
            continue;

        j++;
        if (j >= s.ncols())
            break;
    }

    for (i = 0; i < s.nrows(); i++)
    {
        for (j = i+1; j < s.ncols(); j++)
        {
            T mult(factory<MV>::opposite_unit() * (s.el(i, j) / s.el(i, i)));
            s.addmult_cols(j, i, mult);
            v.addmult_cols(j, i, mult);
        }
    }

/*
if (s != u*m*v)
{
    std::cout << "check failed\n";
    output_aligned(std::cout, u, "|| ", " ||", "  ");
    output_aligned(std::cout, m, "|| ", " ||", "  ");
    output_aligned(std::cout, v, "|| ", " ||", "  ");
    output_aligned(std::cout, u*m*v, "|| ", " ||", "  ");
    throw "check failed";
}
*/

    return s;
}

typedef std::pair<std::size_t, std::size_t> TSmithPivot;

template <class T>
TSmithPivot findSmithPivot(const matrix<T> &m, std::size_t row, std::size_t col)
{
    typedef matrix<T> M;
    typedef typename M::value_type MV;
    std::size_t rowMin = row;
    std::size_t colMin = col;
    T valueMin = m.el(rowMin, colMin);
    for (std::size_t i = row; i < m.nrows(); i++)
        for (std::size_t j = col; j < m.ncols(); j++)
        {
            if
            (
                (m.el(i, j).degree() < valueMin.degree() || is_null(valueMin)) &&
                !is_null(m.el(i, j))
            )
            {
                rowMin = i;
                colMin = j;
                valueMin = m.el(i, j);
            }
        }
    return TSmithPivot(rowMin, colMin);
}

template <typename T>
matrix<T> smithPoly_std(const matrix<T> &m)
{
    typedef matrix<T> M;
    typedef typename M::value_type MV;
    typedef typename M::size_type size_type;

    bool nonZeroFound;
    matrix<T> s(m);
    matrix<T> u(m.nrows(), eye);
    matrix<T> v(m.ncols(), eye);

    size_type i, j;
    size_type k = 0;
    for (;;)
    {
        //        tout << s << det(s) << " = det\n";
        TSmithPivot pivot = findSmithPivot<T>(s, k, k);
        if (!(k == pivot.first && k == pivot.second))
        {
            s.swap_rows(k, pivot.first);
            u.swap_rows(k, pivot.first);
            s.swap_cols(k, pivot.second);
            v.swap_cols(k, pivot.second);
        }
        /*
        if (s.el(k, k) < factory<MV>::null())
        {
            s.mult_row(k, factory<MV>::opposite_unit());
            u.mult_row(k, factory<MV>::opposite_unit());
        }
        */

        nonZeroFound = false;
        for (i = k+1; i < s.nrows(); i++)
        {
            if (!is_null(s.el(i, k)))
            {
                //T tmp(s.el(i, k) / s.el(k, k));
                //T mult(factory<MV>::opposite_unit() * tmp);
                T mult(factory<MV>::opposite_unit() * (s.el(i, k) / s.el(k, k)));
                s.addmult_rows(i, k, mult);
                u.addmult_rows(i, k, mult);
                nonZeroFound = true;
            }
        }
        if (nonZeroFound)
            continue;

        nonZeroFound = false;
        for (i = k+1; i < s.ncols(); i++)
        {
            if (!is_null(s.el(k, i)))
            {
                T mult(factory<MV>::opposite_unit() * (s.el(k, i) / s.el(k, k)));
                s.addmult_cols(i, k, mult);
                v.addmult_cols(i, k, mult);
                nonZeroFound = true;
            }
        }
        if (nonZeroFound)
            continue;

        bool nonDivFound = false;
        for (i = k; i < s.nrows(); i++)
        {
            for (j = k; j < s.ncols(); j++)
                if (!is_null(s.el(i,j) % s.el(k,k)))
                {
                    nonDivFound = true;
                    s.addmult_cols(k, j, factory<MV>::unit());
                    v.addmult_cols(k, j, factory<MV>::unit());
                    break;
                }
            if (nonDivFound)
                break;
        }

        if (!nonDivFound)
        {
            k++;
            if (k == s.nrows() || k == s.ncols())
                break;
        }
    }

    /*
    #if defined(ARAGELI_USE_COUNTERS)
        counter_big_int.suspend();
        counter_big_int_sum.suspend();
        counter_big_int_mul.suspend();
        counter_big_int_div.suspend();
    #endif
        if (s != u*m*v)
        {
            std::cout << "check failed\n";
            std::cout << u << m << v << u*m*v;
            throw "check failed";
        }
    #if defined(ARAGELI_USE_COUNTERS)
        counter_big_int.resume();
        counter_big_int_sum.resume();
        counter_big_int_mul.resume();
        counter_big_int_div.resume();
    #endif
    */

    return s;
}

} // namespace Arageli

#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
