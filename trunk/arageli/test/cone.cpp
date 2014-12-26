/*****************************************************************************

    test/cone.cpp -- general tests for cone class.

    This file is a part of the Arageli library test base.

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

/**
    \file
    This file includes general tests for cone class.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

const int NPOINT_FOR_BRUTEFORCE = 10;

template <typename Cone>
vector<typename Cone::generatrix_element_type> bound_box (const Cone& c)
{
    typename Cone::generatrix_type g = c.generatrix();

    vector<typename Cone::generatrix_element_type> res(g.ncols());

    for(std::size_t i = 0; i < g.nrows(); ++i)
        for(std::size_t j = 0; j < res.size(); ++j)
            res[j] = std::max(res[j], abs(g(i, j)));

    return res + 2;
}


inline double rand_in_range (double a, double b)
{ return a + (b-a)*(double(std::rand())/RAND_MAX); }

inline int rand_in_range (int a, int b)
{
    int res = rand_in_range(double(a), double(b+1));
    ARAGELI_ASSERT_1(res >= a);
    return res > b ? b : res;
}


template <typename Cone>
bool cone_eq (const Cone& a, const Cone& b)
{
    a.normalize_all(); b.normalize_all();

    if
    (
        a.inequation_matrix().nrows() == b.inequation_matrix().nrows() &&
        a.inequation_matrix().ncols() == b.inequation_matrix().ncols() &&
        a.equation_matrix().nrows() == b.equation_matrix().nrows() &&
        a.equation_matrix().ncols() == b.equation_matrix().ncols() &&
        a.generatrix_matrix().nrows() == b.generatrix_matrix().nrows() &&
        a.generatrix_matrix().ncols() == b.generatrix_matrix().ncols() &&
        a.basis_matrix().nrows() == b.basis_matrix().nrows() &&
        a.basis_matrix().ncols() == b.basis_matrix().ncols()
    )
    {
        vector<typename Cone::generatrix_element_type>
            bounda = bound_box(a),
            boundb = bound_box(b);

        for(std::size_t i = 0; i < bounda.size(); ++i)
            bounda[i] = std::max(bounda[i], boundb[i]);

        for(int i = 0; i < NPOINT_FOR_BRUTEFORCE; ++i)
        {
            vector<rational<big_int> > x(bounda.size());
            for(std::size_t j = 0; j < x.size(); ++j)
                x[j] = rand_in_range(double(-bounda[j]), double(bounda[j]));

            if
            (
                a.strict_inside(x) != b.strict_inside(x) ||
                a.strict_outside(x) != b.strict_outside(x)
            )
                return false;
        }

        return true;
    }

    return false;
}


template <typename Cone>
bool test_equality ()
{
    typedef matrix<big_int, false> M;

    {
        Cone a(M("(())"), fromineq), b(M("(())"), fromgen);
        if(!cone_eq(a, b))
        {
            tout
                << "\n!cone_eq(a, b) where"
                << "\n\t a = ";
            output_list(tout, a);
            tout
                << "\n\t b = ";
            output_list(tout, b);

            return false;
        }
    }

    {
        Cone a(M("((1))"), fromineq), b(M("((1))"), fromgen);
        if(!cone_eq(a, b))
        {
            tout
                << "\n!cone_eq(a, b) where"
                << "\n\t a = ";
            output_list(tout, a);
            tout
                << "\n\t b = ";
            output_list(tout, b);

            return false;
        }
    }

    {
        Cone a(M("((1, 1))"), fromineq), b(M("((1, -1), (-1, 1), (1, 1))"), fromgen);
        if(!cone_eq(a, b))
        {
            tout
                << "\n!cone_eq(a, b) where"
                << "\n\t a = ";
            output_list(tout, a);
            tout
                << "\n\t b = ";
            output_list(tout, b);

            return false;
        }
    }

    return true;
}


template <typename Cone>
bool test_cmp ()
{
    typedef matrix<big_int> M;
    typedef matrix<M> MM;

    bool res = true;

    // inequations
    MM cones1 =
        "(( (), () ),"
        " ( ((1)), ((1)) ),"
        " ( ((1, 0)), ((2, 0)) ))";

    // generatrix
    MM cones2 =
        "(( (), () ),"
        " ( ((1)), ((1)) ),"
        " ( ((1, 0), (0, 1), (-1, -1)), ((0, 1), (-2, -2), (5, 4)) ),"
        " ( ((1, 0), (0, 1), (-1, -1)), ((0, 5), (2, 2), (-1, -3)) ),"
        " ( ((1, 1), (-1, 1), (0, -1)), ((0, 4), (1, 1), (-1, -3)) ),"
        " ( ((1, 0)), ((2, 0)) ))";

    // equations
    MM cones3 =
        "(( (), () ),"
        " ( ((1)), ((1)) ),"
        " ( ((5)), ((1)) ),"
        " ( ((5)), ((10)) ),"
        " ( ((1, 0), (0, 1)), ((0, 1), (1, 0)) ),"
        " ( ((1, 0), (0, 1)), ((5, 0), (0, 2)) ),"
        " ( ((1, 0), (0, 1)), ((0, 1), (2, 0)) ),"
        " ( ((1, 1), (-1, 1)), ((4, 4), (-2, 2)) ),"
        " ( ((1, 0), (1, 1)), ((0, 1), (2, 0)) ),"
        " ( ((1, 0)), ((1, 0)) ),"
        " ( ((5, 0)), ((10, 0)) ),"
        " ( ((3, 1)), ((-6, -2)) ),"
        " ( ((1, 0)), ((2, 0)) ))";

    for(std::size_t i = 0; i < cones1.nrows() + cones2.nrows() + cones3.nrows(); ++i)
    {
        Cone c1, c2;

        if(i < cones1.nrows())
        {
            c1 = Cone(cones1(i, 0), fromineq);
            c2 = Cone(cones1(i, 1), fromineq);
            c1.equation_matrix().resize(0, c1.inequation_matrix().ncols());
            c1.set_normal_implicit();
            c2.equation_matrix().resize(0, c2.inequation_matrix().ncols());
            c2.set_normal_implicit();
        }
        else if(i < cones1.nrows() + cones2.nrows())
        {
            c1 = Cone(cones2(i - cones1.nrows(), 0), fromgen);
            c2 = Cone(cones2(i - cones1.nrows(), 1), fromgen);
            //c1.set_normal_parametric();
        }
        else
        {
            c1 = Cone(cones3(i - cones1.nrows() - cones2.nrows(), 0), fromeq);
            c2 = Cone(cones3(i - cones1.nrows() - cones2.nrows(), 1), fromeq);
            c1.inequation_matrix().resize(0, c1.equation_matrix().ncols());
            c1.set_normal_implicit();
            c2.inequation_matrix().resize(0, c2.equation_matrix().ncols());
            c2.set_normal_implicit();
        }

        if(cmp(c1, c2) != 0)
        {
            tout
                << "\ncmp(c1, c2) != 0 where"
                << "\nCone = " << typeid(Cone).name()
                << "\n\t c1 = ";
            output_list(tout, c1);
            tout
                << "\n\t c2 = ";
            output_list(tout, c2);

            res = false;
        }

        if(!cone_eq(c1, c2))
        {
            tout
                << "\n!cone_eq(c1, c2) where"
                << "\nCone = " << typeid(Cone).name()
                << "\n\t c1 = ";
            output_list(tout, c1);
            tout
                << "\n\t c2 = ";
            output_list(tout, c2);

            res = false;
        }
    }

    return res;
}


template <typename Cone>
Cone random_cone (int maxd, int alpha)
{
    int sd = rand_in_range(1, maxd);    // space dimention
    int d = rand_in_range(1, sd);    // cone dimention
    int ssd = 0/*rand_in_range(0, d)*/;    // subspace dimention

    vector<typename Cone::generatrix_element_type> halfcut(sd);
    for(int i = 0; i < sd; ++i)
        halfcut[i] = rand_in_range(-alpha, alpha);

    typename Cone::generatrix_type g(5*(d - ssd + 1), sd);
    for(int i = 0; i < g.nrows(); ++i)
    {
        do
        {
            for(int j = 0; j < g.ncols(); ++j)
                g(i, j) = rand_in_range(-alpha, +alpha);
        }while(is_negative(dotprod(halfcut, g.copy_row(i))));
    }

    //typename Cone::basis_type b(ssd, sd);
    //for(int i = 0; i < g.nrows(); ++i)
    //    for(int j = 0; j < g.ncols(); ++j)
    //        g(i, j) = rand_in_range(-alpha, +alpha);

    Cone c(g, fromgen);

    typename Cone::equation_type eq(sd - d, sd);
    for(int i = 0; i < eq.nrows(); ++i)
        for(int j = 0; j < eq.ncols(); ++j)
            eq(i, j) = rand_in_range(-alpha, alpha);

    Cone cut(eq, fromeq);

    c.intersection(cut);

    return c;
}


struct random_cone_statistics
{
    random_cone_statistics () : pointed(0), space(0) {}

    template <typename Cone>
    void add_record (const Cone& c, int simpleces_cur)
    {
        int space_dim_cur = c.space_dim();
        int dim_cur = c.dim();
        int subspace_dim_cur = c.max_embedded_basis().nrows();

        ++space_dim[space_dim_cur];
        ++dim[dim_cur];
        ++subspace_dim[subspace_dim_cur];
        ++simpleces[simpleces_cur];
        pointed += c.is_pointed();
        space += c.is_space();
        ++gen[c.generatrix_matrix().nrows()];
    }

    std::map<int, int>
        space_dim,
        dim,
        subspace_dim,
        simpleces,
        gen;

    int pointed;
    int space;
};


template <typename Map>
void output_map (std::ostream& out, const Map& m)
{
    for(typename Map::const_iterator i = m.begin(); i != m.end(); ++i)
        out << "\n\t\t" << i->first << " : " << i->second;
}


std::ostream& operator<< (std::ostream& out, const random_cone_statistics& rcs)
{
    out << "\tspace_dim:";
    output_map(out, rcs.space_dim);

    out << "\n\tdim:";
    output_map(out, rcs.dim);

    out << "\n\tsubspace_dim:";
    output_map(out, rcs.subspace_dim);

    out << "\n\tsimpleces:";
    output_map(out, rcs.simpleces);

    out << "\n\tgeneratrices:";
    output_map(out, rcs.gen);

    out << "\n\tpointed : " << rcs.pointed;
    out << "\n\tspace : " << rcs.space;

    return out;
}

template <typename Cone, typename Tr>
bool test_correct_triangulation (const Cone& c, const Tr& tr)
{
    bool valid = true;

    vector<Cone, false> simpleces(tr.nrows());
    typename Cone::generatrix_type g = c.generatrix();
    Cone cu(matrix<int>(c.space_dim(), eye), fromeq);

    for(std::size_t i = 0; i < tr.nrows(); ++i)
    {
        simpleces[i] = Cone(g.copy_rows(tr.copy_row(i)), fromgen);

        if(!simpleces[i].is_simplicial() || c.dim() != simpleces[i].dim())
        {
            tout
                << "\n!simpleces[i].is_simplicial() || c.dim() != simpleces[i].dim():"
                << "\n\tc.dim() = " << c.dim()
                << "\n\tsimpleces[i].dim() = " << simpleces[i].dim()
                << "\n\ti = " << i
                << "\n\tc = " << c
                << "\n\tsimpleces[i] = " << simpleces[i]
                << "\n\ttr = " << tr
                << "\n\ttr.copy_row(i) = " << tr.copy_row(i);

            valid = false;
        }

        cu.conic_union(simpleces[i]);
    }

    if(cmp(c, cu) != 0)
    {
        tout
            << "\ncmp(c, cu) != 0 where"
            << "\n\tc  = " << c
            << "\n\tcu = " << cu;
        valid = false;
    }

    if(!cone_eq(c, cu))
    {
        tout
            << "\n!cone_eq(c, cu) where"
            << "\n\tc  = " << c
            << "\n\tcu = " << cu
            << "\n\ttr = " << tr;

        valid = false;
    }

    for(std::size_t i = 0; i < tr.nrows(); ++i)
        for(std::size_t j = i+1; j < tr.nrows(); ++j)
        {
            Cone cx = intersection(simpleces[i], simpleces[j]);
            if(!cx.is_simplicial() || cx.dim() != 0 && cx.dim() >= simpleces[i].dim())
            {
                tout
                    << "\n!cx.is_simplicial() || cx.dim() != 0 && cx.dim() >= simplices[i].dim():"
                    << "\n\tcx.is_simplicial() = " << cx.is_simplicial()
                    << "\n\tcx.dim() = " << cx.dim()
                    << "\n\tsimpleces[i].dim() = " << simpleces[i].dim()
                    << "\n\tsimpleces[j].dim() = " << simpleces[j].dim()
                    << "\n\ti = " << i
                    << "\n\tj = " << j
                    << "\n\tc = " << c
                    << "\n\tcx = " << cx
                    << "\n\tsimpleces[i] = " << simpleces[i]
                    << "\n\tsimpleces[j] = " << simpleces[j]
                    << "\n\ttr = " << tr;
            }
        }

    return valid;
}


template <typename Cone>
bool random_cone_triangulation ()
{
    bool valid = true;

    const int n = 10;
    random_cone_statistics rcs;

    for(int i = 0; i < n; ++i)
    {
        Cone c = random_cone<Cone>(6, 100);
        const Cone& cc = c;
        matrix<int, false> tr;
        c.normalize_all();
        //output_aligned(std::cout << "\n", c.relation());
        //output_aligned(std::cout << "\n", cc.generatrix_matrix()*transpose(cc.inequation_matrix()));
        triangulate_simple_1(c.relation(), c.dim(), tr, c.max_embedded_basis().nrows());
        rcs.add_record(c, tr.nrows());

        valid &= test_correct_triangulation(c, tr);
    }

    tout << "\n" << rcs;

    return valid;
}





}


TEST_CLASS(cone, "General tests for cone class")
{
    bool is_ok = true;

    try
    {
        is_ok &= test_equality<cone<big_int, matrix<big_int, true> > >();
        is_ok &= test_equality<cone<big_int, matrix<big_int, false> > >();
        is_ok &= test_equality<cone<int, matrix<int, true> > >();
        is_ok &= test_equality<cone<int, matrix<int, false> > >();

        // We cannot correctly deal with cone and float numbers.
        /*
        is_ok &= test_equality<cone<float, matrix<float, false> > >();
        */

        is_ok &= test_cmp<cone<big_int, matrix<big_int, true> > >();
        is_ok &= test_cmp<cone<big_int, matrix<big_int, false> > >();
        is_ok &= test_cmp<cone<int, matrix<int, true> > >();
        is_ok &= test_cmp<cone<int, matrix<int, false> > >();

        // We cannot correctly deal with cone and float numbers.
        /*
        is_ok &= test_cmp<cone<float, matrix<float, false> > >();
        */

        //is_ok &= random_cone_triangulation<cone<big_int, matrix<big_int, true> > >();
        //is_ok &= random_cone_triangulation<cone<double, matrix<double, true> > >();
    }
    catch(const Arageli::exception& e)
    {
        tout << e;
        return resEXCEPT;
    }

    return is_ok ? resOK : resFAIL;
}
