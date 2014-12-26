/*****************************************************************************

    timer.hpp

    This file is a part of Arageli library.

    Copyright (C) 2006--2008 Sergey S. Lyalin

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

/** \file timer.hpp
    \brief Running time measurement.

    This file contains classes for fixing the execution time.
    Use the timer class to measure and temporary store time intervals.
    Use the auto_timer class to exception-safe starting and stopping time
    measurement based on the timer class.
*/


#ifndef _ARAGELI_timer_hpp_
#define _ARAGELI_timer_hpp_

#include "config.hpp"

#if ARAGELI_PLATFORM == ARAGELI_PLATFORM_WINDOWS && defined(ARAGELI_PERFORMANCE_TIMER)
    #define _ARAGELI_WIN_PERFORMANCE_TIMER
#endif

#ifdef _ARAGELI_WIN_PERFORMANCE_TIMER
    #include <windows.h>
    #undef min
    #undef max
#endif

#include <ctime>
#include <iostream>

#include "exception.hpp"
#include "_utility.hpp"


namespace Arageli
{

/// An exception, it's thrown if the system time source isn't available.
class time_source_isnot_available :
    public exception
{};

/// An exception, it's thrown in functions requiring stopped timer if it isn't so.
class timer_isnot_stopped :
    public exception
{};


/// Measures execution time by marking the begin and the end time stamps.
/** The timer can be switched on and off several times during its life time.
    All time ranges are accumulated. The timer can tell what the current
    relative precision of measured time is.

    We do not recommend to use this timer to measure short segments of
    execution. Use resolution function to obtain the temporal resolution.

    All times are expressed in seconds as a double precision floatting
    point number.

    Some functions may throw an exception of time_source_isnot_available
    type if the system time source isn't available. In this case the timer
    object doesn't work and you cannot use it.

    To obtain actual resolution value, timer class needs to be calibrated.
    The calibration is invoked automatically only once during the program
    execution: when the user calls timer::resolution static function or
    when the timer class object is created in the first time. The user can
    call calibration function explicitly to control moment of calibration
    since the calibration needs some time (but less than one second usually).

    Use timer class in following manner:

        timer tm;   // create timer and start measurment immediately

        // Code that have to be evaluated.
        ...

        tm.stop();  // turn off timer

        // Here do something with tm.time(); it is elapsed time in seconds
        // from moment of timer creates up to tm.stop() call.

    Or:

        timer tm(false);    // create turned off timer
        ...
        timer.start();

        // A. The first part of code to be measured.
        ...

        timer.stop();

        // B. Some code that we do not measure.
        ...

        timer.start();

        // C. Another code to be measured.
        ...

        timer.stop();

        // Here do something with tm.time(); it is elapsed time
        // during A and C was been executed (B execution isn't included).

    You can call time() even if timer is activated. Returned value is current
    (up to the call of the function time) elapsed time.
*/
class timer
{
public:

    /// Starts time tracking if turn_on == true.
    timer (bool turn_on = true);

    /// Starts new time interval.
    /** If the timer is already activated, the call doesn't have any effect. */
    void start ();

    /// Ends the current time interval.
    /** Accumulates elapsed time with the previous ones.
        If the timer is already stopped, the call doesn't have any effect. */
    void stop ();

    /// Activation flag. If true, the timer counts time.
    bool is_active () const
    {
        return turn_on_m;
    }

    /// The total elapsed time.
    /** The returned value includes all previous time intervals fixed by
        start-stop calls and the present time interval (if the timer is
        activated at the moment).
        The returned value is expressed in seconds.
        Note that the timer cannot measure the time without an error.
        The current exactness of the measured time is provided by precision
        function. */
    double time () const
    {
        #ifdef _ARAGELI_WIN_PERFORMANCE_TIMER
            ARAGELI_ASSERT_1(is_calibrated());
            return double(clock_time())/freq.QuadPart;
        #else
            return double(clock_time())/CLOCKS_PER_SEC;
        #endif
    }

    /// The minimal amount of time that can be measured. Approximated value.
    /** Duration of the one tick. The returned value is expressed in seconds.
        This value is approximated dependent on the system load and context.
        It can vary from run to run of the program and from one to another call
        of calibrate function. */
    static double resolution ()
    {
        first_calibrate();
        return sdelta;
    }

    /// Relative precision of measuring of the total elapsed time.
    /** Determines the maximum relative error of the value returned by
        the time function. If timer is activated at the moment of call
        and/or was being activated but the accumulated time is 0,
        the functions returns std::numeric_limits<double>::max(). */
    double precision () const;

    /// Reinitializes the timer. The same as the constructor.
    void reset (bool turn_on = true)
    {
        turn_on_m = false;
        duration = 0;
        absprec = 0;
        if(turn_on)
            start();
    }

    /// Calibrates the timer class to get correct approximation for resolution value.
    /** This function is called automatically when one of the timer class functions
        is called for the first time. The moment when the user can call this function
        is TBD. */
    static void calibrate ();

    /// Retruns true iff timer class has been calibrated already.
    static bool is_calibrated ()
    {
        return delta != 0;
    }

    /// Calibrate timer (calls calibrate) if and only if the timer wasn't calibrated before.
    static void first_calibrate ()
    {
        if(!is_calibrated())
            calibrate();
    }

private:

#ifdef _ARAGELI_WIN_PERFORMANCE_TIMER
    typedef LONGLONG tick_type;
#else
    typedef std::clock_t tick_type;
#endif

    friend std::ostream& operator<< (std::ostream& s, const timer& t);
    friend std::istream& operator>> (std::istream& s, timer& t);

    void init (tick_type dur, std::clock_t ap)
    {
        duration = dur;
        absprec = ap;
        turn_on_m = false;
    }

    /// The current elapsed time in ticks.
    tick_type clock_time () const;

    static tick_type kernel_time ()
    {
        #ifdef _ARAGELI_WIN_PERFORMANCE_TIMER
            LARGE_INTEGER curclock;
            if(!QueryPerformanceCounter(&curclock))
                throw time_source_isnot_available();
            return curclock.QuadPart;
        #else
            std::clock_t curclock = std::clock();
            if(curclock == std::clock_t(-1))
                throw time_source_isnot_available();
            return curclock;
        #endif
    }

    tick_type start_stamp;    ///< Beginning of the current interval in ticks.
    tick_type duration;    ///< Total accumulated time in ticks.
    tick_type absprec;    ///< Absolute precision of the total accumulated time in ticks.
    bool turn_on_m;    ///< Activation flag.

    static tick_type delta;  ///< Minimum interval that can be measured in ticks.
    static double sdelta;   ///< Minimum interval that can be measured in seconds.

#ifdef _ARAGELI_WIN_PERFORMANCE_TIMER

    static LARGE_INTEGER freq;  ///< Cached value retrieved by QueryPerformanceFrequency.

#endif

};


/// Stores the timer state to a stream.
/** The timer should be stopped.

    This function and operator>> allow easily measure one time region
    separated, for example, by shutdowns of the application.
    So, you save the state before the shutdown and load it after restarting,
    and all accumulated time is included.

    Note that the save and load state functions cannot guarantee platform
    independence. You should use it to save and load on the same platform. */
std::ostream& operator<< (std::ostream& s, const timer& t);


/// Loads the timer state previously stored by operator<< from a stream.
/** The previous state of the timer object will be lost.
    See comments on complementary operator<<. */
std::istream& operator>> (std::istream& s, timer& t);


/// Starts and stops a timer object automatically.
/** It is based on the constructor and destructor automation to start and
    stop a timer. We recomend to use it locally in functions to make
    code exception safe. */
template <typename Timer = timer>
class auto_timer
{
    Timer& tm;

public:

    /// Type of the controlled timer object.
    typedef Timer timer_type;

    /// Starts the timer.
    auto_timer (timer_type& tma) :
        tm(tma)
    {
        tm.start();
    }

    /// Stops the timer.
    ~auto_timer ()
    {
        tm.stop();
    }
};


/// Timing of the series of tasks.
/** The series_timing function is intended to measure time for several
    tasks managed by integer parameter.
    WARNING! It isn't implemented yet. The signature may be changed soon. */
template <typename Timer, typename Tasks, typename Times>
Times& series_timing (Tasks& tasks, Times& times, Timer& timer);


} // namespace Arageli


#endif  // #ifndef _ARAGELI_timer_hpp_
