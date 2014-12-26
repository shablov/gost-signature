/*****************************************************************************

    test/test4part2.cpp

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


void test4_3 ()
{

    Arageli::vector<big_int> vect(2);
    vect[0] = 0;

}


void test4_4 ()
{
    typedef /*sparse_polynom<rational<> >*/rational<> P;
    typedef matrix<P> M;

    M a = "((-1, 1, 1), (2, -1, -1), (1, 0, 0), (0, 1, 0), (0, 0, 1))";
    M f, q, e;
    skeleton(a, f, q, e);

    output_aligned(cout << "\nF =\n", f);
    output_aligned(cout << "\nQ =\n", q);
    output_aligned(cout << "\nE =\n", e);

    a = "((1, 1, 0), (1, 2, 0), (1, 0, 1), (1, 0, 2))";
    skeleton(a, f, q, e);

    output_aligned(cout << "\n*********************\nF =\n", f);
    output_aligned(cout << "\nQ =\n", q);
    output_aligned(cout << "\nE =\n", e);
}


void test4_5 ()
{
    typedef sparse_polynom<rational<> > P;
    typedef matrix<P> M;

    M a = "((1, 1), (1, x), (1, 0))";
    M f, q, e;
    skeleton(a, f, q, e, ctrl::make_skeleton_slog(cout));

    output_aligned(cout << "\nF =\n", f);
    output_aligned(cout << "\nQ =\n", q);
    output_aligned(cout << "\nE =\n", e);

    a = "((-1, 1), (x, -1))";
    skeleton(a, f, q, e, ctrl::make_skeleton_slog(cout));

    output_aligned(cout << "\n*********************\nF =\n", f);
    output_aligned(cout << "\nQ =\n", q);
    output_aligned(cout << "\nE =\n", e);
}


void test4_6 ()
{
    typedef matrix<rational<int> > M;
    //M a = "((1, 0, 0), (0, 1, 0), (0, 0, 1), (1, -1, -1), (-1/2, 1, 1))";
    M a = "((1, 0, 0), (0, 1, 0), (0, 0, 1), (1, -1, -1), (-1/2, 1, 1))";
    M f, q, e;
    matrix<size_t> tr;
    skeleton(a, f, q, e);
    triangulate_simple_1(q, 3, tr);

    output_aligned(cout << "\nF =\n", f);
    output_aligned(cout << "\nQ =\n", q);
    output_aligned(cout << "\nE =\n", e);
    output_aligned(cout << "\nTr =\n", tr);

}


void test4_7 ()
{
    typedef residue<sparse_polynom<residue<int> > > T;
    T a = "( ((1(mod 2))*x) (mod ((1(mod 2))*x^3)) )";


    for(int i = 0; i < 30; ++i)
    {
        cout << a << '\n';
        a += T("1(mod 2) (mod ((1(mod 2))*x^3))");
    }

}


void test4_8 ()
{
    typedef std::map<std::string, Arageli::matrix<rational<> > > Vars;
    std::map<std::string, Arageli::matrix<rational<> > > vars;

    {
        std::ifstream varsfile("easymath.vars.txt");

        for(;;)
        {
            std::string line;
            std::getline(varsfile, line);
            if(line.empty())break;
            std::istringstream buffer(line);
            buffer >> line;
            buffer >> vars[line];
        }
    }

    try
    {
    for(;;)
    {
        {
            std::ofstream varsfile("easymath.vars.txt");

            for(Vars::iterator i = vars.begin(); i != vars.end(); ++i)
                varsfile << i->first << " " << i->second << "\n";
        }

        std::string line;
        cout << "\n>";
        std::getline(std::cin, line);
        std::istringstream buffer(line);
        std::string command;
        buffer >> command;

        if(command == "exit" || command == "quit")break;
        else if(command == "new")
        {
            buffer >> command;
            if(command == "matrix")
            {
                buffer >> command;
                buffer >> vars[command];
                if(!buffer && !buffer.eof())
                    cout << "Warning. Value is unset.";
            }
            else
                cout << "Error. Unknown type \"" << command << "\".";
        }
        else if(command == "delete")
        {
            buffer >> command;
            Vars::iterator i = vars.find(command);
            if(i == vars.end())
                cout << "Error. Variable \"" << command << "\" is undefined.";
            else
                vars.erase(i);
        }
        else if(command == "list")
        {
            for(Vars::iterator i = vars.begin(); i != vars.end(); ++i)
                Arageli::output_aligned(cout << i->first << " =\n", i->second);
        }
        else if(command == "print")
        {
            buffer >> command;

            if(command == "add" || command == "mul" || command == "sub")
            {
                std::string arg1, arg2;
                buffer >> arg1 >> arg2;
                if(arg2.empty())arg2 = arg1;
                Vars::mapped_type temp;

                Vars::iterator iarg1 = vars.find(arg1), iarg2 = vars.find(arg2);
                if(iarg1 == vars.end() || iarg2 == vars.end())
                {
                    cout
                        << "Error. Variable \"" << arg1 << "\" or \""
                        << arg2 << "\" are undefined.";
                }
                else
                    if(command == "add")
                        temp = iarg1->second + iarg2->second;
                    else if(command == "mul")
                        temp = iarg1->second * iarg2->second;
                    else if(command == "sub")
                        temp = iarg1->second - iarg2->second;
                    else if(command == "div")
                        temp = iarg1->second / iarg2->second;

                output_aligned(cout, temp);
            }
            else
            {
                Vars::iterator i = vars.find(command);
                if(i == vars.end())
                    cout << "Error. Variable \"" << command << "\" is undefined.";
                else
                    Arageli::output_aligned(cout << i->first << " =\n", i->second);
            }
        }
        else if(command == "set")
        {
            buffer >> command;

            if(command == "add" || command == "mul" || command == "sub" || command == "mov")
            {
                std::string arg1, arg2;
                buffer >> arg1 >> arg2;
                if(arg2.empty())arg2 = arg1;

                Vars::iterator iarg1 = vars.find(arg1), iarg2 = vars.find(arg2);
                if(iarg1 == vars.end() || iarg2 == vars.end())
                {
                    cout
                        << "Error. Variable \"" << arg1 << "\" or \""
                        << arg2 << "\" are undefined.";
                }
                else
                    if(command == "add")
                        iarg1->second += iarg2->second;
                    else if(command == "mul")
                        iarg1->second *= iarg2->second;
                    else if(command == "sub")
                        iarg1->second -= iarg2->second;
                    else if(command == "div")
                        iarg1->second /= iarg2->second;
                    else if(command == "mov")
                        iarg1->second = iarg2->second;

                cout << iarg1->first << " =\n";
                output_aligned(cout, iarg1->second);
            }
            else
            {
                buffer >> command;
                Vars::iterator i = vars.find(command);
                if(i == vars.end())
                    cout << "Error. Variable \"" << command << "\" is undefined.";
                else
                    buffer >> i->second;
            }
        }
        else if(command == "addmult")
        {
            size_t i1, i2;
            rational<> alpha, beta;
            std::string var;
            buffer >> command >> var >> i1 >> alpha >> i2 >> beta;

            Vars::iterator i = vars.find(var);
            if(i == vars.end())
                cout << "Error. Variable \"" << var << "\" is undefined.";
            else
            {
                if(command == "row")
                {
                    i->second.addmult_rows(i1, alpha, i2, beta);
                }
                else if(command == "col")
                {
                    i->second.addmult_cols(i1, alpha, i2, beta);
                }

                output_aligned(cout << i->first << " =\n", i->second);
            }

        }
        else if(command == "save")
        {
            std::ofstream varsfile("easymath.vars.txt");

            for(Vars::iterator i = vars.begin(); i != vars.end(); ++i)
                varsfile << i->first << " " << i->second << "\n";
        }
        else if(command == "det")
        {
            buffer >> command;
            Vars::iterator i = vars.find(command);
            if(i == vars.end())
                cout << "Error. Variable \"" << command << "\" is undefined.";
            else
                cout << det(i->second);
        }
        else if(command == "smith")
        {
            buffer >> command;
            Vars::iterator i = vars.find(command);
            if(i == vars.end())
                cout << "Error. Variable \"" << command << "\" is undefined.";
            else
            {
                matrix<big_int> P, Q, res;
                size_t rank;
                big_int det;
                smith(matrix<big_int>(i->second), res, P, Q, rank, det);
                output_aligned(cout << "Smith\n", res);
                output_aligned(cout << "P = \n", P);
                output_aligned(cout << "Q = \n", Q);
            }
        }
        else if(command == "solve_linsys_integer")
        {
            buffer >> command;
            Vars::iterator i = vars.find(command);
            if(i == vars.end())
                cout << "Error. Variable \"" << command << "\" is undefined.";
            else
            {
                buffer >> command;
                Vars::iterator j = vars.find(command);
                if(j == vars.end())
                    cout << "Error. Variable \"" << command << "\" is undefined.";
                else
                {
                    matrix<big_int> gx;
                    vector<big_int> offset;
                    solve_linsys_result status = solve_linsys_integer
                    (
                        matrix<big_int>(i->second),
                        vector<big_int>(j->second.size(), j->second.begin(), fromseq),
                        offset, gx
                    );
                    output_aligned(cout << "Solve integer linear system\ngx =\n", gx);
                    cout << "offset = " << offset << "\n";
                    cout << "status = " << slr_name(status) << '\n';
                }
            }
        }
        else if(command == "solve_linsys_integer_mod")
        {
            buffer >> command;
            Vars::iterator i = vars.find(command);
            if(i == vars.end())
                cout << "Error. Variable \"" << command << "\" is undefined.";
            else
            {
                matrix<big_int> gx;
                vector<big_int> order;
                solve_linsys_result status = solve_linsys_integer_mod
                (
                    matrix<big_int>(i->second),
                    abs(det(matrix<big_int>(i->second))),
                    gx, order
                );
                output_aligned(cout << "Solve integer linear system modulo\ngx =\n", gx);
                cout << "order = " << order << "\n";
                cout << "status = " << slr_name(status) << '\n';
            }
        }
        else if(command == "near_extreme_intpoint")
        {
            buffer >> command;
            Vars::iterator i = vars.find(command);
            if(i == vars.end())
                cout << "Error. Variable \"" << command << "\" is undefined.";
            else
            {
                vector<big_int> v;
                vector<std::size_t> rows;
                nei_result status = near_extreme_intpoint
                    (matrix<big_int>(i->second), v, rows);
                cout << "Find integer point in simplex\nv = " << v << "\nrows = " << rows;
                cout << "\nstatus = " << neir_name(status) << '\n';
            }
        }
        else if(command == "intconvex_triangulate_simple_1_par")
        {
            buffer >> command;
            Vars::iterator i = vars.find(command);
            if(i == vars.end())
                cout << "Error. Variable \"" << command << "\" is undefined.";
            else
            {
                matrix<big_int> gens = i->second;
                cone<> p(gens, fromgen);
                intconvex_triangulate_simple_1(p.generatrix(), p.relation(), p.dim(), gens);
                cone<> p1(gens, fromineq);
                output_aligned(cout << "\nintgens =\n", p1.generatrix()) << '\n';
            }
        }
        else if(command == "zgm2")
        {
            buffer >> command;
            Vars::iterator i = vars.find(command);
            if(i == vars.end())
                cout << "Error. Variable \"" << command << "\" is undefined.";
            else
            {
                matrix<big_int> agd = i->second;
                if(agd.nrows() != 1 || agd.ncols() != 3)
                    cout << "Error. Need 1x3 vector-row.";
                else
                {
                    matrix<big_int, false> v;
                    near_extreme_2d_mod(agd(0, 0), agd(0, 1), agd(0, 2), v, ctrl::intconvex_triangulation_idler());
                    output_aligned(cout << "\npoints =\n", v) << '\n';
                }
            }
        }
        else if(command == "ne23mod")
        {
            buffer >> command;
            Vars::iterator i = vars.find(command);
            if(i == vars.end())
                cout << "Error. Variable \"" << command << "\" is undefined.";
            else
            {
                buffer >> command;
                Vars::iterator j = vars.find(command);
                if(j == vars.end())
                    cout << "Error. Variable \"" << command << "\" is undefined.";
                else
                {
                    buffer >> command;
                    Vars::iterator k = vars.find(command);
                    if(k == vars.end())
                        cout << "Error. Variable \"" << command << "\" is undefined.";
                    else
                    {
                        matrix<big_int> v;
                        near_extreme_2_3_mod
                        (
                            matrix<big_int>(i->second),
                            vector<big_int>(j->second.size(), j->second.begin(), fromseq),
                            vector<big_int>(k->second.size(), k->second.begin(), fromseq),
                            product(vector<big_int>(k->second.size(), k->second.begin(), fromseq)),
                            v
                            , ctrl::intconvex_triangulation_idler()
                        );
                        output_aligned(cout << "\nv =\n", v);
                    }
                }
            }
        }
        else if(command == "clear")
        {
            vars.clear();
        }
        else if(command.length())
        {
            cout << "Error. Unknown command.";
        }
    }

    }
    catch(...)
    {
        std::ofstream varsfile("easymath.vars.txt");

        for(Vars::iterator i = vars.begin(); i != vars.end(); ++i)
            varsfile << i->first << " " << i->second << "\n";
    }

}
