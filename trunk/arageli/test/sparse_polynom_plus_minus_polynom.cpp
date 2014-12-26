/*****************************************************************************

    test/sparse_polynom_plus_minus_polynom.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    REFERENCE ADDITIONAL COPYRIGHTS HERE

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

// Author : Aleksandrov Vladimir
// Tester : Sidnev Alexey
// Creation date : 20.01.2006
// Modification date: 30.01.2006
// Testing date: 14.02.2006
// Description :

#include "stdafx.hpp"

using namespace Arageli;

template <class T>
bool sparse_polynom_plus_minus_polynom (const sparse_polynom<T> A, const sparse_polynom<T> B)
{
    try
    {
        sparse_polynom<T> a=A, x=A, d;
        sparse_polynom<T> b=B;
        d=a+b;
        a+=b;

        if (d!=a)
        {
            tout<<"function plus_minus_polynom failed on 1 check with A="<<A<<", B="<<B<<'\n';
            return true;
        }
        a-=b;

        if (x!=a)
        {
            tout<<"function plus_minus_polynom failed on 2 check with A="<<A<<", B="<<B<<'\n';
            return true;
        }
        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

bool sparse_polynom_plus_minus_polynom_test(int param, int count)
{
    bool fail=false;
    RNG gen(param,1<<16);

        sparse_polynom<int> c = "0";
        fail |= sparse_polynom_plus_minus_polynom(c,c);

        if(fail)
        {
            tout<<"Function sparse_polynom_plus_minus_polynom_test failed on first step.\n";
            return fail;
        }


        for (int k=0; k<count; k++)
        {
            // WARNING! All iterations are identical!

            sparse_polynom<int> S = "2*x^2+5*x-7+3*x";
            sparse_polynom<big_int> B = "1234567891011121314151617181920*x^777-9876543210*x^123+112233445566778899";
            sparse_polynom<double> D = "1.12345*x-1e45*x^2+1234.5678*x^3-0.000002334";
            sparse_polynom<rational<> > R = "1234/56781*x^321+7/9*x+3*x^2-4/256";
            sparse_polynom<int> S1 = "8*x^3+3*x-7+10*x";
            sparse_polynom<big_int> B1 = "12345678910111247345151617181920*x^777-98745343210*x^123+14537845566778899";
            sparse_polynom<double> D1 = "1.121345*x-1e85*x^2+1834.5678*x^3-0.000002384";
            sparse_polynom<rational<> > R1 = "12834/567881*x^31+7/9*x+38*x^2-4/2856";
            //fail |= sparse_polynom_plus_minus_polynom(S,S1);
            //fail |= sparse_polynom_plus_minus_polynom(B,B1);
            //fail |= sparse_polynom_plus_minus_polynom(D,D1);
            fail |= sparse_polynom_plus_minus_polynom(R,R1);

            if(fail)
            {
                tout<<"Function sparse_polynom_plus_minus_polynom_test failed on "<<k+1<<" step.\n";
                return fail;
            }
        }

    return false;
}




TEST(sparse_polynom,plus_minus_polynom,"Test")
{
    bool fail=sparse_polynom_plus_minus_polynom_test(100,2);

    if(fail) return resFAIL;
    return resOK;

}
