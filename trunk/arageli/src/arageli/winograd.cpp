/*****************************************************************************

    winograd.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2007 Sergey V. Lobanov

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
    \file winograd.cpp
    \brief The winograd.hpp file stuff implementation.

    <!--ADD ADDITIONAL FILE DESCRIPTION HERE-->
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_winograd)

// REFERENCE ADDITIONAL HEADERS HERE

#include "winograd.hpp"
#include "cmp.hpp"
#include "factory.hpp"
#include "matrix.hpp"

namespace Arageli
{

template <typename T1,typename T2,typename T3>
void mm_winograd_fund (const T1& G,const T2& H, T3& R)
{
    //__asm int 3
    ARAGELI_ASSERT_0(G.ncols() == H.nrows());
    ARAGELI_ASSERT_0
    (!
        (
            reinterpret_cast<const void*>(&G) ==
                reinterpret_cast<const void*>(&R) ||
            reinterpret_cast<const void*>(&H) ==
                reinterpret_cast<const void*>(&R)
        )
    );

    ARAGELI_ASSERT_0(!G.is_empty());
    ARAGELI_ASSERT_0(!H.is_empty());

    typedef typename T1::size_type size_type;
    typedef typename T1::element_type element_type1;
    typedef typename T2::element_type element_type2;
    typedef typename T3::element_type element_type3;
    size_type a = G.nrows();
    size_type b = H.nrows();
    size_type c = H.ncols();
    size_type bmod2 = b%2;
    size_type b_even = b-bmod2;

    if((R.nrows()!=a)||(R.ncols()!=c))
    {
        R.assign_fromsize(a,c);//Performance: zerofilling and old data are not necessary!!!
    }
    if(is_null(b_even))
    {// .eq. if(b==1)
        for(size_type i = 0;i<a;i++)
        {
            for(size_type j = 0;j<c;j++)
            {
                R(i, j)=G(i, 0)*H(0, j);
            }
        }
    }
    else
    {
        //std::cout<<"main started\n";
        element_type1* rf=new element_type1[a];
        element_type2* cf=new element_type2[c];
        for(size_type i = 0;i<a;i++)
        {
            rf[i] = G(i,0)*G(i,1);
            for(size_type j=2;j<b_even;j+=2)
                rf[i] += G(i,j)*G(i,j+1);
        }
        for(size_type i = 0;i<c;i++)
        {
            cf[i] = H(0,i)*H(1,i);
            for(size_type j= 2; j<b_even;j+=2)
                cf[i] += H(j,i)*H(j+1,i);
        }

        element_type3 tmp=factory<element_type3>::null();
        for(size_type i = 0;i<a;i++)
        {
            for(size_type j = 0;j<c;j++)
            {
                for(size_type k = 0;k<b_even;k+=2)
                    tmp += ((G(i, k)+H(k+1, j))*(H(k, j)+G(i, k+1)));
                R(i,j)=tmp-rf[i]-cf[j];
                tmp=factory<element_type3>::null();
            }
        }

        if(is_unit(bmod2))
        {
            for(size_type i = 0;i<a;i++)
            {
                for(size_type j = 0;j<c;j++)
                {
                    R(i, j)+=(G(i, b_even)*H(b_even, j));
                }
            }
        }
        delete[] rf;
        delete[] cf;
    }
//std::cout<<"winograd finished\n";
}


template <typename T1,typename T2,typename T3>
void mm_winograd (const T1& G,const T2& H, T3& R)
{
    //__asm int 3
    ARAGELI_ASSERT_0(G.ncols() == H.nrows());
    ARAGELI_ASSERT_0
    (!
        (
            reinterpret_cast<const void*>(&G) ==
                reinterpret_cast<const void*>(&R) ||
            reinterpret_cast<const void*>(&H) ==
                reinterpret_cast<const void*>(&R)
        )
    );

    ARAGELI_ASSERT_0(!G.is_empty());
    ARAGELI_ASSERT_0(!H.is_empty());

    typedef typename T1::size_type size_type;
    typedef typename T1::element_type element_type1;
    typedef typename T2::element_type element_type2;
    typedef typename T3::element_type element_type3;
    size_type a = G.nrows();
    size_type b = H.nrows();
    size_type c = H.ncols();
    size_type bmod2 = b%2;
    size_type b_even = b-bmod2;

    R.assign_fromsize(a,c);//after this memory allocation R must be equals to zero-matrix

    if(is_null(b_even))
    {// .eq. if(b==1)
        for(size_type i = 0;i<a;i++)
        {
            for(size_type j = 0;j<c;j++)
            {
                R(i, j)=G(i, 0)*H(0, j);
            }
        }
    }
    else
    {
        //std::cout<<"main started\n";
        element_type1* rf=new element_type1[a];
        element_type2* cf=new element_type2[c];
        for(size_type i = 0;i<a;i++)
        {
            rf[i] = G(i,0)*G(i,1);
            for(size_type j=2;j<b_even;j+=2)
                rf[i] += G(i,j)*G(i,j+1);
        }
        for(size_type i = 0;i<c;i++)
        {
            cf[i] = H(0,i)*H(1,i);
            for(size_type j= 2; j<b_even;j+=2)
                cf[i] += H(j,i)*H(j+1,i);
        }

        element_type3* Rij;
        for(size_type i = 0;i<a;i++)
        {
            for(size_type j = 0;j<c;j++)
            {
                Rij=&R(i,j);
                for(size_type k = 0;k<b_even;k+=2)
                    (*Rij)+=(((G(i, k)+H(k+1, j))*(H(k, j)+G(i, k+1))));
                (*Rij)-=(rf[i]+cf[j]);
            }
        }

        if(is_unit(bmod2))
        {
            for(size_type i = 0;i<a;i++)
            {
                for(size_type j = 0;j<c;j++)
                {
                    R(i, j)+=(G(i, b_even)*H(b_even, j));
                }
            }
        }
        delete[] rf;
        delete[] cf;
    }
}

template <typename T1,typename T2,typename T3>
void mm_winograd_unsafe (const T1& G,const T2& H, T3& R)
{
    //__asm int 3
    ARAGELI_ASSERT_0(G.ncols() == H.nrows());
    ARAGELI_ASSERT_0
    (!
        (
            reinterpret_cast<const void*>(&G) ==
                reinterpret_cast<const void*>(&R) ||
            reinterpret_cast<const void*>(&H) ==
                reinterpret_cast<const void*>(&R)
        )
    );

    ARAGELI_ASSERT_0(!G.is_empty());
    ARAGELI_ASSERT_0(!H.is_empty());

    typedef typename T1::size_type size_type;
    typedef typename T1::element_type element_type1;
    typedef typename T2::element_type element_type2;
    typedef typename T3::element_type element_type3;
    size_type a = G.nrows();
    size_type b = H.nrows();
    size_type c = H.ncols();
    size_type bmod2 = b%2;
    size_type b_even = b-bmod2;
    //after, the variable 'b' is useless

    R.assign_fromsize(a,c);//after this memory allocation R must be equals to zero-matrix

    if(is_null(b_even))
    {// .eq. if(b==1)
        for(size_type i = 0;i<a;i++)
        {
            for(size_type j = 0;j<c;j++)
            {
                R(i, j)=G(i, 0)*H(0, j);
            }
        }
    }
    else
    {
        //std::cout<<"main started\n";
        element_type1* rf=new element_type1[a];
        element_type2* cf=new element_type2[c];
        for(size_type i = 0;i<a;i++)
        {
            rf[i] = G(i,0)*G(i,1);
            for(size_type j=2;j<b_even;j+=2)
                rf[i] += G(i,j)*G(i,j+1);
        }
        for(size_type i = 0;i<c;i++){

            cf[i] = H(0,i)*H(1,i);
            for(size_type j= 2; j<b_even;j+=2)
                cf[i] += H(j,i)*H(j+1,i);
        }

        element_type3* Rij;
        for(size_type i = 0;i<a;i++)
        {
            for(size_type j = 0;j<c;j++)
            {
                Rij=&R(i,j);
                (*Rij)-=rf[i]+cf[j];//Does not works with unsigned types!!!
                for(size_type k = 0;k<b_even;k+=2)
                    (*Rij) += ((G(i, k)+H(k+1, j))*(H(k, j)+G(i, k+1)));
            }
        }

        if(is_unit(bmod2))
        {
            for(size_type i = 0;i<a;i++)
            {
                for(size_type j = 0;j<c;j++)
                {
                    R(i, j)+=(G(i, b_even)*H(b_even, j));
                }
            }
        }
        delete[] rf;
        delete[] cf;
    }
    //std::cout<<"winograd finished\n";
}

}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
