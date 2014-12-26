/*****************************************************************************

    smith/near_optimal.cpp -- See declarations in smith_near_optimal.hpp.

    This file is a part of the Arageli library.

    Copyright (C) 2006 Anna Bestaeva

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
    See description for smith_near_optimal.hpp file.
*/


#include "../config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smith_near_optimal)


#include <cstddef>
#include <algorithm>

#include "../exception.hpp"
#include "../std_import.hpp"
#include "../factory.hpp"
#include "../cmp.hpp"
#include "../intalg.hpp"
#include "../gcd.hpp"
#include "../powerest.hpp"
#include "../bareiss.hpp"
#include "../hermite.hpp"
#include "../vector.hpp"
#include "../matrix.hpp"

#include "near_optimal.hpp"


namespace Arageli
{


namespace _Internal
{

// WARNING! All functions in this piece of namespace _Internal take
// explicit vector and matrix types.
// TODO: Rewrite with less special types.

//////////////////////////////////////////////
//                                          //
//        COMPUTING A SMITH CONDITIONER     //
//                                          //
//////////////////////////////////////////////
/**
    Input:
    - S = diag(NDFS(A))
    - V is unimodular over Z/Z(N) such that
    A*V is left equivalent to NDFS(A) over Z/Z(N)

    Returns matrices C, R such that:
    - C is unit lower triangular
    - R is unit upper triangular
    - A*C*R is left equivalent to NDFS(A) over Z
*/
template
<
    typename T_A,
    bool REFCNT_A,
    typename T_S,
    bool REFCNT_S,
    typename T_V,
    bool REFCNT_V,
    typename T_C,
    bool REFCNT_C,
    typename T_R,
    bool REFCNT_R,
    typename T_N
>
void smith_conditioner
(
    const matrix<T_A, REFCNT_A>& A,
    const vector<T_S, REFCNT_S>& S,      /*    S = diag(NDFS(A))                                    */
    matrix<T_V, REFCNT_V>& V,            /*    V: A*V is left equivalent to NDFS(A) over Z/Z(N)    */
    matrix<T_C, REFCNT_C>& C,            /*    C: A*V is left equivalent to TFS(A)    over Z            */
    matrix<T_R, REFCNT_R>& R,            /*    R: TFS(A)*R is NDFS(A) over Z                        */
    const T_N& N
)
{
    ARAGELI_ASSERT_0(is_positive(N));

    matrix<T_A, false> H;
    std::size_t n = S.size();
    R.assign_eye(n);

    V.transpose();
    cond_gauss(V, C, N);
    C.transpose();            /*    C is smith conditioner for A */
    for(std::size_t i = 1; i < n; i++)
        for(std::size_t j = 0; j < i; j++)
        {
            T_C alpha = S[i]/S[j]*prquot(C(i, j), S[i]/S[j]);
            C.addmult_cols(j, i, -alpha);
        }

        /*    C(i, j) < S[i]/S[j] for i > j */

    H = hermite(A*C);        /*    H is triangular smith form for A */

    for(std::size_t i = 0; i < n - 1; i++)
        for(std::size_t j = i + 1; j < n; j++)
        {
            T_R alpha = prquot(H(i, j), H(i, i));
            R.addmult_cols(j, i, -alpha);
        }
        /* R is unimodular rigth transform
            such that H*R is NDFS(A)
        */
    for(std::size_t i = n - 1; i > 0; i--)
    for(std::size_t j = i; j < n; j++)
    {
        T_R alpha = S[j]/S[i - 1]*prquot(R(i - 1, j), S[j]/S[i - 1]);
        R.addmult_cols(j, i - 1, -alpha);
    }

    /*    R(i, j) < S[j]/S[i] for i < j */
}

//////////////////////////////////////////
//                                      //
//        DIAGONALIZATION OVER Z/Z(N)   //
//                                      //
//////////////////////////////////////////


template
<
    typename T_G,
    bool REFCNT_G,
    typename T_S,
    bool REFCNT_S,
    typename T_V,
    bool REFCNT_V,
    typename T_N
>
void triangular_to_smith
(
    const matrix<T_G, REFCNT_G>& G,
    matrix<T_S, REFCNT_S>& S,
    matrix<T_V, REFCNT_V>& V,
    const T_N& N
)
{
    V.assign_eye(G.ncols());
    S = G;
    triangular_to_bidiagonal(S, V, N);
    bidiagonal_to_smith(S, V, N);
}


//////////////////////////////////////////////
//                                          //
//        TRIANGULARIZATION OVER Z/Z(N)     //
//                                          //
//////////////////////////////////////////////


template
<
    typename T_A,
    bool REFCNT_A,
    typename T_V,
    bool REFCNT_V,
    typename T_N
>
void rcef
(
    matrix<T_A, REFCNT_A>& A,
    matrix<T_V, REFCNT_V>& V,
    const T_N& N,
    typename matrix<T_A, REFCNT_A>::size_type r1,
    typename matrix<T_A, REFCNT_A>::size_type r2,
    typename matrix<T_A, REFCNT_A>::size_type k1,
    typename matrix<T_A, REFCNT_A>::size_type k2
)
{
    for(std::size_t i = r1, k = k1; i < r2 && k < k2; i++)
    {
        for(std::size_t j = k + 1; j < k2; j++)
        {
            T_A u, v, w, z;
            gcdex(A(i, k), A(i, j), N, u, v, w, z);

            vector<T_A, false> A_col_k = A.copy_col(k);
            A.addmult_cols(k, u, j, v);
            A.mod_col(k, N);
            A.addmult_cols(j, z, A_col_k, w);
            A.mod_col(j, N);

            vector<T_V, false> V_col_k = V.copy_col(k);
            V.addmult_cols(k, u, j, v);
            V.mod_col(k, N);
            V.addmult_cols(j, z, V_col_k, w);
            V.mod_col(j, N);
        }

        if(!is_null(A(i, k)))
            k++;
    }

    for(std::size_t i = r1, k = k1; i < r2 && k < k2; i++)
        if(!is_null(A(i, k)))
        {
            T_A c = unit(A(i, k), N);
            A.mult_col(k, c);
            A.mod_col(k, N);
            V.mult_col(k, c);
            V.mod_col(k, N);
            k++;
        }
}

template
<
    typename T_A,
    bool REFCNT_A,
    typename T_N
>
void rref
(
    matrix<T_A, REFCNT_A>& A,
    const T_N& N,
    typename matrix<T_A, REFCNT_A>::size_type r1,
    typename matrix<T_A, REFCNT_A>::size_type r2,
    typename matrix<T_A, REFCNT_A>::size_type k1,
    typename matrix<T_A, REFCNT_A>::size_type k2
)
{
    for(std::size_t j = k1, r = r1; j < k2 && r < r2; j ++)
    {
        for(std::size_t i = r + 1; i < r2; i ++)
        {
            T_A u, v, w, z;
            gcdex(A(r, j), A(i, j), N, u, v, w, z);

            vector<T_A, REFCNT_A> A_row_r = A.copy_row(r);;
            A.addmult_rows(r, u, i, v); A.mod_row(r, N);
            A.addmult_rows(i, z, A_row_r, w); A.mod_row(i, N);
        }

        if(!is_null(A(r, j)))
            r++;
    }

    for(std::size_t j = k1, r = r1; j < k2 && r < r2; j ++)
        if(!is_null(A(r, j)))
        {
            T_A c = unit(A(r, j), N);
            A.mult_row(r, c);
            A.mod_row(r, N);
            r++;
        }
}


//////////////////////////////////////////////////////
//                                                    //
//        MODIFIED GAUSS TRANSFORM OVER Z/Z(N)        //
//                                                    //
//////////////////////////////////////////////////////
/**
    Returns modified gauss form of matrix A
    Requirement: A must be unimodular over Z/Z(N).
*/
template
<
    typename T_A,
    bool REFCNT_A,
    typename T_B,
    bool REFCNT_B,
    typename T_N
>
void cond_gauss
(
    const matrix<T_A, REFCNT_A>& A,
    matrix<T_B, REFCNT_B>& B,
    const T_N& N
)
{
    ARAGELI_ASSERT_0(is_unit(gcd(mod(det_brs(A), N), N)));

    std::size_t n = A.ncols(), m = A.nrows();
    T_B d = unit(d);
    B = A;
    for(std::size_t i = 0, j = 0; i < m && j < n; j++)
    {
        for(std::size_t ii = i + 1; ii < m; ii++)
            if(!is_null(B(ii, j)))
            {
                T_B
                    a = prrem(B(i, j), N),
                    b = prrem(B(ii, j), N),
                    c = /*split*/stab_mod(a, b, N);
                B.addmult_rows(i, ii, c);
            }

        B.mod_row(i, N);
        T_B pivot = B(i, j);

        for(std::size_t ii = i + 1; ii < m; ii++)
        {
            T_B alpha = B(ii, j);
            B.mult_row(ii, pivot);
            B.addmult_rows(ii, j,  -alpha);
            for(std::size_t jj = 0; jj < n; jj++)
                B(ii, jj) = div_mod(B(ii, jj), d, N);
        }

        d = B(i, j);
        for(std::size_t jj = 0; jj < n; jj++)
            B(i, jj) = div_mod(B(i, jj), d, N);
        i++;
    }

    ARAGELI_ASSERT_1(is_unit(product(diagonal(B))));
}

/**
    Returns inversion of matrix A
    Requirement: A must be unit lower triangular.
*/
template <typename T, bool REFCNT>
matrix<T, REFCNT> inverse_ULT (const matrix<T, REFCNT>& A)
{
    std::size_t m = A.nrows();
    matrix<T, false> B = A;
    matrix<T, true> P;
    P.assign_eye(m);

    for(std::size_t r = 0; r < m; r++)
        for(std::size_t i = r + 1; i < m; i++)
        {
            T alpha = B(i, r);
            B.addmult_rows(i, r, -alpha);
            P.addmult_rows(i, r, -alpha);
        }

    return P;
}

/**
    Returns inversion of matrix A
    Requirement: A must be unit upper triangular.
*/
template <typename T, bool REFCNT>
matrix<T, REFCNT> inverse_UUT(const matrix<T, REFCNT>& A)
{
    std::size_t n = A.ncols();
    matrix<T, false> B = A;
    matrix<T, true> Q;
    Q.assign_eye(n);


    for(std::size_t r = 0; r < n; r++)
        for(std::size_t j = r + 1; j < n; j++)
        {
            T alpha = B(r, j);
            B.addmult_cols(j, r, -alpha);
            Q.addmult_cols(j, r, -alpha);
        }

    return Q;
}




template<typename T, bool REFCNT>
matrix<T, REFCNT> bott_pasting(matrix<T, REFCNT>&A, matrix<T, REFCNT> &B)
{
    matrix<T, REFCNT> Res = A;

    for(std::size_t i = 0; i < B.nrows(); i++)
        Res.insert_row(A.nrows() + i, B.copy_row(i));

    return Res;
}

template<typename T, bool REFCNT>
matrix<T, REFCNT> left_pasting(matrix<T, REFCNT>&A, matrix<T, REFCNT> &B)
{
    matrix<T, REFCNT> Res = A;

    for(std::size_t i = 0; i < B.ncols(); i++)
        Res.insert_col(A.ncols() + i, B.copy_col(i));

    return Res;
}

template<typename T, bool REFCNT>
vector<T, REFCNT> diagonal(matrix<T, REFCNT>& A)
{    vector<T, REFCNT> d(0);

    for(std::size_t i = 0; i < std::min(A.nrows(), A.ncols()); i ++)
        if(is_null(A(i, i)))
            break;
        else
            d.push_back(A(i, i));

    return d;
}


} // namespace _Internal


// ---------------------------------------------------------------------------


template
<
    typename MA,
    typename MS,
    typename MP,
    typename MQ,
    typename MU,
    typename MV,
    typename Rank,
    typename T_det
>
void smith_near_optimal
(
    const MA& A,
    MS& S,  MP& P,  MQ& Q,
    MU& U,  MV& V,
    Rank& rank, T_det& det
)
{
    typedef typename MA::element_type T_A;
    typedef typename MU::element_type T_U;
    typedef typename MV::element_type T_V;

    matrix<T_U, false> E, E1, M;
    matrix<T_V, false> F, F1, N, C, R;
    matrix<T_A, false> PAQ, H1, H2, G, B;

    std::size_t  m = A.nrows(), n = A.ncols();
    T_A  detH1, detH2;

    bareiss_pq(A, P, Q, rank, det);
    PAQ = P*A*Q;

    vector<std::size_t> basis(rank);
    for(std::size_t i = 0; i < rank; i++)
        basis[i] = i;

    PAQ = P*A*Q;
    B = PAQ.copy_submatrix(basis, basis);

    hermite_upper(PAQ, H1, M);

    if(m > rank)
        H1.erase_rows(rank, m - rank);
    if(n > rank)
        H1.erase_cols(rank, n - rank);    /*    [H1|*] is left hermite basis for PAQ    */
    E1 = M.take_rows(basis);                    /*    E1: E1*PAQ = [H1|*]                        */
    detH1 = product(_Internal::diagonal(H1));                /*    M is basis left null space for PAQ        */

    if(n == rank)
    {
        F1.assign_eye(rank);
        H2 = B;
        detH2 = abs(det);
        G = E1*PAQ;
    }
    else
    {
        hermite_lower(PAQ, H2, N);
        if(m > rank)
            H2.erase_rows(rank, m - rank);
        if(n > rank)
            H2.erase_cols(rank, n - rank);    /*    [H2|*].transpose() is rigth hermite basis for PAQ    */
        F1 = N.take_cols(basis);                    /*    F1: PAQ*F1 = [H2|*].transpose()                        */
        detH2 = product(_Internal::diagonal(H2));                /*    N is basis rigth null space for PAQ                    */
        G = E1*PAQ*F1;
        _Internal::rref(G, 2*detH2, 0, rank, 0, rank);
    }

    _Internal::triangular_to_smith(G, S, V, 2*detH2);                    /*    G*V is left equivalent to S over Z/Z(2detH2)*/
    _Internal::smith_conditioner(G, _Internal::diagonal(S), V, C, R, 2*detH2);    /*    S is NDFS(G) over Z/Z(2detH2) and Z            */

    F = F1*C*R;
    E = S*_Internal::inverse_UUT(R)*_Internal::inverse_ULT(C)*adjoint(H2)*B*adjoint(H1)*E1;
    E /= detH1*detH2;                                        /*    E, F : E*PAQ*F = S - NDFS(G) over Z    */

    if(m > rank)
        U = _Internal::bott_pasting(E, M);
    else
        U = E;
    if(n > rank)
        V = _Internal::left_pasting(F, N);
    else
        V = F;

    S.insert_rows(rank, m - rank, null(S(0, 0)));
    S.insert_cols(rank, n - rank, null(S(0, 0)));            /*    U, V : E*PAQ*F = S - NDFS(A) over Z    */

    ARAGELI_ASSERT_1(U*P*A*Q*V == S);
    ARAGELI_ASSERT_1(is_unit(abs(det_brs(U))));
    ARAGELI_ASSERT_1(is_unit(abs(det_brs(V))));
}


template
<
    typename MA,
    typename MV,
    typename T_N
>
void triangular_to_bidiagonal
(
    MA &A,
    MV &V,
    const T_N& N
)
{
    typedef typename MA::element_type T_A;
    typedef typename MV::element_type T_V;

    std::size_t n = A.ncols(), b = n;
    while(b > 2)
    {
        std::size_t n1 = b/2 + b - 1,
        n2 = 2*(b - 1),
        s1 = b/2,
        s2 = b - 1;
        for(std::size_t i = 0; i < n/s1; i++)
        {
            std::size_t spos = i*s1,
            mpos = std::min(n, spos + s1),
            epos = std::min(n, spos + n1);
            //_Internal::rcef(A, V, N, spos, mpos, mpos, epos);
            for(std::size_t q = spos, k = mpos; q < mpos && k < epos; q++)
            {
                for(std::size_t j = k + 1; j < epos; j++)
                {
                    T_A u, v, w, z;
                    gcdex(A(q, k), A(q, j), N, u, v, w, z);

                    for(std::size_t ii = spos; ii < epos; ii++)
                    {
                        T_A temp = A(ii, k);
                        A(ii, k) = mod(u*temp + v*A(ii, j), N);
                        A(ii, j) = mod(w*temp + z*A(ii, j), N);
                    }
                    vector<T_V, false> V_col_k = V.copy_col(k);
                    V.addmult_cols(k, u, j, v); V.mod_col(k, N);
                    V.addmult_cols(j, z, V_col_k, w); V.mod_col(j, N);
                }

                if(!is_null(A(q, k)))
                    k++;
            }

            for(std::size_t q = spos, k = mpos; q < mpos && k < epos; q++)
                if(!is_null(A(q, k)))
                {
                    T_A c = unit(A(q, k), N);
                    for(std::size_t ii = spos; ii < epos; ii ++)
                        A(ii, k) = mod(c*A(ii, k), N);
                    V.mult_col(k, c);V.mod_col(k, N);
                    k++;
                }


            for(std::size_t j = 0;;j++)
            {
                if((i + 1)*s1 + j*s2 >= n)
                    break;
                std::size_t spos = (i + 1)*s1 + j*s2,
                epos = std::min(n, spos + 2*s2),
                mpos = std::min(n, spos + s2);

                //_Internal::rref(A, N, spos, mpos, spos, mpos);
                for(std::size_t j = spos, r = spos; j < mpos && r < mpos; j ++)
                {
                    for(std::size_t i = r + 1; i < mpos; i ++)
                    {
                        T_A u, v, w, z;
                        gcdex(A(r, j), A(i, j), N, u, v, w, z);

                        for(std::size_t jj = spos; jj < epos; jj ++)
                        {
                            T_A temp = A(r, jj);
                            A(r, jj) = mod(u*temp + v*A(i, jj), N);
                            A(i, jj) = mod(w*temp + z*A(i, jj), N);
                        }
                    }

                    if(!is_null(A(r, j)))
                        r++;
                }

                for(std::size_t r = spos; r < mpos; r ++)
                {
                    T_A c = unit(A(r, r), N);
                    for(std::size_t jj = spos; jj < epos; jj ++)
                        A(r, jj) = mod(c*A(r, jj), N);
                }

                //_Internal::rcef(A, V, N, spos, mpos, mpos, epos);
                for(std::size_t i = spos, k = mpos; i < mpos && k < epos; i++)
                {
                    for(std::size_t j = k + 1; j < epos; j++)
                    {
                        T_A u, v, w, z;
                        gcdex(A(i, k), A(i, j), N, u, v, w, z);

                        for(std::size_t ii = spos; ii < epos; ii++)
                        {
                            T_A temp = A(ii, k);
                            A(ii, k) = mod(u*temp + v*A(ii, j), N);
                            A(ii, j) = mod(w*temp + z*A(ii, j), N);
                        }

                        vector<T_V, false> V_col_k = V.copy_col(k);
                        V.addmult_cols(k, u, j, v); V.mod_col(k, N);
                        V.addmult_cols(j, z, V_col_k, w); V.mod_col(j, N);
                    }

                    if(!is_null(A(i, k)))
                        k++;
                }

                for(std::size_t i = spos, k = mpos; i < mpos && k < epos; i++)
                    if(!is_null(A(i, k)))
                    {
                        T_A c = unit(A(i, k), N);
                        for(std::size_t ii = spos; ii < epos; ii ++)
                            A(ii, k) = mod(c*A(ii, k), N);
                        V.mult_col(k, c);V.mod_col(k, N);
                        k++;
                    }
            }
        }

        b = b/2 + 1;
    }
}


template
<
    typename MA,
    typename MV,
    typename T_N
>
void bidiagonal_to_smith
(
    MA &A,
    MV &V,
    const T_N& N
)
{
    typedef typename MA::element_type T_A;
    typedef typename MV::element_type T_V;

    std::size_t n = A.ncols();
    for(std::size_t k = 1; k < n; k ++)
    {
        for(std::size_t i = k; i > 0; i--)
        {
            T_A a = A(i - 1, k), b = A(i, k), d = A(i - 1, i - 1),
            c = /*split*/stab_mod(a, b, d, N);
            if(!is_null(c))
            {
                A(i - 1, k) = mod(a + c*b, N);
                if(k + 1 < n)
                    A(i - 1, k + 1) = mod(A(i - 1, k + 1) + c*A(i, k + 1), N);
                if(i < k)
                {
                    T_A alpha = div_mod(mod(c*A(i, i), N), d, N);
                    V.addmult_cols(i, i - 1, -alpha);
                    V.mod_col(i, N);
                }
            }
        }

        for(std::size_t i = 0; i < k; i++)
            if(!is_null(A(i, k)))
            {
                T_A u, v, w, z;
                T_A g = gcdex(A(i, i), A(i, k), N, u, v, w, z);

                A.addmult_cols(i, k, v);
                A.mod_col(i, N);
                A.mult_col(k, z);
                A.mod_col(k, N);
                A(i, i) = g;
                A(i, k) = null(A(i, k));

                vector<T_V> V_col_i = V.copy_col(i);
                V.addmult_cols(i, u, k, v);
                V.mod_col(i, N);
                V.addmult_cols(k, z, V_col_i, w);
                V.mod_col(k, N);
            }

        for(std::size_t i = 0; i < k; i++)
            for(std::size_t ii = i + 1; ii <= k; ii++)
                if(!is_null(A(ii, i)))
                {
                    T_A alpha = A(ii, i)/A(i, i);
                    //  A.addmult_rows(ii, i, -alpha); A.mod_row(ii, N);
                    A(ii, i) = mod(A(ii, i) - alpha * A(i, i), N);
                    if(k + 1 < n)
                        A(ii, k + 1) = mod(A(ii, k + 1) - alpha * A(i, k + 1), N);
                }
    }

    for(std::size_t i = 0; i < n; i++)
    {
        T_A sign = unit(A(i, i), N);
        A(i, i) = mod(sign*A(i, i), N);
    }
}


}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
