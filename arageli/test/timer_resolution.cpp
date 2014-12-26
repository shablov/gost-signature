/*****************************************************************************

    test/timer_resolution.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 2005--2008 Sergey S. Lyalin

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
    \file timer_resolution.cpp
    \brief This file includes test for timer::resolution function.

    <!--ADD ADDITIONAL FILE DESCRIPTION HERE-->
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

double average_resolution (int k, timer& tm)
{
    double resolution = 0;
    for(int i = 0; i < k; ++i)
    {
        double prev = tm.time(), cur = prev;
        do
        {
            cur = tm.time();
        }while(prev == cur);

        resolution += cur - prev;
    }

    return resolution/k;
}

bool multiple_calibrate (int n, int k)
{
    bool res = true;

    for(int i = 0; i < n; ++i)
    {
        timer::calibrate();
        double resolution = timer::resolution();
        //tout << resolution << '\n';
        const double eps = 10;
        const double reseps = resolution * eps;
        timer tm;
        double mr = average_resolution(k, tm);

        if(mr > resolution + reseps)
        {
            tout
                << "resolution = " << resolution
                << "\nmeasured resolution = " << mr
                << '\n';

            res = false;
        }
    }

    return res;
}

}


TEST
(
    timer,
    resolution,
    "Test for timer::resolution."
)
{
    bool is_ok = true;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        is_ok &= multiple_calibrate(100, 10);
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}
