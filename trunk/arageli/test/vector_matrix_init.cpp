/*****************************************************************************

    test/vector_matrix_init.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
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

#include "stdafx.hpp"


using namespace Arageli;
using Arageli::vector;


template <typename T>
void out_info_about
(std::ostream& out, const vector<T>& x, const std::string& tab = "")
{
    out
        << tab << "type = " << typeid(vector<T>).name() << '\n'
        << tab << "object = " << x << '\n'
        << tab << "size = " << x.size() << '\n'
        << tab << "capacity = " << x.capacity() << '\n'
        << tab << "is_null = " << x.is_null() << '\n';
}


template <typename T>
void out_info_about
(std::ostream& out, const matrix<T>& x, const std::string& tab = "")
{
    out
        << tab << "type = " << typeid(matrix<T>).name() << '\n'
        << tab << "object = " << x << '\n'
        << tab << "size = " << x.size() << '\n'
        << tab << "nrows = " << x.size() << '\n'
        << tab << "ncols = " << x.size() << '\n'
        << tab << "capacity = " << x.capacity() << '\n'
        << tab << "is_null = " << x.is_null() << '\n';
}


template <typename T>
void out_not_equal_fail
(
    std::ostream& out,
    const std::string& constructor,
    const T& constructed,
    const T& pattern,
    int line_no,
    const std::string& tab = ""
)
{
    out
        << tab << constructor << " is failed (line # "<< line_no << ").\n"
        << tab << "Pattern object:\n";
    out_info_about(out, pattern, tab + "    ");

    out << tab << "Constructed object:\n";
    out_info_about(out, constructed, tab + "    ");
}


template <typename T>
bool constructors ()
{
    try
    {
        bool fail = false;


        {
            vector<T> a;
            vector<T> b = "()";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Default constructor", a, b, __LINE__);
        }

        {
            matrix<T> a;
            matrix<T> b = "()";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Default constructor", a, b, __LINE__);
        }


        {
            vector<T> a(1);
            vector<T> b = "(0)";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with one argument", a, b, __LINE__);
        }

        {
            matrix<T> a(1);
            matrix<T> b = "((0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with one argument", a, b, __LINE__);
        }


        {
            vector<T> a(2);
            vector<T> b = "(0, 0)";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with one argument", a, b, __LINE__);
        }

        {
            matrix<T> a(2);
            matrix<T> b = "((0, 0), (0, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with one argument", a, b, __LINE__);
        }


        {
            matrix<T> a(2, 1);
            matrix<T> b = "((0), (0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with one argument", a, b, __LINE__);
        }


        {
            matrix<T> a(1, 2);
            matrix<T> b = "((0, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with one argument", a, b, __LINE__);
        }


        {
            matrix<T> a(2, 1, fromval);
            matrix<T> b = "((1, 1), (1, 1))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with one argument", a, b, __LINE__);
        }

        {
            matrix<T> a(3, 2, fromsize);
            matrix<T> b = "((0, 0), (0, 0), (0, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with one argument", a, b, __LINE__);
        }


        {
            vector<T> a(1, 1);
            vector<T> b = "(1)";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(1, 1, fromval);
            matrix<T> b = "((1))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            vector<T> a(2, 3);
            vector<T> b = "(3, 3)";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, 3, fromval);
            matrix<T> b = "((3, 3), (3, 3))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            vector<T> a(1, 1);
            vector<T> b = "(1)";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(1, 1, fromval);
            matrix<T> b = "((1))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            vector<T> a(2, 3);
            vector<T> b = "(3, 3)";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, 3, fromval);
            matrix<T> b = "((3, 3), (3, 3))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        const int valsx[] = {10, 11, 12, 13, 14, 15, 16};
        const int* vals = valsx;

        {
            vector<T> a(1, vals);
            vector<T> b = "(10)";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(1, vals);
            matrix<T> b = "((10))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            vector<T> a(2, vals);
            vector<T> b = "(10, 11)";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, vals);
            matrix<T> b = "((10, 11), (12, 13))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            vector<T> a(1, vals, fromseq);
            vector<T> b = "(10)";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(1, vals, fromseq);
            matrix<T> b = "((10))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            vector<T> a(2, vals, fromseq);
            vector<T> b = "(10, 11)";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, vals, fromseq);
            matrix<T> b = "((10, 11), (12, 13))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        ///////////


        {
            matrix<T> a(1, 2, 1);
            matrix<T> b = "((1, 1))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(1, 2, 1, fromval);
            matrix<T> b = "((1, 1))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(1, 2, vals);
            matrix<T> b = "((10, 11))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        {
            matrix<T> a(2, 2, vals);
            matrix<T> b = "((10, 11), (12, 13))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(1, 2, vals, fromseq);
            matrix<T> b = "((10, 11))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, 2, vals, fromseq);
            matrix<T> b = "((10, 11), (12, 13))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        ///////////


        //{
        //    matrix<T> a(2, vals, colwise);
        //    matrix<T> b = "((10, 12), (11, 13))";
        //    if(a != b)
        //        fail = true,
        //        out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        //}

        //{
        //    matrix<T> a(2, 3, vals, colwise);
        //    matrix<T> b = "((10, 12, 14), (11, 13, 15))";
        //    if(a != b)
        //        fail = true,
        //        out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        //}

        ///////////

        {
            matrix<T> a(2, 3, diag);
            matrix<T> b = "((3, 0), (0, 3))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, 1, 3, diag);
            matrix<T> b = "((3), (0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        {
            matrix<T> a(1, 2, 3, diag);
            matrix<T> b = "((3, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        {
            matrix<T> a(2, 3, diag, fromval);
            matrix<T> b = "((3, 0), (0, 3))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, 1, 3, diag, fromval);
            matrix<T> b = "((3), (0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        {
            matrix<T> a(1, 2, 3, diag, fromval);
            matrix<T> b = "((3, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        {
            matrix<T> a(2, vals, diag);
            matrix<T> b = "((10, 0), (0, 11))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, 3, vals, diag);
            matrix<T> b = "((10, 0, 0), (0, 11, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        {
            matrix<T> a(3, 2, vals, diag);
            matrix<T> b = "((10, 0), (0, 11), (0, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        {
            matrix<T> a(2, vals, diag, fromseq);
            matrix<T> b = "((10, 0), (0, 11))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, 3, vals, diag, fromseq);
            matrix<T> b = "((10, 0, 0), (0, 11, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        {
            matrix<T> a(3, 2, vals, diag, fromseq);
            matrix<T> b = "((10, 0), (0, 11), (0, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, eye);
            matrix<T> b = "((1, 0), (0, 1))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }

        {
            matrix<T> a(2, 3, eye);
            matrix<T> b = "((1, 0, 0), (0, 1, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        {
            matrix<T> a(3, 2, eye);
            matrix<T> b = "((1, 0), (0, 1), (0, 0))";
            if(a != b)
                fail = true,
                out_not_equal_fail(tout, "Constructor with two arguments", a, b, __LINE__);
        }


        return fail;
    }
    catch(const Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename F>
class gen_as_iter
{
    F f;
public:
    gen_as_iter (const F& f_a) : f(f_a) {}
    typename F::result_type operator* () const { return f(); }
};

TEST_FUNCTION
(
    vector_matrix_init,
    "Test initializer and assign functins for vector and matrix."
)
{
    bool fail = false;

    fail |= constructors<size_t>();
    fail |= constructors<int>();
    //fail |= constructors<char>();
    //fail |= constructors<unsigned char>();
    fail |= constructors<short>();
    fail |= constructors<float>();
    fail |= constructors<double>();
    fail |= constructors<rational<int> >();
    fail |= constructors<big_int>();
    fail |= constructors<rational<big_int> >();
    //fail |= constructors<sparse_polynom<int> >();
    //fail |= constructors<sparse_polynom<rational<int> > >();
    //fail |= constructors<sparse_polynom<big_int> >();
    //fail |= constructors<sparse_polynom<rational<big_int> > >();


    if(fail)return resFAIL;
    else return resOK;
}
