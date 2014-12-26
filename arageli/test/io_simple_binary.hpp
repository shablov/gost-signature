/*****************************************************************************

    test/io_simple_binary.hpp

    This file is a part of the Arageli library test base.

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

/**
    \file io_simple_binary.hpp
    \brief Common functions that intended to test The Simple Binary serialization.

    Functions in this file stores and loads a given number in
    The Simple Binary format and check some invariants.
*/


#ifndef _ARAGELI_TEST_io_simple_binary_hpp_
#define _ARAGELI_TEST_io_simple_binary_hpp_

#include "stdafx.hpp"


template <typename T>
bool test_io_simple_binary
(
    const T& x,
    std::size_t n,
    std::size_t mem_reserve
)
{
    char* mem = 0;
    T *c = 0, *d = 0, *cm = 0, *dm = 0;
    try
    {
        mem = new char[mem_reserve];
        T a(x), b;
        T am(x), bm;
        if(n)
        {
            c = new T[n];
            d = new T[n];
            cm = new T[n];
            dm = new T[n];
        }
        std::size_t nam, nbm, ncm, ndm;

        for(std::size_t i = 0; i < n; ++i)
        {
            c[i] = x;
            cm[i] = x;
        }

        {
            std::ofstream file("io_simple_binary.hpp.tmp", std::ios_base::binary);
            output_binary_stream(file, a);
            nam = output_binary_mem(mem, am) - mem;
        }
        {
            std::ifstream file("io_simple_binary.hpp.tmp", std::ios_base::binary);
            input_binary_stream(file, b);
            nbm = input_binary_mem(mem, bm) - mem;
        }

        if(a != b || b != x || a != x)
        {
            tout
                << "ERROR:"
                << "\n\ttest_io_simple_binary, stage 1"
                << "\n\tT = " << typeid(T).name()
                << "\n\tn = " << n
                << "\n\tx = " << x
                << "\n\ta = " << a
                << "\n\tb = " << b
                << std::endl;
            return false;
        }

        if(am != bm || bm != x || am != x)
        {
            tout
                << "ERROR:"
                << "\n\ttest_io_simple_binary, stage 2"
                << "\n\tT = " << typeid(T).name()
                << "\n\tn = " << n
                << "\n\tx = " << x
                << "\n\tam = " << am
                << "\n\tbm = " << bm
                << std::endl;
            return false;
        }

        if(a != am || b != bm)
        {
            tout
                << "ERROR:"
                << "\n\ttest_io_simple_binary, stage 3"
                << "\n\tT = " << typeid(T).name()
                << "\n\tn = " << n
                << "\n\tx = " << x
                << "\n\ta = " << a
                << "\n\tam = " << am
                << "\n\tb = " << b
                << "\n\tbm = " << bm
                << std::endl;
            return false;
        }

        if(nam != nbm)
        {
            tout
                << "ERROR:"
                << "\n\ttest_io_simple_binary, stage 4"
                << "\n\tT = " << typeid(T).name()
                << "\n\tn = " << n
                << "\n\tx = " << x
                << "\n\tnam = " << nam
                << "\n\tnbm = " << nbm
                << "\n\tam = " << am
                << "\n\tbm = " << bm
                << std::endl;
            return false;
        }

        if(nam != calc_binary(am) || nbm != calc_binary(bm))
        {
            tout
                << "ERROR:"
                << "\n\ttest_io_simple_binary, stage 5"
                << "\n\tT = " << typeid(T).name()
                << "\n\tn = " << n
                << "\n\tx = " << x
                << "\n\tnam = " << nam
                << "\n\tnbm = " << nbm
                << "\n\tcalc_binary(am) = " << calc_binary(am)
                << "\n\tcalc_binary(bm) = " << calc_binary(bm)
                << "\n\tam = " << am
                << "\n\tbm = " << bm
                << std::endl;
            return false;
        }

        {
            std::ofstream file("io_simple_binary.hpp.tmp", std::ios_base::binary);
            output_binary_stream(file, c, n);
            ncm = output_binary_mem(mem, cm, n) - mem;
        }
        {
            std::ifstream file("io_simple_binary.hpp.tmp", std::ios_base::binary);
            input_binary_stream(file, d, n);
            ndm = input_binary_mem(mem, dm, n) - mem;
        }

        for(int i = 0; i < n; ++i)
        {
            if(c[i] != d[i] || c[i] != x || d[i] != x)
            {
                tout
                    << "ERROR:"
                    << "\n\ttest_io_simple_binary, stage a1"
                    << "\n\tT = " << typeid(T).name()
                    << "\n\tn = " << n
                    << "\n\tx = " << x
                    << "\n\ti = " << i
                    << "\n\tc[i] = " << c[i]
                    << "\n\td[i] = " << d[i]
                    << std::endl;
                return false;
            }

            if(cm[i] != dm[i] || cm[i] != x || dm[i] != x)
            {
                tout
                    << "ERROR:"
                    << "\n\ttest_io_simple_binary, stage a2"
                    << "\n\tT = " << typeid(T).name()
                    << "\n\tn = " << n
                    << "\n\tx = " << x
                    << "\n\ti = " << i
                    << "\n\tcm[i] = " << cm[i]
                    << "\n\tdm[i] = " << dm[i]
                    << std::endl;
                return false;
            }

            if(c[i] != cm[i] || d[i] != dm[i])
            {
                tout
                    << "ERROR:"
                    << "\n\ttest_io_simple_binary, stage a3"
                    << "\n\tT = " << typeid(T).name()
                    << "\n\tn = " << n
                    << "\n\tx = " << x
                    << "\n\ti = " << i
                    << "\n\tc[i] = " << c[i]
                    << "\n\tcm[i] = " << cm[i]
                    << "\n\td[i] = " << d[i]
                    << "\n\tdm[i] = " << dm[i]
                    << std::endl;
                return false;
            }
        }

        if(ncm != ndm)
        {
            tout
                << "ERROR:"
                << "\n\ttest_io_simple_binary, stage 4"
                << "\n\tT = " << typeid(T).name()
                << "\n\tn = " << n
                << "\n\tx = " << x
                << "\n\tncm = " << ncm
                << "\n\tndm = " << ndm
                << std::endl;
            return false;
        }

        if(ncm != calc_binary(cm, n) || ndm != calc_binary(dm, n))
        {
            tout
                << "ERROR:"
                << "\n\ttest_io_simple_binary, stage 5"
                << "\n\tT = " << typeid(T).name()
                << "\n\tn = " << n
                << "\n\tx = " << x
                << "\n\tncm = " << ncm
                << "\n\tndm = " << ndm
                << "\n\tcalc_binary(cm) = " << calc_binary(cm)
                << "\n\tcalc_binary(dm) = " << calc_binary(dm)
                << std::endl;
        }
    }
    catch(...)
    {
        delete [] mem;
        delete [] c;
        delete [] d;
        delete [] cm;
        delete [] dm;
        throw;
    }

    delete [] mem;
    delete [] c;
    delete [] d;
    delete [] cm;
    delete [] dm;

    return true;
}


#endif    // #ifndef _ARAGELI_TEST_io_simple_binary_hpp_
