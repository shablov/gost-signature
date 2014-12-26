/*****************************************************************************

    test/test5.cpp

    This file is a part of the Arageli library.

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

#include "stdafx.hpp"


using namespace Arageli;
using namespace std;
using Arageli::vector;

namespace Arageli
{
    int _my_counter_1 = 0, _my_counter_2 = 0;
}

rational<> rand_rational (size_t len)
{
    return rational<>
    (
        big_int::random_with_length_or_less(len),
        big_int::random_with_length_or_less(len) + 1
    );
}


sparse_polynom<rational<> > rand_polynom (size_t maxdegree, size_t nummons, size_t lencoef)
{
    sparse_polynom<rational <> > res;
    for(size_t i = 0; i < nummons; ++i)
        res += monom<rational<> >(rand_rational(lencoef), std::rand()%(maxdegree+1));
    return res;
}


void test5_1 ()
{
    for(size_t i = 0; i < 10; ++i)
        cout << "\n" << rand_polynom(5, 3, 3);
}


void test5_2 ()
{
    for(size_t maxdegree = 1; maxdegree < 100; maxdegree *= 4)
        for(size_t lencoef = 3; lencoef < 100; lencoef *= 4)
        {
            Timing tm(false);
            for(size_t j = 0; j < 10; ++j)
            {
                sparse_polynom<rational <> > a, b;
                while(a.is_null())
                    a = rand_polynom(maxdegree, maxdegree/3 + 1, lencoef);
                while(b.is_null())
                    b = rand_polynom(maxdegree, maxdegree/3 + 1, lencoef);

                size_t repmax = 1000 / (a.size() + b.size());
                tm.on();
                for(size_t i = 0; i < repmax; ++i)
                    a / b;
                tm.off();
            }

            cout
                << "\nmaxdegree = " << maxdegree
                << ", lencoef = " << lencoef
                << ", time = " << tm.time();
        }
}


void test5_3 ()
{
    for(big_int i = -500; i < 500; ++i)
        for(big_int j = -500; j < 500; ++j)
            if(euclid(i, j) != euclid_binary(i, j))
                cout
                    << "\ni = " << i << ", j = " << j
                    << ", euclid(i, j) = " << euclid(i, j)
                    << ", euclid_binary(i, j) = " << euclid_binary(i, j);

}


int test5 ()
{
    test5_2();

    return 0;
}


TEST_FUNCTION(rref_gauss_bareiss_for_mp, "Gauss-Bareiss for multipolynomials.")
{
    using namespace Arageli;

    using Arageli::vector;
    using Arageli::exception;

    typedef std::complex<rational<> > T;
    typedef sparse_multipolynom<T> P;
    typedef matrix<P> Mat;

    try
    {

        Mat a =
            "(( (x^(1,2,0)+(2,7)*x^(0,0,1)),"
            " ((0,-1)*x^(1,0,0)+(12,0)*x^(0,1,0))),"
            "(((2,0)*x^(0,0,1)),"
            "((-1,0)*x^(0,3,0)+(11,0)*x^(0,0,1))))",
            b,
            q;

        vector<int> basis;
        P det;

        //output_aligned(tout, a);

        rref_gauss_bareiss(a, b, q, basis, det);

        //output_aligned(tout << "\nb = \n", b);
        //tout << "\ndet = " << det << '\n';
        //tout << "\nsolution is valid: " << std::boolalpha << (b == q * a) << '\n';
        return resOK;
    }
    catch(const exception& e)
    {
        tout << "Exception!\n" << e;
        throw;
    }
    catch(...)
    {
        throw;
    }
}
