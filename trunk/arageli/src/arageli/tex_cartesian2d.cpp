/*****************************************************************************

    tex_cartesian2d.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2007 Sergey S. Lyalin

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
    \file tex_cartesian2d.cpp
    \brief The tex_cartesian2d.hpp file stuff implementation.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_tex_cartesian2d)

#include <cmath>
#include <algorithm>

#include "exception.hpp"
#include "cmp.hpp"
#include "powerest.hpp"
#include "std_import.hpp"
#include "big_int.hpp"
#include "texout.hpp"
#include "tex_cartesian2d.hpp"


namespace Arageli
{

namespace _Internal
{


inline void kl_floor (int& gk, int& gl)
{
    ARAGELI_ASSERT_0(gl >= 0 && gl < 10);

    if(gl == 0)
    {
        gl = 5;
        --gk;
    }
    else if(gl > 5)
        gl = 5;
}


inline void kl_dec (int& gk, int& gl)
{
    ARAGELI_ASSERT_0(gl >= 1 && gl <= 5);

    if(--gl <= 0)
    {
        gl = 5;
        --gk;
    }
}


inline bool is_kl_subgrid (int tk, int tl, int gk, int gl)
{
    ARAGELI_ASSERT_0(gl >= 1 && gl <= 5);

    if(gk > tk || (gk == tk && gl > tl))
        return false;
    return is_divisible(tl*power(big_int(10), (tk - gk)), gl);
}


template <typename T>
void cartesian2d_chart_select_grid
(
    double spp,
    const T& minm,
    const T& maxm,
    double needg,
    double needt,
    double m,
    double& g0,
    double& resg,
    int& tn0,
    int& tn,
    T& r0,
    T& r
)
{
    // WARNING! TEMPORARY VERY SIMPLE IMPLEMENTATION

    needg *= 1.5; needt *= 1.5;    // TEMPORARY

    double dratio = double(maxm - minm)/(spp-2*m);
    T rratio = T(dratio);

    double gz = dratio*needg;
    int gk = floor(log10(gz));
    int gl = floor(gz/pow(10.0, gk));
    kl_floor(gk, gl);

    double tz = dratio*needt;
    int tk = floor(log10(tz));
    int tl = floor(tz/pow(10.0, tk));
    kl_floor(tk, tl);

    while(!is_kl_subgrid(tk, tl, gk, gl))
        kl_dec(gk, gl);

    r = T(gl)*power(T(10), gk);
    r0 = r*ceil((minm-T(m*dratio))/r);
    resg = r/rratio;
    g0 = (r0 - (minm-T(m*dratio)))/dratio;

    T tr = T(tl)*power(T(10), tk);
    T tr0 = tr*ceil((minm-T(m*dratio))/tr);
    double rest = tr/rratio;
    double t0 = (tr0 - (minm-T(m*dratio)))/dratio;

    tn0 = round((t0 - g0)/resg);
    tn = round(rest/resg);
}


} // namespace _Internal

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4288)
#endif

template <typename Out, typename Lines>
void output_pstricks_cartesian2d
(
    Out& out,
    double xbase,
    double ybase,
    const cartesian2d_chart& chart,
    const Lines& lines
)
{
    ARAGELI_ASSERT_0(chart.tx > chart.gx);
    ARAGELI_ASSERT_0(chart.ty > chart.gy);

    ARAGELI_ASSERT_0(lines.size());

    typedef typename Lines::size_type size_type;
    typedef typename Lines::value_type Line;
    typedef typename Line::x_points_type VecX;
    typedef typename Line::y_points_type VecY;
    typedef typename VecX::value_type TX;
    typedef typename VecY::value_type TY;

    // Determination of the legend area size.

    // WARNING! ONLY HORIZONTAL ALIGNMENT YET

    double slx = 0, sly = 0;
    for(size_type i = 0; i < lines.size(); ++i)
    {
        slx += lines[i].cx + chart.lpl + chart.lmt;
        double cury = std::max(lines[i].cy, lines[i].lw);
        if(cury > sly)
            sly = cury;
    }

    slx += (lines.size() + 1)*chart.lmx;
    sly += 2*chart.lmy;

    // Determination of the plotting area sizes.

    double
        spx = chart.sx - chart.cyh - chart.tyw - chart.txmw/2,
        spy = chart.sy - chart.cxh - chart.txh - chart.tymh/2 - sly,
        sppx = spx - chart.cfw,    // true plotting area X size
        sppy = spy - chart.cfw;    // true plotting area Y size

    ARAGELI_ASSERT_0(sppx > chart.tx);
    ARAGELI_ASSERT_0(sppy > chart.ty);

    // Determination of model area with lines.

    ARAGELI_ASSERT_0(lines[0].x.size() && lines[0].y.size());

    TX maxmx = lines[0].x[0], minmx = maxmx;
    TY maxmy = lines[0].y[0], minmy = maxmy;
    for(size_type i = 0; i < lines.size(); ++i)
    {
        ARAGELI_ASSERT_0(lines[i].x.size());
        ARAGELI_ASSERT_0(lines[i].x.size() == lines[i].y.size());

        ARAGELI_ASSERT_0(lines[i].lw == 0);    // WARNING! TEMPORARY LIMITATION

        maxmx = std::max
        (
            maxmx,
            *std::max_element(lines[i].x.begin(), lines[i].x.end())
        );
        maxmy = std::max
        (
            maxmy,
            *std::max_element(lines[i].y.begin(), lines[i].y.end())
        );
        minmx = std::min
        (
            minmx,
            *std::min_element(lines[i].x.begin(), lines[i].x.end())
        );
        minmy = std::min
        (
            minmy,
            *std::min_element(lines[i].y.begin(), lines[i].y.end())
        );
    }

    //TX smx = maxmx - minmx;
    //TY smy = maxmy - minmy;

    ARAGELI_ASSERT_0(is_positive(maxmx - minmx));
    ARAGELI_ASSERT_0(is_positive(maxmy - minmy));

    double gx0, gx;    // actual selected offset and distance between points on axis X
    TX rx0, rx;        // model corresponding sizes
    int txn0, txn;    // offset and distance between labåled points in gx's units.

    _Internal::cartesian2d_chart_select_grid
    (
        sppx,
        minmx,
        maxmx,
        chart.gx,
        chart.tx,
        chart.mx,
        gx0,
        gx,
        txn0,
        txn,
        rx0,
        rx
    );

    double gy0, gy;    // actual selected offset and distance between points on axis Y
    TY ry0, ry;        // model corresponding sizes
    int tyn0, tyn;    // offset and distance between labåled points in gy's units.

    _Internal::cartesian2d_chart_select_grid
    (
        sppy,
        minmy,
        maxmy,
        chart.gy,
        chart.ty,
        chart.my,
        gy0,
        gy,
        tyn0,
        tyn,
        ry0,
        ry
    );

    // Output the captions and the legend.

    // X caption.
    out
        << "\\rput{0}("
        << xbase + chart.cyh + chart.tyw + spx/2 << ","
        << ybase + sly + chart.cxh/2
        << "){" << chart.cx << "}\n";

    // Y caption.
    out
        << "\\rput{90}("
        << xbase + chart.cyh/2 << ","
        << ybase + sly + chart.cxh + chart.txh + spy/2
        << "){" << chart.cy << "}\n";

    // Legend frame box.
    double xlegbase = xbase + (chart.sx - slx)/2;
    out
        << "\\psframe" << chart.lfst
        << "(" << xlegbase << "," << ybase << ")("
        << xlegbase + slx << "," << ybase + sly << ")\n";

    // The legend itself.
    double legycenter = ybase + sly/2;
    double xcurbase = xlegbase;
    for(size_type i = 0; i < lines.size(); ++i)
    {
        xcurbase += chart.lmx;
        out
            << "\\psline" << lines[i].lst << "("
            << xcurbase << "," << legycenter << ")("
            << xcurbase + chart.lpl/2 << "," << legycenter << ")("
            << xcurbase + chart.lpl << "," << legycenter << ")\n";
        xcurbase += chart.lpl + chart.lmt;
        out
            << "\\rput[l]{0}(" << xcurbase << ","
            << legycenter << "){" << lines[i].caption << "}\n";
        xcurbase += lines[i].cx;
    }

    // Chart frame box.

    double
        xchartbase = xbase + chart.cyh + chart.tyw,
        ychartbase = ybase + sly + chart.cxh + chart.txh;
    out
        << "\\psframe" << chart.cfst
        << "(" << xchartbase << "," << ychartbase << ")("
        << xchartbase + spx << "," << ychartbase + spy << ")\n";

    // Labeles on X axis.

    int ii = 0; TX xx = rx0;
    for(double x = gx0; x < sppx; x += gx, ++ii, xx += rx)
    {
        // Serifs.
        out
            << "\\psline" << chart.pst << "("
            << xchartbase + chart.cfw/2 + x << ","
            << ychartbase << ")("
            << xchartbase + chart.cfw/2 + x << ","
            << ychartbase + chart.cfw/2 + chart.gl << ")\n"
            << "\\psline" << chart.pst << "("
            << xchartbase + chart.cfw/2 + x << ","
            << ychartbase + spy << ")("
            << xchartbase + chart.cfw/2 + x << ","
            << ychartbase + spy - chart.cfw/2 - chart.gl << ")\n";

        if(ii - txn0 >= 0 && (ii - txn0)%txn == 0)
        {
            // Serif label.

            out
                << "\\rput[t]{0}("
                << xchartbase + chart.cfw/2 + x << ","
                << ychartbase - chart.cfw/2 << "){";
            if(abs(xx) <= std::numeric_limits<TX>::epsilon())
                xx = 0;
            output_latex(out, xx);
            out << "}\n";
        }
    }

    // Labeles on Y axis.

    TY yy = ry0;
    ii = 0;
    for(double y = gy0; y < sppy; y += gy, ++ii, yy += ry)
    {
        // Serif.
        out
            << "\\psline" << chart.pst << "("
            << xchartbase << ","
            << ychartbase + chart.cfw/2 + y << ")("
            << xchartbase + chart.cfw/2 + chart.gl << ","
            << ychartbase + chart.cfw/2 + y << ")\n"
            << "\\psline" << chart.pst << "("
            << xchartbase + spx << ","
            << ychartbase + chart.cfw/2 + y << ")("
            << xchartbase + spx - chart.cfw/2 - chart.gl << ","
            << ychartbase + chart.cfw/2 + y << ")\n";

        if(ii - tyn0 >= 0 && (ii - tyn0)%tyn == 0)
        {
            // Serif label.

            out
                << "\\rput[r]{0}("
                << xchartbase - chart.cfw/2 << ","
                << ychartbase + chart.cfw/2 + y << "){";
            if(abs(yy) <= std::numeric_limits<TY>::epsilon())
                yy = 0;
            output_latex(out, yy);
            out << "}\n";
        }
    }

    // Chart content.

    for(size_type j = 0; j < lines.size(); ++j)
    {
        int n = lines[j].x.size();

        out << "\\psline" << lines[j].lst;
        for(int ii = 0; ii < n; ++ii)
        {
            out
                << "("
                << gx/rx*(lines[j].x[ii] - rx0) + gx0 + xchartbase + chart.cfw/2
                << ","
                << gy/ry*(lines[j].y[ii] - ry0) + gy0 + ychartbase + chart.cfw/2
                << ")";
        }
        out << "\n";
    }
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif


}


#if ARAGELI_DEBUG_LEVEL > 3

#include <iostream>
#include "vector.hpp"

namespace Arageli
{

template void output_pstricks_cartesian2d
(
    std::ostream& out,
    double xbase,
    double ybase,
    const cartesian2d_chart& chart,
    const vector<cartesian2d_chart_line<vector<double>, vector<double> > >& lines
);

}

#endif


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
