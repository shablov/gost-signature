/*****************************************************************************

    motzkin_burger.cpp -- see motzkin_burger.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
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

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_MOTZKIN_BURGER)

#include <cstddef>

#include "exception.hpp"
#include "cmp.hpp"
#include "gcd.hpp"
#include "vector.hpp"
#include "matrix.hpp"

#include "motzkin_burger.hpp"


namespace Arageli
{

namespace _Internal
{

const int mm_no_modification  = 0;
const int mm_min_modification = 1;
const int mm_max_modification = 2;

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4715)
#endif

template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
class motzkin_burger_algorithm
{
public:

    motzkin_burger_algorithm
    (
        A& a_a,
        F& f_a,
        Q& q_a,
        E& e_a,
        const Ctrler& ctrler_a,
        int modif
    ) :
        a(a_a),
        f(f_a),
        q(q_a),
        e(e_a),
        ctrler(ctrler_a),
        modification(modif)
    {}

    void run ();

private:

    A& a;
    F& f;
    Q& q;
    E& e;

    typedef typename A::size_type index;
    typedef typename type_traits<Q>::element_type QT;

    int modification;
    Ctrler ctrler;

    void gauss();
    bool reroof (index row);
    bool balanced (index j_plus, index j_minus);
    index min_modification ();
    index max_modification ();
    index no_modification ();

    index select_column ()
    {
        switch(modification)
        {
            case mm_no_modification:
                return current_column;
            case mm_min_modification:
                return min_modification();
            case mm_max_modification:
                return max_modification();
            default:
                ARAGELI_ASSERT_1(!"It's impossible.");
        }
    }

    index fun (index j);

    void movie_print3 ()
    {
        ctrler.show_matrices(a, f, q);
    }

    vector<index> common_zero;
    index n, m, r, current_ost, current_column;
};


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

/*    Finds diagonal shape of matrix a through elementary elementary transformation,
    strores in q, after call q = f*transpose(a). */
template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
void motzkin_burger_algorithm<A, F, Q, E, Ctrler>::gauss ()
{
    // WARNING! May be it's possible here to use default implementation
    // of the Gaussian algorithm form gauss.hpp.
    // TODO: Check this possibility.

    index i, j;

    f.resize(n, n);
    e.resize(0, n);

    f.assign_eye(n);
    q = transpose(a);

    ctrler.preamble(a, f, q, e);

    ctrler.begin_gauss();
    movie_print3();

    /* main loop */
    i = 0;
    while(i < q.nrows())
    {
        /* find non-zero entry in the i-th row beginning from i-th entry */
        ctrler.find_non_zero(i);
        j = i;

        while(j < m)
        {
            if(!is_null(q(i, j)))
                break;
            j++;
        }

        if(j >= m)
        {
            /* it's a zero row */
            q.erase_row(i);
            e.insert_row(e.nrows(), f.take_row(i));

            ctrler.zero_row();
            movie_print3();
            continue; // main loop
        }

        if(i != j)
        {
            q.swap_cols(i, j);
            a.swap_rows(i, j);

            ctrler.swap_cols_rows(i, j);
            movie_print3();
        }

        if(is_negative(q(i, i)))
        {
            q.mult_row(i, -1);
            f.mult_row(i, -1);
        }

        // form zero column i
        QT b = q(i, i);
        index ii;

        for(ii = 0; ii < q.nrows(); ii++)
            if(ii != i)
            {
                QT b_ii = q(ii, i);
                QT alpha = gcd(b, b_ii);
                QT b_i = b / alpha;
                b_ii = -b_ii / alpha;
                q.mult_row(ii, b_i);
                q.addmult_rows(ii, i, b_ii);
                f.mult_row(ii, b_i);
                f.addmult_rows(ii, i, b_ii);

                alpha = gcd(gcd(q.copy_row(ii)), gcd(f.copy_row(ii)));

                //ctrler.stream
                //    << "\nq.copy_row(ii) = " << q.copy_row(ii)
                //    << "\ngcd(q.copy_row(ii)) = " << gcd(q.copy_row(ii))
                //    << "\nf.copy_row(ii) = " << f.copy_row(ii)
                //    << "\ngcd(f.copy_row(ii)) = " << gcd(f.copy_row(ii))
                //    << "\nalpha = " << alpha << ", while i = " << i << " and ii = " << ii << "\n";

                q.div_row(ii, alpha);
                f.div_row(ii, alpha);
            }

        ctrler.eliminate_col(i);
        movie_print3();
        i++; //next row
    }
    ctrler.end_gauss();
}


template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
typename motzkin_burger_algorithm<A, F, Q, E, Ctrler>::index
motzkin_burger_algorithm<A, F, Q, E, Ctrler>::fun (index j)
{
    index plus = 0;
    index minus = 0;

    // the number of positive and negative entries in the j-th column

    for(index i = 0; i < current_ost; i++)
    {
        if(is_positive(q(i, j)))
            plus++;
        else if(is_negative(q(i, j)))
            minus++;
    }

    return plus * minus;
}


template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
typename motzkin_burger_algorithm<A, F, Q, E, Ctrler>::index
motzkin_burger_algorithm<A, F, Q, E, Ctrler>::min_modification ()
{
    index min_column, jj, min_fun, cur_fun;

    /* the column with minimal product of the number of positive
    and the number of negative entries */

    min_column = current_column;
    min_fun = fun(min_column);
    for(jj = current_column + 1; jj < m; jj++)
    {
        cur_fun = fun(jj);
        if(cur_fun < min_fun)
        {
            min_column = jj;
            min_fun = cur_fun;
        }
    }

    return min_column;
}


template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
typename motzkin_burger_algorithm<A, F, Q, E, Ctrler>::index
motzkin_burger_algorithm<A, F, Q, E, Ctrler>::max_modification ()
{
    index max_column, jj, max_fun, cur_fun;

    /* the column with maximal product of the number of positive
    and the number of negative entries */

    max_column = current_column;
    max_fun = fun(max_column);
    for(jj = current_column + 1; jj < m; jj++)
    {
        cur_fun = fun(jj);
        if(cur_fun > max_fun)
        {
            max_column = jj;
            max_fun = cur_fun;
        }
    }
    return max_column;
}


template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
bool motzkin_burger_algorithm<A, F, Q, E, Ctrler>::reroof (index row)
{
    // run on all columns with common zeros on the potentially balanced
    // rows and check values at the intersection with the row
    for(index i = 0; i < common_zero.size(); i++)
    {
        index j = common_zero[i];    // the current column number
        if(!is_null(q(row, j)))
            return false;
    }

    // q sould have at least one non-zero element at the intersection
    // of the row and columns with common zeros;
    // here it is'n so -- return true
    return true;
}


template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
bool motzkin_burger_algorithm<A, F, Q, E, Ctrler>::balanced
(index j_plus, index j_minus)
{
    // This function returns true if j_plus and j_minus are
    // actually balanced rows.

    // Fist we construct the set common_zero.

    // run on the rows j_plus and j_minus in parallel
    // from left to right up to current column
    for(index j = 0; j < current_column; ++j)
        if(is_null(q(j_plus, j)) && is_null(q(j_minus, j)))
            // each of the rows has zero in column j
            common_zero.push_back(j);

    if(common_zero.size() < r - 2)
        // number of common zeros is low -- it's not a balanced pair
        return false;

    // Second we do zeros intersection detection.

    // run on the columns of the matrix from top to bottom
    // excluding j_plus and j_minus rows and determine if
    // this pair of rows is actually balanced
    for(index row = 0; row < current_ost; ++row)
        if
        (
            row != j_plus  &&
            row != j_minus  &&
            reroof(row)
        )
            return false;

    return true;
}


template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
void motzkin_burger_algorithm<A, F, Q, E, Ctrler>::run ()
{
    //TODO: Use hpair_matrix to manipulate with (f, q) pair

    m = a.nrows();
    n = a.ncols();

    // Gaussian elimination on the matrix q to form diagonal
    // matrix at the first several rows.
    gauss();    // ctrler.preamble is in gauss

    ctrler.begin_motzkin();

    movie_print3();

    r = n - e.nrows();    // rank of the system
    current_column = r;

    while(current_column < a.nrows())
    {
        vector<index> j_minus, j_plus;

        //for each inequality from a, beginning from(r + 1) th
        current_ost = q.nrows();

        //taking a new inequality depends upon modification
        index new_column = select_column();

        ctrler.select_col(current_column, new_column);

        // swap cols/rows if the new column is'n in the next position
        if(current_column != new_column)
        {
            q.swap_cols(current_column, new_column);
            a.swap_rows(current_column, new_column);

            ctrler.swap_cols_rows(current_column, new_column);
            movie_print3();
        }

        // now we are constructing the sets j_plus, j_minus
        for(index ii = 0; ii < current_ost; ii++)
        {
            if(is_negative(q(ii, current_column)))
                j_minus.push_back(ii);
            else if(is_positive(q(ii, current_column)))
                j_plus.push_back(ii);
        }

        if(j_minus.size() == current_ost)
        {
            //there is no solution except 0
            ctrler.zero_solution();

            f.resize(0, n);
            //e.resize(0, n);    // WARNING
            q.resize(0, m);
            ctrler.end_motzkin();
            ctrler.conclusion(a, f, q, e);
            return;
        }

        if(j_minus.size() == 0)
        {
            // this is corollary inequality
            ctrler.corollary_inequality();

            a.erase_row(current_column);
            q.erase_col(current_column);
            m--;
            continue; //new inequality
        }

        /*
        cerr
            << "Current column: " << current_column
            << " ost: " << current_ost
            << " J_plus: " << j_plus.ncols()
            << " J_minus: " << j_minus.ncols() << endl;
        */

        ctrler.begin_row_balancing();

        for(index jj_m = 0; jj_m < j_minus.size(); jj_m++)
        {
            index j_m = j_minus[jj_m];
            for(index jj_p = 0; jj_p < j_plus.size(); jj_p++)
            {
                index j_p = j_plus[jj_p];
                common_zero.resize(0);
                if(balanced(j_p, j_m))
                {
                    ctrler.balanced_rows(j_p, j_m);

                    QT b_minus = q(j_m, current_column);
                    QT b_plus = q(j_p, current_column);
                    QT delta = gcd(b_minus, b_plus);
                    QT alpha_plus = -b_minus / delta;
                    QT alpha_minus = b_plus / delta;

                    f.insert_row
                    (
                        f.nrows(),
                        f.copy_row(j_p)*alpha_plus + f.copy_row(j_m)*alpha_minus
                    );

                    q.insert_row
                    (
                        q.nrows(),
                        q.copy_row(j_p)*alpha_plus + q.copy_row(j_m)*alpha_minus
                    );

                    delta = gcd
                    (
                        gcd(f.copy_row(f.nrows() - 1)),
                        gcd(q.copy_row(q.nrows() - 1))
                    );

                    f.div_row(f.nrows() - 1, delta);
                    q.div_row(q.nrows() - 1, delta);
                }
            } //Iterate J_plus
        } //Iterate J_minus

        ctrler.end_row_balancing();
        movie_print3();
        ctrler.delete_negates();

        f.erase_rows(j_minus);
        q.erase_rows(j_minus);

        movie_print3();

        current_column++;
    }

    ctrler.end_motzkin();
    ctrler.conclusion(a, f, q, e);
}

}


template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
void skeleton_motzkin_burger
(
    A& a,
    F& f,
    Q& q,
    E& e,
    Ctrler ctrler
)
{
    _Internal::motzkin_burger_algorithm<A, F, Q, E, Ctrler>
        alg(a, f, q, e, ctrler, _Internal::mm_no_modification);
    alg.run();
}


template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
void skeleton_motzkin_burger_min
(
    A& a,
    F& f,
    Q& q,
    E& e,
    Ctrler ctrler
)
{
    _Internal::motzkin_burger_algorithm<A, F, Q, E, Ctrler>
        alg(a, f, q, e, ctrler, _Internal::mm_min_modification);
    alg.run();
}


template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
void skeleton_motzkin_burger_max
(
    A& a,
    F& f,
    Q& q,
    E& e,
    Ctrler ctrler
)
{
    _Internal::motzkin_burger_algorithm<A, F, Q, E, Ctrler>
        alg(a, f, q, e, ctrler, _Internal::mm_max_modification);
    alg.run();
}


} // namesapce Arageli


#endif
