/*****************************************************************************

    groebner_basis.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2002, 2006 Max A. Alekseyev
    Copyright (C) 2007 Sergey S. Lyalin

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
    \file groebner_basis.cpp
    \brief The groebner_basis.hpp file stuff implementation.

    ADD ADDITIONAL FILE DESCRIPTION HERE
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_groebner_basis)

#include <iostream>    // probably only for debugging
#include <map>

#include "groebner_basis.hpp"


namespace Arageli
{

namespace _Internal
{

/*
    Log (Max):
        Fri  2002/11/01 ver 2.0: Everything is rewritten & optimized
        Sat  2002/11/02 ver 2.1: Internal types of polynom class used
        Sat  2002/11/02 ver 2.2: normalf() does complete reduction
        Sun  2002/11/03 ver 3.0: binary relation is used
        Mon  2002/11/04 ver 4.0: new effective representation
        Mon  2006/01/14 ver 4.1: g++ 4.x compatible
        Wed  2006/05/17 ver 4.2: normalf() can return coefficients of reduction

    Log (Sergey):
        Mon  2007/01/08 All stuff from the original Max's grobner.h file
        has been integrated into the library as internal routines
        that are wrapped by the public interface.
*/


/*
    We keep Grobner basis as two maps
    lt(p)->id   and  id->p
    id is increasing with each new polynom
*/

template<class poly_class>
class gbasis :
    public std::map<long,poly_class>
{
public:

    // Inherited types
    typedef typename poly_class::multidegree_type  term_type;
    typedef typename poly_class::coef_type  coef_type;
    typedef typename poly_class::multidegree_less_type term_order;

    // internal types
    typedef long id_type;
    typedef std::map<term_type,id_type,term_order> tid_type;
    typedef std::map<id_type,poly_class> idp_type;

    typedef typename idp_type::iterator iterator;
    typedef typename idp_type::const_iterator const_iterator;
    typedef typename idp_type::reverse_iterator reverse_iterator;

    using idp_type::operator[];
    using idp_type::lower_bound;
    using idp_type::find;

    using idp_type::begin;
    using idp_type::end;
    using idp_type::rbegin;
    using idp_type::rend;

private:
    tid_type tid;
    id_type idcount;

    static std::map<poly_class,poly_class> foo_mpp;

public:

    template <typename V>
    gbasis (V&);

    void normalf (poly_class&, std::map<poly_class, poly_class>& = foo_mpp) const;
    bool add (poly_class&);
    bool addreduce (poly_class&);

}; // class gbasis

template<class poly_class>
std::map<poly_class, poly_class> gbasis<poly_class>::foo_mpp;


// S-polynomial
// Simplified version
template<class poly_class>
void Spoly
(
    const poly_class& p,
    const poly_class& q,
    poly_class& s,
    typename poly_class::multidegree_type& l
)
{
#ifdef DEBUG
    cerr << "Spoly(" << p << "," << q << ")\n";
#endif

    typedef typename poly_class::multidegree_type MD;

    const MD& pmd = p.leading_multidegree();
    const MD& qmd = q.leading_multidegree();

    l = elwisemax(pmd, qmd);
    poly_class u(q.leading_coef(), l - pmd), v(-p.leading_coef(), l - qmd);
    s = u*p + v*q;
}


// Compute normal form of polynomial with respect to Groebner basis
template<class poly_class>
void gbasis<poly_class>::normalf
(
    poly_class& p,
    std::map<poly_class, poly_class>& bsf
) const
{
#ifdef DEBUG
    cerr << "reducepol(" << p << ")\n";
#endif
    if(&bsf != &foo_mpp)
        bsf.clear();

    if(p.is_null())
        return;

    for
    (
        typename poly_class::monom_iterator j = p.monoms_end();
        j != p.monoms_begin();
    )
    {
        --j;
        term_type tr = j->multidegree();
        coef_type cr = j->coef();

        for
        (
            typename tid_type::const_iterator i = tid.begin();
            i != tid.upper_bound(tr);
            i++
        )
            if(all_greater_equal(tr, i->first))
            {
                // WARNING!!!
                // As we have another representation for polynomials,
                // there is a problem --- invalidation of j in
                // the following code.
                // To avoid this proble we store the multidegree
                // where j, and at the end of the block restore j
                // at the monom with the same multidegree.

                ++j;

                // remember where j
                bool jend = (j == p.monoms_end());
                typename poly_class::multidegree_type jmd;
                if(!jend)
                    jmd = j->multidegree();
                // -------

                const_iterator k = find(i->second);
                poly_class q(cr/k->second.leading_coef(), tr - i->first);
                p -= q*k->second;    // invalidates j
                if(&bsf != &foo_mpp) bsf[k->second] += q;

                // restore j
                if(jend)
                    j = p.monoms_end();
                else
                {
                    for(j = p.monoms_begin(); j != p.monoms_end(); ++j)
                        if(j->multidegree() == jmd)
                            break;

                    //ARAGELI_ASSERT_1(j == p.monoms_end());
                }
                // -------

                break;
            }
    }
#ifdef DEBUG
    cerr << p << "\n";
#endif
}


// Add a new polynomial to Grobner basis,
// and recalculate the basis
template<class poly_class>
bool gbasis<poly_class>::add (poly_class& p)
{
#ifdef DEBUG
    cerr << "add(" << p << ")\n";
#endif

#ifdef INDICATOR
    if(++iter==ITERS)
    {
        iter = 0;
        prgs = (prgs+1)%4;
        cerr
            << size() << "/" << (unsigned int)idcount << "/"
            << rbegin()->second.deg() << " " << progress[prgs] << "\r";
    }
#endif
    //cerr << ">";
    if(p.is_null())
        return false;
    normalf(p);
    if(p.is_null())
        return false;
    //cerr << size();
    id_type id = idcount++;
    term_type lp = p.leading_multidegree();

    //cerr << size() << "/" << (unsigned int)idcount << "          \r";

    tid[lp] = id;
    operator[](id) = p;

    // Reducing existing elements wrt p
    bool loop = true;
    id_type id_iter = id;
    while(loop)
    {
        //cerr << "[";
        loop = false;

        for(iterator i = lower_bound(id_iter); i != begin();)
            if(all_greater_equal((--i)->second.leading_multidegree(), lp) )
            {
                id_iter = i->first;
                poly_class q = i->second, r;
                //cerr << "-";
                erase(i);
                tid.erase(q.leading_multidegree());

                divide(q, p, r, q);
                if(add(q)) if(find(id) == end())
                    return true;        // p was removed from the basis
                loop = true;
                break;
            }
    }

    // Adding all Spoly(p,q) for all q with id(q) < id(p)
    loop = true;
    id_iter = id;
    while(loop)
    {
        //cerr << "]";
        loop = false;
        for(const_iterator i=lower_bound(id_iter);i!=begin();)
        {
            i--;
            poly_class s;
            term_type l;

            Spoly(p,i->second,s,l);

            // first Buchberger criterion
            if(sum(l) == p.degree() + i->second.degree())
                continue;

            // second Buchberger criterion
            bool b2 = false;
            for(const_iterator j = i; j != begin();)
                if(all_greater_equal(l, (--j)->second.leading_multidegree()))
                {
                    //cerr << "B";
                    b2 = true;
                    break;
                }

                if(b2)continue;

                id_iter = i->first;
                //cerr << "T";

                if(add(s))
                {
                    if(find(id) == end())
                        return true;        // p was removed from the basis
                    loop = true;
                    break;
                }
        }
    }
    return true;
}


// Add a new polynomial to the basis,
// and reduce its current elements
// Return 'true' if element was added
// INVALIDATE ALL gbasis::iterators
template<class poly_class>
bool gbasis<poly_class>::addreduce (poly_class& p)
{
#ifdef DEBUG
    cerr << "addreduce(" << p << ")\n";
#endif
    normalf(p);
    if(p.is_null())
        return false;

    id_type id = idcount++;
    term_type lp = p.leading_multidegree();

    tid[lp] = id;
    operator[](id) = p;

    // Reducing existing elements wrt p
    bool loop = true;
    id_type id_iter = id;

    while(loop)
    {
        loop = false;
        for(iterator i = lower_bound(id_iter); i != begin();)
            if(all_greater_equal((--i)->second.leading_multidegree(), lp))
            {
                id_iter = i->first;
                poly_class q = i->second, r;
                //cerr << "-";
                erase(i);
                tid.erase(q.leading_multidegree());

                divide(q, p, r, q);
                if(addreduce(q)) if(find(id) == end())
                    return true;        // p was removed from the basis
                loop = true;
                break; }
    }

    return true;
}


// Constructor
template <class poly_class>
template <typename V>
gbasis<poly_class>::gbasis (V& basis)
{
#ifdef DEBUG
    cerr << "gbasis(...)\n";
#endif
    idcount = 0;

    // add all the elements to the basis
    for(int i = 0; i < basis.size(); i++)
        addreduce(basis[i]);

    // add all Spoly
    id_type id_iter = idcount;
    bool loop = true;

    while(loop)
    {
        loop = false;
        for(const_iterator i = lower_bound(id_iter); i != begin();)
        {
            i--;
            for(const_iterator j = i; j != begin();)
            {
                j--;

                poly_class s;
                term_type l;

                Spoly(i->second, j->second, s, l);

                // first Buchberger criterion
                if(sum(l) == i->second.degree() + j->second.degree())
                    continue;

                id_iter = i->first;

                if(add(s))
                {
                    // WARNING!!!
                    // What does the following output mean?
                    //std::cerr << "+++++++\n";
                    loop = true;
                    break;
                }
            }
            if(loop)break;
        }
    }
}


} // namespace _Internal


template <typename VP, typename VGB>
void groebner_basis (const VP& vp, VGB& vgb)
{
    // WARNING! I don't know how to expand existing basis
    // that isn't represented as _Internal::gbasis.
    // So, I just add vp to vgb and rebuild the basis.

    for(typename VP::size_type i = 0; i < vp.size(); ++i)
        vgb.push_back(vp[i]);

    typedef _Internal::gbasis<typename VP::element_type> GB;
    GB gb(vgb);

    vgb.resize(gb.size());
    typename VGB::size_type j = 0;
    for(typename GB::iterator i = gb.begin(); i != gb.end(); ++i, ++j)
        vgb[j] = i->second;
}


}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

    // PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
