/*****************************************************************************

    test/rand.cpp

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

// Author : Sidnev Alexey
// Tester :
// Creation date : 20.01.2006
// Modification date: 01.02.2006
// Testing date:
// Description : Code of random number generation class.

#include "stdafx.hpp"
#include "rand.hpp"

int RNG::ShortRand()
{
    return prev=(prev*ARAGELI_TESTSYS_RAND_A+ARAGELI_TESTSYS_RAND_C)%ARAGELI_TESTSYS_RAND_M;
}

void RNG::init (int tParam, int tRange)
{
    bool def=false;

    if(tParam<0)
        def=true;

    if(tRange<=0)
        def=true;

    if(def)
        param=0, range=1<<16, prev=0;
    else
    {
        param=tParam;
        range=tRange;
        prev=param;
    }
    bottom=0;

    int realRange;
    for( realRange=0; range>1; range/=2,realRange++ );
    range=1<<realRange;//Если не хотите по хорошему :)

    mas[0]=param;
    for(int i=1; i<55; i++)
        mas[i]=ShortRand();

    for(int i=0; i< ARAGELI_TESTSYS_RAND_COUNT; i++)
        Rand();
}


int RNG::Rand()
{
    int tem=mas[bottom]=(mas[bottom]+mas[(bottom+22)%55])%range;
    bottom=(++bottom)%55;

    return tem;
}

RNG::~RNG()
{}
