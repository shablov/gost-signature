/*****************************************************************************

    test/sparse_polynom_multiply_divide_monom.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2008 Sergey S. Lyalin

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
// Testing date: 12.02.2006
// Modified by Sergey S. Lyalin, 2006-08-31
// Description :

//1)Неизвестный символ D1
//--Падение теста адекватно

#include "stdafx.hpp"

using namespace Arageli;

template <class T>
bool sparse_polynom_multiply_divide_monom (const sparse_polynom<T> A, const T B, const int C)
{
    try
    {
        typedef typename sparse_polynom<T>::monom monom;

        sparse_polynom<T> a=A, x=A;
        a *= monom(T (B),C);
        a /= monom(T (B),C);

        if (x!=a)
        {
            tout<<"function multiply_divide_monom failed on 1 check with A="<<A<<", B="<<B<<", C="<<C<<'\n';
            return true;
        }

        if(a != a/monom(B, C)*monom(B, C) + a%monom(B, C))
        {
            tout
                << "function multiply_divide_monom failed on 2 check with A = "
                << A << ", B = " << B << ", C = " << C
                << "\na = " << a
                << "\n(a/monom(T (B),C))*monom(T (B),C)+a%monom(T (B),C)) = "
                << (a/monom(T (B),C))*monom(T (B),C)+a%monom(T (B),C)
                << "\na/monom(T (B),C) = "
                << a/monom(T (B),C)
                << "\na%monom(T (B),C)) = "
                << a%monom(T (B),C)
                << "\n(a/monom(T (B),C))*monom(T (B),C) = "
                << (a/monom(T (B),C))*monom(T (B),C)
                << "\nT = " << typeid(T).name() << '\n';
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

bool sparse_polynom_multiply_divide_monom_test(int param, int count)
{
    bool fail=false;
    RNG gen(param,1<<16);

        sparse_polynom<int> c = "0";
        fail |= sparse_polynom_multiply_divide_monom(c,1,0);

        if(fail)
        {
            tout<<"Function sparse_polynom_multiply_divide_monom_test failed on first step.\n";
            return fail;
        }


        for (int k=0; k<count; k++)
        {
            int a = std::rand();
            a=((a==0)?3:a);
            int b = std::rand();
            sparse_polynom<int> S = "2*x^2+5*x-7+3*x";
            sparse_polynom<big_int> B = "1234567891011121314151617181920*x^777-9876543210*x^123+112233445566778899";
            sparse_polynom<double> D = "1.12345*x-1e45*x^2+1234.5678*x^3-0.000002334";
            sparse_polynom<rational<> > R = "1234/56781*x^321+7/9*x+3*x^2-4/256";
            //fail |= sparse_polynom_multiply_divide_monom(S,a,b);
            //fail |= sparse_polynom_multiply_divide_monom(B,big_int (a),b);
//        1    fail |= sparse_polynom_multiply_divide_monom(D,D1);
            fail |= sparse_polynom_multiply_divide_monom(R,rational<>(a,2),b);

            if(fail)
            {
                tout<<"Function sparse_polynom_multiply_divide_monom_test failed on "<<k+1<<" step.\n";
                return fail;
            }
        }

    return false;
}




TEST(sparse_polynom,multiply_divide_monom,"Test")
{
/*
    sparse_polynom<big_int> sp("2*x^2+5*x-7+3*x");
    monom<big_int> m(41,3456);
    tout << sp<<"  "<<m <<"  "<<sp/m<<'\n';
*/
    bool fail=sparse_polynom_multiply_divide_monom_test(100, 100);

    if(fail) return resFAIL;
    return resOK;

}
