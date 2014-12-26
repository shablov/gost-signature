/*****************************************************************************

    test/winograd.cpp

    This file is a part of the Arageli library test base.

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
    \brief This file includes test for mm_winograd function.

    This test compares result of function mm_winograd(..)
    with result of function matrix::mul_matrix(..)
*/


#include "stdafx.hpp"
#include "arageli/winograd.hpp"
using namespace Arageli;


namespace
{
bool check_adequancy_residue(){
    typedef residue<int> T;
    int mod = 13;
    matrix<T> A = "((35, 13, 22), (11, 22, 7), (41, 3, 2))";
    for(matrix<T>::iterator i = A.begin(); i < A.end(); ++i)
    {
        i->module() = mod;
        i->normalize();
    }
    matrix<T> R;
    mm_winograd(A,A,R);
    if(R!=A*A){
        tout<<"\ncheck_adequancy_residue() FAILED!!!\n";
        return false;
    }
    //output_aligned(tout,A);//<<"\n"<<A<<"n"<<R<<"\n"<<A*A;
    return true;
}

template <typename T>
bool check_adequacy(int seed,int coverage,int advw2){
    int ww=Arageli::sqrt(coverage)/4+4;
    int hh=coverage/(16*ww)+4;
    matrix<T> W;
    int num_of_err = 0;
    try{
        for(int w=1;w<=ww;w++)
            for(int h=1;h<=hh&&num_of_err < 10;h++){
                matrix<T> A(w,h,fromsize);
                matrix<T> A1(h,w+advw2,fromsize);
                for(int w1=0;w1<w;w1++)//generates A
                    for(int h1=0;h1<h;h1++){
                        A(w1,h1)=w1+2*h1-3+seed;
                    }
                for(int w1=0;w1<w+advw2;w1++)//generates A1
                    for(int h1=0;h1<h;h1++){
                        A1(h1,w1)=2*w1-h1+5-seed;
                    }//end for w2
                mm_winograd(A,A1,W);
                if(W!=A*A1){
                    tout
                        <<"Error while comparing results\n"
                        <<"w="<<w<<" ; h="<<h<<"\n";
                    tout
                        <<"A="<<A<<"\nA1="<<A1<<"\nW="<<W<<"\nA*A1="<<A*A1<<"\n";
                    num_of_err++;
                }
            }//end for h
    }//end try
    catch(const Arageli::exception& e)
    {
        tout << "\n" << e;
        ++num_of_err;
        tout
            << "\nWhere\n"
            << "\n\tT = " << typeid(T).name();

    }
    catch(const std::exception& e)
    {
        tout << "\n" << e.what();
        ++num_of_err;
        tout
            << "\nWhere\n"
            << "\n\tT = " << typeid(T).name();

    }
    catch(...)
    {
        tout << "\nUnknown exception\n";
        ++num_of_err;
        tout
            << "\nWhere\n"
            << "\n\tT = " << typeid(T).name();
    }

    return num_of_err == 0;
}


TEST_FUNCTION(mm_winograd, "General tests for mm_winograd")
{

    int seed = 1, coverage = ARAGELI_TESTSYS_COVERAGE_DEFAULT;

    bool is_ok=true;
    try
    {
        is_ok&=check_adequancy_residue();
        is_ok&=check_adequacy<big_int>(seed,coverage,0);
        is_ok&=check_adequacy<int>(1,100,0);
        is_ok&=check_adequacy<big_int>(seed,coverage,1);
        is_ok&=check_adequacy<big_int>(seed,coverage,2);
        is_ok&=check_adequacy<big_int>(seed,coverage,3);
        is_ok&=check_adequacy<big_int>(seed,coverage,5);
        is_ok&=check_adequacy<sparse_polynom<big_int> >(seed,coverage,0);
        //test function
    }
    catch(const Arageli::exception& e)
    {
        tout << e;
        return resEXCEPT;
    }
    catch(const std::exception& e)
    {
        tout << e.what();
        return resEXCEPT;
    }

    return is_ok ? resOK : resFAIL;

}
}
