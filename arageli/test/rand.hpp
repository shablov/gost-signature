/*****************************************************************************

    test/rand.hpp

    This file is a part of the Arageli library test base.

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
// Modification date: 01.02.2006, 16.07.2006 (S. Lyalin)
// Testing date:
// Description : Header of random number generation class.

#ifndef RandH
#define RandH

#include "stdafx.hpp"

#define ARAGELI_TESTSYS_RAND_COUNT 100
#define ARAGELI_TESTSYS_RAND_A 8193//5
#define ARAGELI_TESTSYS_RAND_C 4097//49
#define ARAGELI_TESTSYS_RAND_M (1<<16)//(1<<8)

class RNG//RNG - random number generation
{
private:
    int mas[55];
    int param, range, prev;
    int bottom;

    int ShortRand();

    void init (int tParam, int tRange);

public:
    RNG(int tParam) { init(tParam, 1<<16); }

    //tRange должно быть степенью двойки(округляется в меньшую сторону)
    RNG(int tParam, int tRange) { init(tParam, tRange); }

    ~RNG();

    int Rand();
};

#endif
