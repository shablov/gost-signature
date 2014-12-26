/*****************************************************************************

    sqrfree.hpp

    This file is a part of Arageli library.

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
    \file sqrfree.hpp
    \brief Algorithms for doing square-free factorization of polynomial.
*/


#ifndef _ARAGELI_sqrfree_hpp_
#define _ARAGELI_sqrfree_hpp_

#include "config.hpp"

#include <cstddef>

#include "type_traits.hpp"
#include "factory.hpp"
#include "_utility.hpp"
#include "misc.hpp"


#include "std_import.hpp"

//****************************************************************************

namespace Arageli
{


/// Makes h square-free.
template <typename P>
inline P sqrfree_poly_rational (P h)
{
    return
        h.is_const() ?
        h :
        h/gcd(h, diff(h));
}


/// Makes h square-free over residue field Zp. WARNING! WRONG IMPLEMENTATION. DO NOT USE.
template <typename P>
P sqrfree_poly_mod (P h)
{
    if(h.is_const())
        return h;

    typename P::coef_type lc = h.leading_coef();
    h /= lc;    // is it needed?
    P res = unit<P>();

    typedef typename P::coef_type T;
    typedef typename T::module_type M;

    // obtain the module
    M m = h.leading_coef().module();
    ARAGELI_ASSERT_1(m > unit(m));

    std::size_t k1 = 0;

    for(;;)
    {

    step2:

        if(h.is_const())
            break;

        P diffh = diff(h);

        P g = gcd(h, diffh);
        P c = h/g;
        //COUTCOM std::cerr << "\nStep 3: \ng = " << g << ", \nc = " << c << "\n";
        k1 = 0;

        for(;;)
        {

            if(c.is_const())
            {
                for
                (
                    typename P::degree_iterator i = g.degrees_begin();
                    i != g.degrees_end();
                    ++i
                )
                {
                    ARAGELI_ASSERT_1(is_divisible(*i, m));
                    *i /= m;
                }

                h = g;
                //COUTCOM std::cerr << "\nStep 4: h = " << h;
                goto step2;    // to Step 2
            }

            //COUTCOM std::cerr << "\nStep 5: ";
            ++k1;
            //COUTCOM std::cerr << "k1 = " << k1;

            /*
            if(is_divisible(k1, m))
            {
                ARAGELI_ASSERT_1(is_divisible(g, c));
                g /= c;
                ++k1;
                //COUTCOM std::cerr << "\nis_divisible(g, c), \ng = " << g << ", \nk1 = " << k1;
            }
            */

            //COUTCOM std::cerr << "\nStep 6: ";
            P w = gcd(g, c), a = c/w;
            c = w;
            g /= c;

            //COUTCOM std::cerr << "\nw = " << w << ", \na = " << a << ", \nc = " << c << ", \ng = " << g << "\n";

            if(!a.is_const())res *= a;
        }

        break;

    /*
    {
        //COUTCOM std::cout << "h = " << h;
        output_aligned(//COUTCOM std::cout << "\nres =\n", res);
        P t = unit<P>();
        for(std::size_t i = 0; i < res.size(); ++i)
            t *= power(res[i], i+1);
        //COUTCOM std::cout << "t = " << t << "\n";
        if(t != h)//COUTCOM std::cout << "############## AAAAAAAAAA #################\n";
    }
    */

    } // for(;;)

    return res;
}


template <typename P, typename V>
V& sqrfree_factorize_poly_rational (P h, V& res)
{
    //TODO: Make it faster!
    //TODO: Resolve normalization of the gcd result.

    if(h.is_const())
    {
        res.assign_fromval(1, h);
        return res;
    }

    res.resize(0);

    typename P::coef_type lc = h.leading_coef();
    h /= lc;    // is it needed?

    P g = gcd(h, diff(h));
    g /= safe_reference(g.leading_coef());
    P c, d;
    c = h/g;
    d = diff(h)/g - diff(c);

    ARAGELI_ASSERT_1(!c.is_unit());

    while(!c.is_unit())
    {
        res.push_back(gcd(c, d));
        res.back() /= safe_reference(res.back().leading_coef());
        c = c/res.back();
        d = d/res.back() - diff(c);
    }

    ARAGELI_ASSERT_1(!res.is_empty());
    res[0] *= lc;
    return res;
}


template <typename P, typename V>
V& sqrfree_factorize_poly_integer (P h, V& res)
{
    typedef typename P::coef_type T;
    typedef typename P::template other_coef<rational<T> >::type PR;
    PR pr = h;    // correct if T is integer type
    typedef typename V::template other_element_type<PR >::type VR;
    VR rres;
    sqrfree_factorize_poly_rational(pr, rres);

    // check if all coefs are integers
    ARAGELI_DEBUG_EXEC_1
    (
        for(typename VR::size_type i = 0; i < rres.size(); ++i)
            ARAGELI_ASSERT_1(PR(P(rres[i])) == rres[i]);
    );

    return res = rres;
}


// WARNING!
template <typename P, typename V>
V& sqrfree_factorize_poly_mod (P h, V& res)
{
    ARAGELI_ASSERT_ALWAYS(!"Function sqrfree_factorize_poly_mod works properly!");  // ???

    ARAGELI_DEBUG_EXEC_1(P _debug_origh = h);    // for proof at the tail

    if(h.is_const())
    {
        res.assign_fromval(1, h);
        return res;
    }

    res.resize(0);
    typename P::coef_type lc = h.leading_coef();
    h /= lc;    // is it needed?

    typedef typename P::coef_type T;
    typedef typename T::module_type M;

    // obtain the module
    M m = h.leading_coef().module();
    ARAGELI_ASSERT_1(m > unit(m));

    std::size_t k1 = 0, k2 = 0;
    M k = unit<M>();

    for(;;)
    {

    step2:

        if(h.is_const())
        {
            break;
        }

        P diffh = diff(h);

        #if 0

        if(is_null(diffh))
        {
            //COUTCOM std::cout << "\nPHASE 1\n";
            P h1 = h;
            k = 0;

            for
            (
                typename P::degree_iterator i = h1.degrees_begin();
                i != h1.degrees_end();
                ++i
            )
            {
                ARAGELI_ASSERT_1(is_divisible(*i, m));
                *i /= m;
            }

            V tres;
            sqrfree_factorize_poly_mod(h1, tres);
            res.assign_fromval(m-1, unit(h1));
            res.insert_fromvec(res.end(), tres);
        }
        else
        {
            //COUTCOM std::cout << "\nPHASE 2\n";
            P g = gcd(h, diffh);
            //g /= safe_reference(g.leading_coef());    // is it needed?
            P c/*, d*/;
            c = h/g;

            // This step differs from rational version
            // See Xavier-Francois Roblot:
            // Polynomial Factorization Algorithms over Number Fields,
            // J. Symbolic Computation (2002) 11, 1--14
            // Actually from Cohen, H. (1993). A Course in Computational
            // Algebraic Number Theory, volume 138 of Graduate Texts in Math.Springer-Verlag.
            ++k;
            if(is_divisible(k, m))
            {
                ARAGELI_ASSERT_1(is_divisible(g, c));
                g /= c;
                ++k;
            }

            //d = diffh/g - diff(c);
            res.push_back(c/gcd(c, g));
            //COUTCOM std::cout
                << "\nh = " << h
                << "\ndiffh = " << diffh
                << "\ngcd(h, diffh) = " << gcd(h, diffh)
                << "\ng = " << g
                << "\nc = " << c
                //<< "\nd = " << d
                << "\n";
            V tres;
            sqrfree_factorize_poly_mod(g, tres, k);    // WARNING! RECURSION!
            res.insert_fromvec(res.end(), tres);
        }

        #endif

        P g = gcd(h, diffh);
        P c = h/g;
        //COUTCOM std::cerr << "\nStep 3: \ng = " << g << ", \nc = " << c << "\n";
        k1 = 0;

        for(;;)
        {

            if(c.is_const())
            {
                for
                (
                    typename P::degree_iterator i = g.degrees_begin();
                    i != g.degrees_end();
                    ++i
                )
                {
                    ARAGELI_ASSERT_1(is_divisible(*i, m));
                    *i /= m;
                }

                //if(k == 0/*!g.is_const()*/)res.insert_fromvec(res.end(), V(m-1, unit(h), fromval));
                k *= m;
                h = g;
                //COUTCOM std::cerr << "\nStep 4: h = " << h;
                goto step2;    // to Step 2
            }

            //COUTCOM std::cerr << "\nStep 5: ";
            ++k1;
            //COUTCOM std::cerr << "k = " << k;

            //if(is_divisible(k1, m))
            //{
            //    ARAGELI_ASSERT_1(is_divisible(g, c));
            //    g /= c;
            //    //res.insert_fromvec(res.end(), V(m-1, unit(h), fromval));
            //    ++k1;/*k2 += k;*/
            //    //COUTCOM std::cerr << "\nis_divisible(g, c), \ng = " << g << ", \nk = " << k;
            //}

            //COUTCOM std::cerr << "\nStep 6: ";
            P w = gcd(g, c), a = c/w;
            c = w;
            g /= c;

            //COUTCOM std::cerr << "\nw = " << w << ", \na = " << a << ", \nc = " << c << ", \ng = " << g << "\n";

            if(!a.is_const())
            {
                if(k + k2 > 1)
                {
                    res.insert_fromvec(res.end(), V(k - unit(k) + k2, unit(h), fromval));
                    k2 = 0;
                }

                res.push_back(a);
                //if(res.is_empty())res.push_back(unit(h));
            }
            else
                k2 += k;
        }

        break;

        /*
        {
            //COUTCOM std::cout << "h = " << h;
            output_aligned(//COUTCOM std::cout << "\nres =\n", res);
            P t = unit<P>();
            for(std::size_t i = 0; i < res.size(); ++i)
                t *= power(res[i], i+1);
            //COUTCOM std::cout << "t = " << t << "\n";
            if(t != h)//COUTCOM std::cout << "############## AAAAAAAAAA #################\n";
        }
        */

    } // for(;;)

    ARAGELI_ASSERT_1(!res.is_empty());
    res.front() *= lc;

    ARAGELI_DEBUG_EXEC_1
    (
        P _debug_t = unit<P>();
        for(std::size_t _debug_i = 0; _debug_i < res.size(); ++_debug_i)
            ARAGELI_DEBUG_EXEC_1(_debug_t *= power(res[_debug_i], _debug_i+1))
    );

    {
        //COUTCOM std::cerr << "\nh = " << h;
        //COUTCOM std::cerr << "\n_debug_origh = " << _debug_origh;
        //output_aligned(//COUTCOM std::cerr << "\nres =\n", res);
        P t = unit<P>();
        for(std::size_t i = 0; i < res.size(); ++i)
            t *= power(res[i], i+1);
        //COUTCOM std::cerr << "t = " << t << "\n";
    }

    ARAGELI_ASSERT_1(_debug_t == _debug_origh);

    return res;
}


} // namespace Arageli


#if 0
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SQRFREE
    #include "sqrfree.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SQRFREE
#endif
#endif

#endif  //  #ifndef _ARAGELI_sqrfree_hpp_
