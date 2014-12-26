/*****************************************************************************

    intconvex.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2006--2007 Sergey S. Lyalin

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
    \file intconvex.cpp
    \brief The intconvex.hpp file stuff implementation.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_intconvex)

#include <limits>

#include "type_traits.hpp"
#include "exception.hpp"
#include "big_int.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "gauss.hpp"
#include "skeleton.hpp"
#include "solve_linsys.hpp"
#include "triangulation.hpp"


#include "intconvex.hpp"


namespace Arageli
{

template <typename Gen, typename IntGen>
void intconvex_simple (const Gen& gen, IntGen& intgen)
{
    // Note, there is a difference between notation used here and
    // in the source of the idea (Emelichev V.A., Kovalev M.M...).
    // In the book a polyhedron is extended to a cone by adding the last
    // component in coordinate. But here we extend by the first component.

    ///////////////////////////////////////////////////////////
    //std::cout << "\nDebug output for intconvex_simple.";
    ///////////////////////////////////////////////////////////

    ARAGELI_ASSERT_0(gen.ncols() > 0);

    typedef typename Gen::element_type T;
    typedef typename Gen::size_type size_type;
    typedef vector<T, false> Vec;
    typedef vector<size_type, false> Idxvec;

    ARAGELI_ASSERT_0(type_traits<T>::is_integer_number);    // temporary limitation

    size_type d = gen.ncols()-1;    // dimention of the space with polyhedron

    ///////////////////////////////////////////////////////////
    //std::cout << "\nd = " << d;
    ///////////////////////////////////////////////////////////


    // Separate vertices and directions of recession in gen.

    Gen verts;    // firstly, here the vertices from gen will be stored
    Idxvec reces;    // indeces of the directions of recession in gen
    intgen.assign(0, gen.ncols());

    for(size_type i = 0; i < gen.nrows(); ++i)
    {
        ARAGELI_ASSERT_0(!is_negative(gen(i, 0)));
        if(!is_null(gen(i, 0)))    // if i-th generatrix is a vertex
            verts.insert_row(verts.nrows(), gen.copy_row(i));
        else    // if i-th generatirx is a direction of recession
        {
            reces.push_back(i);
            intgen.insert_row(intgen.nrows(), gen.copy_row(i));
        }
    }

    ///////////////////////////////////////////////////////////
    //std::cout
    //    << "\nverts.nrows() = " << verts.nrows()
    //    << "\nreces.size() = " << reces.size()
    //    << "\nreces = " << reces;
    ///////////////////////////////////////////////////////////


    // Build in verts parametric representaion (points) of an intersection
    // of set Q' = closing(union(Q, q^1, ..., q^t)) (p. 107, bottom)
    // and hyperplane x0 = 1.

    ARAGELI_ASSERT_0
    (
        verts.nrows()*power(big_int(2), reces.size()) <=
        big_int(std::numeric_limits<size_type>::max())
    );

    vector<bool, false> reces_mask(reces.size());
    size_type nreccomb = power(size_type(2), reces_mask.size());    // WARNING! Why not pow?
    size_type norigverts = verts.nrows();
    //verts.reserve(norigverts*nreccomb, verts.ncols());

    // pass the first combination because already done:
    if(!reces_mask.is_empty())
        reces_mask[0] = true;

    #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
    ///////////////////////////////////////////////////////////
    std::cout << "\nnreccomb = " << nreccomb;
    ///////////////////////////////////////////////////////////
    #endif

    for(size_type i = 1; i < nreccomb; ++i)    // along to all possible reces_mask
    {
        Vec reccomb(verts.ncols());
        for(size_type j = 0; j < reces_mask.size(); ++j)
            if(reces_mask[j])
                reccomb += gen.copy_row(reces[j]);

        for(size_type j = 0; j < norigverts; ++j)
            verts.insert_row
            (
                verts.nrows(),
                verts.copy_row(j) + verts(j, 0)*reccomb
            );

        // move to the next combination
        for(size_type j = 0; j < reces_mask.size(); ++j)
            if(reces_mask[j])
                reces_mask[j] = false;
            else
            {
                reces_mask[j] = true;
                break;
            }
    }


    // Build minimal implicit representation of the polytope
    // with vertices verts.
    // At the same time system verts is being partially reduced.

    ///////////////////////////////////////////////////////////
    //std::cout
    //    << "\nbefore skeleton: verts.nrows() = " << verts.nrows();
    ///////////////////////////////////////////////////////////

    Gen ineqs, q, eqs;
    skeleton(verts, ineqs, q, eqs);

    ///////////////////////////////////////////////////////////
    //std::cout
    //    << "\nafter skeleton: verts.nrows() = " << verts.nrows()
    //    << "\nineqs.nrows() = " << ineqs.nrows()
    //    << "\neqs.nrows() = " << eqs.nrows();
    ///////////////////////////////////////////////////////////


    // Build bounding box for set G (1.6).

    Vec minlim(d), maxlim(d);    // limits of the bounding box

    for(size_type i = 0; i < verts.nrows(); ++i)
    {
        Vec v = verts.copy_row(i);
        v /= safe_reference(v.front());    // note, round to zero

        for(size_type j = 0; j < minlim.size(); ++j)
        {
            if(v[j+1] < minlim[j])
                minlim[j] = v[j+1];
            if(v[j+1] > maxlim[j])
                maxlim[j] = v[j+1];
        }
    }

    ///////////////////////////////////////////////////////////
    //std::cout
    //    << "\nminlim = " << minlim
    //    << "\nmaxlim = " << maxlim
    //    << "\nsize box = " << maxlim - minlim + 1;
    ///////////////////////////////////////////////////////////


    // Look at any integer point in the bounding box (BB) and determine
    // whether G includes it with the help of the system of inequation ineqs.
    // Add all such points to intgen.

    // the number of integer points in the bounding box
    T nbb = product(maxlim - minlim + unit<T>());

    ///////////////////////////////////////////////////////////
    //std::cout << "\nnbb = " << nbb;
    ///////////////////////////////////////////////////////////

    Vec point = minlim;
    point.push_front(unit<T>());    // the first component is always 1

    for(T i = 0; i < nbb; ++i)    // through all integer points in the BB
    {
        // determine if G includes this point
        if
        (
            all_greater_equal(ineqs*point, null<T>()) &&
            is_null(eqs*point)
        )
            intgen.insert_row(intgen.nrows(), point);

        // move to the next point in BB
        for(size_type j = 1; j < point.size(); ++j)
            if(point[j] == maxlim[j-1])
                point[j] = minlim[j-1];
            else
            {
                ++point[j];
                break;
            }
    }

    ///////////////////////////////////////////////////////////
    //std::cout << "\nintgen.nrows() = " << intgen.nrows();
    ///////////////////////////////////////////////////////////
}

#undef ARAGELI_INLINE_DEBUG_OUTPUT


template <typename A, typename V, typename Rows>
nei_result near_extreme_intpoint (const A& a, V& v, Rows& rows)
{
    ARAGELI_ASSERT_0(a.is_square());
    ARAGELI_ASSERT_0(!a.is_empty());

    typedef typename A::size_type size_type;
    typedef typename A::element_type T;

    vector<size_type, false> nonzeros;
    bool nonint = false, isint = false;
    for(size_type i = 0; i < a.nrows(); ++i)
    {
        ARAGELI_ASSERT_0(!is_negative(a(i, 0)));

        if(!is_null(a(i, 0)))
        {
            if(!is_unit(a(i, 0)))
                nonint = true;
            else
                isint = true;
            nonzeros.push_back(i);
        }
        else isint = true;
    }

    ARAGELI_ASSERT_0(!nonzeros.is_empty());

    if(!nonint)
        return NEIR_ALL_VERTS;

    matrix<T, false> gx;
    vector<T, false> order;

    // WARNING! This will be calculated in solve_linsys_integer_mod.
    T d = abs(det(a));

    ARAGELI_ASSERT_0(!is_null(d));

    if(solve_linsys_integer_mod(transpose(a), d, gx, order) == SLR_UNIQUE)
        return isint ? NEIR_AMONG_VERTS : NEIR_EMPTY;
    else
    {
        vector<T, false> beta(order.size());
        beta[0] = 1;
        order -= unit<T>();
        vector<T, false> p;
        vector<T, false> eq = a.copy_col(0);

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
        //////////////////////////////////////////////////////////////////////
        std::cout << "\nnear_extreme_intpoint not empty search...\n";
        big_int _debug_all, _debug_eq, _debug_product, _debug_sum;
        //////////////////////////////////////////////////////////////////////
        #endif

        while(!is_null(beta))
        {
            p = gx*beta % d;

            #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
            /////////////////////////////////////////////////////////////////
            ++_debug_all;
            if(dotprod(p, eq) == d)
                ++_debug_eq;
            if(product(p + unit<T>()) <= d)
                ++_debug_product;
            if(sum(p) < d)
                ++_debug_sum;
            /////////////////////////////////////////////////////////////////
            #endif

            if
            (
                dotprod(p, eq) == d &&
                /*product(p + unit<T>())<= d && */
                sum(p) < d
            )
            {
                v = p*a;
                ARAGELI_ASSERT_1(is_divisible(v, d));
                v /= d;
                for(size_type i = 0; i < p.size(); ++i)
                    if(!is_null(p[i]))
                        rows.push_back(i);

                ARAGELI_ASSERT_1(rows.size() > 1);

                #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
                //////////////////////////////////////////////////////////////////////
                std::cout
                    << "There is a new point = " << v
                    << "\n_debug_all = " << _debug_all
                    << "\n_debug_eq = " << _debug_eq
                    << "\n_debug_product = " << _debug_product
                    << "\n_debug_sum = " << _debug_sum << "\n";
                //////////////////////////////////////////////////////////////////////
                #endif

                return NEIR_NEW;
            }


            // move to the next combination
            for(size_type j = 0; j < order.size(); ++j)
                if(beta[j] == order[j])
                    beta[j] = null<T>();
                else
                {
                    ++beta[j];
                    break;
                }
        }

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
        //////////////////////////////////////////////////////////////////////
        std::cout
            << "There is a new point = " << v
            << "\n_debug_all = " << _debug_all
            << "\n_debug_eq = " << _debug_eq
            << "\n_debug_product = " << _debug_product
            << "\n_debug_sum = " << _debug_sum << "\n";
        //////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////
        std::cout << "There are no new points.\n";
        //////////////////////////////////////////////////////////////////////
        #endif

        return isint ? NEIR_AMONG_VERTS : NEIR_EMPTY;
    }

}

//#define ARAGELI_INLINE_DEBUG_OUTPUT


template <typename Gen, typename TR, typename IntIneq, typename Ctrler>
void intconvex_triangulation
(
    const Gen& gen,
    const TR& tr,
    IntIneq& intineq,
    Ctrler ctrler
)
{
    typedef typename TR::element_type Index;
    typedef typename Gen::element_type T;
    typedef typename Gen::size_type size_type;

    std::list<vector<Index> > trq;

    for(size_type i = 0; i < tr.nrows(); ++i)
        trq.push_back(tr.copy_row(i));

    matrix<T, false> allgen = gen;

    #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
    /////////////////////////////////////////////////////////////////
    //ctrler.stream() << "\nintconvex_triangulation:\n";
    //ctrler.stream() << "\n:\n";
    //output_aligned(ctrler.stream() << "\ngen =\n", gen);
    //output_aligned(ctrler.stream() << "\ntr =\n", tr);
    /////////////////////////////////////////////////////////////////
    #endif

    while(!trq.empty())
    {
        vector<Index> simplex = trq.front();
        trq.pop_front();
        vector<T, false> v;
        vector<Index, false> rows;

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
        //////////////////////////////////////////////////////////
        //ctrler.stream() << "\nLook at simplex " << simplex;
        output_aligned(ctrler.stream() << "\nРассмотрим симплекс:\n", allgen.copy_rows(simplex));
        ctrler.stream() << "определитель = " << det(allgen.copy_rows(simplex));
        //output_aligned(ctrler.stream() << "\nsimplex matrix =\n", allgen.copy_rows(simplex));
        //////////////////////////////////////////////////////////
        #endif

        if(NEIR_NEW == near_extreme_intpoint(allgen.copy_rows(simplex), v, rows))
        {
            // new point
            allgen.insert_row(allgen.nrows(), v);

            // new simpleces
            for(size_type i = 0; i < rows.size(); ++i)
            {
                trq.push_back(simplex);
                trq.back()[rows[i]] = allgen.nrows() - 1;
            }

            #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
            //////////////////////////////////////////////////////////
            //ctrler.stream() << "\nNew point " << v << ", rows = " << rows;
            ctrler.stream() << "\nНовая точка под номером " << allgen.nrows()-1 << " = " << v
                << ",\nкоэфф. комбинации строк = " << rows;
            //////////////////////////////////////////////////////////
            #endif
        }

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
        //////////////////////////////////////////////////////////
        else
        {
            //ctrler.stream() << "\nThere are no new points.";
            ctrler.stream() << "\nНет новых целых точек.";
        }
        //////////////////////////////////////////////////////////
        #endif
    }

    #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
    //////////////////////////////////////////////////////////
    //output_aligned(ctrler.stream() << "\nallgen before erasing =\n", allgen);
    //////////////////////////////////////////////////////////
    #endif

    // Erase in allgen all non-integer points.

    vector<size_type, false> erased;
    bool has_vertexray = false;
    for(size_type i = 0; i < allgen.nrows(); ++i)
    {
        if(allgen(i, 0) > unit<T>())
            erased.push_back(i);
        if(!is_null(allgen(i, 0)))
            has_vertexray = true;
    }

    if(has_vertexray)
        allgen.erase_rows(erased);
    else
    {
        // The polihedron has no integer points.
        intineq.assign_fromsize(2, allgen.ncols());
        intineq(0, 0) = unit<T>();
        intineq(0, 0) = opposite_unit<T>();
    }

    #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
    //////////////////////////////////////////////////////////
    //output_aligned(ctrler.stream() << "\nallgen after erasing =\n", allgen);
    {
        vector<vector<big_int>, false> vvv;
        for(size_t i = 0; i < allgen.nrows(); ++i)
            vvv.push_back(allgen.copy_row(i));
        std::sort(vvv);
        vvv.erase(std::unique(vvv), vvv.end());
        matrix<big_int> uallgen;
        for(size_t i = 0; i < vvv.size(); ++i)
            uallgen.insert_row(uallgen.nrows(), vvv[i]);
        output_aligned(ctrler.stream() << "\nВсе целые точки, подозрительные на 'крайность', и рец.напр.: \n", uallgen);
    }
    //////////////////////////////////////////////////////////
    #endif

    // Build a convex hull as a inequation system.
    matrix<T, false> q, e;
    skeleton(allgen, intineq, q, e);

    if(!e.is_empty())
    {
        intineq.insert_matrix_bottom(e);
        vector<T, false> lastrow = e.copy_row(0);
        for(size_type i = 1; i < e.nrows(); ++i)
            lastrow += e.copy_row(i);
        opposite(&lastrow);
        intineq.insert_row(intineq.nrows(), lastrow);
    }


    #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
    //////////////////////////////////////////////////////////
    //output_aligned(ctrler.stream() << "\nobtainted inequations =\n", intineq);
    //////////////////////////////////////////////////////////
    #endif
}


template <typename Gen, typename Q, typename Dim, typename IntIneq>
void intconvex_triangulate_simple_1
(
    const Gen& gen,
    const Q& q,
    const Dim& dim,
    IntIneq& intineq
)
{
    matrix<typename Gen::size_type, false> tr;
    triangulate_simple_1(q, dim, tr);
    intconvex_triangulation(gen, tr, intineq);
}

template
<
    typename Alpha,
    typename Gamma,
    typename Delta,
    typename V,
    typename Ctrler
>
void near_extreme_2d_mod
(
    Alpha alpha,
    Gamma gamma,
    Delta delta,
    V& v,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(gamma < delta);
    ARAGELI_ASSERT_0(!is_negative(gamma));

    ARAGELI_ASSERT_0
    (
        -delta/2 < alpha && alpha <= delta/2 ||
        alpha >= null(alpha) && alpha < delta
    );

    if(alpha > delta/2)
        alpha -= delta;

    #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
    /////////////////////////////////////////////
    ctrler.stream()
        << "\nx" << (is_negative(alpha) ? " - " : " + ") << abs(alpha) << "*y = "
        << gamma << " (mod " << delta << ")\n";
    /////////////////////////////////////////////
    #endif

    if(is_null(gamma))
    {
        // {(0, 0)}

        v.resize(1, 2);
        v(0, 0) = null(alpha);
        v(0, 1) = v(0, 0);

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
        ////////////////////////////////////////////////////////////
        ctrler.stream() << v;
        ////////////////////////////////////////////////////////////
        #endif

        return;
    }

    v.resize(0, 2);

    if(is_null(alpha))
    {
        // {(gamma, 0)}

        v.resize(1, 2);
        v(0, 0) = gamma;
        v(0, 1) = null(alpha);

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
        ////////////////////////////////////////////////////////////
        ctrler.stream() << v;
        ////////////////////////////////////////////////////////////
        #endif

        return;
    }

    if(is_unit(alpha))
    {
        // {(gamma, 0), (0, gamma)}

        v.resize(2, 2);
        v(0, 0) = gamma;
        v(0, 1) = null(alpha);
        v(1, 0) = v(0, 1);
        v(1, 1) = v(0, 0);

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
        ////////////////////////////////////////////////////////////
        ctrler.stream() << v;
        ////////////////////////////////////////////////////////////
        #endif

        return;
    }

    if(is_opposite_unit(alpha))
    {
        // {(gamma, 0), (0, delta - gamma)}

        v.resize(2, 2);
        v(0, 0) = gamma;
        v(0, 1) = null(alpha);
        v(1, 0) = v(0, 1);
        v(1, 1) = delta - gamma;

        #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
        ////////////////////////////////////////////////////////////
        ctrler.stream() << v;
        ////////////////////////////////////////////////////////////
        #endif

        return;
    }

    ARAGELI_ASSERT_1(alpha > unit(alpha) || alpha < opposite_unit(alpha));

    Alpha oldalpha = alpha;
    Gamma oldgamma = gamma;
    Delta olddelta = delta;

    v.resize(1, 2);
    v(0, 0) = gamma;
    v(0, 1) = null(alpha);

    #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
    ////////////////////////////////////////////////////////////
    ctrler.stream() << v;
    ////////////////////////////////////////////////////////////
    #endif

    if(alpha > unit(alpha))
    {
        gamma = mod(gamma, alpha);
        delta = mod(-delta, alpha);
        swap(alpha, delta);

        matrix<typename V::element_type, false> vv;
        near_extreme_2d_mod(alpha, gamma, delta, vv, ctrler);    // WARNING! Recursion.

        ARAGELI_ASSERT_1
        (
            is_divisible
            (
                vv.copy_col(1)*olddelta + oldgamma - vv.copy_col(0),
                oldalpha
            )
        );

        vv.assign_col
        (
            1,
            (vv.copy_col(1)*olddelta + oldgamma - vv.copy_col(0))/oldalpha
        );

        v.insert_matrix_bottom(vv);
    }
    else
    {
        //ctrler.stream() << "alpha < -1 isn't implemented yet\n";

        opposite(&alpha);
        gamma = mod(gamma - delta, alpha);
        delta = mod(delta, alpha);
        swap(alpha, delta);

        matrix<typename V::element_type, false> vv;
        near_extreme_2d_mod(alpha, gamma, delta, vv, ctrler);    // WARNING! Recursion.

        /*
        output_aligned(ctrler.stream() << "\nvv =\n", vv);
        ctrler.stream() << "a = " << (vv.copy_col(1) + unit(olddelta))*olddelta - oldgamma + vv.copy_col(0);
        ctrler.stream() << "\na/alpha = " << -((vv.copy_col(1) + unit(olddelta))*olddelta - oldgamma + vv.copy_col(0))/oldalpha;
        ctrler.stream() << "\noldalpha = " << oldalpha;
        ctrler.stream() << "\noldgamma = " << oldgamma;
        ctrler.stream() << "\nolddelta = " << olddelta;
        */

        ARAGELI_ASSERT_1
        (
            is_divisible
            (
                (vv.copy_col(1) + unit(olddelta))*olddelta - oldgamma + vv.copy_col(0),
                oldalpha
            )
        );

        vv.assign_col
        (
            1,
            -((vv.copy_col(1) + unit(olddelta))*olddelta - oldgamma + vv.copy_col(0))/
                oldalpha
        );

        v.insert_matrix_bottom(vv);
    }
}

//#define ARAGELI_INLINE_DEBUG_OUTPUT

template <typename A, typename B, typename VD, typename Delta, typename V, typename Ctrler>
void _near_extreme_2_mod
(A a, B b, const VD& vd, const Delta& delta, V& v, Ctrler ctrler)
{
    ARAGELI_ASSERT_0(a.ncols() >= a.nrows());
    ARAGELI_ASSERT_0(a.nrows() == b.size());
    ARAGELI_ASSERT_0(vd.size() == b.size());
    ARAGELI_ASSERT_0(is_positive(delta));
    ARAGELI_ASSERT_0(a.nrows() == 1);
    ARAGELI_ASSERT_0(a.ncols() == 2);

    a.mod_row(0, vd[0]);
    b[0] = mod(b[0], vd[0]);

    #ifdef ARAGELI_INLINE_DEBUG_OUTPUT
    /////////////////////////////////////////////
    ctrler.stream()
        << "\n" << a(0, 0) << "*x' + " << a(0, 1) << "*y' = "
        << b[0] << " (mod " << vd[0] << ")\n";
    /////////////////////////////////////////////
    #endif

    if(is_unit(a(0, 0)))
        near_extreme_2d_mod(mod(a(0, 1), vd[0]), mod(b[0], vd[0]), vd[0], v, ctrler);
    else if(is_opposite_unit(a(0, 0)))
        near_extreme_2d_mod(mod(-a(0, 1), vd[0]), mod(-b[0], vd[0]), vd[0], v, ctrler);
    else if(is_unit(a(0, 1)))
    {
        near_extreme_2d_mod(mod(a(0, 0), vd[0]), mod(b[0], vd[0]), vd[0], v, ctrler);
        v.swap_cols(0, 1);
    }
    else if(is_opposite_unit(a(0, 1)))
    {
        near_extreme_2d_mod(mod(-a(0, 0), vd[0]), mod(-b[0], vd[0]), vd[0], v, ctrler);
        v.swap_cols(0, 1);
    }
    else if(!is_null(a(0, 0)))
    {
        near_extreme_2d_mod(div_mod(a(0, 1), a(0, 0), vd[0]), div_mod(b[0], a(0, 0), vd[0]), vd[0], v, ctrler);
    }
    else if(!is_null(a(0, 1)))
    {
        near_extreme_2d_mod(div_mod(a(0, 0), a(0, 1), vd[0]), div_mod(b[0], a(0, 1), vd[0]), vd[0], v, ctrler);
        v.swap_cols(0, 1);
    }
}

template
<
    typename A,
    typename B,
    typename VD,
    typename Delta,
    typename V,
    typename Ctrler
>
void near_extreme_2_3_mod
(
    const A& a,
    const B& b,
    const VD& vd,
    const Delta& delta,
    V& v,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(a.ncols() >= a.nrows());
    ARAGELI_ASSERT_0(a.nrows() == b.size());
    ARAGELI_ASSERT_0(vd.size() == b.size());
    ARAGELI_ASSERT_0(is_positive(delta));
    ARAGELI_ASSERT_0(a.ncols() <= 3 && a.ncols() >= 2);
    ARAGELI_ASSERT_0(a.nrows() == 1);

    if(a.ncols() == 2)
    {
        _near_extreme_2_mod(a, b, vd, delta, v, ctrler);
    }
    else // a.ncols() == 3
    {
        Delta bound = pow(double(delta), 1.0/3) - 1;
        typedef typename A::size_type size_type;
        ctrler.stream() << "\nСуществует i, v[i] <= " << bound << " <= " << pow(double(delta), 1.0/3) - 1;

        v.resize(0, 3);
        for(size_type i = 0; i < 3; ++i)
            for(Delta vi = null(bound); vi <= bound; ++vi)
            {
                A ta = a;
                ctrler.stream() << "\nv[" << i+1 << "] = " << vi;
                ta(0, i) *= vi;
                B tb = b;
                tb[0] -= ta(0, i);
                ta.erase_col(i);
                V vv;
                _near_extreme_2_mod(ta, tb, vd, delta, vv, ctrler);
                vv.insert_col(i, vector<Delta>(vv.nrows(), vi, fromval));
                v.insert_matrix_bottom(vv);
            }
    }

    {
        vector<vector<big_int>, false> vvv;
        for(size_t i = 0; i < v.nrows(); ++i)
            vvv.push_back(v.copy_row(i));
        std::sort(vvv);
        vvv.erase(std::unique(vvv), vvv.end());
        v.resize(0, v.ncols());
        for(size_t i = 0; i < vvv.size(); ++i)
            v.insert_row(v.nrows(), vvv[i]);
        output_aligned(ctrler.stream() << "\nВсе потенциально крайние точки: \n", v);
    }
}


template
<
    typename A,
    typename B,
    typename VD,
    typename Delta,
    typename Beta,
    typename Res,
    typename Ctrler
>
void minimize_2_3_mod
(
    const A& a,
    const B& b,
    const VD& vd,
    const Delta& delta,
    const Beta& beta,
    Res& res,
    Ctrler ctrler
)
{
    matrix<typename A::element_type> v;
    near_extreme_2_3_mod(a, b, vd, delta, v, ctrler);
    Beta vals = v*beta;
    output_aligned(ctrler.stream() << "\nЗначения целевой функции, соотв. точкам: \n", vals);
    typename Beta::iterator i = std::min_element(vals.begin(), vals.end());
    ctrler.stream() << "\nМинимальное значение: " << *i;
    ctrler.stream() << "\nСоотв. точка: " << (res = v.copy_row(i - vals.begin()));
}


}


#if ARAGELI_DEBUG_LEVEL > 3

namespace Arageli
{

template void intconvex_simple
(
    const matrix<big_int, true>& gen,
    matrix<big_int, true>& intgen
);


template void intconvex_simple
(
    const matrix<big_int, false>& gen,
    matrix<big_int, false>& intgen
);


template void intconvex_simple
(
    const matrix<int, true>& gen,
    matrix<int, true>& intgen
);


template void intconvex_simple
(
    const matrix<int, false>& gen,
    matrix<int, false>& intgen
);


template nei_result near_extreme_intpoint
(
    const matrix<big_int>& a,
    vector<big_int>& v,
    vector<std::size_t>& rows
);


template nei_result near_extreme_intpoint
(
    const matrix<int>& a,
    vector<int>& v,
    vector<std::size_t>& rows
);


template void intconvex_triangulation
(
    const matrix<big_int>& gen,
    const matrix<std::size_t>& tr,
    matrix<big_int>& intineq
);


template void intconvex_triangulation
(
    const matrix<int>& gen,
    const matrix<std::size_t>& tr,
    matrix<int>& intineq
);


template void intconvex_triangulate_simple_1
(
    const matrix<big_int>& gen,
    const matrix<bool>& q,
    const big_int& dim,
    matrix<big_int>& intineq
);

template void intconvex_triangulate_simple_1
(
    const matrix<big_int>& gen,
    const matrix<bool>& q,
    const std::size_t& dim,
    matrix<big_int>& intineq
);

template void intconvex_triangulate_simple_1
(
    const matrix<int>& gen,
    const matrix<int>& q,
    const std::size_t& dim,
    matrix<int>& intineq
);


}

#endif


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
