/*****************************************************************************

    arageli.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2010 Nikolai Yu. Zolotykh
    Copyright (C) 2006--2010 Sergey S. Lyalin

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

#ifndef _ARAGELI_arageli_hpp_
#define _ARAGELI_arageli_hpp_

/**
    \file arageli.hpp
    \brief Root header file for all public headers of the library.

    This file includes all general public header files of the library.
    You can just include this file and nothing more to use the whole library.
*/

/**

    \mainpage

    This is a documentation for library
    <a href="http://www.arageli.org">Arageli</a>
    founded by
    <a href="http://www.uic.nnov.ru/~zny">Nikolai Yu. Zolotykh</a>
    and maintaining by
    <a href="http://www.arageli.org/people.html">Arageli Project Team</a>.

    <b>WARNING!</b> The library is being developed. Some features are defined
    only partially, some parts have bugs. Please use it with care.

    <b>WARNING!</b> The library is documented PARTIALLY. Moreover, some parts
    and fragments of this documentation are presented in Russian only. If you
    have problems to read such documentation, please write a letter to
    <a href="mailto:support.arageli@gmail.com">Arageli Support Service</a>.

    Arageli is a C++ library for computations in <b>AR</b>ithmetic,
    <b>A</b>lgebra, <b>GE</b>ometry,
    <b>L</b>inear and <b>I</b>nteger linear programming.
    Arageli is a library for
    dealing with precise, i.e. symbolic or algebraic, computations.  It contains definitions
    to model basic algebraic structures such as integer numbers with arbitrary precision,
    rational numbers, vectors, matrices, polynomials etc.  Arageli is written in C++ and
    uses the power and expressiveness of the language.

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

    Any comments and suggestions are welcome.
    Please send it to <a href="mailto:support.arageli@gmail.com">Arageli Support Service</a>.

*/

#include "config.hpp"

#include "type_opers.hpp"
#include "type_traits.hpp"
#include "function_traits.hpp"
#include "type_pair_traits.hpp"

#include "exception.hpp"

#include "std_import.hpp"
#include "basefuncs.hpp"
#include "mixcomp.hpp"
#include "misc.hpp"
#include "rndbase.hpp"

#include "std_import.hpp"
#include "timer.hpp"

#include "factory.hpp"
#include "cmp.hpp"
#include "powerest.hpp"
#include "gcd.hpp"    // WARNING! Conflict: gcd.hpp needs vector.hpp
#include "intalg.hpp"
#include "prime.hpp"

#include "functional.hpp"

#include "gauss.hpp"
#include "intcount_barvinok.hpp"
#include "bareiss.hpp"

#include "subvector/indexed.hpp"
#include "submatrix/hpair.hpp"

#include "hermite/classic.hpp"
#include "hermite/hafner.hpp"
#include "hermite/storjohann.hpp"
#include "hermite/domich.hpp"
#include "hermite.hpp"

#include "smith/classic.hpp"
#include "smith/storjohann.hpp"
#include "smith/near_optimal.hpp"
#include "smith.hpp"

#include "io.hpp"
#include "iomanip.hpp"

//#include "random.hpp"
#include "solve_linsys.hpp"
#include "simplex_method.hpp"
#include "linopt.hpp"
#include "motzkin_burger.hpp"
#include "skeleton.hpp"
#include "triangulation.hpp"
#include "lll.hpp"
#include "gram_schmidt.hpp"
#include "intconvex.hpp"
#include "ctrl_slog.hpp"
#include "texout.hpp"
#include "ctrl_latexlog.hpp"
#include "polyalg.hpp"
#include "sqrfree.hpp"
#include "sturm.hpp"
#include "resultant.hpp"
#include "algebrslt.hpp"

#include "frwrddecl.hpp"

#include "big_int.hpp"
#include "big_const.hpp"
#include "logarithm.hpp"
#include "big_float.hpp"

#include "residue.hpp"
#include "rational.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "sparse_polynom.hpp"
#include "interval.hpp"
#include "algebraic.hpp"
#include "cone.hpp"
#include "polyhedron.hpp"
#include "polynom.hpp"
#include "sparse_multipolynom.hpp"
#include "sideset.hpp"

#include "tex_cartesian2d.hpp"

#include "berlekamp_hensel.hpp"
#include "advsimplmeth.hpp"
#include "groebner_basis.hpp"

#include "subvector/indexed.hpp"
#include "submatrix/hpair.hpp"
#include "submatrix/indexed.hpp"

#include "setenumrnd.hpp"
#include "setenumrnd/integer.hpp"
#include "setenumrnd/grid.hpp"
#include "setenumrnd/vector.hpp"

#include "karatsuba.hpp"
#include "fft.hpp"
#include "pollard_mult.hpp"
#include "generic.hpp"
#include "polyhedron_relation.hpp"
#include "orthogonal.hpp"
#include "cantor_zassenhaus.hpp"


/// All definitions of the library.
/** Be carefull some names in this namespace conflict with
    the same names from the std namespace, for example `vector'
    and several mathematical functions. Note that all macros
    that are related to the library have a prefix ARAGELI_
    in accordance with Arageli namespace. */
namespace Arageli
{
    /// Hidden definitions for internal use in the library.
    /** If you are an ordinary external user, do not use any facilities
        from this namespace. */
    namespace _Internal
    {}
}


/// Standard C++ Library namespace. In the context of Arageli it contains extentions.
/** Contains extensins of the Standard C++ Library for types from
    Arageli library: std::swap, mathematical functions,
    std::numeric_limits etc.

    Be careful! Some extentions of the std namespace slightly violate
    the C++ Standard but are accepted by many of existing C++
    implementations. */
namespace std
{}


#endif // #ifndef _ARAGELI_arageli_hpp_
