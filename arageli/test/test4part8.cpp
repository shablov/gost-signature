/*****************************************************************************

    test/test4part8.cpp

    This file is a part of the Arageli library.

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

#pragma warning (disable : 4503)


using namespace Arageli;
using namespace Arageli::simplex_method;
using namespace Arageli::ctrl;
using namespace Arageli::ctrl::simplex_method;
using Arageli::vector;

using std::ostream;
using std::cout;
using std::cin;
using std::ofstream;
using std::ifstream;
using std::cerr;
using std::endl;
using std::string;


void test4_61 ()
{
    typedef Arageli::rational<> R;
    typedef sparse_multipolynom<R> P;

    for(;;)
    {
        Arageli::vector<P> vp;
        cin.clear();
        cin >> vp;
        if(vp.is_empty())break;
        output_aligned(std::cout, vp);
        cout << "BG = \n";
        output_aligned(std::cout, groebner_basis(vp));
    }
}


void test4_62 ()
{
    big_int a1 = "0", a2 = "-123", a3 = "292345375937495234088340238", a4 = "-9238402384028420394";
    std::cout << a1 << '\n';
    std::cout << a2 << '\n';
    std::cout << a3 << '\n';
    std::cout << a4 << '\n';

    std::ostringstream buf;
    output_binary(buf, a1);
    output_binary(buf, a2);
    output_binary(buf, a3);
    output_binary(buf, a4);

    std::cout << "\n" << buf.str() << "\n\n";

    big_int b1, b2, b3, b4;

    std::istringstream ibuf(buf.str());
    input_binary(ibuf, b1);
    input_binary(ibuf, b2);
    input_binary(ibuf, b3);
    input_binary(ibuf, b4);

    std::cout << b1 << '\n';
    std::cout << b2 << '\n';
    std::cout << b3 << '\n';
    std::cout << b4 << '\n';
}

void test4_63 ()
{
    sparse_polynom<rational<> > p = "x^2-2*x+1";
    std::cout << "p(x) = " << p;
    std::cout << "\np(x^-1+1) = " << p.subs(sparse_polynom<rational<> >("x^-1+1"));
}

void test4_64 ()
{
    const int m = 1000, n = 10;

    std::ofstream test("skeleton-test-1.ine");
    test << m << ' ' << n << '\n';
    for(int i = 0; i < m; ++i)
    {
        test << std::rand()/10 << ' ';
        for(int j = 1; j < n; ++j)
            test << std::rand() - RAND_MAX/2 << ' ';
        test << '\n';
    }
}

void test4_65 ()
{
    // Magic squares.

    const int size = 5;
    const int nvars = size*size;

    // Build the system.

    typedef big_int T;

    matrix<T> ineq(nvars + 1 + 2*(2*size + 2), nvars + 1 + 1, fromsize);

    // basic inequalities x_i >= 1
    ineq(0, 0) = 1;
    for(int i = 1; i <= nvars; ++i)
    {
        ineq(i, 0) = -1;
        ineq(i, i) =  1;
    }

    // horizontal and vertical magic constraints
    for(int i = 0; i < size; ++i)
    {
        // horizontal:
        for(int j = 0; j < size; ++j)
        {
            ineq(nvars+1 + 4*i + 0, 1 + i*size + j) =  1;
            ineq(nvars+1 + 4*i + 1, 1 + i*size + j) = -1;
        }
        ineq(nvars+1 + 4*i + 0, nvars + 1) = -1;
        ineq(nvars+1 + 4*i + 1, nvars + 1) =  1;

        // vertical:
        for(int j = 0; j < size; ++j)
        {
            ineq(nvars+1 + 4*i + 2, 1 + j*size + i) =  1;
            ineq(nvars+1 + 4*i + 3, 1 + j*size + i) = -1;
        }
        ineq(nvars+1 + 4*i + 2, nvars + 1) = -1;
        ineq(nvars+1 + 4*i + 3, nvars + 1) =  1;
    }

    // diagonal constraints
    for(int i = 0; i < size; ++i)
    {
        ineq(nvars+1 + 4*size + 0, 1 + i*size + i) =  1;
        ineq(nvars+1 + 4*size + 1, 1 + i*size + i) = -1;

        ineq(nvars+1 + 4*size + 2, 1 + i*size - i + size - 1) =  1;
        ineq(nvars+1 + 4*size + 3, 1 + i*size - i + size - 1) = -1;
    }
    ineq(nvars+1 + 4*size + 0, nvars + 1) = -1;
    ineq(nvars+1 + 4*size + 1, nvars + 1) =  1;
    ineq(nvars+1 + 4*size + 2, nvars + 1) = -1;
    ineq(nvars+1 + 4*size + 3, nvars + 1) =  1;

    std::ofstream file("magicNxN.ine");
    file << ineq.nrows() << " " << ineq.ncols() << "\n";
    output_aligned(file, ineq, "", "", " ");

    //Arageli::cone<> ascone(ineq, fromineq);
    //ascone.normalize_all();
    //std::cout << "\nspace = " << ascone.space_dim();
    //std::cout << "\ndim = " << ascone.dim();
    //std::cout << "\nvertex count = " << ascone.min_generatrix().nrows();
    //output_aligned(std::cout << "\n", ascone.min_generatrix());
}
