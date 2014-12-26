/*****************************************************************************

    lll.cpp -- LLL basis reduction.

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006, 2010 Nikolai Yu. Zolotykh

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
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_LLL)


#include <cmath>

#include "vector.hpp"
#include "matrix.hpp"
#include "lll.hpp"
#include "powerest.hpp"

#include "rational.hpp"    // WARNING! TEMPORARY AS A WORKAROUND TO A BUG


namespace Arageli
{

namespace _Internal
{

// The following 3 functions are used only internally in lll_reduction

template <typename B_type, typename Bst_type, typename index>
inline void lll_gram_schmidt(B_type& B, Bst_type& Bst, Bst_type& Mu, index k)
{
    Bst.assign_col(k, B.copy_col(k));

    for(index j = 0; j < k; j++)
    {
        Mu(k, j) = dotprod(B.copy_col(k), Bst.copy_col(j)) / Mu(j, j);
        Bst.assign_col(k, Bst.copy_col(k) - Bst.copy_col(j) * Mu(k, j));
        Mu(k, k) = dotsquare(Bst.copy_col(k));
    }
}

template <typename B_type, typename H_type, typename Mu_type, typename index, typename T>
inline void lll_size_reduction(B_type& B, H_type& H, Mu_type& Mu, index k, index l, T& v_1_2)
{
    T q = std::floor(Mu(k, l) + v_1_2);
    B.assign_col(k, B.copy_col(k) - B.copy_col(l) * q);
    H.assign_col(k, H.copy_col(k) - H.copy_col(l) * q);
    Mu(k, l) = Mu(k, l) - q;
    for (index i = 0; i < l; i++)
        Mu(k, i) = Mu(k, i) - q * Mu(l, i);
}


template <typename B_type, typename Bst_type, typename H_type, typename Mu_type, typename index>
inline void lll_interchange(B_type& B, Bst_type& Bst, H_type& H, Mu_type& Mu, index k, index k_max)
{
    typedef typename Mu_type::value_type T;

    B.swap_cols(k, k - 1);
    H.swap_cols(k, k - 1);
    for (index j = 0; j < k - 1; j++)
        std::swap(Mu(k, j), Mu(k - 1, j));
    T mu = Mu(k, k - 1);
    T b2 = Mu(k, k) + mu * mu * Mu(k - 1, k - 1);
    Mu(k, k - 1) = mu * Mu(k - 1, k - 1) / b2;
    vector<T> b = Bst.copy_col(k - 1);
    Bst.assign_col(k - 1, Bst.copy_col(k) + b * mu);
    Bst.assign_col
    (
        k,
        Bst.copy_col(k) * (-Mu(k, k - 1)) + b * (Mu(k, k)/b2)
    );
    Mu(k, k) = Mu(k - 1, k - 1) * Mu(k, k) / b2;
    Mu(k - 1, k - 1) = b2;

    for (index i = k + 1; i <= k_max; i++)
    {
        T t = Mu(i, k);
        Mu(i, k) = Mu(i, k - 1) - mu * t;
        Mu(i, k - 1) = t + Mu(k, k - 1) * Mu(i, k);
    }
}

} // namespace _Internal

template <typename B_type, typename H_type>
bool lll_reduction (B_type& B, H_type& H)
{
    using namespace _Internal;

    typedef typename B_type::difference_type index;
    typedef typename B_type::value_type T;

    index m = B.nrows();
    index n = B.ncols();
    matrix<T, false> Bst(m, n, fromsize);
    matrix<T, false> Mu(n); // In fact we use only lower and diagonal part of the matrix!

    Bst.assign_col(0, B.copy_col(0));
    Mu(0, 0) = dotsquare(B.copy_col(0));
    H = H_type(n, eye);

    T v_3_4 = T(3, 4);
    T v_1_2(1, 2);

    index k_max = 0;

    // std::cout << "B = " << std::endl; output_aligned(std::cout, B);
    // std::cout << "Bst = " << std::endl; output_aligned(std::cout, Bst);
    // std::cout << "Mu = " << std::endl; output_aligned(std::cout, Mu);

    for(index k = 1; k < n; )
    {
        // std::cout << "**** k = " << k + 1 << std::endl;

        if(k > k_max)
        {
            k_max = k;
            // std::cout << "**** I. new k_max = " << k_max + 1 << std::endl;
            // std::cout << "        Gram-Schmidt" << std::endl;

            lll_gram_schmidt(B, Bst, Mu, k);

            // std::cout << "           B = " << std::endl; output_aligned(std::cout, B);
            // std::cout << "           Bst = " << std::endl; output_aligned(std::cout, Bst);
            // std::cout << "           Mu = " << std::endl; output_aligned(std::cout, Mu);

            if(is_null(Mu(k, k)))
                return false; // B(i) did not form the basis
        }

        // std::cout << "     II. ? Mu(" << k+1 << ", " << k << ") > 1/2 ?" << std::endl;

        if (std::abs(Mu(k, k - 1)) > v_1_2)
        {
            // std::cout << "     yes: Mu(" << k+1 << ", " << k << ") > 1/2";
            // std::cout << "     Size reduction" << std::endl;
            lll_size_reduction(B, H, Mu, k, k - 1, v_1_2);
            // std::cout << "B = " << std::endl; output_aligned(std::cout, B);
            // std::cout << "Bst = " << std::endl; output_aligned(std::cout, Bst);
            // std::cout << "Mu = " << std::endl; output_aligned(std::cout, Mu);
        }


        // std::cout << "      III. Mu(" << k+1 << ", " << k+1 << ")  < (3/4 - Mu(" << k+1 << ", " << k << ")^2) * Mu(" << k << ", " << k << ") ?" << std::endl;
        if (Mu(k, k) < (v_3_4 - square(Mu(k, k - 1))) * Mu(k - 1, k - 1))
        {
            // std::cout << "   Yes! Interchange:" << k+1 << ", " << k << std::endl;
            lll_interchange(B, Bst, H, Mu, k, k_max);
            // std::cout << "B = " << std::endl; output_aligned(std::cout, B);
            // std::cout << "Bst = " << std::endl; output_aligned(std::cout, Bst);
            // std::cout << "Mu = " << std::endl; output_aligned(std::cout, Mu);
            k = max(static_cast<index>(1), k - 1);
        }
        else
        {
            // std::cout << "  IV. No: Size reductions..." << std::endl;
            // std::cout << "                 " << k-1 << ", " << k-2 << ", ...," << 1 << std::endl;
            for (index l = k - 1; l > 0; l--)
                if(std::abs(Mu(k, l - 1)) > v_1_2)
                    lll_size_reduction(B, H, Mu, k, l - 1, v_1_2);

            // std::cout << "B = " << std::endl; output_aligned(std::cout, B);
            // std::cout << "Bst = " << std::endl; output_aligned(std::cout, Bst);
            // std::cout << "Mu = " << std::endl; output_aligned(std::cout, Mu);
            k++;
        }

    }

    return true;
}

namespace _Internal
{

// The following 3 functions are used only internally in lll_reduction_int

template
<
    typename B_type,
    typename H_type,
    typename Lambda_type,
    typename d_type,
    typename index
>
inline void lll_size_reduction_int
(
    B_type& B,
    H_type& H,
    Lambda_type& Lambda,
    d_type& d,
    index k,
    index l
)
{
    typedef typename Lambda_type::value_type T;

    T q = prquot(2*Lambda(k, l) + d[l + 1], 2*d[l + 1]);
    B.assign_col(k, B.copy_col(k) - B.copy_col(l) * q);
    H.assign_col(k, H.copy_col(k) - H.copy_col(l) * q);
    Lambda(k, l) = Lambda(k, l) - q*d[l + 1];
    for (index i = 0; i < l; i++)
        Lambda(k, i) -= q * Lambda(l, i);
}


template
<
    typename B_type,
    typename H_type,
    typename Lambda_type,
    typename d_type,
    typename index
>
inline void lll_interchange_int
(
    B_type& B,
    H_type& H,
    Lambda_type& Lambda,
    d_type& d,
    index k,
    index k_max
)
{
    typedef typename Lambda_type::value_type T;

    B.swap_cols(k, k - 1);
    H.swap_cols(k, k - 1);

    for (index j = 0; j < k - 1; j++)
        std::swap(Lambda(k, j), Lambda(k - 1, j));

    T lambda = Lambda(k, k - 1);
    T b = (d[k - 1]*d[k + 1] + square(lambda))/d[k];

    for (index i = k + 1; i <= k_max; i++)
    {
        T t = Lambda(i, k);
        Lambda(i, k) = (d[k + 1]*Lambda(i, k - 1) - lambda * t)/d[k];
        Lambda(i, k - 1) = (b*t + lambda*Lambda(i, k))/d[k + 1];
    }

    d[k] = b;
}


template
<
    typename B_type,
    typename Lambda_type,
    typename d_type,
    typename index
>
inline void lll_gram_schmidt_int
(
    B_type& B,
    Lambda_type& Lambda,
    d_type& d,
    index k
)
{
    typedef typename B_type::value_type T;

    for(index j = 0; j <= k; j++)
    {
        T u = dotprod(B.copy_col(k), B.copy_col(j));
        for (index i = 0; i < j; i++)
            u = (d[i + 1]*u - Lambda(k, i)*Lambda(j, i))/d[i];
        if (j < k)
            Lambda(k, j) = u;
        else
            d[k + 1] = u;
    }
}

} // namespace _Internal

template <typename B_type, typename H_type>
bool lll_reduction_int(B_type& B, H_type& H)
{
    using namespace _Internal;

    typedef typename B_type::difference_type index;
    typedef typename B_type::value_type T;

    index m = B.nrows();
    index n = B.ncols();

    vector<T, false> d(n + 1, fromsize);
    matrix<T, false> Lambda(n); // In fact we use only lower part of the matrix!

    H = H_type(n, eye);

    d[0] = 1;
    d[1] = dotsquare(B.copy_col(0));

    index k_max = 0;

    for (index k = 1; k < n; )
    {
        if (k > k_max)
        {
            k_max = k;

            lll_gram_schmidt_int(B, Lambda, d, k);

            if (is_null(d[k + 1]))
                return false; // B(i) did not form the basis
        }

        if (2*std::abs(Lambda(k, k - 1)) > d[k])
            lll_size_reduction_int(B, H, Lambda, d, k, k - 1);


        if (4*d[k + 1]*d[k - 1] < 3*square(d[k]) - 4*square(Lambda(k, k - 1)))
        {
            lll_interchange_int(B, H, Lambda, d, k, k_max);
            k = max(1, k - 1);
        }
        else
        {
            for (index l = k - 1; l > 0; l--)
                if (2*std::abs(Lambda(k, l - 1)) > d[l])
                    lll_size_reduction_int(B, H, Lambda, d, k, l - 1);
            k++;
        }

    }

    return true;
}

} // namespace Arageli

#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
