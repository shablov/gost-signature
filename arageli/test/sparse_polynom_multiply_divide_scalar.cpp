/*****************************************************************************

    test/sparse_polynom_multiply_divide_scalar.cpp

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
bool sparse_polynom_multiply_divide_scalar (const sparse_polynom<T> A, const T B)
{
    try
    {
        //std::cerr << "\nA = " << A;
        //std::cerr << "\nB = " << B;
        sparse_polynom<T> a=A, x=A, d;
        //std::cerr << "\na = " << a;
        //std::cerr << "\nx = " << x;
        T b=B;
        //std::cerr << "\nb = " << b;
        d=a*b;
        //std::cerr << "\nd = a*b = " << d;
        a*=b;
        //std::cerr << "\n(a*=b) = " << a;
        //std::cerr << "\nMark 1";

        if (d!=a)
        {
            tout<<"function multiply_divide_scalar failed on 1 check with A="<<A<<", B="<<B<<'\n';
            return true;
        }
        //std::cerr << "\nMark 2";
        a/=b;
        //std::cerr << "\nMark 3";

        if (x!=a)
        {
            tout<<"function multiply_divide_scalar failed on 2 check with A="<<A<<", B="<<B<<'\n';
            return true;
        }

        //std::cerr << "\nMark 4";
        if ((a!=(a/b)*b+a%b))
        {
            tout<<"function multiply_divide_scalar failed on 3 check with A="<<A<<", B="<<B<<'\n';
            return true;
        }

        //std::cerr << "\nMark 5";
        return false;
    }
    catch(Arageli::exception& e)
    {
        ARAGELI_EXCEPT_LOC(e);
        tout
            << "EXCEPTION:"
            << "\n" << e
            << "\n\tWhere:"
            << "\n\tT = " << typeid(T).name()
            << "\n\tA = " << A
            << "\n\tB = " << B
            << "\n";
        return true;
    }
}

bool sparse_polynom_multiply_divide_scalar_test(int param, int count)
{
    bool fail=false;
    RNG gen(param,1<<16);

        sparse_polynom<int> c = "0";
        fail |= sparse_polynom_multiply_divide_scalar(c,0);

        if(fail)
        {
            tout<<"Function sparse_polynom_multiply_divide_scalar_test failed on first step.\n";
            return fail;
        }


        for (int k=0; k<count; k++)
        {
            int b=gen.Rand();
            int a=gen.Rand();
            sparse_polynom<int> S = "2*x^2+5*x-7+3*x";
            sparse_polynom<big_int> B = "1234567891011121314151617181920*x^777-9876543210*x^123+112233445566778899";
            sparse_polynom<double> D = "1.12345*x-1e45*x^2+1234.5678*x^3-0.000002334";
            sparse_polynom<rational<> > R = "1234/56781*x^321+7/9*x+3*x^2-4/256";
            fail |= sparse_polynom_multiply_divide_scalar(S,b);
            fail |= sparse_polynom_multiply_divide_scalar(B,big_int (b));
            //fail |= sparse_polynom_multiply_divide_scalar(D,double (b));
            fail |= sparse_polynom_multiply_divide_scalar(R,rational<>(a,b));

            if(fail)
            {
                tout<<"Function sparse_polynom_multiply_divide_scalar_test failed on "<<k+1<<" step.\n";
                return fail;
            }
        }

    return false;
}




TEST(sparse_polynom,multiply_divide_scalar,"Test")
{
    bool fail=sparse_polynom_multiply_divide_scalar_test(100, 100);

    if(fail) return resFAIL;
    return resOK;

}
