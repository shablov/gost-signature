/*****************************************************************************

    exception.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2010 Sergey S. Lyalin

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
    \file exception.hpp
    \brief Basic exceptions, debugging and asserts.

    This file contains basic exception classes definitions and some macros
    for assertions that controlled by debug level value (see config.hpp).

    The tracing framework have several macro and a global stream to putout
    trace messages.
*/


#ifndef _ARAGELI_exception_hpp_
#define _ARAGELI_exception_hpp_

#include "config.hpp"

#include <cstddef>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <list>

#include <typeinfo> //gcc 4.3 fix
//****************************************************************************

namespace Arageli
{


/// Base for all exception classes.
class exception
{
public:

    typedef std::list<std::string> descriptions_type;

    exception ()
    {}

    exception (const std::string& description_a) :
        descriptions_m(1, description_a)
    {}

    const descriptions_type& descriptions () const
    {
        return descriptions_m;
    }

    descriptions_type& descriptions ()
    {
        return descriptions_m;
    }

    virtual void add_description (const std::string& desc)
    {
        descriptions_m.push_back(desc);
    }

    virtual void add_location
    (
        const std::string& src,
        std::size_t line,
        const std::string& desc = std::string()
    );

    /// The message of exception.
    virtual std::string msg () const;

    /// Outputs the message of exception to out stream.
    virtual void output (std::ostream& out) const
    {
        out << msg();
    }

    virtual ~exception () {}

private:

    descriptions_type descriptions_m;

};


inline std::ostream& operator<< (std::ostream& out, const exception& e)
{
    e.output(out);
    return out;
}


#define ARAGELI_EXCEPT_LOC(e) e.add_location(__FILE__, __LINE__)
#define ARAGELI_EXCEPT_LOC_DESC(e, d) e.add_location(__FILE__, __LINE__, d)

#define ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN    \
    {    \
        try    \
        {

#define ARAGELI_EXCEPT_LOCCTRL_REGION_END    \
        }    \
        catch(::Arageli::exception& e)    \
        {    \
            ARAGELI_EXCEPT_LOC_DESC(e, "Exception gate.");    \
            throw;    \
        }    \
    }


/// Exception class that can be trhowed when failed the some assertion.
/** Holds information about failed expression, line and sourse name where
    placed the activated assertion. */
class assert_failed : public virtual exception
{
public:

    assert_failed
    (
        const char* expr_a,
        const char* source_a,
        std::size_t line_a
    ) throw() :
        expr_m(expr_a),
        source_m(source_a),
        line_m(line_a)
    {}

    assert_failed
    (
        const char* expr_a,
        const char* source_a,
        std::size_t line_a,
        const std::string& desc_a
    ) :
        expr_m(expr_a),
        source_m(source_a),
        line_m(line_a)
    {
        add_description(desc_a);
    }

    /// Returns string notation of an expression that has been failed.
    const char* expr () const throw()
    {
        return expr_m;
    }

    /// Returns name of source file in which failure has been occured.
    const char* source () const throw()
    {
        return source_m;
    }

    /// Returns a number of line in file source() with failure.
    std::size_t line () const throw()
    {
        return line_m;
    }

    /// Formats a human readable message with source, line and expression.
    virtual std::string msg () const
    {
        std::ostringstream buf;
        output(buf);
        return buf.str();
    }

    /// Formats and outputs a human readable message with source, line and expression.
    virtual void output (std::ostream& out) const;

private:

    const char* expr_m;
    const char* source_m;
    std::size_t line_m;

};


/// Exception situation: an index out of range while indexing the some structure.
class out_of_range : public virtual exception
{
public:

    virtual std::string msg () const
    {
        return
            "Out of range at an index expression."
            "\nAdditional information:\n" +
            exception::msg();
    }
};


/// Exceprion situation: invalid argument in call of the some operation.
class invalid_argument : public virtual exception
{
public:

    virtual std::string msg () const
    {
        return
            "Invalid argument for the operation"
            "\nAdditional information:\n" +
            exception::msg();
    }
};


/// Exception situation: division by zero.
class division_by_zero : public invalid_argument
{
public:

    virtual std::string msg () const
    {
        return
            "Division by zero"
            "\nAdditional information:\n" +
            exception::msg();
    }
};


/// Exception situation: the some matrix is singular.
class matrix_is_singular : public invalid_argument
{
public:

    virtual std::string msg () const
    {
        return "Matrix is singular"
            "\nAdditional information:\n" +
            exception::msg();
    }
};


/// Exception situation: the some object have failed initialization from a string.
class incorrect_string : public invalid_argument
{
    std::string str_m;

public:

    incorrect_string (const std::string& str_a) :
        str_m(str_a)
    {}

    virtual std::string msg () const
    {
        std::string res = "Incorrect string";
        if(!str_m.empty())
            res += " \"" + str_m + "\"";
        return res + "\nAdditional information:\n" + exception::msg();
    }

    const std::string& str () const
    {
        return str_m;
    }

};


/// Controllers -- classes for control algorithms.
/** A controller is a class (or class template) intended to allow
    some control and retriving information for the controlled
    function while this function is executing. */
namespace ctrl
{

/// Hidden definitions for internal use in the library controllers subsystem.
/** It's analogue for Arageli::_Internal but for Arageli::ctrl namespace.
    If you are an ordinary external user, do not use any facilities
    from this namespace. */
namespace _Internal
{}


/// Parent for all controller's exceptions that inform about early end of an execution.
class abort : public virtual exception
{
public:

    virtual std::string msg () const
    {
        return std::string("Controller \"") +
            typeid(*this).name() +
            "\" breaks execution of the function."
            + "\nAdditional information:\n" + exception::msg();
    }
};

}


/// Construct for throwing exception assert_failed or harder failure raising.
#ifdef ARAGELI_ASSERT_THROW_EXCEPTION

    #define ARAGELI_THROW_ASSERT_FAILED(EXPR)    \
    {    \
        throw ::Arageli::assert_failed(#EXPR, __FILE__, __LINE__);    \
    }

    #define ARAGELI_THROW_ASSERT_FAILED_EX1(EXPR, ARG1)    \
    {    \
        ::std::ostringstream _arageli_tmp_1;    \
        _arageli_tmp_1 << "ARG1 = " << ARG1;    \
        throw ::Arageli::assert_failed(#EXPR, __FILE__, __LINE__, _arageli_tmp_1.str());    \
    }

#else

    #ifdef NDEBUG

        #define ARAGELI_THROW_ASSERT_FAILED(EXPR)    \
        {    \
            ::std::cerr << '\n' << ::Arageli::assert_failed(#EXPR, __FILE__, __LINE__);    \
            ::std::cerr << "\nPress Enter key for exit.";    \
            ::std::cin.get();    \
            ::std::abort();    \
        }

        #define ARAGELI_THROW_ASSERT_FAILED_EX1(EXPR, ARG1)    \
        {    \
            ::std::ostringstream _arageli_tmp_1;    \
            _arageli_tmp_1 << "ARG1 = " << ARG1;    \
            ::std::cerr << '\n' << ::Arageli::assert_failed(#EXPR, __FILE__, __LINE__, _arageli_tmp_1.str());    \
            ::std::cerr << "\nPress Enter key for exit.";    \
            ::std::cin.get();    \
            ::std::abort();    \
        }

    #else

        #define ARAGELI_THROW_ASSERT_FAILED(EXPR)    \
        {    \
            assert(!#EXPR);    \
        }

        #define ARAGELI_THROW_ASSERT_FAILED_EX1(EXPR, ARG1)    \
        {    \
            ::std::cerr << "\nAssertion ARG1 = " << ARG1;    \
            assert(!#EXPR);    \
        }

    #endif
#endif


/// Always check assertion with a particular exception object.
#define ARAGELI_ASSERT_ALWAYS_CUST(EXPR, EXCEPT_OBJ)    \
{    \
    if(!(EXPR))    \
        throw EXCEPT_OBJ;    \
}


/// Always check assertion with standard reaction when assertion fails.
#define ARAGELI_ASSERT_ALWAYS(EXPR)    \
{    \
    if(!(EXPR))    \
        ARAGELI_THROW_ASSERT_FAILED(EXPR);    \
}


/// Always check assertion with standard reaction when assertion fails.
/** This macro takes one additional argument that can be any type and provided
    by user. It should be able to output by operator<< into stream.
    This argument is used as an additional piece of information about
    a failure. */
#define ARAGELI_ASSERT_ALWAYS_EX1(EXPR, ARG1)    \
{    \
    if(!(EXPR))    \
        ARAGELI_THROW_ASSERT_FAILED_EX1(EXPR, ARG1);    \
}


#ifdef ARAGELI_ASSERT_ASSUME
    #define _ARAGELI_ASSERT_ASSUME(X)    __assume(X)
#else
    #define _ARAGELI_ASSERT_ASSUME(X)    /* nothing */
#endif


#if ARAGELI_DEBUG_LEVEL > 0

    #define ARAGELI_ASSERT_0_CUST(EXPR, EXCEPT_OBJ)    \
        ARAGELI_ASSERT_ALWAYS_CUST(EXPR, EXCEPT_OBJ)

    #define ARAGELI_ASSERT_0(EXPR)    \
        ARAGELI_ASSERT_ALWAYS(EXPR)

    #define ARAGELI_ASSERT_EX1_0(EXPR, ARG1)    \
        ARAGELI_ASSERT_ALWAYS_EX1(EXPR, ARG1)

    #define ARAGELI_DEBUG_EXEC_0(WHAT) WHAT

#else

    #define ARAGELI_ASSERT_0_CUST(EXPR, EXCEPT_OBJ)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_ASSERT_0(EXPR)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_ASSERT_EX1_0(EXPR, ARG1)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_DEBUG_EXEC_0(WHAT)    /* nothing */

#endif


#if ARAGELI_DEBUG_LEVEL > 1

    #define ARAGELI_ASSERT_1_CUST(EXPR, EXCEPT_OBJ)    \
        ARAGELI_ASSERT_ALWAYS_CUST(EXPR, EXCEPT_OBJ)

    #define ARAGELI_ASSERT_1(EXPR)    \
        ARAGELI_ASSERT_ALWAYS(EXPR)

    #define ARAGELI_ASSERT_EX1_1(EXPR, ARG1)    \
        ARAGELI_ASSERT_ALWAYS_EX1(EXPR, ARG1)

    #define ARAGELI_DEBUG_EXEC_1(WHAT) WHAT

#else

    #define ARAGELI_ASSERT_1_CUST(EXPR, EXCEPT_OBJ)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_ASSERT_1(EXPR)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_ASSERT_EX1_1(EXPR, ARG1)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_DEBUG_EXEC_1(WHAT)    /* nothing */

#endif


#if ARAGELI_DEBUG_LEVEL > 2

    #define ARAGELI_ASSERT_2_CUST(EXPR, EXCEPT_OBJ)    \
        ARAGELI_ASSERT_ALWAYS_CUST(EXPR, EXCEPT_OBJ)

    #define ARAGELI_ASSERT_2(EXPR)    \
        ARAGELI_ASSERT_ALWAYS(EXPR)

    #define ARAGELI_ASSERT_EX1_2(EXPR, ARG1)    \
        ARAGELI_ASSERT_ALWAYS_EX1(EXPR, ARG1)

    #define ARAGELI_DEBUG_EXEC_2(WHAT) WHAT

#else

    #define ARAGELI_ASSERT_2_CUST(EXPR, EXCEPT_OBJ)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_ASSERT_2(EXPR)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_ASSERT_EX1_2(EXPR, ARG1)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_DEBUG_EXEC_2(WHAT)    /* nothing */

#endif


#if ARAGELI_DEBUG_LEVEL > 3

    #define ARAGELI_ASSERT_3_CUST(EXPR, EXCEPT_OBJ)    \
        ARAGELI_ASSERT_ALWAYS_CUST(EXPR, EXCEPT_OBJ)

    #define ARAGELI_ASSERT_3(EXPR)    \
        ARAGELI_ASSERT_ALWAYS(EXPR)

    #define ARAGELI_ASSERT_EX1_3(EXPR, ARG1)    \
        ARAGELI_ASSERT_ALWAYS_EX1(EXPR, ARG1)

    #define ARAGELI_DEBUG_EXEC_3(WHAT) WHAT

#else

    #define ARAGELI_ASSERT_3_CUST(EXPR, EXCEPT_OBJ)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_ASSERT_3(EXPR)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_ASSERT_EX1_3(EXPR, ARG1)    _ARAGELI_ASSERT_ASSUME(EXPR)
    #define ARAGELI_DEBUG_EXEC_3(WHAT)    /* nothing */

#endif


#ifdef ARAGELI_TRACING

/// A global collector for tracing messages.
extern std::ostream& tracing_common;

/// A deep of trace point in some hierarchy.
extern std::size_t tracing_deep;

inline std::string tracing_indent ()
{ return std::string(tracing_deep, ' '); }

#define ARAGELI_TRACE(STATEMENT)    \
{    \
    STATEMENT;    \
}

#define ARAGELI_TRACE_EXTSCOPE(STATEMENT)  STATEMENT
#define ARAGELI_TRACE_BEGIN(STATEMENT) { STATEMENT; ++::Arageli::tracing_deep; }
#define ARAGELI_TRACE_END(STATEMENT) { --::Arageli::tracing_deep; STATEMENT; }

#define ARAGELI_TRACE_BEGIN(STATEMENT)    \
{    \
    STATEMENT;    \
    ++tracing_deep;    \
}

#define ARAGELI_TRACE_END(STATEMENT)    \
{    \
    --tracing_deep;    \
    STATEMENT;    \
}

#define ARAGELI_TRACE_BEGIN_AUTO(STATEMENT1, STATEMENT2)    \
    ARAGELI_TRACE_BEGIN(STATEMENT1);    \
    struct __tracing_tmp_struct    \
    {    \
        ~__tracing_tmp_struct ()    \
        {    \
            ARAGELI_TRACE_END(STATEMENT2);    \
        }    \
    } __tracing_tmp_object;

#else

#define ARAGELI_TRACE(STATEMENT)    /* nothing */
#define ARAGELI_TRACE_EXTSCOPE(STATEMENT)    /* nothing */
#define ARAGELI_TRACE_BEGIN(STATEMENT)    /* nothing */
#define ARAGELI_TRACE_END(STATEMENT)    /* nothing */
#define ARAGELI_TRACE_BEGIN_AUTO(STATEMENT1, STATEMENT2)    /* nothing */

#endif


#ifdef NDEBUG
    #define ARAGELI_ERROR(MSG)    \
        {    \
            ::std::ostringstream buf;    \
            buf << MSG << "\nFile: " << __FILE__ << " : " << __LINE__ << '\n';    \
            throw ::Arageli::exception(buf.str());    \
        }
#else
    #define ARAGELI_ERROR(MSG)    \
    {    \
        ::std::cerr << MSG << "\nFile: " << __FILE__ << " : " << __LINE__ << '\n';    \
        ::std::cin.get();    \
        ::std::abort();    \
    }
#endif


} // namespace Arageli


#endif  //  #ifndef _ARAGELI_exception_hpp_
