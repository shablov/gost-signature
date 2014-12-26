/*****************************************************************************

    tex_cartesian2d.hpp

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
    \file tex_cartesian2d.hpp
    \brief Output in LaTeX-file with pstricks commands to make 2D Cartesian
        charts.

    WARNING! The implementation isn't completed.
*/


#ifndef _ARAGELI_tex_cartesian2d_hpp_
#define _ARAGELI_tex_cartesian2d_hpp_

#include "config.hpp"

#include <string>


namespace Arageli
{


#define _ARAGELI_MEMBER_INIT(NAME) this->NAME = NAME

/// Properties of the 2D chart.
struct cartesian2d_chart
{
    cartesian2d_chart
    (
        double sx,
        double sy,
        double txmw,
        double tyw,
        const std::string& cx,
        const std::string& cy,
        double gx = 1,
        double gy = 1,
        double tx = 2,
        double ty = 2,
        double txh = 0.3,
        double tymh = 0.25,
        double cxh = 0.8,
        double cyh = 0.6,
        double gl = 0.1,
        double mx = 0.2,
        double my = 0.2,
        double lmt = 0.4,
        double lpl = 1.5,
        double lmx = 0.2,
        double lmy = 0.2,
        double cfw = 0.1,
        const std::string& pst = "",
        const std::string& cfst = "",
        const std::string& lfst = ""
    )
    {
        _ARAGELI_MEMBER_INIT(sx);
        _ARAGELI_MEMBER_INIT(sy);
        _ARAGELI_MEMBER_INIT(txh);
        _ARAGELI_MEMBER_INIT(txmw);
        _ARAGELI_MEMBER_INIT(tyw);
        _ARAGELI_MEMBER_INIT(tymh);
        _ARAGELI_MEMBER_INIT(cxh);
        _ARAGELI_MEMBER_INIT(cyh);
        _ARAGELI_MEMBER_INIT(gx);
        _ARAGELI_MEMBER_INIT(gy);
        _ARAGELI_MEMBER_INIT(tx);
        _ARAGELI_MEMBER_INIT(ty);
        _ARAGELI_MEMBER_INIT(gl);
        _ARAGELI_MEMBER_INIT(mx);
        _ARAGELI_MEMBER_INIT(my);
        _ARAGELI_MEMBER_INIT(lmt);
        _ARAGELI_MEMBER_INIT(lpl);
        _ARAGELI_MEMBER_INIT(lmx);
        _ARAGELI_MEMBER_INIT(lmy);
        _ARAGELI_MEMBER_INIT(cfw);
        _ARAGELI_MEMBER_INIT(pst);
        _ARAGELI_MEMBER_INIT(cfst);
        _ARAGELI_MEMBER_INIT(lfst);
        _ARAGELI_MEMBER_INIT(cx);
        _ARAGELI_MEMBER_INIT(cy);
    }

    double
        sx,        // whole picture size X
        sy,        // whole picture size Y
        txh,        // height of bar with lables on X axis points
        txmw,        // max width of each lable on X axis points
        tyw,        // width of bar with lables on Y axis points
        tymh,        // max height of each lable on Y axis points
        cxh,        // height of X axis caption (vertical size)
        cyh,        // height of Y axix caption (horizontal size)
        gx,         // optimal distance between points on X axis
        gy,        // optimal distance between points on Y axis
        tx,        // optimal distance between labeled points on X axis
        ty,        // optimal distance between labeled points on Y axis
        gl,            // length of point serifs
        mx,        // additional chart content to frame box margins along X
        my,        // additional chart content to frame box margins along Y
        lmt,        // distance between pattern line and its caption in legend box
        lpl,        // length of pattern line in legend box
        lmx,    // additional spaces between legend box content and its frame box along X
        lmy,    // additional spaces between legend box content and its frame box along Y
        cfw;        // bounding width of chart frame box line

    std::string
        pst,        // point serifs style, string in PSTricks
        cfst,        // chart frame box style, string in PSTricks
        lfst,        // legend frame box style, string in PSTricks
        cx,        // captions on X axes
        cy;        // captions on Y axes
};


/// Properties of the one line of the 2D chart.
template <typename VecX, typename VecY>
struct cartesian2d_chart_line
{
    typedef VecX x_points_type;
    typedef VecY y_points_type;

    cartesian2d_chart_line
    (
        const std::string& caption,
        const VecX& x,
        const VecY& y,
        const std::string& lst,
        double cx = 4,
        double cy = 1,
        double lw = 0    // WARNING! TEMPORARY. TODO: REPLACE BY 1
    )
    {
        _ARAGELI_MEMBER_INIT(lw);
        _ARAGELI_MEMBER_INIT(cx);
        _ARAGELI_MEMBER_INIT(cy);
        _ARAGELI_MEMBER_INIT(caption);
        _ARAGELI_MEMBER_INIT(lst);
        _ARAGELI_MEMBER_INIT(x);
        _ARAGELI_MEMBER_INIT(y);
    }

    double
        lw,            // bounding weight of the line (to calculate margines)
        cx,        // legend caption sizes along X
        cy;        // legend caption sizes along Y

    std::string
        caption,    // legend caption
        lst;        // line style, string in PSTricks

    VecX x;        // x point coordinates of the line
    VecY y;        // y point coordinates of the line
};


/// Output 2D Cartesian graphs into LaTeX-file with using PSTricks.
template <typename Out, typename Lines>
void output_pstricks_cartesian2d
(
    Out& out,
    double xbase,
    double ybase,
    const cartesian2d_chart& chart,
    const Lines& lines
);


#undef _ARAGELI_MEMBER_INIT


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_tex_cartesian2d
    #include "tex_cartesian2d.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_tex_cartesian2d
#endif

#endif    // #ifndef _ARAGELI_tex_cartesian2d_hpp_
