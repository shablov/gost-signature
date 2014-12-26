/*****************************************************************************

    triangulation.cpp -- See declarations in triangulation.hpp.

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
    \file
    See description for triangulation.hpp file.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_triangulation)

#include <stack>

#include "submatrix/indexed.hpp"
#include "vector.hpp"

#include "triangulation.hpp"


namespace Arageli
{

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4018)
#endif

template
<
    typename Q,
    typename Dim1,
    typename TR,
    typename Dim2,
    typename Ctrler
>
void triangulate_simple_1
(
    const Q& q,
    const Dim1& dim,
    TR& tr,
    const Dim2& subspdim,
    Ctrler ctrler
)
{
    typedef typename Q::size_type Idx;
    typedef indexed_submatrix<const Q> SubQ;

    ctrler.preamble(q, dim, subspdim);    // CTRLER

    SubQ curq(&q, fromall);
    vector<Idx, false> cursimplex;
    Idx curineq = 0;
    vector<Idx, false> erasedrows;
    Idx curdim = dim;

    // stack of pairs (number of inequation, corresponding q matrix)
    typedef std::pair<Idx, SubQ> IdxAndSubQ;
    std::stack<IdxAndSubQ> stackq;

    for(;;)
    {
        if(curq.is_empty())
        {
            ctrler.before_subspace(subspdim);    // CTRLER

            // Add subsimplexes from a subspace (if any).
            for(Idx i = 0; i <= subspdim; ++i)
            {
                for(Idx j = 0; j <= subspdim; ++j)
                    if(i != j)
                        cursimplex.push_back(j + q.nrows());
                if(tr.ncols() < cursimplex.size())
                    tr.assign();
                if(tr.ncols() <= cursimplex.size())
                {
                    tr.insert_row(tr.nrows(), cursimplex);
                    ctrler.new_simplex(cursimplex, tr, curq, q);    // CTRLER
                }
                cursimplex.erase(cursimplex.size() - subspdim, subspdim);
            }

            ctrler.after_subspace(subspdim);    // CTRLER
            curineq = curq.ncols();
        }

        for(; curineq < curq.ncols() && is_null(curq(0, curineq)); ++curineq);

        if(curineq < curq.ncols())
        {
            erasedrows.push_back(0);

            for(Idx i = 1; i < curq.nrows(); ++i)
                if(!is_null(curq(i, curineq)))
                    erasedrows.push_back(i);

            if(erasedrows.size() < curq.nrows() || curq.nrows() == 1)
            {
                // Reduction of the matrix q to the next facet
                // that is not incidented to the first ray.

                cursimplex.push_back(curq.row_index()[0]);
                ctrler.select_ray(0, curq, q);    // CTRLER
                ctrler.select_facet(curineq, curq, q);    // CTRLER
                stackq.push(IdxAndSubQ(curineq, curq));
                curq.row_index().erase_subvector(erasedrows);
                curq.col_index().erase(curineq);
                curineq = 0;
                --curdim;

                ctrler.before_eliminating_ineqs(curq, q);    // CTRLER

                for(Idx j = 0; j < curq.ncols();)
                {
                    Idx zeros = 0;
                    for(Idx i = 0; i < curq.nrows(); ++i)
                        zeros += is_null(curq(i, j));

                    if(zeros < curdim-subspdim-1)
                        curq.col_index().erase(j);
                    else
                        ++j;
                }

                ctrler.after_eliminating_ineqs(curq, q);    // CTRLER
            }
            else
            {
                ++curineq;
            }

            erasedrows.assign();
        }
        else
        {
            if(stackq.empty())
                break;    // end of triangulation process
            curineq = stackq.top().first + 1;    // move to the next inequation
            curq = stackq.top().second;
            stackq.pop();
            ARAGELI_ASSERT_1(!cursimplex.is_empty());
            cursimplex.pop_back();
            ++curdim;
        }
    }

    ctrler.conclusion(q, tr);
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
