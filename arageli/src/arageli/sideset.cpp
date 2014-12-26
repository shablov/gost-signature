/*****************************************************************************

    sideset.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2006--2010 Sergey S. Lyalin

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
    \file sideset.cpp
    \brief See description for sideset.hpp file.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_sideset)

// REFERENCE ADDITIONAL HEADERS HERE

#include "setenumrnd/integer.hpp"
#include "setenumrnd/vector.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "sideset.hpp"
#include "exception.hpp"


namespace Arageli
{


template <typename C, typename VVV>
VVV& sides_structure_cone (const C& c, VVV& v)
{
    // Using the brute force: counting all subsets of the rays set
    // and for each one determining if it is a side and what its dimention is.

    // We find each side, determine its dimention and store it
    // in corresponding entry of v as a set of base rays (numbers of them).

    ARAGELI_ERROR
    (
        "It is known that there are inputs for which this funciton works incorrectly: sides_structure_cone.\n"
        "If you are sure that you need to call this function anyway, please comment the lines that produces "
        "this error message. "
    );

    c.normalize_all();    // make relation matrix minimal
    matrix<bool> rel = c.relation();

    typedef typename VVV::element_type VV;
    typedef typename VV::element_type V;
    typedef typename VVV::size_type size_type;
    typedef vector<int, false> Vecsubset;    // bool indicators to mark any side subset
    typedef set::inonnegative<int> Subsetel;    // bool indicator to mark a side
    typedef set::vector_fixed_size<Vecsubset, Subsetel> Vecset;    // all subsets of the rays set

    Subsetel subsetel(1);    // {0, 1}
    Vecset vecset(rel.nrows(), subsetel);

    size_type subsp_offset = c.max_embedded_basis().nrows();    // maximum subspace dimention

    // WARNING! TEMPORARY LIMITATION.
    ARAGELI_ASSERT_0(subsp_offset == 0);

    v.assign_fromval(c.dim() + 1, VV());
    v.back().push_back(V(rel.nrows(), fromsize));    // count the cone itself
    for(size_type i = 0; i < rel.nrows(); ++i)
        v.back().front()[i] = i;

    v[subsp_offset].assign_fromval(1, V());    // count the side with minimal dimention -- max subspace

    for(enumer::fast<Vecset> rayenum(vecset); !rayenum.is_end(); ++rayenum)
    {
        V rays;
        for(size_type i = 0; i < rayenum->size(); ++i)
            if((*rayenum)[i])
                rays.push_back(i);

        size_type n = rays.size();
        if(n == 0)
            continue;
        if(n == 1)
        {
            v[subsp_offset + 1].push_back(rays);    // can be taken out of the cycle
            continue;
        }

        // collecting common zeros
        V cz;

        for(size_type i = 0; i < rel.ncols(); ++i)
        {
            bool allcolzeros = true;    // indicator that all subcolumn contains only zeros
            for(size_type j = 0; j < rel.nrows(); ++j)
                if((*rayenum)[j])    // only for marked rays
                    if(rel(j, i))
                    {
                        allcolzeros = false;
                        break;
                    }

            if(allcolzeros)
                cz.push_back(i);
        }

        // check if found rays form a side

        bool allrowzeros = true;

        for(size_type i = 0; i < rel.nrows(); ++i)
        {
            if((*rayenum)[i])
                continue;
            allrowzeros = true;
            for(size_type j = 0; j < cz.size(); ++j)
                if(rel(i, cz[j]))
                {
                    allrowzeros = false;
                    break;
                }

            if(allrowzeros)
                break;    // this rays don't form a side
        }

        if(allrowzeros)
            continue;

        // Here cz has facet numbers which crossing gives a hyperplane
        // that contains considered side. So, we can evaluate the side
        // dimention from the number of elements in cz.

        v[v.size() - cz.size() - 1].push_back(rays);
    }

    return v;
}


template <typename C, typename V>
V& f_vector_cone (const C& c, V& v)
{
    typedef vector<vector<vector<typename V::size_type> > > VVV;
    VVV vvv;
    sides_structure_cone(c, vvv);
    v.resize(vvv.size());
    for(typename VVV::size_type i = 0; i < vvv.size(); ++i)
        v[i] = vvv[i].size();
    return v;
}


}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
