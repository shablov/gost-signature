/*****************************************************************************

    auto_link.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2005 Andrey Somsikov

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
    \file auto_link.hpp
    \brief Directive to automatic link arageli.lib with user's application.

    As I know (Sergey Lyalin, 2006) this file is useful only for Microsoft
    Windows environment and Microsoft C++ compiler or Intel C++ Compiler.
*/


#ifndef __ARAGELI_auto_link_hpp__
#define __ARAGELI_auto_link_hpp__

#define ARAGELI_STRINGIZE(macro) ARAGELI_DO_STRINGIZE(macro)
#define ARAGELI_DO_STRINGIZE(name) #name

namespace Arageli
{

#ifndef ARAGELI_SOURCE
    #pragma comment(lib, ARAGELI_STRINGIZE(ARAGELI_LIB_NAME) ".lib")
#endif

// undef any macros we have defined
#undef ARAGELI_LIB_NAME

} // namespace Arageli

#endif /*__ARAGELI_auto_link_hpp__*/
/* End of file auto_link.hpp */
