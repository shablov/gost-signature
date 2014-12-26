/*****************************************************************************

    test/integer_divide.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
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

#include "stdafx.hpp"

using namespace Arageli;

TEST_FUNCTION(integer_divide_prdivide, "Test division of integers with mathematically correct and Fortran-like sign of reminder for big_int and builtin numbers.")
{
    const int test_amount = 100;
    const int max_fails = 10;
    int fail = 0;

    for(int i = -test_amount; i < test_amount; ++i)
        for(int j = -test_amount; j < test_amount; ++j)
        {
            int stage = 0;
            try
            {
                if(!j)continue;

                big_int bi = i, bj = j;

                if(bi != i || bj != j)
                {
                    tout
                        << "Conversion from int to big_int failed:"
                        << "\n\ti = " << i
                        << "\n\tj = " << j
                        << "\n\tbi = " << bi
                        << "\n\tbj = " << bj << '\n';
                    if(++fail == max_fails)return resFAIL;
                }

                stage = 1;

                int i_div_j = i/j, i_rem_j = i%j;
                big_int bi_div_bj = bi/bj, bi_rem_bj = bi%bj;

                if(i_div_j != bi_div_bj || i_rem_j != bi_rem_bj)
                {
                    tout
                        << "Int vs. big_int standard division mismatch:"
                        << "\n\ti = " << i
                        << "\n\tj = " << j
                        << "\n\ti/j = " << i_div_j
                        << "\n\tbi/bj = " << bi_div_bj
                        << "\n\ti%j = " << i_div_j
                        << "\n\tbi%bj = " << bi_div_bj << '\n';
                    if(++fail == max_fails)return resFAIL;
                }

                stage = 2;

                int i_prdiv_j, i_prrem_j;
                big_int bi_prdiv_bj, bi_prrem_bj;

                stage = 3;
                prdivide(i, j, i_prdiv_j, i_prrem_j);
                stage = 4;
                prdivide(bi, bj, bi_prdiv_bj, bi_prrem_bj);

                if(i_prdiv_j != bi_prdiv_bj)
                {
                    tout
                        << "Int vs. big_int pr-division mismatch:"
                        << "\n\ti = " << i
                        << "\n\tj = " << j
                        << "\n\ti/j = " << i_prdiv_j
                        << "\n\tbi/bj = " << bi_prdiv_bj
                        << "\n\ti%j = " << i_prdiv_j
                        << "\n\tbi%bj = " << bi_prdiv_bj << '\n';
                    if(++fail == max_fails)return resFAIL;
                }
            }
            catch(const Arageli::exception& e)
            {
                tout << '\n' << e.msg();
                tout
                    << "\n\ti = " << i
                    << "\n\tj = " << j
                    << "\n\tstage = " << stage << '\n';
                if(++fail == max_fails)return resFAIL;
            }
        }


    return resOK;
}
