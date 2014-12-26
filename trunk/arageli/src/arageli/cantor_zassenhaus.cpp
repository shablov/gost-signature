/*****************************************************************************

    cantor_zassenhaus.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2006--2007 Sergey V. Lobanov

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
    \file cantor_zassenhaus.cpp
    \brief The cantor_zassenhaus.hpp file stuff implementation.

    Algorithms: Sparse versions: Distinct Degree Factorization,
                Fast polynomial composition by Brent and Kung

    References:<BR/>
    1. Victor Shoup. A new polynomial factorization algorithm and its implementation, 1996<BR/>
    2. Erich Kaltofen and Victor Shoup. Subquadratic-time factoring of polynomials
        over finite field, 1998<BR/>
    3. R. P. Brent and H. T. Kung. Fast Algorithms for Manipulating Formal Power Series, 1978<BR/>
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_cantor_zassenhaus)

// REFERENCE ADDITIONAL HEADERS HERE
//TODO: remove sstream and iostream
#include "factory.hpp"
#include "vector.hpp"
#include "powerest.hpp"
#include "gcd.hpp"
#include <iostream>    //for debugging and assert
#include "sqrfree.hpp" //for assert
#include <sstream> //for assert
#include "cantor_zassenhaus.hpp"


namespace Arageli
{

namespace _Internal
{
///It is an internal function. Please DO NOT USE.
/*  PR=polynom-result, PG=polynom-g,VPH=vector<polynom-H>,
    VT=vector-temp,GIT=iterator for g,N=vector-temp-length,
    HH=type(H)::value_type*/
template
<
    typename HH,
    typename PR,
    typename PG,
    typename VPH,
    typename VT,
    typename GIT,
    typename T1
>
bool perv_inner_product_sparse
(
    PR& res,
    const PG& g,
    const T1& st,
    const VPH& H,
    //const N& n,
    VT& vt,GIT& iter
)
{
    //com std::cout<<"\nin_pr started g="<<g<<"\n st="<<st<<"\n H="<<H<<"\n vt="<<vt<<"\n\n";//<<" iter="<<iter;
    //com std::cout<<"\nst="<<st<<"\n";
    //fin=min(fin,v.degree());
    //HH::degree_type i;
    GIT first_monom=g.monoms_begin();
    while(true)//WARNING. If bug then :(
    {
        //i=iter->degree();
        if(iter->degree()>=st)
        {
            const HH& h=H[iter->degree()-st];
            // //com std::cout<<"iterdegst="<<iter->degree()-st<<"h="<<h<<"\n";
            const typename HH::coef_type& gi=iter->coef();
            typedef typename HH::monom_const_iterator hmonoms;
            //main actions
            for(hmonoms j=h.monoms_begin(),h_last_monom=h.monoms_end();j!=h_last_monom;++j)
                vt[j->degree()] += gi * j->coef();//main action
            //go to next coef of polynomial g
            if(iter!=first_monom)
                iter--;
            else
            {
                // //com std::cout<<"\nAssign from: "<<vt<<"\n";
                res.assign_fromvec(vt);
                //com std::cout<<"res_in="<<res<<"\n";
                return false;
            }
        }
        else
        {
            break;//exit from while(true)
        }
    }
    // //com std::cout<<"\nAssign from: "<<vt<<"\n";
    res.assign_fromvec(vt);
    //com std::cout<<"res_in="<<res<<"\n";
    return true;
}

///res=g(h) mod f, deg(h)<deg(f) and deg(g)<deg(f)
template<typename P1,typename P2,typename P3,typename P4>
void fast_poly_composition_sparse(const P1& g, const P2& h, const P3& f,P4& res)
{
    if(!g.is_const())
    {
        typedef typename P1::degree_type gdeg_type;
        typedef typename P1::coef_type gcoef_type;//residue<..>
        typedef typename P1::monom_const_iterator monoms;
        typedef typename P2::degree_type hdeg_type;
        typedef typename P2::coef_type hcoef_type;
        typedef typename P3::degree_type fdeg_type;
        gdeg_type deg_g=g.degree();
        fdeg_type deg_f=f.degree();
        typename gcoef_type::module_type p=g.leading_coef().module();
        ARAGELI_ASSERT_0(h.degree()<deg_f);
        ARAGELI_ASSERT_1(deg_g<deg_f);//is it needed?
        gdeg_type gdeg_1=factory<gdeg_type>::unit();
        gdeg_type m=sqrt(deg_g+gdeg_1);
        vector<P2> H;//maybe vector<P2&> H;

        H.push_back
        (
            P2
            (
                hcoef_type(factory<typename hcoef_type::value_type>::unit(), p),
                factory<hdeg_type>::null()
            )
        );//H[0]=1(mod p)*x^0 in Z_p[x]

        H.push_back(h);//H[1]=h;
        for(gdeg_type i=gdeg_1;i<m;i++)
            H.push_back((H[i]*h)%f);//is it correct?

        gdeg_type l=(deg_g+m)/m-1;
        //com std::cout<<"\nm="<<m<<"  l="<<l<<"\n";
        //com std::cout<<"\nH="<<H<<"\n";
        P4 s,t;
        vector<gcoef_type> vt(deg_f);
        for(fdeg_type i=factory<fdeg_type>::null();i<deg_f;i++)
            vt[i].module()=p;
        const P2& Hm=H[m];
        //com std::cout<<"\nH[m]="<<Hm<<"\n";
        monoms iter=--g.monoms_end();
        bool do_next_perv_inner_product =
            _Internal::perv_inner_product_sparse<P2>(t,g,m*l,H,vt,iter);
        bool next_add_s=true;
        //com std::cout<<"\nt="<<t<<"\n";
        for(gdeg_type i=--l; i>=factory<gdeg_type>::null(); i--)
        {
            if(do_next_perv_inner_product)
            {
                for(fdeg_type ii=factory<fdeg_type>::null();ii<deg_f;ii++)//clear vt
                    vt[ii].value()=factory<typename gcoef_type::value_type>::null();
                ////com std::cout<<"!";
                do_next_perv_inner_product=
                    _Internal::perv_inner_product_sparse<P2>(s,g,m*i,H,vt,iter);
                if(!do_next_perv_inner_product)
                {
                    t*=Hm;
                    t%=f;
                    t+=s;
                    next_add_s=false;
                    continue;
                }
            }
            t*=Hm;
            t%=f;
            if(next_add_s)
                t+=s;
            //com std::cout<<"curr_t="<<t<<"\n";
        }
        res=t;//maybe replace res <- t and remove variable res ??
    }
    else
    {
        res=P4(g);//g=const(x); const(h) mod f = const; g(h) mod f = g; res=g
    }
}

#define ARAGELI_INTERNAL_FAST_POLY_COMPOSITION_SPARSE
///res=g(h) mod f
//It is wrapper for fast_poly_composition_sparse function
template<typename P1,typename P2,typename P3,typename P4>
void poly_composition_sparse(const P1& g, const P2& h, const P3& f,P4& res)
{
    #ifdef ARAGELI_INTERNAL_FAST_POLY_COMPOSITION_SPARSE

        ARAGELI_DEBUG_EXEC_2
        (
            P1 g2=P1(g);
            P2 h2=P2(h);
            P3 f2=P3(f);
            P4 res2=g2.subs(h2)%f2;
        );

        fast_poly_composition_sparse(g,h,f,res);
        ARAGELI_DEBUG_EXEC_2
        (
            if(res!=res2)
            {
                std::ostringstream msg;
                msg
                    << "\n!!!!!!!\nfast_poly_composition FAILED\n"
                    << "g="<<g2<<"\n\nh="<<h2<<"\n\nf="<<f2<<"\n\n"
                    << "res="<<res<<"\n\ng.subs(h)%f="<<res2<<"\n!!!!!!!!\n";
                std::cout<<msg.str();//replace it to correspondence macros
                ARAGELI_ASSERT_2(false);
            }
        );
    #else
        res=g.subs(h)%f;
    #endif
}

#ifdef ARAGELI_INTERNAL_FAST_POLY_COMPOSITION_SPARSE
    #undef ARAGELI_INTERNAL_FAST_POLY_COMPOSITION_SPARSE
#endif

}

template<typename P,typename V, typename V2>
void DDF_sparse(const P& f_in, V& res,V2& v_deg)
{
    typedef typename P::degree_type deg_type;
    if(f_in.is_const())
    {
        res.push_back(f_in);
        v_deg.push_back(factory<deg_type>::null());
        return;
    }
    //std::cout<<"sqrfree="<<sqrfree_poly_mod(f_in)<<"\n";
    ARAGELI_ASSERT_0(sqrfree_poly_mod((P(f_in))/=f_in.leading_coef())==(P(f_in)/=f_in.leading_coef()));
    typedef typename P::coef_type coef_type;
    typedef typename coef_type::module_type mod_type;
    P f=P(f_in);//copy an input polynomial
    coef_type lc=f.leading_coef();//lc <- lc(f)
    mod_type p=lc.module();//extracting module; p <- lc.module
    ARAGELI_ASSERT_0(!is_null(p));//incorrect module
    deg_type n=f.degree();//n <- deg(f)
    if(!is_null(res.size()))
        res.resize(factory<typename V::size_type>::null());
    if(!is_null(v_deg.size()))
        v_deg.resize(factory<typename V2::size_type>::null());
    //if(is_null(n))//input polynomial const?
    if(!is_unit(lc))// lc==1 ?
    {
        //f.div_scalar(lc);
        f/=lc;// f <- f/lc;
        res.push_back(P(lc,factory<deg_type>::null()));
        v_deg.push_back(factory<deg_type>::null());
    }
    if(is_unit(n))//input polynomial degree == 1 ?
    {
        res.push_back(f);
        v_deg.push_back(n);
        return;
    }
    vector<P> h;
    vector<P> H(1);//H[0] is useless
    //step 0:
    deg_type B=n/2;
    deg_type l=sqrt(B);// sqrt(1) must be equal to 1 !!!
    ARAGELI_ASSERT_2(is_positive(B));//n/2 must be greatear than 0
    ARAGELI_ASSERT_1(is_positive(l));//sqrt(n/2) must be greatear than 0
    deg_type m,r;
    divide(B,l,m,r);
    if(!is_null(r))
        m++;
    coef_type coef_one=factory<coef_type>::unit();//coef_one=1(mod 0)
    coef_one.module()=p;//coef_one=1(mod p)
    P tmp=P(coef_one,factory<deg_type>::unit());//tmp=x in Z_p[x]
    h.push_back(tmp);//h[0]=x in Z_p[x]
    #if 0
        std::cout<<"STEP 0 result: \n";
        std::cout<<"f="<<f;
        std::cout<<"\nB="<<B<<"\nl="<<l<<"\nm="<<m<<"\n";
        std::cout<<"h="<<h<<"\n";
        std::cout<<"H="<<H<<"\n";
    #endif
    //step 1:
    P h1;
    tmp.leading_monom().degree()=p;//tmp=x^p
    h1=tmp%f;//h1 <- (x^p mod f(x)) in Z_p[x]
    h.push_back(h1);
    #if 0
        std::cout<<"STEP 1 result: \n";
        std::cout<<"h="<<h<<"\n";
    #endif
    //step 2:
    tmp=h1;//copy h1 to tmp.
    for(deg_type i=factory<deg_type>::unit();i<l;i++)
    {
        _Internal::poly_composition_sparse(tmp,h1,f,tmp);
        h.push_back(tmp);//h[i+1]=(h[i](h1)) mod f
    }
    #if 0
        std::cout<<"STEP 2 result: \n";
        std::cout<<"h="<<h<<"\n";
    #endif
    //step 3:
    H.push_back(tmp);//H[1]=h[l]
    P H1=tmp;//copy tmp to H1
    for(deg_type j=factory<deg_type>::null();j<m;j++)
    {
        //std::cout<<"j="<<j<<"\n";
        _Internal::poly_composition_sparse(tmp,H1,f,tmp);
        H.push_back(tmp);//H[j+2]=(H[j+1](H1)) mod f
    }
    #if 0
    std::cout<<"STEP 3 result: \n";
    std::cout<<"H="<<H<<"\n";
    #endif
    //step 4:
    vector<P> I;
    I.resize(1);//I[0] is useless element
    for(deg_type j=factory<deg_type>::unit();j<=m;j++)
    {
        const P& Hj=H[j];
        tmp=(Hj-h[0])%f;
        for(deg_type i=factory<deg_type>::null();i<l;i++)
        {
            tmp*=(Hj-h[i]);
            tmp%=f;
        }
        I.push_back(tmp);
    }
    #if 0
        std::cout<<"STEP 4 result: \n";
        std::cout<<"I="<<I<<"\n";
    #endif
    //Step 5:
    //std::cout<<"\nm="<<m<<" l="<<l<<"\n";

    deg_type counter=f.degree();
    for(deg_type j=factory<deg_type>::unit();j<=m;j++)
    {
        //std::cout<<"\nf="<<f<<"  I[j]="<<I[j]<<"\n";
        P& g=gcd(f,I[j]);
        //std::cout<<"\nj="<<j<<"  g="<<g<<"\n";
        f/=g;
        const P& Hj=H[j];
        for(deg_type i=l-factory<deg_type>::unit();i>=factory<deg_type>::null();i--)
        {
            const P& tmp_g=gcd(g,Hj-h[i]);
            if(!is_unit(tmp_g))
            {
                res.push_back(tmp_g);
                v_deg.push_back(l*j-i);
                counter-=tmp_g.degree();
                if(is_null(counter))
                {
                    //std::cout<<"\nCOUNTER\n";
                    return;
                }
                g/=tmp_g;
            }
        }
    }
    if(!f.is_const())
    {
        res.push_back(f);
        v_deg.push_back(f.degree());
    }
    //std::cout<<"DDF finished\n";
}

namespace _Internal
{

    ///Partial inner product for dense polynomials
    template
    <
        typename HH,
        typename PR,
        typename PG,
        typename VPH,
        typename VT,
        typename T1,
        typename T2
        /*,typename N*/
    >
    bool perv_inner_product_dense
    (
        PR& res,
        const PG& g,
        const T1& st,
        const T2& fin_in,
        const VPH& H,
        VT& vt
    )
    {
        //com std::cout<<"\nin_pr started g="<<g<<"\n st="<<st<<"\n H="<<H<<"\n vt="<<vt<<"\n\n";//<<" iter="<<iter;
        //com std::cout<<"\nst="<<st<<"\n";
        typename PG::degree_type g_deg=g.degree();
        T2 fin=min(fin_in,g_deg);
        for (T2 i = fin; i >= st; i--)
        {
            const typename HH::coef_type& gi = g[i];
            //com std::cout<<"\np_i_p h="<<h<<"\n gi="<<gi<<"\n m="<<m<<"\n";
            if(!is_null(gi))
            {
                const HH& h=H[i-st];
                typename HH::degree_type m = h.degree();
                for (typename HH::degree_type j = factory<typename HH::degree_type>::null(); j <= m; j++)
                {
                    //com std::cout<<"p_i_p main for. gi="<<gi<<" h[j]="<<h[j]<<"\n";
                    vt[j]+=gi*h[j];
                }
            }
        }
        //com std::cout<<"\np_i_p vt="<<vt<<"\n";
        res.assign_fromvec(vt);
        return true;
    }

    template<typename PG,typename PH,typename PF,typename PR>
    void poly_composition_dense(const PG& g, const PH& h, const PF& f,PR& res)
    {
        if(!g.is_const())
        {
            typedef typename PG::degree_type gdeg_type;
            typedef typename PG::coef_type gcoef_type;//residue<..>
            typedef typename PH::degree_type hdeg_type;
            typedef typename PH::coef_type hcoef_type;
            typedef typename PF::degree_type fdeg_type;
            gdeg_type deg_g=g.degree();
            fdeg_type deg_f=f.degree();
            typename gcoef_type::module_type p=g.leading_coef().module();
            ARAGELI_ASSERT_0(h.degree()<deg_f);
            ARAGELI_ASSERT_1(deg_g<deg_f);//is it needed?
            gdeg_type gdeg_1=factory<gdeg_type>::unit();
            gdeg_type m=sqrt(deg_g+gdeg_1);
            vector<PH> H;//maybe vector<P2&> H;

            H.push_back
            (
                PH
                (
                    hcoef_type(factory<typename hcoef_type::value_type>::unit(),p),
                    fromval
                )
            );//H[0]=1(mod p)*x^0 in Z_p[x]

            H.push_back(h);//H[1]=h;
            for(gdeg_type i=gdeg_1;i<m;i++)
                H.push_back((H[i]*h)%f);//is it correct?

            gdeg_type l=(deg_g+m)/m-1;
            //com std::cout<<"\nm="<<m<<"  l="<<l<<"\n";
            //com std::cout<<"\nH="<<H<<"\n";
            PR s,t;
            vector<gcoef_type> vt(deg_f);
            for(fdeg_type i=factory<fdeg_type>::null();i<deg_f;i++)
                vt[i].module()=p;
            const PH& Hm=H[m];
            //com std::cout<<"\nH[m]="<<Hm<<"\n";
            _Internal::perv_inner_product_dense<PH>(t,g,m*l,m+m*l-1,H,vt);
            //com std::cout<<"\nt="<<t<<"\n";
            for(gdeg_type i=--l; i>=factory<gdeg_type>::null(); i--)
            {
                for(fdeg_type ii=factory<fdeg_type>::null();ii<deg_f;ii++)//clear vt
                    vt[ii].value()=factory<typename gcoef_type::value_type>::null();
                _Internal::perv_inner_product_dense<PH>(s,g,m*i,m+m*i-1,H,vt);
                t*=Hm;
                t%=f;
                if(!s.is_null())
                    t+=s;
                //com std::cout<<"curr_t="<<t<<"\n";
            }
            res=t;//maybe replace res <- t and remove variable res ??
        }
        else
        {
            res=PR(g);//g=const(x); const(h) mod f = const; g(h) mod f = g; res=g
        }
    }

}


template<typename P,typename V, typename V2>
void DDF_dense(const P& f_in, V& res,V2& v_deg)
{
    typedef typename P::degree_type deg_type;
    if(f_in.is_const())
    {
        res.push_back(f_in);//copy?
        v_deg.push_back(factory<deg_type>::null());
        return;
    }
    ////com std::cout<<"sqrfree="<<sqrfree_poly_mod(f_in)<<"\n";
    typedef typename P::coef_type coef_type;
    typedef typename coef_type::module_type mod_type;
    P f=P(f_in);//copy an input polynomial
    coef_type lc=f.leading_coef();//lc <- lc(f)
    mod_type p=lc.module();//extracting module; p <- lc.module
    ARAGELI_ASSERT_0(!is_null(p));//incorrect module
    deg_type n=f.degree();//n <- deg(f)
    if(!is_null(res.size()))
        res.resize(factory<typename V::size_type>::null());
    if(!is_null(v_deg.size()))
        v_deg.resize(factory<typename V2::size_type>::null());
    //if(is_null(n))//input polynomial const?
    if(!is_unit(lc))// lc==1 ?
    {
        //f.div_scalar(lc);
        for(deg_type i=factory<deg_type>::null();i<=n;i++)// f <- f/lc;
            f[i]/=lc;
        res.push_back(P(lc,fromval));
        v_deg.push_back(factory<deg_type>::null());
    }
    if(is_unit(n))//input polynomial degree == 1 ?
    {
        res.push_back(f);
        v_deg.push_back(n);
        return;
    }
    vector<P> h;
    vector<P> H(1);//H[0] is useless
    //step 0:
    deg_type B=n/2;
    deg_type l=sqrt(B);// sqrt(1) must be equals to 1 !!!
    ARAGELI_ASSERT_2(is_positive(B));//n/2 must be greatear than 0
    ARAGELI_ASSERT_1(is_positive(l));//sqrt(n/2) must be greatear than 0
    deg_type m,r;
    divide(B,l,m,r);
    if(!is_null(r))
        m++;
    coef_type coef_one=factory<coef_type>::unit();//coef_one=1(mod 0)
    coef_one.module()=p;//coef_one=1(mod p)
    P tmp=P(coef_one,factory<deg_type>::unit(),fromval);//tmp=x in Z_p[x]
    h.push_back(tmp);//h[0]=x in Z_p[x]
    #if 0
        std::cout<<"STEP 0 result: \n";
        std::cout<<"f="<<f<<"\n";
        std::cout<<"B="<<B<<"\nl="<<l<<"\nm="<<m<<"\n";
        std::cout<<"h="<<h<<"\n";
        std::cout<<"H="<<H<<"\n";
    #endif
    //step 1:
    P h1;
    tmp=P(coef_one,p,fromval);//tmp=x^p
    h1=tmp%f;//h1 <- (x^p mod f(x)) in Z_p[x]
    h.push_back(h1);
    #if 0
        std::cout<<"STEP 1 result: \n";
        std::cout<<"h="<<h<<"\n";
    #endif
    //step 2:
    tmp=h1;//copy h1 to tmp.
    //std::cout<<"l="<<l<<"\n";
    for(deg_type i=factory<deg_type>::unit();i<l;i++)
    {
        _Internal::poly_composition_dense(tmp,h1,f,tmp);
        h.push_back(tmp);//h[i+1]=(h[i](h1)) mod f
        //std::cout<<"i="<<i<<"\n";
    }
    #if 0
        std::cout<<"STEP 2 result: \n";
        std::cout<<"h="<<h<<"\n";
    #endif
    //step 3:
    H.push_back(tmp);//H[1]=h[l]
    P H1=tmp;//copy tmp to H1
    std::cout<<"m="<<m<<"\n";
    for(deg_type j=factory<deg_type>::null();j<m;j++)
    {
        //com std::cout<<"j="<<j<<"\n";
        _Internal::poly_composition_dense(tmp,H1,f,tmp);
        std::cout<<"j="<<j<<"\n";
        H.push_back(tmp);//H[j+2]=(H[j+1](H1)) mod f
    }
    #if 0
    std::cout<<"STEP 3 result: \n";
    std::cout<<"H="<<H<<"\n";
    #endif
    //step 4:
    vector<P> I;//interval polynomials
    I.resize(1);//I[0] is useless element
    for(deg_type j=factory<deg_type>::unit();j<=m;j++)
    {
        const P& Hj=H[j];
        tmp=(Hj-h[0])%f;
        for(deg_type i=factory<deg_type>::null();i<l;i++)
        {
            tmp*=(Hj-h[i]);
            tmp%=f;
        }
        I.push_back(tmp);

    }
    #if 0
        std::cout<<"STEP 4 result: \n";
        std::cout<<"I="<<I<<"\n";
    #endif
    //Step 5:
    ////com std::cout<<"\nm="<<m<<" l="<<l<<"\n";
    deg_type counter=f.degree();
    for(deg_type j=factory<deg_type>::unit();j<=m;j++)
    {
        //std::cout<<"\nf="<<f<<"  I[j]="<<I[j]<<"\n";
        P& g=gcd(f,I[j]);
        //std::cout<<"\nj="<<j<<"  g="<<g<<"\n";
        f /= g;
        const P& Hj=H[j];
        for(deg_type i=l-factory<deg_type>::unit();i>=factory<deg_type>::null();i--)
        {
            const P& tmp_g=gcd(g,Hj-h[i]);
            if(!is_unit(tmp_g))
            {
                res.push_back(tmp_g);
                v_deg.push_back(l*j-i);
                counter-=tmp_g.degree();
                if(is_null(counter))
                {
                    ////com std::cout<<"\nCOUNTER\n";
                    return;
                }
                g/=tmp_g;
            }
        }
    }
    if(!f.is_const())
    {
        res.push_back(f);
        v_deg.push_back(f.degree());
    }
    ////com std::cout<<"DDF finished\n";
}


}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
