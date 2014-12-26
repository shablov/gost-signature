/*****************************************************************************

    polyhedron.cpp -- See declarations in polyhedron.hpp.

    This file is a part of the Arageli library.

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
    See description for polyhedron.hpp file.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_polyhedron)

// REFERENCE ADDITIONAL HEADERS HERE

#include "frwrddecl.hpp"
#include "polyhedron.hpp"
#include "vector.hpp"
#include "matrix.hpp"


namespace Arageli
{


template <typename T, typename M, typename C, typename CFG>
template <typename D>
polyhedron<T, M, C, CFG>::polyhedron (const D& dim, const fromspace_t&)
{
    cone_m.inequaton_matrix().assign_eye(1, dim + 1);    // one inequation x[0] >= 0
    cone_m.equation_matrix().assign(0, dim + 1);
    set_normal_implicit();
}


template <typename T, typename M, typename C, typename CFG>
template <typename M1>
polyhedron<T, M, C, CFG>::polyhedron (const M1& ineqmat, const fromineq_t&) :
    cone_m(ineqmat, fromineq)
{
    ARAGELI_ASSERT_0(cone_m.inequation_matrix().ncols() > 0);

    // Add one additional inequation to cut all points with x[0] < 0.
    // May be it's superfluous, but the determination of superfluity is expensive.

    cone_m.inequation_matrix().insert_row
    (
        cone_m.inequation_matrix().nrows(),
        Arageli::null<inequation_element_type>()
    );

    cone_m.inequation_matrix()(cone_m.inequation_matrix().nrows() - 1, 0) =
        unit<inequation_element_type>();
}


template <typename T, typename M, typename C, typename CFG>
template <typename M1>
polyhedron<T, M, C, CFG>::polyhedron (const M1& vert, const fromivert_t&)
{
    cone_m.generatrix_matrix() = vert;
    cone_m.generatrix_matrix().insert_col
        (0, vector<T, false>(vert.nrows(), unit<T>(), fromval));
    cone_m.basis_matrix().resize(0, cone_m.generatrix_matrix().ncols());
    //output_aligned(std::cout << "\ncone_m.generatrix_matrix() = \n", cone_m.generatrix_matrix());
}


template <typename T, typename M, typename C, typename CFG>
template <typename M1>
polyhedron<T, M, C, CFG>::polyhedron (const M1& vert, const fromvert_t&)
{
    // WARNING! This is too expensive!
    // TODO: Make it faster!

    vector<T, false> b;
    b.reserve(vert.nrows());
    cone_m.generatrix_matrix().resize(0, vert.ncols());

    for(size_type i = 0; i < vert.nrows(); ++i)
    {
        T lcmval = unit<T>();
        for(size_type j = 0; j < vert.ncols(); ++j)
            lcmval = lcm(lcmval, vert(i, j).denominator());

        cone_m.generatrix_matrix().insert_row
            (cone_m.generatrix_matrix().nrows(), lcmval*vert.copy_row(i));
        b.push_back(lcmval);
    }

    cone_m.generatrix_matrix().insert_col(0, b);
    cone_m.basis_matrix().resize(0, cone_m.generatrix_matrix().ncols());
}


template <typename T, typename M, typename C, typename CFG>
template <typename P1>
polyhedron<T, M, C, CFG>& polyhedron<T, M, C, CFG>::minkowski_diff
(const P1& x)
{
    // Temporary this function works only for polytopes.

    ARAGELI_ASSERT_0(space_dim() == x.space_dim());
    typedef matrix<rational<> > Vertices;
    Vertices g1 = vertices<Vertices>();
    Vertices g2 = x.vertices<Vertices>();
    ARAGELI_ASSERT_1(g1.ncols() == g2.ncols());
    Vertices g(g1.nrows()*g2.nrows(), g1.ncols(), fromsize);

    for(std::size_t i1 = 0; i1 < g1.nrows(); ++i1)
        for(std::size_t i2 = 0; i2 < g2.nrows(); ++i2)
            g.assign_row
            (
                i1*g2.nrows() + i2,
                g1.copy_row(i1) - g2.copy_row(i2)
            );

    *this = polyhedron<T, M, C, CFG>(g, fromvert);
    return *this;
}


template <typename T, typename M, typename C, typename CFG>
template <typename T1>
polyhedron<T, M, C, CFG>& polyhedron<T, M, C, CFG>::translate (const T1& x)
{
    // Temporary this function works only for polytopes.

    typedef matrix<rational<> > Vertices;
    Vertices g = vertices<Vertices>();

    for(std::size_t i = 0; i < g.nrows(); ++i)
        g.assign_row(i, g.copy_row(i) + x);

    *this = polyhedron<T, M, C, CFG>(g, fromvert);
    return *this;

}


template <typename T, typename M, typename C, typename CFG>
sideset polyhedron<T, M, C, CFG>::sides () const
{
    // WARNING! THIS FUNCTIONS IS CORRECT ONLY FOR POLYHEDRONS
    // WITHOUT NONZERO EMBEDDED SUBSPACE.

    // WARNING! THE FOLLOWING CODE TREATS A SINGLE POINT
    // AS A POLYHEDRON THAT DOESN'T CONTAIN ANY FACETS.

    normalize_all();

    matrix<T> a = inequation_matrix();  // only facet inequations
    vector<vector<std::size_t> > res;
    matrix<bool> q = relation();
    ARAGELI_DEBUG_EXEC_1(dim_type d = dim());
    std::size_t except = std::numeric_limits<std::size_t>::max();

    for(std::size_t i = 0; i < q.ncols(); ++i)
    {
        vector<std::size_t> curres;

        for(std::size_t j = 0; j < q.nrows(); ++j)
            if(!q(j, i))    // if i is adjacent to j
                curres.push_back(j);

        if(curres.is_empty())
        {
            except = i;
            continue;
        }

        ARAGELI_ASSERT_1(curres.size() + equation_matrix().nrows() >= d);
        ARAGELI_ASSERT_1(curres.size() < q.nrows());    // ONLY FOR POLYHEDRONS WITHOUT SUBPLANES

        res.push_back(curres);
    }

    return sideset(res, except);
}


template <typename T, typename M, typename C, typename CFG>
polyhedron<T, M, C, CFG> polyhedron<T, M, C, CFG>::facet
(sideset::const_facet_iterator fi) const
{
    // Turn an inequality with a given index to an equality
    // and reduce the system.

    ARAGELI_ASSERT_0(*fi >= 0 && *fi < inequation_matrix().nrows());

    polyhedron res = *this;

    res.equation_matrix().insert_row
    (
        res.equation_matrix().nrows(),
        res.inequation_matrix().take_row(*fi)
    );

    res.normalize_all();    // WARNING!
    return res;
}


template
<
    typename Out,
    typename T,
    typename R,
    typename M,
    typename CFG
>
void output_vrml (Out& out, const polyhedron<T, R, M, CFG>& p)
{
    ARAGELI_ASSERT_0(p.space_dim() == 3);

    typedef polyhedron<T, R, M, CFG> Polyhedra;
    typedef sideset Side_set;
    typedef typename Side_set::vertex_indices_set Vertex_set;

    // VRML header
    out <<
        "#VRML V2.0 utf8\n"
        "#This file is automatically generated by Arageli library.\n"
        "\n"
        "Shape { appearance Appearance { material Material { diffuseColor 1 1 1 } }"
        "geometry IndexedFaceSet { solid TRUE coord Coordinate { point [\n";

    output_aligned(out, p.template vertices<matrix<double> >(), "", ",", " ");

    out << "] } coordIndex [\n";

    Side_set sides = p.sides();

    for(typename Side_set::const_iterator i = sides.begin(); i != sides.end(); ++i)
    {
        Vertex_set vi = *i;
        for
        (
            typename Vertex_set::const_iterator j = vi.begin();
            j != vi.end(); ++j
        )
            out << *j << ", ";
        out << "-1,\n";
    }

    out << "] } }\n";
}


template <typename Out, typename P>
void output_polyhedron_2d
(
    Out& out,        ///< output stream
    const P& p,        ///< 2-polyhedron
    double left,    ///< left of a rectangle bounding polytope picture
    double bottom,    ///< bottom of it
    double right,    ///< right of it
    double top,        ///< top of it
    double extreme_margin,    ///< margin from the border of the picture for all extreme points
    double axis_margin,    ///< margin from the coordinate lines to the border
    double axis_linewidth,    ///< width of lines for axis
    double poly_linewidth,    ///< width of lines for polyhedron
    const std::string& cn_internal,    ///< internal area color name
    const std::string& cn_polyborder,        ///< polyhedron line color name
    bool show_extreme,    ///< show extreme points as *-dots
    bool show_intinternal        ///< show internal integer points as o-dots
)
{
    ARAGELI_ASSERT_0(p.space_dim() == 2);
    ARAGELI_ASSERT_0(p.is_pointed());    // WARNING! TEMPORARY LIMITATION.
    ARAGELI_ASSERT_0(p.dim() == 2);    // WARNING! TEMPORARY LIMITATION.
    ARAGELI_ASSERT_1(p.basis_matrix().is_empty());    // because p is a pointed
    ARAGELI_ASSERT_0(left < right);
    ARAGELI_ASSERT_0(bottom < top);
    ARAGELI_ASSERT_0(extreme_margin == axis_margin);    // WARNING! TEMPORARY LIMITATION.

    p.normalize_all();

    // Build bounding box for all interesting points
    // without corresponding margins.

    typedef typename P::generatrix_element_type T;
    typedef rational<T> R;    // WARINIG! What happens if T will be already rational?

    R rl, rb, rr, rt;    // limits of the bounding box (left, bottom, ...)
    const typename P::generatrix_type& gens = p.generatrix_matrix();
    typedef typename P::size_type size_type;

    // each row of rgens is corresponding to row in gens
    matrix<R, false> rgens(gens.nrows(), 2, fromsize);

    vector<size_type, false> recdir;    // indexes of recessive directions

    // less or equal then 2 recessive directions is possible to be
    recdir.reserve(2);

    // run along all vertices
    for(size_type i = 0; i < gens.nrows(); ++i)
        if(!is_null(gens(i, 0)))
        {
            R& x = rgens(i, 0) =
                noncopy_cast<R>(gens(i, 1))/noncopy_cast<R>(gens(i, 0));
            R& y = rgens(i, 1) =
                noncopy_cast<R>(gens(i, 2))/noncopy_cast<R>(gens(i, 0));

            if(x < rl)
                rl = x;
            if(x > rr)
                rr = x;
            if(y < rb)
                rb = y;
            if(y > rt)
                rt = y;
        }
        else
        {
            rgens(i, 0) = gens(i, 1);
            rgens(i, 1) = gens(i, 2);
            recdir.push_back(i);
        }

    ARAGELI_ASSERT_1(recdir.size() <= 2);
    //ARAGELI_ASSERT_ALWAYS(recdir.is_empty());    // WARNING! TEMPORARY LIMITATION.

    // at least one vertex must be present
    ARAGELI_ASSERT_1(rgens.nrows() - recdir.size() >= 1);

    if(rl == rr && rb == rt)
    {
        ARAGELI_ASSERT_1(is_null(rl) && is_null(rb));

        // WARNING! The default constant in this case should be able to customize.
        rl = opposite_unit(rl);
        rr = unit(rr);
        rb = opposite_unit(rb);
        rt = unit(rt);
    }

    // Add margin for recessive directions.

    // WARING! 2 should be able to customize
    R recadd = 2*std::max(rr - rl, rt - rb);    // additional over-measure

    // WARING! Here we don't take into account the recessive directions
    // point of attack.

    //std::cerr << "\n" << rl << " " << rr << " " << rb << " " << rt << "\n";

    R addrl, addrb, addrr, addrt;

    for(size_type i = 0; i < recdir.size(); ++i)
    {
        const R& x = rgens(recdir[i], 0);
        const R& y = rgens(recdir[i], 1);
        const R norm = std::max(abs(x), abs(y));    // WARNING!
        R sx = recadd*x/norm, sy = recadd*y/norm;

        if(sx < addrl)
            addrl = sx;
        if(sx > addrr)
            addrr = sx;
        if(sy < addrb)
            addrb = sy;
        if(sy > addrt)
            addrt = sy;
    }

    rl += addrl;
    rb += addrb;
    rr += addrr;
    rt += addrt;

    // Cust the rational bounding box to floating point BB.

    R
        rxsize = rr - rl,
        rysize = rt - rb,
        fxsize = right - left,
        fysize = top - bottom,
        margin = 2*extreme_margin,
        fxcenter = (left + right)/2,
        fycenter = (bottom + top)/2,
        rxcenter = (rl + rr)/2,
        rycenter = (rt + rb)/2;

    //std::cerr << "\n" << rl << " " << rr << " " << rb << " " << rt << "\n";

    ARAGELI_ASSERT_0(margin < fxsize && margin < fysize);

    R
        phi = std::min(-(margin - fxsize)/rxsize, -(margin - fysize)/rysize),
        fxb = fxcenter - phi*rxcenter,
        fyb = fycenter - phi*rycenter;

    //output_aligned(std::cerr, rel);

    polyhedron<T> clipp = p;

    // add clip inequations

    //clipp.normalize_parametric();
    //output_aligned(std::cerr, static_cast<const polyhedron<T>&>(clipp).generatrix_matrix());

    R
        cliperl = (R(left) - fxb)/phi,
        cliperr = (R(right) - fxb)/phi,
        cliperb = (R(bottom) - fyb)/phi,
        clipert = (R(top) - fyb)/phi;

    matrix<R, false> clipervert;
    clipervert.insert_row(0, make_vector(cliperl, cliperb));
    clipervert.insert_row(1, make_vector(cliperl, clipert));
    clipervert.insert_row(2, make_vector(cliperr, clipert));
    clipervert.insert_row(3, make_vector(cliperr, cliperb));
    polyhedron<T> cliper(clipervert, fromvert);

    //output_aligned(std::cerr, static_cast<const polyhedron<T>&>(clipp).inequation_matrix());
    //output_aligned(std::cerr, static_cast<const polyhedron<T>&>(clipp).generatrix_matrix());

    clipp.intersection(cliper);

    clipp.normalize_all();
    matrix<R, false> cliprgens = clipp.generatrix_matrix();

    //cliprgens *= phi;
    //cliprgens.assign_col(1, cliprgens.copy_col(1)/cliprgens.copy_col(0)*phi + fxb);
    //cliprgens.assign_col(2, cliprgens.copy_col(2)/cliprgens.copy_col(0)*phi + fyb);
    cliprgens.assign_col(1, cliprgens.copy_col(1)/cliprgens.copy_col(0));
    cliprgens.assign_col(2, cliprgens.copy_col(2)/cliprgens.copy_col(0));
    cliprgens.erase_col(0);
    matrix<double, false> fgens = cliprgens;

    size_type i = 0;    // current vertex index
    matrix<bool> rel = clipp.relation();
    vector<size_type, false> vertexidx;
    vertexidx.reserve(cliprgens.nrows() + 1);
    vector<bool, false> isorigin(vertexidx.size());
    isorigin.reserve(cliprgens.nrows() + 1);

    // Draw interior.

    do
    {
        vertexidx.push_back(i);
        isorigin.push_back(false);
        for(size_type k = 0; k < rgens.nrows(); ++k)
            if(cliprgens.copy_row(i) == rgens.copy_row(k))
                isorigin.back() = true;

        size_type j = 0;
        for(; j < rel.ncols() && rel(i, j); ++j);
        rel(i, j) = true;    // hide direct edge in the relations

        for(i = 0; i < rel.nrows() && rel(i, j); ++i);
        rel(i, j) = true;    // hide back edge in the relations
    }while(i != 0);

    vertexidx.push_back(vertexidx.front());
    isorigin.push_back(isorigin.front());

    cliprgens.assign_col(0, cliprgens.copy_col(0)*phi + fxb);
    cliprgens.assign_col(1, cliprgens.copy_col(1)*phi + fyb);

    out << "\\pspolygon*[linecolor=" << cn_internal << ",linewidth=0,fillcolor=" << cn_internal << "]";
    for(size_type i = 0; i < vertexidx.size() - 1; ++i)
        out << "(" << double(cliprgens(vertexidx[i], 0)) << "," << double(cliprgens(vertexidx[i], 1)) << ")";
    out << "\n";

    // Draw axis.

    out
        // Ox
        << "\\psline[linewidth=" << axis_linewidth
        << ",linecolor=black]{->}(" << double(left) << "," << double(fyb)
        << ")(" << double(right) << "," << double(fyb) << ")\n"
        // Oy
        << "\\psline[linewidth=" << axis_linewidth
        << ",linecolor=black]{->}(" << double(fxb) << "," << double(bottom)
        << ")(" << double(fxb) << "," << double(top) << ")\n";

    // Draw a polyhedron border.

    for(size_type i = 0; i < vertexidx.size() - 1; ++i)
    {
        if(isorigin[i] || isorigin[i+1])
            out << "\\psline[linewidth=" << poly_linewidth
                << ",linecolor=" << cn_polyborder << "]("
                << double(cliprgens(vertexidx[i], 0)) << ","
                << double(cliprgens(vertexidx[i], 1)) << ")("
                << double(cliprgens(vertexidx[i+1], 0)) << ","
                << double(cliprgens(vertexidx[i+1], 1)) << ")\n";

        if(show_extreme && isorigin[i])
            out << "\\psdots("
                << double(cliprgens(vertexidx[i], 0)) << ","
                << double(cliprgens(vertexidx[i], 1)) << ")\n";

    }


    if(show_intinternal)
    {
        // Draw internal integer points.

        T crl = ceil((R(left) - fxb)/phi);
        T crr = floor((R(right) - fxb)/phi);
        T crb = ceil((R(bottom) - fyb)/phi);
        T crt = floor((R(top) - fyb)/phi);

        for(T x = crl; x <= crr; ++x)
            for(T y = crb; y <= crt; ++y)
            {
                vector<T, false> v(2);
                v[0] = x; v[1] = y;

                if(clipp.unstrict_inside(v))
                    out
                        << "\\psdots[dotstyle=o](" << double(x*phi + fxb) << ","
                        << double(y*phi + fyb) << ")\n";

            }
    }
}


#if ARAGELI_DEBUG_LEVEL > 3

template void output_polyhedron_2d
(
    std::ostream& out,
    const polyhedron<big_int>& p,
    double left,
    double bottom,
    double right,
    double top,
    double extreme_margin,
    double axis_margin,
    double axis_linewidth,
    double poly_linewidth,
    const std::string& cn_internal,
    const std::string& cn_polyborder,
    bool show_extreme,
    bool show_intinternal
);

template void output_polyhedron_2d
(
    std::ostream& out,
    const polyhedron<int>& p,
    double left,
    double bottom,
    double right,
    double top,
    double extreme_margin,
    double axis_margin,
    double axis_linewidth,
    double poly_linewidth,
    const std::string& cn_internal,
    const std::string& cn_polyborder,
    bool show_extreme,
    bool show_intinternal
);

#endif



}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
