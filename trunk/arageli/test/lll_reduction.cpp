/*****************************************************************************

    test/lll_reduction.cpp

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

//#include "matrix_ops.cpp"
#include "stdafx.hpp"

#pragma warning(disable : 4244)


using namespace Arageli;

TEST_FUNCTION(lll_reduction, "Test lll_reduction function")
{
    const int test_deep = ARAGELI_TESTSYS_COVERAGE_DEFAULT;

    //////////////////////////////////////

    size_t
        up_size = std::max(2.0,
            std::pow(double(test_deep)/ARAGELI_TESTSYS_COVERAGE_DEFAULT*1000, 1.0/4)),
        coverage = std::max(1.0,
            std::sqrt(double(test_deep)/ARAGELI_TESTSYS_COVERAGE_DEFAULT*1000)/5);

    TestResult res = resOK;

    typedef matrix<rational<big_int>, false> Matrix;

    for(size_t n = 2; n <= up_size; ++n)
    {
        //std::cout << n << '\n';

        Matrix b, h, b_orig;

        for(size_t i = 0; i < coverage; ++i)try
        {
            do
            {
                b_orig = b = rand_matrix<int, true>(n, n, 1000);
            }while(!lll_reduction(b, h));

            rational<> det_h = det(h);

            //output_aligned(std::cout << "\nb_orig = \n", b_orig);
            //std::cout << "\nb = " << b << "\nh = " << h;

            if(!is_unit(det_h) && !is_opposite_unit(det_h))
            {
                res = resFAIL;
                output_aligned(tout << "Output matrix H is not unimodular, H =\n", h);
                output_aligned(tout << "For input matrix B =\n", b_orig);
            }
            else
            {
                if(b != b_orig*h)
                {
                    res = resFAIL;
                    tout << "output_B != H*input_B";
                    output_aligned(tout << "\nInput matrix B =\n", b_orig);
                    output_aligned(tout << "Output matrix B =\n", b);
                    output_aligned(tout << "Output matrix H =\n", h);
                    output_aligned(tout << "H*input_B =\n", h*b_orig);
                }
            }
        }
        catch(const Arageli::exception& e)
        {
            tout << "EXCEPTION: ";
            e.output(tout); tout << '\n';
            if(res != resFAIL)res = resEXCEPT;
        }

    }

    return res;
}
