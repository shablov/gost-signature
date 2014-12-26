/*****************************************************************************

    test/test1.hpp

    This file is a part of the Arageli library test base.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007 Sergey S. Lyalin

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

#ifndef _ARAGELI_test1_hpp_
#define _ARAGELI_test1_hpp_

#include "stdafx.hpp"

//****************************************************************************

int test3 ();    // see test3.cpp
int test4 ();    // see test4.cpp
int test5 ();    // see test5.cpp

namespace Arageli
{

template <typename T>
void random_int_matrix (matrix<T>& m, size_t rows, size_t cols)
{
    m.assign_fromsize(rows, cols);
    std::generate(m.begin(), m.end(), std::rand);
}


void test1 (std::ostream& report);
int test2 ();    // see test2.cpp
void test_vector (std::ostream& report);


class Timing
{
public:

    Timing (bool on_a = true) : on_m(on_a), time_m(0)
    { if(on_m)start_m = std::clock(); }

    bool is_on () const { return on_m; }

    void on (bool on_a = true)
    {
        if(on_m)return;
        start_m = std::clock();
        on_m = true;
    }

    void off ()
    {
        if(!on_m)return;
        time_m += double(std::clock() - start_m)/CLOCKS_PER_SEC;
        on_m = false;
    }

    double time () const
    {
        if(on_m)
            return time_m + double(std::clock() - start_m)/CLOCKS_PER_SEC;
        else
            return time_m;
    }

private:

    bool on_m;
    std::clock_t start_m;
    double time_m;
};


inline const char* slr_name (solve_linsys_result status)
{
    static const char* names[] =
    {
        "SLR_EMPTY",
        "SLR_UNIQUE",
        "SLR_MULTIPLE"
    };

    return names[status];
}


inline const char* neir_name (nei_result status)
{
    static const char* names[] =
    {
        "NEIR_EMPTY",
        "NEIR_ALL_VERTS",
        "NEIR_AMONG_VERTS",
        "NEIR_NEW"
    };

    return names[status];
}


} // namespace Arageli


namespace std
{

template <typename T1, typename T2>
inline ostream& operator<< (ostream& out, const pair<T1, T2>& x)
{
    return out << "key = " << x.first << ", value = " << x.second;
}

}


#endif  //  #ifndef _ARAGELI_sparse_polynom_test_hpp_
