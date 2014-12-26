/*****************************************************************************

    test/vector_take_subvector.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
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

#include "stdafx.hpp"


using namespace Arageli;
using namespace std;
using Arageli::vector;

namespace
{

using Arageli::vector;

template <typename T, bool REFCNT>
void rand (vector<T, REFCNT>& v, size_t size, int maxvalue = RAND_MAX)
{
    v.resize(size);
    for(size_t i = 0; i < v.size(); ++i)
        v[i] = std::rand()%(maxvalue+1);
}

template <typename T1, typename T2, typename T3>
bool is_subvector (const T1& orig, const T2& indexes, const T3& res)
{
    if(orig.is_empty() && (!indexes.is_empty() || !res.is_empty()))
    {
        tout << "For empty original vector we have not empty result vector.\n";
        return false;
    }

    if(res.size() != indexes.size())
    {
        tout << "Indexes vector and taken subvector have differed size.\n";
        return false;
    }

    for(size_t i = 0; i < indexes.size(); ++i)
        if(orig[indexes[i]] != res[i])
        {
            tout << "Original vector and taken subvector are differed at " << i
                << " index item.\n";
            return false;
        }

    return true;
}

template <typename T, bool REFCNT>
bool test_kernel_1 ()
{
    bool fail = false;

    try
    {
        for(int i = 0; i < 10; ++i)
        for(size_t size = 1; size < 10; ++size)
        {
            typedef vector<T, REFCNT> V;

            V orig;
            rand(orig, size);

            for(int idxsize = 0; idxsize < 20; ++idxsize)
            {
                vector<size_t, true> indexes;
                rand(indexes, idxsize, size-1);
                V orig1 = orig, orig2 = orig, res1, res2;
                bool orig1_valid = false, orig2_valid = false;
                try
                {
                    //std::cout << "\norig = " << orig << ", orig1 = " << orig1;
                    orig1.take_subvector(indexes, res1);
                    orig1_valid = true;
                    res2 = orig2.take_subvector(indexes);
                    orig2_valid = true;

                    if(!is_subvector(orig, indexes, res1))
                    {
                        tout
                            << " For vectors: orig = " << orig << ",\nindexes = " << indexes
                            << ", res1 = " << res1 << ".\n";
                        if(res1 == res2){ fail = true; continue; }
                    }

                    if(!is_subvector(orig, indexes, res2))
                    {
                        tout
                            << " For vectors: orig = " << orig << ",\nindexes = " << indexes
                            << ", res2 = " << res2 << ".\n";
                        fail = true;
                    }

                    if(res1 != res2)
                    {
                        tout
                            << "Two ways of taking subvector get differed results:\n"
                            << "take_subvector(x, y) = " << res1 << ",\n"
                            << "take_subvector(x) = " << res2 << ".\n";
                        fail = true;
                    }
                }
                catch(Arageli::exception& e)
                {
                    tout << "EXCEPTION: ";
                    e.output(tout); tout << '\n';
                    tout
                        << "WITH:"
                        << "\n\torig = " << orig
                        << "\n\tindexes = " << indexes;
                    if(orig1_valid)tout << "\n\torig1 = " << orig1;
                    if(orig2_valid)tout << "\n\torig2 = " << orig2;
                    tout
                        << "\n\tT = " << typeid(T).name()
                        << "\n\tREFCNT = " << boolalpha << REFCNT << noboolalpha
                        << '\n';
                    fail = true;
                    return fail;
                }
            }
        }
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        fail = true;
    }

    return fail;
}

TestResult test ()
{
    bool fail = false;

    fail |= test_kernel_1<int, true>();
    fail |= test_kernel_1<char, true>();
    fail |= test_kernel_1<big_int, true>();
    fail |= test_kernel_1<rational<>, true>();
    fail |= test_kernel_1<int, false>();
    fail |= test_kernel_1<char, false>();
    fail |= test_kernel_1<big_int, false>();
    fail |= test_kernel_1<rational<>, false>();

    return fail ? resFAIL : resOK;
}

}

TEST(vector, take_subvector, "Test take_subvector method of template class vector.")
{
    return test();
}
