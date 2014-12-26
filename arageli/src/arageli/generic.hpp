/*****************************************************************************

    generic.hpp

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
    \file generic.hpp
    \brief A generic type for representing a wide range of basic Arageli type.

    This file includes a declaration of several classes to implement the
    generic_type type concept generic_type type.
    This type maps the static polymorphism of basic Arageli types at compile
    time to dynamic    polymorphism at runtime. This concept is the same as
    JavaScript uses for each variable.
*/


#ifndef _ARAGELI_generic_hpp_
#define _ARAGELI_generic_hpp_

#include "config.hpp"

#include <string>
#include <iostream>

#include "exception.hpp"
#include "big_int.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "sparse_multipolynom.hpp"


namespace Arageli
{


/// An abstract class to implement the generic_type class. Base for a class family.
class generic_internal
{
public:

    class exception :
        public Arageli::exception
    {};

    class unsupported_operation :
        public exception
    {};

    static generic_internal* create_from_string (const char* s)
    {
        // just a stub
        return 0;
    }

    static generic_internal* create_from_input (std::istream& in);

    virtual ~generic_internal ()
    {}

    virtual generic_internal* clone () const
    {
        throw unsupported_operation();
    }

    virtual std::string type () const
    {
        throw unsupported_operation();
    }

    virtual bool operator_bool () const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* logical_not () const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* unary_plus () const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* unary_minus () const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* postfix_increment ()
    {
        throw unsupported_operation();
    }

    virtual generic_internal* postfix_decrement ()
    {
        throw unsupported_operation();
    }

    virtual generic_internal* prefix_increment ()
    {
        throw unsupported_operation();
    }

    virtual generic_internal* prefix_decrement ()
    {
        throw unsupported_operation();
    }

    virtual generic_internal* assign_plus (const generic_internal* x)
    {
        throw unsupported_operation();
    }

    virtual generic_internal* assign_minus (const generic_internal* x)
    {
        throw unsupported_operation();
    }

    virtual generic_internal* assign_multiplies (const generic_internal* x)
    {
        throw unsupported_operation();
    }

    virtual generic_internal* assign_divides (const generic_internal* x)
    {
        throw unsupported_operation();
    }

    virtual generic_internal* assign_modulus (const generic_internal* x)
    {
        throw unsupported_operation();
    }

    virtual generic_internal* assign_bitwise_or (const generic_internal* b)
    {
        throw unsupported_operation();
    }

    virtual generic_internal* assign_bitwise_and (const generic_internal* b)
    {
        throw unsupported_operation();
    }

    virtual generic_internal* assign_bitwise_xor (const generic_internal* b)
    {
        throw unsupported_operation();
    }

    virtual generic_internal* assign_shift_left (const generic_internal* b)
    {
        throw unsupported_operation();
    }

    virtual generic_internal* assign_shift_right (const generic_internal* b)
    {
        throw unsupported_operation();
    }

    virtual generic_internal* plus (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* minus (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* multiplies (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* divides (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* modulus (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* equal_to (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* not_equal_to (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* greater (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* less (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* greater_equal (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* less_equal (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* bitwise_or (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* bitwise_and (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* bitwise_xor (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* shift_left (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual generic_internal* shift_right (const generic_internal* b) const
    {
        throw unsupported_operation();
    }

    virtual void output (std::ostream& out) const
    {
        throw unsupported_operation();
    }

    virtual void output_aligned (std::ostream& out) const
    {
        output(out);
    }
};


class generic_vector;
class generic_matrix;

class generic_big_int : public generic_internal
{
    big_int value;
    friend class generic_vector;
    friend class generic_matrix;

public:

    generic_big_int (const big_int& x = big_int()) :
        value(x)
    {}

    virtual generic_big_int* clone () const;
    virtual std::string type () const;
    virtual bool operator_bool () const;
    virtual generic_big_int* logical_not () const;
    virtual generic_big_int* unary_plus () const;
    virtual generic_big_int* unary_minus () const;
    virtual generic_internal* postfix_increment ();
    virtual generic_internal* postfix_decrement ();
    virtual generic_internal* prefix_increment ();
    virtual generic_internal* prefix_decrement ();
    virtual generic_internal* assign_plus (const generic_internal* x);
    virtual generic_internal* assign_minus (const generic_internal* x);
    virtual generic_internal* assign_multiplies (const generic_internal* x);
    virtual generic_internal* assign_divides (const generic_internal* x);
    virtual generic_internal* assign_modulus (const generic_internal* x);
    virtual generic_internal* assign_bitwise_or (const generic_internal* b);
    virtual generic_internal* assign_bitwise_and (const generic_internal* b);
    virtual generic_internal* assign_bitwise_xor (const generic_internal* b);
    virtual generic_internal* assign_shift_left (const generic_internal* b);
    virtual generic_internal* assign_shift_right (const generic_internal* b);
    virtual generic_internal* plus (const generic_internal* b) const;
    virtual generic_internal* minus (const generic_internal* b) const;
    virtual generic_internal* multiplies (const generic_internal* b) const;
    virtual generic_internal* divides (const generic_internal* b) const;
    virtual generic_internal* modulus (const generic_internal* b) const;
    virtual generic_internal* equal_to (const generic_internal* b) const;
    virtual generic_internal* not_equal_to (const generic_internal* b) const;
    virtual generic_internal* greater (const generic_internal* b) const;
    virtual generic_internal* less (const generic_internal* b) const;
    virtual generic_internal* greater_equal (const generic_internal* b) const;
    virtual generic_internal* less_equal (const generic_internal* b) const;
    virtual generic_internal* bitwise_or (const generic_internal* b) const;
    virtual generic_internal* bitwise_and (const generic_internal* b) const;
    virtual generic_internal* bitwise_xor (const generic_internal* b) const;
    virtual generic_internal* shift_left (const generic_internal* b) const;
    virtual generic_internal* shift_right (const generic_internal* b) const;
    virtual void output (std::ostream& out) const;

    virtual void output_aligned (std::ostream& out) const
    {
        output(out);
    }

    const big_int& get_value () const
    {
        return value;
    }
};


/// Generic type that can dynamically change type of hold object.
class generic_type
{
    generic_internal* value_m;    ///< Held value. Can be 0.

    static generic_internal* safe_clone (const generic_type& x)
    {
        return x.value_m ? x.value_m->clone() : 0;
    }

public:

    /// Construct not initialized object.
    /** Such object should be initialized before use it in non-trivial operations. */
    generic_type () :
        value_m(0)
    {}

    /// Initialization from a string.
    /** The syntax differs from the basic ones because should containt information
        about the type of the object that should be constructed. */
    generic_type (const char* s) :
        value_m(generic_internal::create_from_string(s))
    {}

    /// Just copy constructor.
    generic_type (const generic_type& x) :
        value_m(safe_clone(x))
    {}

    /// Construct from an internal value.
    generic_type (generic_internal* xi) :
        value_m(xi)
    {}

    generic_type (unsigned int x) :
        value_m(new generic_big_int(x))
    {}

    generic_type (int x) :
        value_m(new generic_big_int(x))
    {}

    generic_type (const big_int& x) :
        value_m(new generic_big_int(x))
    {}

    template <bool REFCNT>
    generic_type (const vector<generic_type, REFCNT>& x);

    template <bool REFCNT>
    generic_type (const matrix<generic_type, REFCNT>& x);

    generic_type (const sparse_multipolynom<generic_type>& x);

    operator big_int () const;
    operator vector<generic_type> () const;
    operator matrix<generic_type> () const;

    /// Just assignment operator.
    /** The type of the old value isn't used and it is completely destroyed. */
    generic_type& operator= (const generic_type& x)
    {
        ARAGELI_ASSERT_0(this != &x);
        delete value_m;
        value_m = safe_clone(x);
        return *this;
    }

    /// Assignment from the internal type.
    /** The type of the old value isn't used and it is completely destroyed. */
    generic_type& operator= (generic_internal* x)
    {
        ARAGELI_ASSERT_0(x == 0 || x != value_m);
        delete value_m;
        value_m = x;
        return *this;
    }

    ~generic_type ()
    {
        delete value_m;
    }

    // Access to properties.

    /// It's true if this object doesn't hold any value_m.
    bool is_nothing () const
    {
        return value_m == 0;
    }

    /// The name of the held type.
    std::string type () const
    {
        if(value_m)
            return value_m->type();
        else
        {
            static const std::string s = "nothing";
            return s;
        }
    }

    generic_internal* value ()
    {
        return value_m;
    }

    const generic_internal* value () const
    {
        return value_m;
    }

    // Some operations.

    generic_type operator! () const
    {
        return value_m->logical_not();
    }

    operator bool () const
    {
        return value_m->operator_bool();
    }

    generic_type operator~ () const
    {
        throw generic_internal::unsupported_operation();
    }

    generic_type operator+ () const
    {
        return value_m->unary_plus();
    }

    generic_type operator- () const
    {
        return value_m->unary_minus();
    }

    generic_type& operator+= (const generic_type& x)
    {
        value_m->assign_plus(x.value_m);
        return *this;
    }

    generic_type& operator-= (const generic_type& x)
    {
        value_m->assign_minus(x.value_m);
        return *this;
    }

    generic_type& operator*= (const generic_type& x)
    {
        value_m->assign_multiplies(x.value_m);
        return *this;
    }

    generic_type& operator/= (const generic_type& x)
    {
        value_m->assign_divides(x.value_m);
        return *this;
    }

    generic_type& operator%= (const generic_type& x)
    {
        value_m->assign_modulus(x.value_m);
        return *this;
    }

    generic_type& operator%= (int x)
    {
        return *this %= generic_type(x);
    }

    generic_type& operator<<= (const generic_type& x)
    {
        value_m->assign_shift_left(x.value_m);
        return *this;
    }

    generic_type& operator>>= (const generic_type& x)
    {
        value_m->assign_shift_right(x.value_m);
        return *this;
    }

    generic_type& operator<<= (unsigned int x)
    {
        return *this <<= generic_type(x);
    }

    generic_type& operator>>= (unsigned int x)
    {
        return *this >>= generic_type(x);
    }

    generic_type& operator++ ()
    {
        value_m->prefix_increment();
        return *this;
    }

    generic_type& operator-- ()
    {
        value_m->prefix_decrement();
        return *this;
    }

    generic_type operator++ (int)
    {
        return value_m->postfix_increment();
    }

    generic_type operator-- (int)
    {
        return value_m->postfix_increment();
    }

};


inline generic_type operator+ (const generic_type& a, const generic_type& b)
{
    return a.value()->plus(b.value());
}

inline generic_type operator- (const generic_type& a, const generic_type& b)
{
    return a.value()->minus(b.value());
}

inline generic_type operator* (const generic_type& a, const generic_type& b)
{
    return a.value()->multiplies(b.value());
}

inline generic_type operator/ (const generic_type& a, const generic_type& b)
{
    return a.value()->divides(b.value());
}

inline generic_type operator% (const generic_type& a, const generic_type& b)
{
    return a.value()->modulus(b.value());
}

inline generic_type operator% (const generic_type& a, int b)
{
    return a % generic_type(b);
}

inline generic_type operator| (const generic_type& a, const generic_type& b)
{
    return a.value()->bitwise_or(b.value());
}

inline generic_type operator^ (const generic_type& a, const generic_type& b)
{
    return a.value()->bitwise_xor(b.value());
}

inline generic_type operator& (const generic_type& a, const generic_type& b)
{
    return a.value()->bitwise_and(b.value());
}

inline generic_type operator>> (const generic_type& a, const generic_type& b)
{
    return a.value()->shift_right(b.value());
}

inline generic_type operator<< (const generic_type& a, const generic_type& b)
{
    return a.value()->shift_left(b.value());
}

inline generic_type operator>> (const generic_type& a, unsigned int b)
{
    return a >> generic_type(b);
}

inline generic_type operator<< (const generic_type& a, unsigned int b)
{
    return a << generic_type(b);
}


inline generic_type operator== (const generic_type& a, const generic_type& b)
{
    return a.value()->equal_to(b.value());
}

inline generic_type operator!= (const generic_type& a, const generic_type& b)
{
    return a.value()->not_equal_to(b.value());
}

inline generic_type operator<  (const generic_type& a, const generic_type& b)
{
    return a.value()->less(b.value());
}

inline generic_type operator>  (const generic_type& a, const generic_type& b)
{
    return a.value()->greater(b.value());
}

inline generic_type operator<= (const generic_type& a, const generic_type& b)
{
    return a.value()->less_equal(b.value());
}

inline generic_type operator>= (const generic_type& a, const generic_type& b)
{
    return a.value()->greater_equal(b.value());
}


// WARNING! Problem: Why are the following definitions not able to be compiled
// with uncommented the template headers?

//template <typename Stream>
inline std::ostream& operator<< (std::ostream& out, const generic_type& x)
{
    if(x.is_nothing())
        out << "nothing";
    else
        x.value()->output(out);
    return out;
}

//template <typename Stream>
inline std::istream& operator>> (std::istream& in, generic_type& x)
{
    x = generic_internal::create_from_input(in);
    if(!x.is_nothing())
        return in;
    else
        in.clear(std::ios_base::badbit);
    return in;
}


template <typename Stream>
inline void output_aligned (Stream& out, const generic_type& x)
{
    if(x.is_nothing())
        out << "nothing";
    else
        x.value()->output_aligned(out);
}


class generic_vector : public generic_internal
{
    vector<generic_type> value;
    friend class generic_big_int;
    friend class generic_matrix;

public:

    generic_vector ()
    {}

    generic_vector (const vector<generic_type>& x) :
        value(x)
    {}

    virtual generic_vector* clone () const;
    virtual std::string type () const;
    virtual generic_internal* plus (const generic_internal* b) const;
    virtual generic_internal* minus (const generic_internal* b) const;
    virtual generic_internal* multiplies (const generic_internal* b) const;
    virtual generic_internal* divides (const generic_internal* b) const;
    virtual generic_internal* modulus (const generic_internal* b) const;
    virtual void output (std::ostream& out) const;

    virtual void output_aligned (std::ostream& out) const
    {
        Arageli::output_aligned(out, value);
    }

    const vector<generic_type>& get_value () const
    {
        return value;
    }
};


class generic_matrix : public generic_internal
{
    matrix<generic_type> value;
    friend class generic_big_int;
    friend class generic_vector;

public:

    generic_matrix ()
    {}

    generic_matrix (const matrix<generic_type>& x) :
        value(x)
    {}

    virtual generic_matrix* clone () const;
    virtual std::string type () const;
    virtual generic_internal* plus (const generic_internal* b) const;
    virtual generic_internal* minus (const generic_internal* b) const;
    virtual generic_internal* multiplies (const generic_internal* b) const;
    virtual generic_internal* divides (const generic_internal* b) const;
    virtual generic_internal* modulus (const generic_internal* b) const;
    virtual void output (std::ostream& out) const;

    virtual void output_aligned (std::ostream& out) const
    {
        Arageli::output_aligned(out, value);
    }

    const matrix<generic_type>& get_value () const
    {
        return value;
    }
};


class generic_sparse_multipolynom : public generic_internal
{
    sparse_multipolynom<generic_type> value;

public:

    generic_sparse_multipolynom ()
    {}

    generic_sparse_multipolynom (const sparse_multipolynom<generic_type>& x) :
        value(x)
    {}

    virtual generic_sparse_multipolynom* clone () const;

    virtual std::string type () const
    {
        return "sparse_multipolynom";
    }

    virtual generic_internal* plus (const generic_internal* b) const;
    virtual generic_internal* minus (const generic_internal* b) const;
    virtual generic_internal* multiplies (const generic_internal* b) const;
    virtual generic_internal* divides (const generic_internal* b) const;
    virtual generic_internal* modulus (const generic_internal* b) const;
    virtual void output (std::ostream& out) const;

    virtual void output_aligned (std::ostream& out) const
    {
        Arageli::output_var(out, value);
    }

    const sparse_multipolynom<generic_type>& get_value () const
    {
        return value;
    }
};


template <bool REFCNT>
inline generic_type::generic_type (const vector<generic_type, REFCNT>& x) :
    value_m(new generic_vector(x))
{}


template <bool REFCNT>
inline generic_type::generic_type (const matrix<generic_type, REFCNT>& x) :
    value_m(new generic_matrix(x))
{}

inline generic_type::generic_type (const sparse_multipolynom<generic_type>& x) :
    value_m(new generic_sparse_multipolynom(x))
{}


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_generic
    #include "generic.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_generic
#endif

#endif    // #ifndef _ARAGELI_generic_hpp_
