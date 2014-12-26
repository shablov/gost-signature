/*****************************************************************************

    berlekamp_hensel.cpp

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

/**
    \file berlekamp_hensel.cpp
    \brief The berlekamp_hensel.hpp file stuff implementation.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_berlekamp_hensel)

#include <cstddef>
#include <iostream>    // WARNING! ONLY FOR DEBUGGING!

#include "exception.hpp"
#include "polyalg.hpp"
#include "sqrfree.hpp"
#include "sparse_polynom.hpp"
#include "matrix.hpp"
#include "residue.hpp"

#include "berlekamp_hensel.hpp"


namespace Arageli
{

//#define TIME_MEASURE//turn on timing information for berlekamp_hensel algorithm
#ifdef TIME_MEASURE
    #include <time.h>
    time_t stbh;//global variable
    #define ARAGELI_TM_PRINT(a) std::cout << a << "\n";
#else
    #define ARAGELI_TM_PRINT(a)
#endif

namespace _Internal
{

template <typename T>
sparse_polynom<T> residue_to_int_sparse_polynom
(const sparse_polynom<residue<T> > & x)
{
    sparse_polynom<T> res;
    typedef typename sparse_polynom<T>::monom monom;
    for
    (
        typename sparse_polynom<residue<T> >::monom_const_iterator i = x.monoms_begin();
        i != x.monoms_end();
        ++i
    )
        res.insert(res.monoms_end(), monom(i->coef().value(), i->degree()));
    return res;
}


template <typename T>
vector<sparse_polynom<T> > residue_to_int_sparse_polynom_vector
(const vector<sparse_polynom<residue<T> > > & x)
{
    vector<sparse_polynom<T> > res;
    res.reserve(x.size());

    for(std::size_t i = 0; i < x.size(); ++i)
        res.push_back(residue_to_int_sparse_polynom(x[i]));

    return res;
}


template <class T>
void wrapper_my_rref_mod
(
    const matrix<residue<T> >& A_mod,
    matrix<residue<T> >& rref_A,
    vector<std::size_t>& basis
)
{
    ARAGELI_TM_PRINT("wrapper_my_rref_mod(..) started. Time="<<time(0)-stbh);
    matrix<T> A(A_mod.ncols(), A_mod.nrows(), fromsize);
    for (std::size_t i = 0; i < A_mod.nrows(); i++)
        for (std::size_t j = 0; j < A_mod.ncols(); j++)
            A(i, j) = A_mod(i, j).value();

    my_rref_mod(A, basis, A_mod(0, 0).module());
    rref_A = A;
    ARAGELI_TM_PRINT("wrapper_my_rref_mod(..) finished. Time="<<time(0)-stbh);
}



template <class T>
void my_rref_mod
(
    matrix<T>& A,
    vector<std::size_t>& basis,
    T mod
)
{
    ARAGELI_TM_PRINT("my_rref_mod(..) started. Time="<<time(0)-stbh);
    basis = vector<std::size_t>(0, fromsize);

    std::size_t m = A.nrows();
    std::size_t n = A.ncols();

    for (std::size_t i = 0, j = 0; j < n; j++)
    {
        std::size_t k;
        for (k = i; k < m; k++)
        {
            if (A(k, j) != 0)
                break;
        }
        if (k == m)
            continue;

        for (std::size_t jj = j; jj < n; jj++)
            std::swap(A(k, jj), A(i, jj));

        basis.push_back(j);

        T inv_pivot = inverse_mod(A(i, j), mod);


        for (std::size_t jj = j; jj < n; jj++)
        {
            A(i, jj) = (A(i, jj) * inv_pivot) % mod;
        }
        T q_useless;
        T r;

        for (std::size_t l = 0; l < m; l++)
            if (l != i)
            {
                T pv = A(l, j);
                for (std::size_t jj = j; jj < n; jj++)
                {
                    prdivide(A(l, jj) - pv*A(i, jj), mod,q_useless,r);
                    A(l, jj) = r;
                    //A(l, jj) = prrem(A(l, jj) - pv*A(i, jj), mod);
                }
            }
        i++;
        ARAGELI_TM_PRINT("j="<<j<<" of "<<n<<" "<<time(0)-stbh);
    }
    ARAGELI_TM_PRINT("my_rref_mod(..) finished. Time="<<time(0)-stbh);
}


/** @param f monic square-free polynomial(Z<sub>p</sub>[x])*/
template <typename T>//T
vector<sparse_polynom<T> > berlekamp (const sparse_polynom<T>& f)
{
    ARAGELI_TM_PRINT("berlekamp started. Time="<<time(0)-stbh);
    vector< sparse_polynom<T> > factors;
    matrix <T> Q(f.degree(),fromsize);
    form_matrix_Q_minus_I(f,Q);
    ARAGELI_TM_PRINT("form_matrix _Q(..) finished . Time="<<time(0)-stbh);
    //COUTCOM output_aligned(std::cout, Q);
    ARAGELI_TM_PRINT("Preparing to start null_spase_basis . Time="<<time(0)-stbh);
    //vector< sparse_polynom<T> > v = null_space_basis(Q - matrix<T>(Q.length(), eye));
    vector< sparse_polynom<T> > v = null_space_basis(Q);
    //output_aligned(std::cerr, v);
    ARAGELI_TM_PRINT("null_spase_basis finished . Time="<<time(0)-stbh);
    Q.assign_fromsize(0);//delete from memory
    //com std::cout << v.length() << " factors" <<std::endl;//COUTCOM


    factors.push_back(f);
    /*dbg*///com std::cout<<"factors="<<factors<<"\n";

    if (v.length() == 1)
    {
        return factors; // f is irreducible
    }
    ARAGELI_TM_PRINT("berlekamp. start searching. Time="<<time(0)-stbh);
    typedef typename T::module_type module_type;
    module_type module=f.leading_coef().module();
    T s=factory<T>::unit();
    s.module() = module;//normalize? no
    sparse_polynom<T> h;
    for (std::size_t m = 0; ; m++) // the index of factor considered
    {
        //com std::cout << "basis vector No = " << m << std::endl;//COUTCOM
        for (std::size_t j = 1; j < v.length(); j++) // for each v[j]
        {
            h = gcd(factors[m], v[j]);
            if ((!is_unit(h)) && h != factors[m])
            {
                factors.push_back(h);
                factors[m] /= h;
                if (factors.length() == v.length())
                {
                    return factors;
                }
            }
            for (module_type ss = 1; ss < module; ss++)
            {
                v[j].sub_scalar(s);//v[j]=v[j]-1(mod p)*x^0
                h = gcd(factors[m], v[j]);
                if ((!is_unit(h)) && h != factors[m])
                {
                    factors.push_back(h);
                    factors[m] /= h;
                    if (factors.length() == v.length())
                    {
                        return factors;
                    }
                }
            }
        }
    }
}


template <typename P>
vector<P> pre_berlekamp (const P& p)
{
    ARAGELI_ASSERT_1(p.is_normal());
    /*dbg*///com std::cout<<"pre_berlekamp stated, p="<<p<<"\n";
    if(p.is_const())return vector<P>(1, p, fromval);
    /*dbg*///com std::cout<<"type of P="<<typeid(P).name()<<"\n";
    typename P::coef_type lc = p.leading_coef();
    P pp = p;
    pp /= lc;
    /*dbg*///com std::cout<<"pp="<<pp<<"\n";
    P sfp = sqrfree_poly_mod(pp);
    /*dbg*///com std::cout<<"sfp="<<sfp<<"\n";
    vector<P> res;

    ARAGELI_ASSERT_1(is_unit(sfp.leading_coef_cpy()));
    vector<P> locres = berlekamp(sfp);
    /*dbg*///com std::cout<<"locres="<<locres<<"\n";

    for(std::size_t i = 0; i < locres.size(); ++i)
    {
        std::size_t d = 0;

        // compute the degree
        for(;;)
        {
            P q, r;
            divide(pp, locres[i], q, r);
            //std::cerr << "\npp = " << pp << "\nlocres[" << i << "] = " << locres[i] << "\nq = " << q << "\nr = " << r;
            if(!is_null(r))break;
            pp = q;
            ++d;
        }

        ARAGELI_ASSERT_1(d > 0);

        for(std::size_t j = 0; j < d; ++j)
            res.push_back(locres[i]);
    }

    res[0] *= lc;

    #if ARAGELI_DEBUG_LEVEL >= 1
    {
        // Check if all res elements are normal.
        typedef typename vector<P>::const_iterator VPCI;
        for(VPCI i = res.begin(); i != res.end(); ++i)
            ARAGELI_ASSERT_1(i->is_normal());
    }
    #endif

    ARAGELI_ASSERT_1(product(res) == p);
    /*dbg*///com std::cout<<"res="<<res<<"\npre_berlekamp finished\n\n";
    return res;
}


template <typename T> void form_matrix_Q_minus_I (const sparse_polynom<T>& f,matrix<T>& Q)
{
    ARAGELI_ASSERT_1(Q.is_square()&&Q.ncols()>0);
    typedef typename T::module_type module_type;
    typedef typename T::value_type value_type;
    typedef typename sparse_polynom<T>::degree_type degree_type;
    module_type module=f.leading_coef().module();
    T one=factory<T>::unit();//T one = 1;
    one.module() = module;
    degree_type f_degree=f.degree();
    //one.normalize();//what for? module > 1 !

    sparse_polynom<T> h(one);//h=1(mod p)*x^0

    //matrix<T> Q(f.degree(), f.degree(), fromsize);
    //Q.assign_fromsize(f_degree);//square matrix, which size is degree x degree

    for (degree_type k = 0; k < f_degree; k++)
    {
        typedef typename sparse_polynom<T>::monom_const_iterator monoms;
        for(monoms mi = h.monoms_begin(), mj = h.monoms_end(); mi != mj ; ++mi)
            Q(mi->degree(), k) = mi->coef();

        //h = h*(1(mod p)*x^p)
        typedef typename sparse_polynom<T>::monom_iterator monoms_i;
        for(monoms_i mi = h.monoms_begin(), mj = h.monoms_end(); mi != mj ; ++mi)
            mi->degree()+=module;

        h %= f;
    }

    value_type minus1modp=value_type(module-factory<module_type>::unit());
    for (degree_type i = 0; i < f_degree; i++)
        for (degree_type j = 0; j < f_degree; j++)
        {
            if(i!=j)
            {
                Q(i, j).module() = module;
            }
            else
            {
                //Q=Q-I(mod p)

                Q(i, j).value() += minus1modp;
                Q(i, j).module() = module;
                Q(i, j).normalize();
            }
        }
}


template <class T>
matrix<T> null_space_basis_into_matrix (const matrix<T>& A)
{
    ARAGELI_TM_PRINT("null_space_basis_into_matrix started. Time="<<time(0)-stbh);
    matrix<T> rref_A, A_inv;
    vector<std::size_t> basis;
    T d;

    //rref(A, rref_A, A_inv, basis, d);
    wrapper_my_rref_mod(A, rref_A, basis);

    // wrapper_my_rref_mod function doesn't seem to set
    // correct module value for all the elements. Set them.
    for(typename matrix<T>::iterator i = rref_A.begin(); i != rref_A.end(); ++i)
    {
        i->module() = A(0, 0).module();
        i->normalize(); // is it necessary?
    }

    //output_aligned(std::cerr << "\nrref_A = \n", rref_A);

    // This is for the next statment where we take one of the elements
    // of A matrix. If A is supposed to be sometimes empty,
    // replace this assert by an if-statement with the next statment.
    ARAGELI_ASSERT_0(!A.is_empty());

    matrix<T> Q(A.ncols(), A.ncols() - basis.length(), null(A(0, 0)));

    for (std::size_t j = 0, bj = 0, nbj = 0; j < rref_A.ncols(); j++)
    {
        if (bj < basis.length() && j == basis[bj])
        {
            bj++;
        }
        else
        {
            vector<T> q(rref_A.ncols());

            q[j] = 1;
            for (std::size_t i = 0; i < q.length(); i++) // It's ugly
            {
                q[i].module() = A(0, 0).module();
                q[i].normalize();
            }

            for (std::size_t i = 0; i < basis.length(); i++)
                q[basis[i]] = -rref_A(i, j);

            for (std::size_t i = 0; i < Q.nrows(); i++)
                Q(i, nbj) = q[i];

            nbj++;
        }
    }
    ARAGELI_TM_PRINT("null_space_basis_into_matrix finished. Time="<<time(0)-stbh);
    return Q;
}


template <class T>
vector<sparse_polynom<T> > null_space_basis (const matrix<T>& A)
{
    ARAGELI_TM_PRINT("null_space_basis(..) started. Time="<<time(0)-stbh);
    matrix<T> Q = null_space_basis_into_matrix(A);
    //output_aligned(std::cerr << "\ninternal =\n", Q);

    vector< sparse_polynom<T> > v(Q.ncols());

    for (std::size_t j = 0; j < Q.ncols(); j++)
    {
        for (std::size_t i = 0; i < Q.nrows(); i++)
        {
            if (Q(i, j) != 0)
                v[j] += monom<T>(Q(i, j), i);
        }
    }
    ARAGELI_TM_PRINT("null_space_basis(..) finished. Time="<<time(0)-stbh);
    return v;
}


template <class T>
void new_module (sparse_polynom<residue<T> >& f, const T& q)
{
    typedef typename sparse_polynom< residue<T> >::coef_iterator coefs;

    for(coefs ci = f.coefs_begin(), cj = f.coefs_end(); ci != cj ; ++ci)
    {
        ci -> module() = q;
        ci -> normalize();
    }

    f.normalize();
}

template <class T>
vector<T> prod_all_without_one (const vector<T>& u)
{
    ARAGELI_ASSERT_0(u.length() > 0);

    T prod = u[0];

    for (std::size_t i = 1; i < u.length(); i++)
        prod *= u[i];

    vector<T> w(u.length(), fromsize);

    for (std::size_t i = 0; i < u.length(); i++)
        w[i] = prod / u[i];

    return w;
}


/** gcd(a[0],a[1],...,a[n-1])=u[0]*a[0]+...+u[n-1]*a[n-1]
    n > 1
    gcd(0, 0, 0) = 0 (и это правильно)
*/
template <class T>
T euclid_bezout (const vector<T>& a, vector<T>& u)
{
    std::size_t n = a.length();

    ARAGELI_ASSERT_0(n > 1);

    u.resize(n);
    vector<T> v(n, fromsize);

    T d = a[0];

    for (std::size_t i = 1; i < n; i++)
    {
        d = euclid_bezout(d, a[i], u[i-1], v[i]);
    }

    for (std::size_t i = n - 2; i > 0; i--)
    {
        u[i - 1] *= u[i];
    }

    u[n - 1] = v[n - 1];

    for (std::size_t i = 1; i < n - 1; i++)
    {
        u[i] *= v[i];
    }

    return d;

}


template <class T>
void make_degrees_in_bezout_coeff_less
(const vector<T>& u, vector<T>& v)
{
    T q, r;
    for (std::size_t i = 0; i < u.length() - 1; i++)
    {
        prdivide(v[i], u[i], q, r);
        v[i] = r;
        v[i + 1] += q*u[i + 1];
    }
    return;

}


template <class T>
void hensel_quadratic_lifting
(
    sparse_polynom<T>& f,
    vector< sparse_polynom<T> >& u,
    vector< sparse_polynom<T> >& v,
    vector< sparse_polynom<T> >& w,
    T q
)
{
    std::size_t r = u.length();

    ARAGELI_ASSERT_0(r > 0);

    // find u[i]

    sparse_polynom<T> prod = u[0];

    for (std::size_t i = 1; i < r; i++)
    {
        prod = mul_mod(prod, u[i], q*q);
    }

    sparse_polynom<T> t = sub_mod(f, prod, q*q) / q;

    for (std::size_t i = 0; i < r; i++)
    {
        u[i] += q*res_mod(mul_mod(t, v[i], q), u[i], q);
    }

    // find v[i]

    w = prod_all_without_one(u); // Этого нет у Панкратьева, но это нужно.

    t = T(1);

    for (std::size_t i = 0; i < r; i++)
    {
        t = sub_mod(t, mul_mod(v[i], w[i], q*q), q*q);
    }

    t /= q;

    for (std::size_t i = 0; i < r; i++)
    {
        v[i] += q*res_mod(mul_mod(t, v[i], q), u[i], q);
    }
}


/// Next combination
template <typename I>
void next_combination (vector<I>& c, std::size_t n, std::size_t m)
{
    std::size_t j;  // j - 1 is pivot

    for (j = m; j > 0; j--)
    {
        if (c[j - 1] < n - m + j - 1)
            break;
    }

    if (j == 0)
    {
        c = vector<std::size_t>(0, fromsize);
        return;
    }

    c[j - 1]++;

    for (std::size_t i = j; i < m; i++)
    {
        c[i] = c[j - 1] + i - j + 1;
    }
}


/// Next combination in subset
template <typename I1, typename I2>
void next_combination
(
    vector<I1>& c,
    std::size_t n,
    std::size_t m,
    const vector<I2>& subset
)
{
    while (1)
    {
        next_combination(c, n, m);

        if (!c.length())
            break;

        int in_subset = 1;

        for (std::size_t j = 0; j < m; j++)
            if (!subset[c[j]])
            {
                in_subset = 0;
                break;
            }

        if (in_subset)
            break;
    }
}


/// First combination in subset
template <typename I>
vector<std::size_t> first_combination
(
    std::size_t n, std::size_t m,
    const vector<I>& subset
)
{
    if (n < m)
    return vector<std::size_t>(0, fromsize);

    vector<std::size_t> c = vector<std::size_t>(m, fromsize);

    for (std::size_t j = 0; j < m; j++)
    {
        c[j] = j;
    }

    while (1)
    {
        int in_subset = 1;

        for (std::size_t j = 0; j < m; j++)
            if (!subset[c[j]])
            {
                in_subset = 0;
                break;
            }

        if (in_subset)
            break;

        next_combination(c, n, m);

        if (!c.length())
            break;
    }

    return c;
}



template <class T>
T norm (sparse_polynom<T>& f)
{
    typedef typename sparse_polynom<T>::coef_iterator coefs;

    T nrm = 0;

    for(coefs ci = f.coefs_begin(), cj = f.coefs_end(); ci != cj ; ++ci)
    {
        nrm += std::abs(*ci);
    }

    return nrm;
}

template <class T>
sparse_polynom<T> diff (sparse_polynom<T>& f)
{
    typedef typename sparse_polynom<T>::monom_iterator monoms;

    sparse_polynom<T> difff;

    for(monoms ci = f.monoms_begin(), cj = f.monoms_end(); ci != cj ; ++ci)
    {
        difff += monom<T>((ci -> coef())*(ci -> degree()), ci -> degree() - 1);
    }

    return difff;
}


template <typename T>
T cont (sparse_polynom<T>& f)
{
    typedef typename sparse_polynom<T>::coef_iterator coefs;

    vector<T> a;

    T lc;
    if(!is_unit(lc=f.leading_coef()))
    {
        for(coefs ci = f.coefs_begin(), cj = f.coefs_end(); ci != cj ; ++ci)
        {
            a.push_back(*ci);
        }
        return gcd(a);
    }
    else{
        return lc;
    }

}


template <class T>
sparse_polynom<T> add_mod
(
    const sparse_polynom<T>& f,
    const sparse_polynom<T>& g,
    T mod
)
{
    sparse_polynom<residue<T> > f_mod = f;
    sparse_polynom<residue<T> > g_mod = g;

    new_module(f_mod, mod);
    new_module(g_mod, mod);

    return residue_to_int_sparse_polynom(f_mod + g_mod);
}


template <class T>
sparse_polynom<T> sub_mod
(
    const sparse_polynom<T>& f,
    const sparse_polynom<T>& g,
    T mod
)
{
    sparse_polynom<residue<T> > f_mod = f;
    sparse_polynom<residue<T> > g_mod = g;

    new_module(f_mod, mod);
    new_module(g_mod, mod);

    return residue_to_int_sparse_polynom(f_mod - g_mod);
}


template <class T>
sparse_polynom<T> mul_mod
(
    const sparse_polynom<T>& f,
    const sparse_polynom<T>& g,
    T mod
)
{
    sparse_polynom<residue<T> > f_mod = f;
    sparse_polynom<residue<T> > g_mod = g;

    new_module(f_mod, mod);
    new_module(g_mod, mod);

    return residue_to_int_sparse_polynom(f_mod * g_mod);
}


template <class T>
sparse_polynom<T> div_mod
(
    const sparse_polynom<T>& f,
    const sparse_polynom<T>& g,
    T mod
)
{
    sparse_polynom<residue<T> > f_mod = f;
    sparse_polynom<residue<T> > g_mod = g;

    new_module(f_mod, mod);
    new_module(g_mod, mod);

    return residue_to_int_sparse_polynom(f_mod / g_mod);
}


template <class T>
sparse_polynom<T> res_mod
(
    const sparse_polynom<T>& f,
    const sparse_polynom<T>& g,
    T mod
)
{
    sparse_polynom<residue<T> > f_mod = f;
    sparse_polynom<residue<T> > g_mod = g;

    new_module(f_mod, mod);
    new_module(g_mod, mod);

    return residue_to_int_sparse_polynom(f_mod % g_mod);
}



template <class T>
vector<sparse_polynom<T> > factor (sparse_polynom<T>& f, T p_hint)
{
    ////com std::cout << "\nfactor input: " << f << "\n";
    ARAGELI_TM_PRINT("function factor(..) started. Time="<<time(0)-stbh);
    vector<sparse_polynom<T> > factors;

    T p = p_hint - 1;

    while(1)
    {
        p = next_prime(p);
        //COUTCOM //com std::cout << "p = " << p;
        if (f.leading_coef() % p != T(0))
        {
            sparse_polynom<residue<T> > fp = f;
            new_module(fp, p);
            sparse_polynom<residue<T> > difffp = diff(f);
            new_module(difffp, p);
            if (gcd(fp, difffp) == sparse_polynom<residue<T> >(residue<T>(1)))
            {
                break;
            }
        }
    }


    T B = std::abs(f.leading_coef())*power(T(2), f.degree() - 1)*norm(f);//?????

    T q;
    for (q = p; q*q < B; q *= q)
        ;
    //COUTCOM std::cout << "q = " << q << std::endl;

    sparse_polynom<residue<T> > fq = f;
    new_module(fq, q);
    sparse_polynom<residue<T> > f1q = fq/fq.leading_coef();
    sparse_polynom<T> f1 = residue_to_int_sparse_polynom(f1q);

    new_module(f1q, p);
    new_module(fq, p);

    //COUTCOM std::cout << "Berlekamp" << std::endl;
    ARAGELI_TM_PRINT("p and q select ok. Time="<<time(0)-stbh);
    vector<sparse_polynom<residue<T> > > up = berlekamp(f1q);
    ARAGELI_TM_PRINT("berlekamp(f1q) finished. Count="<< up.length() <<". Time="<<time(0)-stbh);
    //exit(0);

    //COUTCOM std::cout << "up.length() = " << up.length() << std::endl;
    //COUTCOM std::cout << "up = " << up << std::endl;

    if (up.length() == 1)
    {
        //COUTCOM std::cout << "! polynomial is irreducible" << std::endl;
        T cnt = cont(f);
        //if (cnt != T(1))
        if(!is_unit(cnt))
        {
            factors.push_back(f/cnt);
            factors.push_back(cnt);
        }
        else
            factors.push_back(f);

        return factors;
    }

    vector<sparse_polynom<residue<T> > > vp, wp;
    ARAGELI_TM_PRINT("Starting prod_all_without_one(up)... Time="<<time(0)-stbh);
    wp = prod_all_without_one(up);
    ARAGELI_TM_PRINT("Starting euclid_bezout(wp, vp). Time="<<time(0)-stbh);
    //COUTCOM std::cout << "euclid bezout" << std::endl;
    euclid_bezout(wp, vp);
    //COUTCOM std::cout << "make degree less" << std::endl;
    ARAGELI_TM_PRINT("Starting make_degrees_in_bezout_coeff_less(up, vp). Time="<<time(0)-stbh);
    make_degrees_in_bezout_coeff_less(up, vp); // makes degree(v[i]) < degree(u[i])
    ARAGELI_TM_PRINT("Starting residue_to_int.. . Time="<<time(0)-stbh);
    vector<sparse_polynom<T> > u = residue_to_int_sparse_polynom_vector(up);
    vector<sparse_polynom<T> > v = residue_to_int_sparse_polynom_vector(vp);
    vector<sparse_polynom<T> > w = residue_to_int_sparse_polynom_vector(wp);
    ARAGELI_TM_PRINT("Hensel lifting starting... Time="<<time(0)-stbh);
    //COUTCOM std::cout << "Hensel lifting:";
    for (q = p; q*q < B; q *= q)
    {
        //COUTCOM std::cout << " q = " << q*q;
        hensel_quadratic_lifting(f1, u, v, w, q);
    }
    //COUTCOM std::cout << std::endl;
    ARAGELI_TM_PRINT("Hensel lifting finished. Time="<<time(0)-stbh);
    vector<int> M(u.length(), fromsize);

    for (std::size_t j = 0; j < u.length(); j++)
        M[j] = 1;
    ARAGELI_TM_PRINT("Searching phase(recombination) starting. Time="<<time(0)-stbh);
    for (int t = 1; t <= sum(M)/2; t++)
    {
        //COUTCOM std::cout << "subsets cardinality = " << t << std::endl;

        for
        (
            vector<std::size_t> subs = first_combination(u.length(), t, M);
            subs.length();
            next_combination(subs, u.length(), t, M)
        )
        {
            sparse_polynom<T> g = u[subs[0]];

            for (int j = 1; j < t; j++)
                g = mul_mod(g, u[subs[j]], q);

            g = mul_mod(g, sparse_polynom<T>(f.leading_coef()), q);

            typedef typename sparse_polynom< T >::coef_iterator coefs;

            //std::cout << "\n factor.g = " << g;

            for(coefs ci = g.coefs_begin(), cj = g.coefs_end(); ci != cj ; ++ci)
            {
                if (*ci > q/2)
                    *ci -= q;
            }

            g.normalize();
            //ARAGELI_ASSERT_1

            //std::cout << "\n factor.g after normalization = " << g << "\n";

            g /= cont(g);

            ARAGELI_ASSERT_1(!is_null(g));

            //if ((f.leading_coef() * f) % g == sparse_polynom<T>(T(0)))
            if (is_null((f.leading_coef() * f) % g))
            {
                //COUTCOM std::cout << "! New factor = " << g << std::endl;
                factors.push_back(g);
                f /= g;

                for (int j = 0; j < t; j++)
                    M[subs[j]] = 0;
            }

        }

    }
    ARAGELI_TM_PRINT("Searching phase finished. Time="<<time(0)-stbh);

    if (f != sparse_polynom<T>(T(1)))
    {
        //COUTCOM std::cout << "!! New factor = " << f << std::endl;
        factors.push_back(f);
    }

    return factors;
}


template <class T>
vector<sparse_polynom<T> > factor (sparse_polynom<T>& f)
{
    return factor(f, T(2));
}

template <typename P, typename M, typename VP, typename Func>
void residue_berlekamp_router (const P& p, const M& m, VP& res, Func func)
{
    // WARNING! TEMPORARY SOLUTION!
    /*dbg*///std::cout<<"residue_berlekamp_router started\n";
    typedef typename P::coef_type Coef;
    typedef residue<Coef, M> RCoef;
    typedef typename P::template other_coef<RCoef>::type RP;
    typedef typename RP::monom_iterator RPMI;
    typedef typename RP::coef_iterator RPCI;
    typedef typename P::monom PM;

    RP rp = p;
    /*dbg*///com std::cout<<"rp"<<p<<"\n";
    // applying the module to each coefficient explicitly
    for(RPCI i = rp.coefs_begin(); i != rp.coefs_end(); ++i)
    {
        i->module() = m;
        i->normalize();
    }
    rp.normalize(); // to shrink possible zero coefficients
    /*dbg*///com std::cout<<"2rp="<<rp<<"\n";
    vector<RP> rres = rp.is_const() ? vector<RP>(1, rp, fromval) : func(rp);
    /*dbg*///com std::cout<<"rres="<<rres<<"\n";
    res.assign(rres.size());
    /*dbg*///com std::cout<<"res="<<res<<"\n";
    // moving to non-residue type
    for(typename VP::size_type i = 0; i < res.size(); ++i)
    {
        RP& rcur = rres[i];
        P& cur =  res[i];

        for(RPMI j = rcur.monoms_begin(); j != rcur.monoms_end(); ++j)
            cur.insert(cur.monoms_end(), PM(j->coef().value(), j->degree()));
    }
    /*dbg*///com std::cout<<"2res="<<res<<"\n";
}


struct berlekamp_f
{
    template <typename P>
    vector<P> operator() (const P& p) const
    {
        return berlekamp(p);
    }
};


struct pre_berlekamp_f
{
    template <typename P>
    vector<P> operator() (const P& p) const
    {
        return pre_berlekamp(p);
    }
};


} // namespace _Internal


// WARNING! TODO Make it inline.
template <typename P, typename M, typename VP, typename Ctrler>
void factorize_berlekamp (const P& p, const M& m, VP& res, Ctrler ctrler)
{
    _Internal::residue_berlekamp_router(p, m, res, _Internal::pre_berlekamp_f());
}


// WARNING! TODO Make it inline.
template <typename P>
vector<P> factorize_berlekamp (const P& p)
{
    return _Internal::pre_berlekamp(p);
}


// WARNING! TODO Make it inline.
template <typename P, typename M, typename VP, typename Ctrler>
void factorize_berlekamp_msf (const P& p, const M& m, VP& res, Ctrler ctrler)
{
    _Internal::residue_berlekamp_router(p, m, res, _Internal::berlekamp_f());
}


// WARNING! TODO Make it inline.
template <typename P>
vector<P> factorize_berlekamp_msf (const P& p)
{
    return p.is_const() ? p : _Internal::berlekamp(p);
}


template <typename P, typename VP, typename Ctrler>
void factorize_berlekamp_hensel
(
    const P& p,
    VP& res,
    Ctrler ctrler
)
{
    #ifdef TIME_MEASURE
        stbh=time(0);
    #endif
    ARAGELI_TM_PRINT("factorize_berlekamp_hensel started. input="<<p<<". Time= "<<stbh<<"=0");

    if(p.degree() <= 1)
    {
        res.assign_fromval(1, p);
        return;
    }

    vector<P> sff;
    sqrfree_factorize_poly_integer(p, sff);
    ARAGELI_TM_PRINT("sqrfree step finished. sff.length()="<<sff.length()<<". Time="<<time(0)-stbh);
    //com   std::cout<<"\nsff="<<sff<<"\n";
    res.resize(0);

    for(std::size_t i = 0; i < sff.size(); ++i)
        if
        (
            //sff[i].is_const() && !is_unit(sff[i].leading_coef()) || //FIXED
            is_positive(sff[i].degree())
        )
        {
            //com           std::cout<<"sff[i]"<<sff[i]<<"\n";
            ARAGELI_TM_PRINT("sff["<<i<<"] factorization started. Time="<<time(0)-stbh);
            vector<P> locres = _Internal::factor(sff[i]);
            ARAGELI_TM_PRINT("sff["<<i<<"] factorization ok. Time="<<time(0)-stbh);
            for(std::size_t j = 0; j <= i; ++j)
                res.insert_fromvec(res.end(), locres);
        }
        else if(sff[i].is_const())
        {
            //anti-bug code
            if(!is_unit(sff[i].leading_coef()))
                res.push_back(sff[i]);
        }
    ARAGELI_TM_PRINT("factorize_berlekamp_hensel finished at: "<<time(0)-stbh);
    //res;
}


// WARNING! TODO Make it inline.
template <typename P, typename VP, typename Ctrler>
void factorize_berlekamp_hensel_sf
(
    const P& p,
    VP& res,
    Ctrler ctrler
)
{
    P pp = p;
    if(!pp.is_const())
        res = _Internal::factor(pp);
    else
        res = p;
}



} // namespace Arageli


#if ARAGELI_DEBUG_LEVEL > 3

#include "big_int.hpp"

namespace Arageli
{

template vector<sparse_polynom<residue<int> > >
factorize_berlekamp
(const sparse_polynom<residue<int> >& p);

template vector<sparse_polynom<residue<big_int> > >
factorize_berlekamp
(const sparse_polynom<residue<big_int> >& p);

template vector<sparse_polynom<int> >
factorize_berlekamp
(
    const sparse_polynom<int>& p,
    const int& m
);

template vector<sparse_polynom<big_int> >
factorize_berlekamp
(
    const sparse_polynom<big_int>& p,
    const big_int& m
);


} // namespace Arageli

#endif


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
