/*****************************************************************************

    timer.cpp

    This file is a part of Arageli library.

    Copyright (C) 2006--2007 Sergey S. Lyalin

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
    \file timer.cpp
    \brief The timer.hpp file stuff implementation.
*/

#include <limits>
#include "timer.hpp"


namespace Arageli
{


timer::timer (bool turn_on) :
    turn_on_m(false),
    duration(0),
    absprec(0)
{
    first_calibrate();
    if(turn_on)
        start();
}


void timer::start ()
{
    if(turn_on_m)return;
    start_stamp = kernel_time();
    turn_on_m = true;
}


void timer::stop ()
{
    if(!turn_on_m)return;
    tick_type curclock = kernel_time();
    duration += (curclock - start_stamp);

    ARAGELI_ASSERT_1(is_calibrated());
    absprec += delta;

    turn_on_m = false;
}


timer::tick_type timer::clock_time () const
{
    tick_type tm = duration;

    if(turn_on_m)
    {
        tick_type curclock = kernel_time();
        tm += (curclock - start_stamp);
    }

    return tm;
}


double timer::precision () const
{
    tick_type tm = clock_time();
    tick_type curabsprec = absprec;
    if(turn_on_m)
    {
        ARAGELI_ASSERT_1(is_calibrated());
        curabsprec += delta;
    }

    if(tm == 0)
        if(curabsprec == 0)
            return 0;
        else
            return std::numeric_limits<double>::max();
    else
        return double(curabsprec)/double(tm);
}


void timer::calibrate ()
{
    // It is average approximation for duration of
    // the minimum measured time interval.

    #ifdef _ARAGELI_WIN_PERFORMANCE_TIMER
        if(!QueryPerformanceFrequency(&freq))
            throw time_source_isnot_available();
    #endif

    const int ncalibs = 10;  // number of calibration runs
    ARAGELI_ASSERT_1(ncalibs >= 1);

    tick_type curclock = kernel_time();

    // Pass the first partial period.
    while(curclock == kernel_time());

    tick_type startclock = curclock;    // mark start of interval

    // Pass ncalibs whole periods.
    for(int i = 0; i < ncalibs; ++i)
    {
        tick_type prevclock = curclock;

        // Wait actively for changing of kernel_time returned value.
        do
        {
            curclock = kernel_time();
            ARAGELI_ASSERT_1(curclock != -1);
        }while(curclock == prevclock);

        ARAGELI_ASSERT_1(curclock > prevclock);
    }

    delta = (kernel_time() - startclock)/ncalibs + 1;    // +1 is for the upper estimate

    #ifdef _ARAGELI_WIN_PERFORMANCE_TIMER
        sdelta = double(delta)/freq.QuadPart;
    #else
        sdelta = double(delta)/CLOCKS_PER_SEC;
    #endif
}


timer::tick_type timer::delta = 0;
double timer::sdelta;

#ifdef _ARAGELI_WIN_PERFORMANCE_TIMER
    LARGE_INTEGER timer::freq;
#endif


std::ostream& operator<< (std::ostream& s, const timer& t)
{
    if(t.is_active())
        throw timer_isnot_stopped();
    s << '(' << t.duration << ',' << t.absprec << ')';
    return s;
}


std::istream& operator>> (std::istream& s, timer& t)
{
    if(_Internal::is_bad_read_literal(s, "("))
        return s;
    timer::tick_type duration;
    s >> duration;
    if(!s || _Internal::is_bad_read_literal(s, ","))
        return s;
    timer::tick_type absprec;
    s >> absprec;
    if(!s || _Internal::is_bad_read_literal(s, ")"))
        return s;
    t.init(duration, absprec);
    return s;
}


} // namespace Arageli
