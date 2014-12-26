/*****************************************************************************

    test/matrix_ops.hpp

    This file is a part of the Arageli library test base.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2008 Sergey S. Lyalin

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

/** \file matrix_ops.h
    \brief Operations with the matrix.

    Contains extended operations with matrix object that is necessary for testing
    purposes.
*/

#ifndef __ta_matrix_ops_hpp__
#define __ta_matrix_ops_hpp__


#include "stdafx.hpp"


using namespace Arageli;

/// Create matrix in \f$T^{n \times m}\f$ with random elements not grater than max
template <typename T, bool REFCNT>
matrix<T, REFCNT> rand_matrix(size_t m, size_t n, const T& max)
{
    typedef set::grid1<T> int_grid;
    typedef rnd::equiprob<int_grid> rnd_int_grid;

    // store seed to make different matrices for each call of this function
    static typename rnd_int_grid::seed_type local_seed = 0;
    ++local_seed;

    int_grid int_0_max(0, max); // integer set from [0, max]
    rnd::equiprob<int_grid> gen(local_seed, int_0_max);   // random generator from [0, max]

    matrix<T, REFCNT> res(m, n, fromsize);
    for (typename matrix<T>::size_type i = 0; i < res.nrows(); i++)
        for (typename matrix<T>::size_type j = 0; j < res.ncols(); j++)
            res.el(i, j) = gen();
    return res;
}

/**
Create upper diagonal matrix \f$M \in T^{n \times n}\f$ with element 1 on the
diagonal and with random elements the not grater than max on other
positions.
Note that \f$M\f$ is in hermite form and \f$det(M)=1\f$.
*/
template <typename T, bool REFCNT>
matrix<T, REFCNT> rand_matrix_upper_diagonal(size_t n, const T& max)
{
    typedef set::grid1<T> int_grid;
    typedef rnd::equiprob<int_grid> rnd_int_grid;

    // store seed to make different matrices for each call of this function
    static typename rnd_int_grid::seed_type local_seed = 0;
    ++local_seed;

    int_grid int_0_max(0, max); // integer set from [0, max]
    rnd::equiprob<int_grid> gen(local_seed, int_0_max);   // random generator from [0, max]

    matrix<T, REFCNT> res(n, eye);
    for (typename matrix<T>::size_type i = 0; i < res.nrows(); i++)
        for (typename matrix<T>::size_type j = i+1; j < res.ncols(); j++)
            res.el(i, j) = gen();
    return res;
}

/**
Shuffles input matrix \f$m\f$ by multiplying it on the upper diagonal
random matrix acquired from rand_matrix_upper_diagonal(m.ncols(), max) call.
Note that this operation does not change determinant of the source matrix.
*/
template <typename T, bool REFCNT>
void shuffle(matrix<T, REFCNT> &m, const T& max)
{
    m *= rand_matrix_upper_diagonal(m.ncols(), max);
}


template <typename M>
inline void output_map (const M& m)
{
    for(typename M::const_iterator i = m.begin(); i != m.end(); ++i)
        tout << "[" << i->first << "] = " << i->second << "\n";
}


#endif /*__ta_matrix_ops_hpp__*/
/* End of file matrix_ops.hpp */
