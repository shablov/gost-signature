/*****************************************************************************

    test/vector.hpp

    This file is a part of the Arageli library test base.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    REFERENCE ADDITIONAL COPYRIGHTS HERE

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

// Author : Sidnev Alexey
// Tester :
// Creation date : 03.02.2006
// Modification date: 10.02.2006
// Testing date:
// Description : All tests of class vector.

#ifndef vectorH
#define vectorH

#include "stdafx.hpp"

using namespace Arageli;

//Проверка метода is_null()
template <typename type>
bool t_vector_is_null(Arageli::vector<type> *per)
{
    try
    {
        Arageli::vector<type> a=*per;//Копирование
        if(a!=(*per))
        {
            tout<<"Function t_vector_is_null failed on first check with args=("<<*per<<")\n";
            return true;
        }

        bool test_null1=a.is_null();
        bool test_null2=is_null(a);

        bool real_null=true;

        for(int i=0; i<a.size(); i++)
            if(a[i]!=0)
                real_null=false;

        if(test_null1 != real_null )
        {
            tout<<"Function t_vector_is_null failed on 2 check with args=("<<*per<<")\n";
            return true;
        }

        if(test_null2 != real_null )
        {
            tout<<"Function t_vector_is_null failed on 3 check with args=("<<*per<<")\n";
            return true;
        }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_is_empty(Arageli::vector<type> *per)
{
    try
    {
        Arageli::vector<type> a=*per;//Копирование
        if(a!=(*per))
        {
            tout<<"Function t_vector_is_empty failed on first check with args=("<<*per<<")\n";
            return true;
        }

        if(a.is_empty()==(bool)a.size() )
        {
            tout<<"Function t_vector_is_empty failed on 2 check with args=("<<*per<<")\n";
            return true;
        }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_size(int size)
{
    try
    {
        Arageli::vector<type> v(size);

        if(v.size()!=size )
        {
            tout<<"Function t_vector_size failed on 1 check with args=("<<size<<")\n";
            return true;
        }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

//Проверка +,-
template <typename type1, typename type2>
bool t_vector_plus_minus(Arageli::vector<type1> ta, Arageli::vector<type2> tb)
{
    try
    {
        Arageli::vector<type1> a=ta;
        Arageli::vector<type2> b=tb;

        int size=a.size();
        if(b.size()<size)
            size=b.size();

        for(int i=0; i<size; i++)
            if((a+b)[i]!=(a[i]+b[i]))
            {
                tout<<"Function t_rational_plus_minus failed on 1."<<i+1<<" check with args=("<<ta<<','<<tb<<")\n";
                return true;
            }


        if(a.size()>b.size())
            for(int i=size; i<a.size(); i++)
                if((a+b)[i]!=a[i])
                {
                    tout<<"Function t_rational_plus_minus failed on 2."<<i+1<<" check with args=("<<ta<<','<<tb<<")\n";
                    return true;
                }
        else
            for(int i=size; i<b.size(); i++)
                if((a+b)[i]!=b[i])
                {
                    tout<<"Function t_rational_plus_minus failed on 3."<<i+1<<" check with args=("<<ta<<','<<tb<<")\n";
                    return true;
                }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_zero_constructor()
{
    try
    {
        Arageli::vector<type> a;

        if(!is_null(a) )
        {
            tout<<"Function t_rational_zero_constructor failed on 1 check \n";
            return true;
        }

        if(a.size()!=0 )
        {
            tout<<"Function t_rational_zero_constructor failed on 2 check \n";
            return true;
        }

        return false;

    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_is_unit(Arageli::vector<type> a)
{
    try
    {
        bool isu=true;
        if(a.is_empty())
            isu=false;

        bool isou=isu;

        for(int i=0; i<a.size(); i++)
            if(!is_unit(a[i]))
                isu=false;

        for(int i=0; i<a.size(); i++)
            if(!is_opposite_unit(a[i]))
                isou=false;


        if( !a.is_unit()&&isu )
        {
            tout<<"Function t_vector_is_unit failed on 1 check with args=("<<a<<") \n";
            return true;
        }

        if(!a.is_opposite_unit()&&isou)
        {
            tout<<"Function t_vector_is_unit failed on 2 check with args=("<<a<<") \n";
            return true;
        }

        return false;

    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_bitwise_not(Arageli::vector<type> a)
{
    try
    {
        Arageli::vector<type> b=a;
        b.bitwise_not();

        bool work=true;
        for(int i=0; i<a.size(); i++)
            if(!is_null(a[i]&b[i]))
                work=false;

        if(!work)
        {
            tout
                << "Function t_vector_bitwise_not failed on 1 check with"
                << "\n\ta = " << a
                << "\n\tb = " << b
                << "\n\ta & b = " << (a & b)
                << "\n\ttype = " << typeid(type).name() << '\n';
            return true;
        }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_logical_not(Arageli::vector<type> a)
{
    try
    {
        Arageli::vector<type> b=a;
        b.logical_not();

        bool work=true;
        for(int i=0; i<a.size(); i++)
            if((bool)a[i]==(bool)b[i])
                work=false;

        if(!work)
        {
            tout<<"Function t_vector_logical_not failed on 1 check with args=("<<(a)<<")\n";
            return true;
        }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_resize(Arageli::vector<type> a, type value, int size)
{
    try
    {
        int old_size=a.size();
        Arageli::vector<type> b=a, c=b;
        b.resize(size);
        c.assign_fromval(size, value);

        if(b.size()!=size)
        {
            tout<<"Function t_vector_resize failed on 1 check with args=("<<a<<", "<<value<<", "<<size<<")\n";
            return true;
        }

        if(c.size()!=size)
        {
            tout<<"Function t_vector_resize failed on 2 check with args=("<<a<<", "<<value<<", "<<size<<")\n";
            return true;
        }

        bool work=true;
        for(int i=0; i<b.size(); i++)
            if(i<old_size)
            {
                if(a[i]!=b[i])
                {
                    work=false;
                    break;
                }
            }
            //else
            //    if(!is_null(b[i]))
            //    {
            //        work=false;
            //        break;
            //    }

        if(!work)
        {
            tout<<"Function t_vector_resize failed on 3 check with args=("<<a<<", "<<value<<", "<<size<<")\n";
            return true;
        }

        for(int i=0; i<c.size(); i++)
            {
                if(value!=c[i])
                {
                    work=false;
                    break;
                }
            }
            //else
                //if(c[i]!=value)
                //{
                //    work=false;
                //    break;
                //}

        if(!work)
        {
            tout
                << "Function t_vector_resize failed on 4 check with"
                << "\n\ta = " << a
                << "\n\tvalue = " << value
                << "\n\tsize = " << size
                << "\n\tc = " << c << '\n';
            return true;
        }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_scalar_plus_minus(Arageli::vector<type> a, type value)
{
    size_t size = a.size();

    try
    {
        Arageli::vector<type> b = a;
        if((b.assign_plus_val(value)).assign_minus_val(value) != a)
        {
            tout
                << "Function t_vector_scalar_plus_minus failed on 1 check with"
                << "\n\ta = " << a
                << "\n\tb = " << b
                << "\n\tsize = " << size
                << "\n\tvalue = " << value
                << "\n\ttype = " << typeid(type).name() << '\n';
            return true;
        }

        b = a;
        if((b.assign_minus_val(value)).assign_plus_val(value) != a)
        {
            tout
                << "Function t_vector_scalar_plus_minus failed on 2 check with"
                << "\n\ta = " << a
                << "\n\tb = " << b
                << "\n\tsize = " << size
                << "\n\tvalue = " << value
                << "\n\ttype = " << typeid(type).name() << '\n';
            return true;
        }

        b = a;
        if(((b += value) -= value) != a)
        {
            tout
                << "Function t_vector_scalar_plus_minus failed on 3 check with"
                << "\n\ta = " << a
                << "\n\tb = " << b
                << "\n\tsize = " << size
                << "\n\tvalue = " << value
                << "\n\ttype = " << typeid(type).name() << '\n';
            return true;
        }

        b = a;
        if(is_null(value))
        {
            if(!is_null(b.assign_multiplies_val(value)))
            {
                tout
                    << "Function t_vector_scalar_plus_minus failed on 4 check with"
                    << "\n\ta = " << a
                    << "\n\tb = " << b
                    << "\n\tsize = " << size
                    << "\n\tvalue = " << value
                    << "\n\ttype = " << typeid(type).name() << '\n';
                return true;
            }

            b = a;
            if(!is_null(b*value))
            {
                tout
                    << "Function t_vector_scalar_plus_minus failed on 5 check with"
                    << "\n\ta = " << a
                    << "\n\tb = " << b
                    << "\n\tsize = " << size
                    << "\n\tvalue = " << value
                    << "\n\ttype = " << typeid(type).name() << '\n';
                return true;
            }
        }

        b=a;
        Arageli::vector<bool> vb(b.size());
        vb = b.logical_not();

        bool work=true;
        for(int i=0; i<b.size(); i++)
            if(bool(vb[i]) != !a[i]) // bool is needed here because the operator!= maybe isn't defined for std::vector<bool>::reference (See ISO C++ 23.2.5) and Arageli::vector<bool> probably incapsulates std::vector<bool>.
            {
                work=false;
                break;
            }

        if(!work)
        {
            tout
                << "Function t_vector_scalar_plus_minus failed on 6 check with"
                << "\n\ta = " << a
                << "\n\tb = " << b
                << "\n\tsize = " << size
                << "\n\tvalue = " << value
                << "\n\ttype = " << typeid(type).name() << '\n';
            return true;
        }


        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_shlr(Arageli::vector<type> a, type value)
{
    try
    {
        Arageli::vector<type> b = a, c = a;

        if((b.assign_shift_left_val(value)).assign_shift_right_val(value) != a)
        {
            tout
                << "Function t_vector_shlr failed on 1 check with"
                << "\n\ta = " << a
                << "\n\tb = " << b
                << "\n\tc = " << c
                << "\n\tvalue = " << value
                << "\n\ttype = " << typeid(type).name() << '\n';
            return true;
        }

        b = a;

        if(((b << value) >> value) != b)
        {
            tout
                << "Function t_vector_shlr failed on 2 check with"
                << "\n\ta = " << a
                << "\n\tb = " << b
                << "\n\tc = " << c
                << "\n\tvalue = " << value
                << "\n\ttype = " << typeid(type).name() << '\n';
            return true;
        }

        b=a;
        b.assign_shift_left_val(value);
        for(int i=0; i<value; i++)
            c*=2;

        if(c!=b)
        {
            tout
                << "Function t_vector_shlr failed on 3 check with"
                << "\n\ta = " << a
                << "\n\tb = " << b
                << "\n\tc = " << c
                << "\n\tvalue = " << value
                << "\n\ttype = " << typeid(type).name() << '\n';
            return true;
        }

        b=a;c=a;
        b.assign_shift_right_val(value);
        for(int i=0; i<value; i++)
            c/=2;

        if(c!=b)
        {
            tout
                << "Function t_vector_shlr failed on 4 check with"
                << "\n\ta = " << a
                << "\n\tb = " << b
                << "\n\tc = " << c
                << "\n\tvalue = " << value
                << "\n\ttype = " << typeid(type).name() << '\n';
            return true;
        }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_scalar_mul_div(Arageli::vector<type> a, type value)
{
    try
    {
        Arageli::vector<type> b=a;

        if(is_null(value))
        {
            if(!is_null(b.assign_multiplies_val(value)))
            {
                tout<<"Function t_vector_scalar_mul_div failed on 1 check with args=("<<a<<", "<<value<<")\n";
                return true;
            }
        }
        else
        {
            b=a;
            if((b.assign_multiplies_val(value)).assign_divides_val(value)!=b)
            {
                tout<<"Function t_vector_scalar_mul_div failed on 2 check with args=("<<a<<", "<<value<<")\n";
                return true;
            }

            b=a;
            bool work=true;
            b.assign_modulus_val(value);
            for(int i=0; i<b.size(); i++)
                if(b[i]!=a[i]%value)
                {
                    work=false;
                    break;
                }

            if(!work)
            {
                tout<<"Function t_vector_scalar_mul_div failed on 3 check with args=("<<a<<", "<<value<<")\n";
                return true;
            }
        }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_access(Arageli::vector<type> a)
{
    try
    {
        Arageli::vector<type> b=a;

        bool work=true;
        for(int i=0; i< b.size(); i++)
            if((b[i]!=b.el(i))||(b[i]!=b(i))||(b[i]!=b.at(i)))
            {
                work=false;
                break;
            }

        if(!work)
        {
            tout<<"Function t_vector_access failed on 1 check with args=("<<a<<")\n";
            return true;
        }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_set_get(Arageli::vector<type> a, type value)
{
    try
    {
        Arageli::vector<type> b=a;

        b.insert(b.size(), value);
        if(b[b.size()-1]!=value)
        {
            tout<<"Function t_vector_set_get failed on 1 check with args=("<<a<<", "<<value<<")\n";
            return true;
        }

        b.insert(b.size(), value+1);
        b.erase(b.size()-2);
        if(b[b.size()-1]!=value+1)
        {
            tout<<"Function t_vector_set_get failed on 2 check with args=("<<a<<", "<<value<<")\n";
            return true;
        }

        b=a;
        b.push_front(value);
        if(b[0]!=value)
        {
            tout<<"Function t_vector_set_get failed on 3 check with args=("<<a<<", "<<value<<")\n";
            return true;
        }

        b.push_back(value+1);
        if(b[b.size()-1]!=value+1)
        {
            tout<<"Function t_vector_set_get failed on 4 check with args=("<<a<<", "<<value<<")\n";
            return true;
        }

        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_vector_swap(Arageli::vector<type> a, int first, int second)
{
    try
    {
        Arageli::vector<type> b=a;

        b.swap_els(first, second);

        if((b[first]!=a[second])||(a[first]!=b[second]) )
        {
            tout<<"Function t_vector_swap failed on 1 check \n";
            return true;
        }

        b=a;
        b.swap_els(second, first);
        if((b[first]!=a[second])||(a[first]!=b[second]) )
        {
            tout<<"Function t_vector_swap failed on 2 check \n";
            return true;
        }

        return false;

    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

#endif
