/* /////////////////////////////////////////////////////////////////////////////
//
//  File:       strcompare.hpp
//  Created:    2005/4/10    10:19
//
//  Author: Andrey Somsikov
*/

#ifndef __strcompare_hpp__
#define __strcompare_hpp__

#include <string>

bool match(const std::string& str, const std::string& sheme)
{
    if (sheme == "*")
        return true;
    if (sheme == "?")
        return !str.empty();
    if (sheme == str)
        return true;

    return false;
}

#endif /*__strcompare_hpp__*/
/* End of file strcompare.hpp */
