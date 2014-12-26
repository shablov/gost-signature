/*****************************************************************************

    io.hpp

    This file is a part of Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007 Sergey S. Lyalin

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


#ifndef _ARAGELI_io_hpp_
#define _ARAGELI_io_hpp_

#include "config.hpp"

#include <iostream>
#include <complex>


namespace Arageli
{


/// Default ouputting method for subexpression as the first coefficient in a polynomial.
template <typename T, typename Ch, typename ChT>
inline std::basic_ostream<Ch, ChT>& output_polynom_first_default
(
    std::basic_ostream<Ch, ChT>& out,
    const T& x
)
{
    return out << x;
}

/// Default ouputting method for subexpression as an internal coefficient in a polynomial.
template <typename T, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_polynom_internal_default
(
    std::basic_ostream<Ch, ChT>& out,
    const T& x
);

/// Default ouputting method for subexpression as a degree of variable in a polynomial.
template <typename T, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_pow_default
(
    std::basic_ostream<Ch, ChT>& out,
    const T& x
);

/// Default inputting method for subexpression as the first coefficient in a polynomial.
template <typename T, typename Ch, typename ChT>
std::basic_istream<Ch, ChT>& input_polynom_first_default
(
    std::basic_istream<Ch, ChT>& in,
    T& x
);

/// Default inputting method for subexpression as an internal coefficient in a polynomial.
template <typename T, typename Ch, typename ChT>
std::basic_istream<Ch, ChT>& input_polynom_internal_default
(
    std::basic_istream<Ch, ChT>& in,
    T& x
);

/// Default inutting method for subexpression as a degree of variable in a polynomial.
template <typename T, typename Ch, typename ChT>
inline std::basic_istream<Ch, ChT>& input_pow_default
(
    std::basic_istream<Ch, ChT>& in,
    T& x
)
{
    return in >> x;
}


template <typename T, typename Ch, typename ChT>
inline std::basic_ostream<Ch, ChT>& output_polynom_first
(
    std::basic_ostream<Ch, ChT>& out,
    const T& x
)
{
    return output_polynom_first_default(out, x);
}

template <typename T, typename Ch, typename ChT>
inline std::basic_ostream<Ch, ChT>& output_polynom_internal
(
    std::basic_ostream<Ch, ChT>& out,
    const T& x)
{
    return output_polynom_internal_default(out, x);
}

template <typename T, typename Ch, typename ChT>
inline std::basic_ostream<Ch, ChT>& output_pow
(
    std::basic_ostream<Ch, ChT>& out,
    const T& x
)
{
    return output_pow_default(out, x);
}

template <typename T, typename Ch, typename ChT>
inline std::basic_istream<Ch, ChT>& input_polynom_first
(
    std::basic_istream<Ch, ChT>& in,
    T& x
)
{
    return input_polynom_first_default(in, x);
}

template <typename T, typename Ch, typename ChT>
inline std::basic_istream<Ch, ChT>& input_polynom_internal
(
    std::basic_istream<Ch, ChT>& in,
    T& x
)
{
    return input_polynom_internal_default(in, x);
}

template <typename T, typename Ch, typename ChT>
inline std::basic_istream<Ch, ChT>& input_pow
(
    std::basic_istream<Ch, ChT>& in,
    T& x
)
{
    return input_pow_default(in, x);
}


// Specializations for std::complex.


/// Default ouputting method for subexpression as an internal coefficient in a polynomial.
template <typename T, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_polynom_internal_default
(
    std::basic_ostream<Ch, ChT>& out,
    const std::complex<T>& x
);

/// Default ouputting method for subexpression as a degree of variable in a polynomial.
template <typename T, typename Ch, typename ChT>
inline std::basic_ostream<Ch, ChT>& output_pow_default
(
    std::basic_ostream<Ch, ChT>& out,
    const std::complex<T>& x
)
{
    out << x;
}


/// Represents a pointer as a stream in memory for writing.
class memory_ostream
{
    char* mem;

public:

    memory_ostream (char* addr) :
        mem(addr)
    {}

    operator char* () const
    {
        return mem;
    }

    bool operator! () const
    {
        return false;
    }

    operator bool () const
    {
        return true;
    }

    void write (const char* x, std::size_t n)
    {
        mem = std::copy(x, x + n, mem);
    }
};


/// Represents a pointer as a stream in memory for reading.
class memory_istream
{
    const char* mem;

public:

    memory_istream (const char* addr) :
        mem(addr)
    {}

    operator const char* () const
    {
        return mem;
    }

    bool operator! () const
    {
        return false;
    }

    operator bool () const
    {
        return true;
    }

    void read (char* x, std::size_t n)
    {
        const char* end = mem + n;
        std::copy(mem, end, x);
        mem = end;
    }
};


/// Interface for a collection of functions to do seft-delimeted binary serialization.
/** Serialization is in the Simple Binary format.
    It is just a common template with required interface.
    There are no functions defined; only signatures with semantics. */
template <typename T>
class io_binary
{
public:

    /// Stores an object state to a binary stream. Seft-delimeted binary serialization.
    /** This function doesn't have implementation.
        See specializations for concrete types. */
    template <typename Stream>
    static Stream& output_stream (Stream& out, const T& x);

    /// Loads an object state from a binary stream. Compatible with output_stream.
    /** This function doesn't have implementation.
        See specializations for concrete types. */
    template <typename Stream>
    static Stream& input_stream (Stream& in, T& x);

    /// Stores an array of object states to a binary stream. Seft-delimeted binary serialization.
    /** This function doesn't have implementation.
        See specializations for concrete types. */
    template <typename Stream>
    static Stream& output_stream
    (
        Stream& out,
        const T* x,
        std::size_t n
    );

    /// Loads an array of object states from a binary stream. Compatible with output_stream.
    /** This function doesn't have implementation.
        See specializations for concrete types. */
    template <typename Stream>
    static Stream& input_stream
    (
        Stream& in,
        T* x,
        std::size_t n
    );

    /// Calculates the number of chars required to store a given object in The Simple Binary form.
    /** This function doesn't have implementation.
        See specializations for concrete types. */
    static std::size_t calc (const T& x);

    /// Calculates the number of chars required to store a given array of objects in The Simple Binary form.
    /** This function doesn't have implementation.
        See specializations for concrete types. */
    static std::size_t calc (const T* x, std::size_t n);

    /// Stores an object state to a memory location. Seft-delimeted binary serialization.
    /** This function doesn't have implementation.
        See specializations for concrete types. */
    static char* output_mem (char* out, const T& x);

    /// Loads an object state from a memory location. Compatible with output_stream.
    /** This function doesn't have implementation.
        See specializations for concrete types. */
    static const char* input_mem (const char* in, T& x);

    /// Stores an array of object states to a memory location. Seft-delimeted binary serialization.
    /** This function doesn't have implementation.
        See specializations for concrete types. */
    static char* output_mem
    (
        char* out,
        const T* x,
        std::size_t n
    );

    /// Loads an array of object states from a memory location. Compatible with output_stream.
    /** This function doesn't have implementation.
        See specializations for concrete types. */
    static const char* input_mem
    (
        const char* in,
        T* x,
        std::size_t n
    );
};


/// Implements those functions of io_binary that deal with memory and arrays.
/** This class template implements some of the functions of io_binary
    class template. Those functions are: memory functions, input and
    output arrays of objects.
    This class provides typical implementation of those functions for
    not raw objects but can be used for raw objects too (for raw
    objects we can do this better, see io_binary_raw class template). */
template <typename T>
class io_binary_base
{
public:

    /// Stores an array of object states to a binary stream.
    /** The functions do that by means of a simple loop. */
    template <typename Stream>
    static Stream& output_stream
    (
        Stream& out,
        const T* x,
        std::size_t n
    );

    /// Stores an array of object states from a binary stream.
    /** The functions do that by means of a simple loop. */
    template <typename Stream>
    static Stream& input_stream
    (
        Stream& in,
        T* x,
        std::size_t n
    );

    /// Calculates the number of chars required to store a given array of objects.
    /** The functions do that by means of a simple loop. */
    static std::size_t calc (const T* x, std::size_t n);

    /// Stores an object state to a memory location.
    static char* output_mem (char* out, const T& x);

    /// Loads an object state from a memory location.
    static const char* input_mem (const char* in, T& x);

    /// Stores an array of object states to a memory location.
    /** The functions do that by means of a simple loop. */
    static char* output_mem
    (
        char* out,
        const T* x,
        std::size_t n
    );

    /// Loads an array of object states from a memory location.
    /** The functions do that by means of a simple loop. */
    static const char* input_mem
    (
        const char* in,
        T* x,
        std::size_t n
    );
};


/// Collection of functions to do seft-delimeted binary serialization for raw objects.
/** Serialization is in the Simple Binary format.
    Type T is supposed to be a raw type, i.e. state of any variable of
    type T can be represented just as sequence of sizeof(T) bytes in memory
    behind this variable. T doesn't have non-trivial constructor.
    T can be such a type as int, double, a pointer, an array of int etc. */
template <typename T>
class io_binary_raw
{
public:

    /// Stores a raw object state to a binary stream. Seft-delimeted binary serialization.
    /** This function supposes T doesn't have a non trivial constructor/destructor pair.
        The format that this function uses is call The Simple Binary format. */
    template <typename Stream>
    static inline Stream& output_stream (Stream& out, const T& x)
    {
        out.write(reinterpret_cast<const char*>(&x), sizeof(T));
        return out;
    }


    /// Loads a raw object state from a binary stream. Compatible with output_stream.
    /** This function supposes T doesn't have a non trivial constructor/destructor pair.
        The format that this function uses is call The Simple Binary format. */
    template <typename Stream>
    static inline Stream& input_stream (Stream& in, T& x)
    {
        in.read(reinterpret_cast<char*>(&x), sizeof(T));
        return in;
    }


    /// Stores an array of a raw object states to a binary stream. Seft-delimeted binary serialization.
    /** This function supposes T doesn't have a non trivial constructor/destructor pair.
        The format that this function uses is call The Simple Binary format. */
    template <typename Stream>
    static inline Stream& output_stream
    (
        Stream& out,
        const T* x,
        std::size_t n
    )
    {
        if(n)
            out.write(reinterpret_cast<const char*>(x), n*sizeof(T));
        return out;
    }


    /// Loads an array of a raw object states from a binary stream. Compatible with output_stream.
    /** This function supposes T doesn't have a non trivial constructor/destructor pair.
        The format that this function uses is call The Simple Binary format. */
    template <typename Stream>
    static inline Stream& input_stream
    (
        Stream& in,
        T* x,
        std::size_t n
    )
    {
        if(n)
            in.read(reinterpret_cast<char*>(x), n*sizeof(T));
        return in;
    }


    /// Calculates the number of chars required to store a given object in The Simple Binary form.
    /** This function calculates precise number of chars that will emit
        any function outputs in The Simple Binary format for one object,
        for example, output_binary_mem function. */
    static inline std::size_t calc (const T& x)
    {
        return sizeof(T);
    }


    /// Calculates the number of chars required to store a given array of objects in The Simple Binary form.
    /** This function calculates precise number of chars that will emit
        any function outputs in The Simple Binary format for an array of
        objects, for example, output_binary_mem function. */
    static inline std::size_t calc (const T* x, std::size_t n)
    {
        return n*sizeof(T);
    }


    /// Stores a raw object state to a memory location. Seft-delimeted binary serialization.
    /** This function supposes T doesn't have a non trivial constructor/destructor pair.
        The format that this function uses is call The Simple Binary format. */
    static inline char* output_mem (char* out, const T& x)
    {
        return
            std::copy
            (
                reinterpret_cast<const char*>(&x),
                reinterpret_cast<const char*>(&x) + sizeof(T),
                out
            );
    }


    /// Loads a raw object state from a memory location. Compatible with output_stream.
    /** This function supposes T doesn't have a non trivial constructor/destructor pair.
        The format that this function uses is call The Simple Binary format. */
    static inline const char* input_mem (const char* in, T& x)
    {
        const char* in_end = in + sizeof(T);

        std::copy
        (
            in,
            in_end,
            reinterpret_cast<char*>(&x)
        );

        return in_end;
    }


    /// Stores an array of a raw object states to a memory location. Seft-delimeted binary serialization.
    /** This function supposes T doesn't have a non trivial constructor/destructor pair.
        The format that this function uses is call The Simple Binary format. */
    static inline char* output_mem
    (
        char* out,
        const T* x,
        std::size_t n
    )
    {
        return
            std::copy
            (
                reinterpret_cast<const char*>(x),
                reinterpret_cast<const char*>(x) + n*sizeof(T),
                out
            );
    }


    /// Loads an array of a raw object states from a memory location. Compatible with output_stream.
    /** This function supposes T doesn't have a non trivial constructor/destructor pair.
        The format that this function uses is call The Simple Binary format. */
    static inline const char* input_mem
    (
        const char* in,
        T* x,
        std::size_t n
    )
    {
        const char* in_end = in + n*sizeof(T);

        std::copy
        (
            in,
            in_end,
            reinterpret_cast<char*>(x)
        );

        return in_end;
    }
};


#define ARAGELI_IO_BINARY_DEFINE_RAW(T)    \
    template <>    \
    class io_binary<T> : public io_binary_raw<T>    \
    {};


ARAGELI_IO_BINARY_DEFINE_RAW(char)
ARAGELI_IO_BINARY_DEFINE_RAW(signed char)
ARAGELI_IO_BINARY_DEFINE_RAW(unsigned char)
ARAGELI_IO_BINARY_DEFINE_RAW(signed short)
ARAGELI_IO_BINARY_DEFINE_RAW(unsigned short)
ARAGELI_IO_BINARY_DEFINE_RAW(signed int)
ARAGELI_IO_BINARY_DEFINE_RAW(unsigned int)
ARAGELI_IO_BINARY_DEFINE_RAW(signed long)
ARAGELI_IO_BINARY_DEFINE_RAW(unsigned long)

#ifdef ARAGELI_INT64_SUPPORT

    ARAGELI_IO_BINARY_DEFINE_RAW(signed __int64)
    ARAGELI_IO_BINARY_DEFINE_RAW(unsigned __int64)

#endif

#ifdef ARAGELI_LONG_LONG_SUPPORT

    ARAGELI_IO_BINARY_DEFINE_RAW(signed long long)
    ARAGELI_IO_BINARY_DEFINE_RAW(unsigned long long)

#endif

ARAGELI_IO_BINARY_DEFINE_RAW(float)
ARAGELI_IO_BINARY_DEFINE_RAW(double)
ARAGELI_IO_BINARY_DEFINE_RAW(long double)


/// Binary serializer for a pointer; just a raw serialization.
template <typename T>
class io_binary<T*> : public io_binary_raw<T*>
{};


/// Avoids wrong automatic specialization of io_binary for arrays.
template <typename T, std::size_t N>
class io_binary<T[N]>;


/// Stores an object state to a binary stream. Seft-delimeted binary serialization.
/** Serialization in the Simple Binary format corresponding to type T. */
template <typename Stream, typename T>
inline Stream& output_binary_stream (Stream& out, const T& x)
{
    return io_binary<T>::output_stream(out, x);
}


template <typename Out, typename T>
inline Out& output_binary (Out& out, const T& x)
{
    return output_binary_stream(out, x);
}


/// Loads an object state from a binary stream. Compatible with output_binary_stream.
/** Serialization in the Simple Binary format corresponding to type T. */
template <typename Stream, typename T>
inline Stream& input_binary_stream (Stream& in, T& x)
{
    return io_binary<T>::input_stream(in, x);
}


template <typename In, typename T>
inline In& input_binary (In& in, T& x)
{
    return input_binary_stream(in, x);
}


/// Stores an array of object states to a binary stream. Seft-delimeted binary serialization.
/** Serialization in the Simple Binary format corresponding to type T. */
template <typename Stream, typename T>
inline Stream& output_binary_stream
(
    Stream& out,
    const T* x,
    std::size_t n
)
{
    return io_binary<T>::output_stream(out, x, n);
}


template <typename Out, typename T>
inline Out& output_binary
(
    Out& out,
    const T* x,
    std::size_t n
)
{
    return output_binary_stream(out, x, n);
}


/// Loads an array of object states from a binary stream. Compatible with output_binary_stream.
/** Serialization in the Simple Binary format corresponding to type T. */
template <typename Stream, typename T>
inline Stream& input_binary_stream
(
    Stream& in,
    T* x,
    std::size_t n
)
{
    return io_binary<T>::input_stream(in, x, n);
}


template <typename In, typename T>
inline In& input_binary
(
    In& in,
    T* x,
    std::size_t n
)
{
    return input_binary_stream(in, x, n);
}


/// Calculates the number of chars required to store a given object in The Simple Binary form.
/** Serialization in the Simple Binary format corresponding to type T. */
template <typename T>
inline std::size_t calc_binary (const T& x)
{
    return io_binary<T>::calc(x);
}

/// Calculates the number of chars required to store a given array of objects in The Simple Binary form.
/** Serialization in the Simple Binary format corresponding to type T. */
template <typename T>
inline std::size_t calc_binary (const T* x, std::size_t n)
{
    return io_binary<T>::calc(x, n);
}


/// Stores an object state to a memory location. Seft-delimeted binary serialization.
/** Serialization in the Simple Binary format corresponding to type T. */
template <typename T>
inline char* output_binary_mem (char* out, const T& x)
{
    return io_binary<T>::output_mem(out, x);
}


template <typename T>
inline char* output_binary (char* out, const T& x)
{
    return output_binary_mem(out, x);
}


/// Loads an object state from a memory location. Compatible with output_binary_stream.
/** Serialization in the Simple Binary format corresponding to type T. */
template <typename T>
inline const char* input_binary_mem (const char* in, T& x)
{
    return io_binary<T>::input_mem(in, x);
}


template <typename T>
inline const char* input_binary (const char* in, T& x)
{
    return input_binary_mem(in, x);
}


/// Stores an array of object states to a memory location. Seft-delimeted binary serialization.
/** Serialization in the Simple Binary format corresponding to type T. */
template <typename T>
inline char* output_binary_mem
(
    char* out,
    const T* x,
    std::size_t n
)
{
    return io_binary<T>::output_mem(out, x, n);
}


template <typename T>
inline char* output_binary
(
    char* out,
    const T* x,
    std::size_t n
)
{
    return output_binary_mem(out, x, n);
}


/// Loads an array of object states from a memory location. Compatible with output_stream.
/** Serialization in the Simple Binary format corresponding to type T. */
template <typename T>
inline const char* input_binary_mem
(
    const char* in,
    T* x,
    std::size_t n
)
{
    return io_binary<T>::input_mem(in, x, n);
}


template <typename T>
inline const char* input_binary
(
    const char* in,
    T* x,
    std::size_t n
)
{
    return input_binary_mem(in, x, n);
}


template <typename T>
inline char* io_binary_base<T>::output_mem (char* out, const T& x)
{
    memory_ostream ms(out);
    output_binary_stream(ms, x);
    return ms;
}


template <typename T>
inline const char* io_binary_base<T>::input_mem (const char* in, T& x)
{
    memory_istream ms(in);
    input_binary_stream(ms, x);
    return ms;
}



} // namespace Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_IO
    #include "io.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_IO
#endif


#endif  //  #ifndef _ARAGELI_io_hpp_
