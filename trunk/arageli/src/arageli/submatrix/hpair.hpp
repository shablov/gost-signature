/*****************************************************************************

    submatrix/hpair.hpp -- Structures for representation of matrix consisting
        two matrices as blocks placed horizontaly.

    This file is a part of Aragali library.

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
    \file
*/


#ifndef _ARAGELI_submatrix_hpair_hpp_
#define _ARAGELI_submatrix_hpair_hpp_

#include "../config.hpp"

#include "../frwrddecl.hpp"

#include "../std_import.hpp"


namespace Arageli
{

/// Represents horizontal pair (M1|M2) of two matrices M1 and M2 as a whole matrix.
/** This class keeps a references to incopsulated matrices and represent their as
    a whole solid matrix. */
template <typename M1, typename M2 = M1>
class hpair_matrix
{
    // TODO: Complete the class implementation.

    M1* m1; M2* m2;

public:

    // TODO: select appropriate size_type from M1 or M2 (not only from M1)
    typedef typename M1::size_type size_type;

    // TODO: select appropriate element_type from M1 or M2 (not only from M1)
    typedef typename M1::element_type element_type;

    hpair_matrix () :
        m1(0),
        m2(0)
    {}

    hpair_matrix (M1& m1a, M2& m2a) :
        m1(&m1a),
        m2(&m2a)
    {
        ARAGELI_ASSERT_0(m1 && m2);
        ARAGELI_ASSERT_0(m1->nrows() == m2->nrows());
    }


    size_type nrows () const
    {
        ARAGELI_ASSERT_0(m1 && m2);
        ARAGELI_ASSERT_0(m1->nrows() == m2->nrows());

        return m1->nrows();
    }


    size_type ncols () const
    {
        ARAGELI_ASSERT_0(m1 && m2);
        ARAGELI_ASSERT_0(m1->nrows() == m2->nrows());

        return m1->ncols() + m2->ncols();
    }


    // TODO: Need to use several operator(), for example, for submatrix
    const element_type& operator() (size_type i, size_type j) const
    {
        ARAGELI_ASSERT_0(m1 && m2);
        ARAGELI_ASSERT_0(m1->nrows() == m2->nrows());

        if(j < m1->ncols())
            return (*m1)(i, j);
        else
            return (*m2)(i, j - m1->ncols());
    }


    element_type& operator() (size_type i, size_type j)
    {
        ARAGELI_ASSERT_0(m1 && m2);
        ARAGELI_ASSERT_0(m1->nrows() == m2->nrows());

        if(j < m1->ncols())
            return (*m1)(i, j);
        else
            return (*m2)(i, j - m1->ncols());
    }


    template <typename T>
    void mult_row (size_type i, const T& x)
    {
        ARAGELI_ASSERT_0(m1 && m2);
        ARAGELI_ASSERT_0(m1->nrows() == m2->nrows());

        m1->mult_row(i, x);
        m2->mult_row(i, x);
    }


    template <typename T>
    void div_row (size_type i, const T& x)
    {
        ARAGELI_ASSERT_0(m1 && m2);
        ARAGELI_ASSERT_0(m1->nrows() == m2->nrows());

        m1->div_row(i, x);
        m2->div_row(i, x);
    }


    template <typename T>
    void addmult_rows (size_type i, size_type j, const T& x)
    {
        ARAGELI_ASSERT_0(m1 && m2);
        ARAGELI_ASSERT_0(m1->nrows() == m2->nrows());

        m1->addmult_rows(i, j, x);
        m2->addmult_rows(i, j, x);
    }


    void swap_rows (size_type i, size_type j)
    {
        ARAGELI_ASSERT_0(m1 && m2);
        ARAGELI_ASSERT_0(m1->nrows() == m2->nrows());

        m1->swap_rows(i, j);
        m2->swap_rows(i, j);
    }

    vector<element_type> copy_row (size_type i)
    {
        ARAGELI_ASSERT_0(m1 && m2);
        ARAGELI_ASSERT_0(m1->nrows() == m2->nrows());

        vector<element_type> res = m1->copy_row(i), t = m2->copy_row(i);
        res.insert_fromseq(res.size(), t.size(), t.begin());
        return res;
    }

};

}


//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SUBMATRIX_HPAIR
//    #include "hpair.cpp"
//    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SUBMATRIX_HPAIR
//#endif


#endif

