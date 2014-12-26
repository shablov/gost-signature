/*****************************************************************************

    test/test4part5.cpp

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


double rand_in_range (double x, double y)
{ return (double(std::rand())/RAND_MAX)*(y-x) + x; }

template <typename V1, typename V2>
double angle (const V1& v1, const V2& v2)
{
    return dotprod(v1, v2)/(std::sqrt(dotprod(v1, v1))*sqrt(dotprod(v2, v2)));
}


std::string num_to_mystr (int i)
{
    std::ostringstream buf; buf << i;
    std::string s = buf.str();
    for(int j = 0; j < s.length(); ++j)
        s[j] = s[j] - '0' + 'a';
    return s;
}


void test4_27 ()
{
    typedef big_int T;
    typedef polyhedron<T> Polyhedra;
    typedef matrix<rational<T> > Vertices;

    Vertices vertices(0, 3);

    const double prec = 1000000;

    #if 1

    for(Vertices::size_type i = 0; i < 200; ++i)
    {
        double phi = rand_in_range(0, 3.14159*2);
        double theta = rand_in_range(-3.14159/2, 3.14159/2);

        vector<rational<T> > vertex(3);

        double x = std::cos(phi)*std::cos(theta);
        double y = std::sin(phi)*std::cos(theta);
        double z = std::sin(theta);

        vertex[0] = rational<T>(prec*x, prec);
        vertex[1] = rational<T>(prec*y, prec);
        vertex[2] = rational<T>(prec*z, prec);

        vertices.insert_row(vertices.nrows(), vertex);
    }

    {
        Polyhedra p1(vertices, fromvert);
        p1.normalize_all();

        ofstream vrmlfile("sphere.wrl");

        output_vrml(vrmlfile, p1);
    }

    vertices.resize(0, 3);


    #endif

    #if 0

    matrix<rational<T> > rot = "((1/2, 1/2, 0), (-1/2, 1/2, 0), (0, 0, 1))";
    rot *= matrix<rational<T> >("((1/3, 0, 2/3), (0, 1, 0), (-2/3, 1, 1/3))");

    for(int i = 0; i < 3; ++i)
        rot.div_row(i, rational<T>(prec*std::sqrt(double(dotprod(rot.copy_row(i), rot.copy_row(i)))), prec));

    for(Vertices::size_type i = 0; i < 200; ++i)
    {
        vector<rational<T> > vertex(3);

        vertex[0] = rational<T>(prec*rand_in_range(-1, 1), prec);
        vertex[1] = rational<T>(prec*rand_in_range(-0.5, 0.5), prec);
        vertex[2] = rational<T>(prec*rand_in_range(-1, 1), prec);

        vertices.insert_row(vertices.nrows(), rot*vertex);
    }

    #endif


    Polyhedra p(vertices, fromvert);
    p.normalize_all();

    {
        ofstream vrmlfile("polytope1.wrl");

        output_vrml(vrmlfile, p);
    }

#if 0
    {
        matrix<double> vertices = p.vertices();

        output_aligned(cout << "vertices:\n", vertices, "ISMVec3f(", "),", ", ");

        vertices *= 5;
        vertices += 5;

        //output_aligned(cout << "vertices:\n", vertices, "ISMVec3f(", "),", ", ");

        Polyhedra::sideset sides = p.facets();    // all sides with dimention 2

        //matrix<double> ps(0, 2);
        std::ofstream file("polytope.tex");
        std::ofstream ssm("sphere-200.txt");

        output_aligned(ssm, vertices, "ISMVec3f(", "),", ", ");
        ssm << "\n\n";

        file << "\\documentclass[a4paper, 12pt]{article}\n\\usepackage[cp1251]{inputenc}"
            "\n\\usepackage[russian]{babel}\n\\usepackage{pstricks}\n\\begin{document}"
            "\n\\title{Arageli:\\\\—лучайна€ триангул€ци€ сферы\\\\300 вершин}"
            "\n\\maketitle\n";

        // making colors

        vector<double> lightdir = "(2, 1, -1)";

        double begin_col = 0.4, end_col = 0.95;
        int ncols = 200;

        for(int i = 0; i <= ncols; ++i)
        {
            double c = begin_col + i*(end_col-begin_col)/ncols;
            file << "\\definecolor{mygray" << num_to_mystr(i) << "}{rgb}{" << c << "," << c << "," << c << "}";
        }

        file << "\n\\begin{figure}[h]\n\\centering\n\\begin{pspicture}(10,10)\n";

        for(Polyhedra::sideset::const_iterator i = sides.begin(); i != sides.end(); ++i)
        {
            Polyhedra::sideset::vertex_indices_set vi = *i;
            for
            (
                Polyhedra::sideset::vertex_indices_set::const_iterator j = vi.begin();
                j != vi.end(); ++j
            )
            {
                ssm << *j << ", ";
            }
            ssm << "\n";

            std::size_t idx = i - sides.begin();
            if(is_positive(p.inequation(idx, 1)))
            {
                vector<double> n = p.facet_normal(idx);
                int ci = ncols*(angle(n, lightdir) + 1)/2;
                cout << ci << " ";
                if(ci < 0)ci = 0;
                if(ci > ncols)ci = ncols;

                file << "\\pspolygon[fillstyle=solid,linewidth=0,fillcolor=mygray" << num_to_mystr(ci) << "]";
                for
                (
                    Polyhedra::sideset::vertex_indices_set::const_iterator j = vi.begin();
                    j != vi.end(); ++j
                )
                {
                    file << "(" << vertices(*j, 1) << "," << vertices(*j, 2) << ")";
                }
                file << "\n";


                file << "\\psline[arrows=c-c](" << vertices(vi.back(), 1) << "," << vertices(vi.back(), 2) << ")";
                for
                (
                    Polyhedra::sideset::vertex_indices_set::const_iterator j = vi.begin();
                    j != vi.end()-1; ++j
                )
                {
                    file << "(" << vertices(*j, 1) << "," << vertices(*j, 2) << ")";
                    file << "\n\\psline[arrows=c-c](" << vertices(*j, 1) << "," << vertices(*j, 2) << ")";
                }
                file << "(" << vertices(vi.back(), 1) << "," << vertices(vi.back(), 2) << ")";
                file << "\n";
            }
        }

        file << "\n\\end{pspicture}\n\\end{figure}\n\\end{document}\n";

    }

#endif

}


void test4_28 ()
{
    typedef matrix<rational<> > Exactmat;
    typedef matrix<double> Roundmat;
    typedef polyhedron<> P;

    const size_t n = 20, m = 10;

    Roundmat rm(0, 3);
    //rm.reserve(3*n);

    for(size_t i = 0; i < n; ++i)
    {
        vector<double> x(3);
        x[0] = rand_in_range(-1, 1);
        x[1] = rand_in_range(-1, 1);
        x[2] = rand_in_range(-1, 1);

        rm.insert_row(rm.nrows(), x);
    }


    for(int i = 0;;)
    {
        // norm

        for(size_t j = 0; j < n; ++j)
            rm.div_row(j, std::sqrt(dotprod(rm.copy_row(j), rm.copy_row(j))));

        // view

        std::ostringstream buf;
        buf << std::setw(2) << std::setfill('0') << i;
        std::ofstream file(("mut." + buf.str() + ".wrl").c_str());
        P p(Exactmat(rm), fromvert);
        p.normalize_all();
        output_vrml(file, p);

        if(++i >= m)break;

        // shufle

        for(int k = 0; k < 10; ++k)
        {
            vector<double> sum(3);
            for(size_t j = 0; j < n; ++j)
                sum += rm.copy_row(j);

            vector<double> x(3);

            for(size_t j = 0; j < n; ++j)
                if(rand_in_range(0, 10) < 2)
                {
                    x[0] = rand_in_range(-0.1, 0.1);
                    x[1] = rand_in_range(-0.1, 0.1);
                    x[2] = rand_in_range(-0.1, 0.1);
                    rm.assign_row(j, (2*rm.copy_row(j) - sum + x));
                }
        }
    }
}


void test4_29 ()
{
    typedef big_int T;
    typedef cone<T> Cone;

    for(;;)
    {
        Cone::inequation_type ineq;

        cout << "\ninequations>";
        cin >> ineq;
        if(ineq.size() == 1 && ineq(0, 0) == -17)break;

        Cone c1(ineq, fromineq);

        output_list(cout << "\ncone =\n", c1);

        c1.normalize_all();

        output_list(cout << "\ncone after normalization =\n", c1);
        output_list(cout << "\ninequation =\n", c1.inequation());
        output_list(cout << "\ngeneratrix =\n", c1.generatrix());
    }
}


void test4_30 ()
{
    cout << "\n***********\n";
    {
        matrix<bool, false> q = "((1))";
        matrix<int> tr;
        triangulate_simple_1(q, 1, tr);
        output_aligned(cout << "q = (" << q.nrows() << ", " << q.ncols() << ") =\n", q);
        output_aligned(cout << "tr = (" << tr.nrows() << ", " << tr.ncols() << ") =\n", tr);
    }
    cout << "\n***********\n";
    {
        matrix<bool, false> q = "()";
        matrix<int> tr;
        triangulate_simple_1(q, 0, tr);
        output_aligned(cout << "q = (" << q.nrows() << ", " << q.ncols() << ") =\n", q);
        output_aligned(cout << "tr = (" << tr.nrows() << ", " << tr.ncols() << ") =\n", tr);
    }
    cout << "\n***********\n";
    {
        matrix<bool, false> q = "((0 , 1), (1 , 0))";
        matrix<int> tr;
        triangulate_simple_1(q, 2, tr);
        output_aligned(cout << "q = (" << q.nrows() << ", " << q.ncols() << ") =\n", q);
        output_aligned(cout << "tr = (" << tr.nrows() << ", " << tr.ncols() << ") =\n", tr);
    }
    cout << "\n***********\n";
    {
        matrix<bool, false> q = "((1 , 0), (0 , 1))";
        matrix<int> tr;
        triangulate_simple_1(q, 2, tr);
        output_aligned(cout << "q = (" << q.nrows() << ", " << q.ncols() << ") =\n", q);
        output_aligned(cout << "tr = (" << tr.nrows() << ", " << tr.ncols() << ") =\n", tr);
    }
    cout << "\n***********\n";
    {
        matrix<bool, false> q = "()";
        matrix<int> tr;
        triangulate_simple_1(q, 1, tr, 1);
        output_aligned(cout << "q = (" << q.nrows() << ", " << q.ncols() << ") =\n", q);
        output_aligned(cout << "tr = (" << tr.nrows() << ", " << tr.ncols() << ") =\n", tr);
    }
    cout << "\n***********\n";
    {
        matrix<bool, false> q = "()";
        matrix<int> tr;
        triangulate_simple_1(q, 2, tr, 2);
        output_aligned(cout << "q = (" << q.nrows() << ", " << q.ncols() << ") =\n", q);
        output_aligned(cout << "tr = (" << tr.nrows() << ", " << tr.ncols() << ") =\n", tr);
    }
    cout << "\n***********\n";
    {
        matrix<bool, false> q = "()";
        matrix<int> tr;
        triangulate_simple_1(q, 3, tr, 3);
        output_aligned(cout << "q = (" << q.nrows() << ", " << q.ncols() << ") =\n", q);
        output_aligned(cout << "tr = (" << tr.nrows() << ", " << tr.ncols() << ") =\n", tr);
    }
    cout << "\n***********\n";
    {
        matrix<bool, false> q = "((1))";
        matrix<int> tr;
        triangulate_simple_1(q, 1, tr, 1);
        output_aligned(cout << "q = (" << q.nrows() << ", " << q.ncols() << ") =\n", q);
        output_aligned(cout << "tr = (" << tr.nrows() << ", " << tr.ncols() << ") =\n", tr);
    }
    cout << "\n***********\n";
    {
        matrix<bool, false> q = "((1))";
        matrix<int> tr;
        triangulate_simple_1(q, 2, tr, 2);
        output_aligned(cout << "q = (" << q.nrows() << ", " << q.ncols() << ") =\n", q);
        output_aligned(cout << "tr = (" << tr.nrows() << ", " << tr.ncols() << ") =\n", tr);
    }
    cout << "\n***********\n";
    {
        matrix<bool, false> q =
            "((0 , 1 , 1 , 0),"
            " (0 , 0 , 1 , 1),"
            " (1 , 0 , 0 , 1),"
            " (1 , 1 , 0 , 0))";
        matrix<int> tr;
        triangulate_simple_1(q, 4, tr);
        output_aligned(cout << "q = (" << q.nrows() << ", " << q.ncols() << ") =\n", q);
        output_aligned(cout << "tr = (" << tr.nrows() << ", " << tr.ncols() << ") =\n", tr + 1);
    }
}


void test4_31 ()
{
    //int a, b;
    //char c;

    //std::cin >> a >> c >> b;
    //cout << a << c << b;

    cout << std::cin.getloc().name();
}


void test4_32 ()
{
    vector<big_int> a = "(1, 2, 3)";

    output_aligned(cout, a);
    output_aligned(cout, a, "[");
    output_aligned(cout, a, "[", "]");
}


void test4_33 ()
{
    {
        matrix<big_int> a = "((2, -3, -3), (2, 1, 0), (2, 0, 1), (1, 0, 0))", f, q, e;

        skeleton(a, f, q, e);

        a = f;
        if(!e.is_empty())
        {
            a.insert_matrix_bottom(e);
            vector<big_int> row(0, e.ncols());
            for(std::size_t i = 0; i < e.nrows(); ++i)
                row += e.copy_row(i);
            a.insert_row(a.nrows(), -row);
        }

        output_aligned(cout << "\nvertices =\n", a);
        intconvex_simple(a, f);
        output_aligned(cout << "\nipoints =\n", f);
        skeleton(f, a, q, e);
        output_aligned(cout << "\niineqs = \n", a);
        skeleton(a, f, q, e);
        output_aligned(cout << "\niverts = \n", f);
        output_aligned(cout << "\nibasis = \n", e);
    }
    {
        matrix<big_int> a = "((9, -2, -2), (1, 2, 0), (7, 0, -2), (1, 0, 0))", f, q, e;

        skeleton(a, f, q, e);

        a = f;
        if(!e.is_empty())
        {
            a.insert_matrix_bottom(e);
            vector<big_int> row(0, e.ncols());
            for(std::size_t i = 0; i < e.nrows(); ++i)
                row += e.copy_row(i);
            a.insert_row(a.nrows(), -row);
        }

        output_aligned(cout << "\nvertices =\n", a);
        intconvex_simple(a, f);
        output_aligned(cout << "\nipoints =\n", f);
        skeleton(f, a, q, e);
        output_aligned(cout << "\niineqs = \n", a);
        skeleton(a, f, q, e);
        output_aligned(cout << "\niverts = \n", f);
        output_aligned(cout << "\nibasis = \n", e);
    }
}


void test4_34 ()
{
    matrix<big_int, false> reces = "((1, 1, 1), (1, -1, 1), (1, 1, -1), (1, -1, -1), (1, 0, 2))";
    vector<bool, false> reces_mask(reces.nrows());
    reces_mask[0] = true;
    matrix<big_int, false> vert= "((0, 0, 0))";
    std::size_t n = power(2, reces_mask.size());

    for(std::size_t i = 1; i < n; ++i)
    {
        cout << "\nreces_mask = " << reces_mask;
        vector<big_int> v = "(0, 0, 0)";
        for(std::size_t j = 0; j < reces_mask.size(); ++j)
            if(reces_mask[j])
                v += reces.copy_row(j);
        vert.insert_row(vert.nrows(), v);

        // move to the next combination
        for(std::size_t j = 0; j < reces_mask.size(); ++j)
            if(reces_mask[j])reces_mask[j] = false;
            else
            {
                reces_mask[j] = true;
                break;
            }
    }

    polyhedron<> p(vert, fromivert);
    std::ofstream file("parallelepiped.wrl");
    p.normalize_all();
    output_vrml(file, p);
}


void test4_35 ()
{
    matrix<big_int> a = "((1, 0, 0), (0, 0, 1), (-3, 1, 4))", b, q;
    output_aligned(cout << "a =\n", a);
    cout << "\ndet(a) = " << det(a);
    cout << "\nRREF";
    vector<size_t, false> basis;
    big_int det;
    rref_gauss_bareiss(a, b, q, basis, det, ctrl::make_rref_gauss_bareiss_slog(cout));
}
