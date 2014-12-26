/*****************************************************************************

    test/stdafx.hpp

    This file is a part of the Arageli library test base.

    Copyright (C) 1999--2007 Sergey S. Lyalin

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

// stdafx.hpp : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <arageli/config.hpp>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <strstream>    // non standard feature
#include <ctime>
#include <iomanip>
#include <string>
#include <map>

#include <arageli/arageli.hpp>
#include <arageli/random.hpp>

#include <ts/ts.hpp>

#include "matrix_ops.hpp"
#include "rand.hpp"
#include "rational.hpp"
#include "vector.hpp"
#include "universal.hpp"
#include "test1.hpp"
#include "io_simple_binary.hpp"

// TODO: reference additional headers your program requires here



#define ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN    \
    {    \
        try    \
        {    \
            ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN


#define ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END    \
            ARAGELI_EXCEPT_LOCCTRL_REGION_END    \
        }    \
        catch(const ::std::exception& e)    \
        {    \
            ::tout    \
                << "\nException from the standard library:"    \
                << "\n\twhat() = " << e.what();    \
            return resEXCEPT;    \
        }    \
        catch(const ::Arageli::exception& e)    \
        {    \
            ::tout << "\nException from Arageli:\n" << e;    \
            return resEXCEPT;    \
        }    \
        catch(...)    \
        {    \
            ::tout << "\nUnknown exception.";    \
            return resEXCEPT;    \
        }    \
    }


////////////////////////////////////////////////////////////////

#pragma warning(disable : 4018)
#pragma warning(disable : 4800)
