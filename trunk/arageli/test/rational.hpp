/*****************************************************************************

    test/rational.hpp

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
// Creation date : 20.01.2006
// Modification date: 01.02.2006
// Testing date:
// Description : All tests of class rational.

#ifndef rationalH
#define rationalH

#include "stdafx.hpp"

using namespace Arageli;

//Проверка метода is_null() и оператора приведения типов bool()
template <typename type>
bool t_rational_is_null(rational<type> *per)
{
    try
    {
        rational<type> A=*per;//Копирование
        if(A!=(*per))
        {
            tout<<"Function t_rational_is_null failed on first check with args=("<<*per<<")\n";
            return true;
        }

        bool temp_null1=A.is_null();
        bool temp_null2=!(bool)A;

        bool real_null=!A.numerator();

        if(temp_null1 != real_null )
        {
            tout<<"Function t_rational_is_null failed on 2 check with args=("<<*per<<")\n";
            return true;
        }

        if(temp_null2 != real_null )
        {
            tout<<"Function t_rational_is_null failed on 3 check with args=("<<*per<<")\n";
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
bool t_rational_plus_minus(rational<type1> ta, rational<type2> tb)
{
    try
    {
        rational<type1> a=ta;
        rational<type2> b=tb;
        if((a+b).numerator()*a.denominator()*b.denominator()!=
            (a+b).denominator()*(a.denominator()*b.numerator()+a.numerator()*b.denominator()) )
        {
            tout<<"Function t_rational_plus_minus failed on 1 check with args=("<<ta<<','<<tb<<")\n";
            return true;
        }

        if((b+a).numerator()*b.denominator()*a.denominator()!=
            (b+a).denominator()*(b.denominator()*a.numerator()+b.numerator()*a.denominator()) )
        {
            tout<<"Function t_rational_plus_minus failed on 2 check with args=("<<ta<<','<<tb<<")\n";
            return true;
        }

        if((a-b).numerator()*a.denominator()*b.denominator()!=
            (a-b).denominator()*(-a.denominator()*b.numerator()+a.numerator()*b.denominator()) )
        {
            tout<<"Function t_rational_plus_minus failed on 3 check with args=("<<ta<<','<<tb<<")\n";
            return true;
        }

        if((b-a).numerator()*b.denominator()*a.denominator()!=
            (b-a).denominator()*(-b.denominator()*a.numerator()+b.numerator()*a.denominator()) )
        {
            tout<<"Function t_rational_plus_minus failed on 4 check with args=("<<ta<<','<<tb<<")\n";
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

//Проверка метода is_normal()
template <typename type>
bool t_rational_is_normal(rational<type> a)
{
    try
    {
        type numerator=a.numerator(), denominator=a.denominator();
        bool norm=true;

        type numeratorTem=numerator, denominatorTem=denominator;
        for(int i=2;i<numeratorTem+1;i++)
        {
            if(!(numeratorTem%i))
                if(!(denominatorTem%i))
                {
                    numeratorTem/=i;
                    denominatorTem/=i;
                    i=1;
                    norm=false;
                    break;
                }
        }

        if(norm == a.is_normal() ) return false;
        else
        {
            tout<<"Function t_rational_is_normal failed on 1 check with args=("<<numerator<<','<<denominator<<")\n";
            return true;
        }

    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}

template <typename type>
bool t_rational_numerator_denominator(type numerator, type denominator)
{
    try
    {
        rational<type> a(numerator,denominator),b;

        type numeratorTem=numerator, denominatorTem=denominator;
        for(int i=2; i<(numeratorTem)+1;i++)
        {
            if(!( ((bool)(denominatorTem%i)) || ((bool)(numeratorTem%i)) ))
            {
                denominatorTem=denominatorTem/i;
                numeratorTem=numeratorTem/i;
                i=1;
            }
        }

        if(numeratorTem==0)
            if(denominatorTem!=0)
                denominatorTem=1;

        if(denominatorTem==0)
            if(numeratorTem!=0)
                numeratorTem=1;

        if(a.numerator() != numeratorTem )
        {
            tout<<"Function t_rational_numer_denom failed on 1 check with args=("<<numerator<<','<<denominator<<")\n";
            return true;
        }

        if(a.denominator() != denominatorTem )
        {
            tout<<"Function t_rational_numer_denom failed on 2 check with args=("<<numerator<<','<<denominator<<")\n";
            return true;
        }

        a.numerator()+=a.denominator();
        if(a.numerator() != numeratorTem+denominatorTem)
        {
            tout<<"Function t_rational_numer_denom failed on 3 check with args=("<<numerator<<','<<denominator<<")\n";
            return true;
        }

        a.denominator()+=a.numerator();
        if(a.denominator() != numeratorTem+denominatorTem+denominatorTem)
        {
            tout<<"Function t_rational_numer_denom failed on 4 check with args=("<<numerator<<','<<denominator<<")\n";
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
bool t_rational_zero_constructor()
{
    try
    {
        rational<type> a;

        if(a != type(0) )
        {
            tout<<"Function t_rational_zero_constructor failed on 1 check \n";
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

//Проверяются конструкторы и операторы приведения типов
template <typename type>
bool t_rational_constructor(type *a, type *b)
{
    try
    {
        rational<type> c(*a), d(*a, *b), e(*b), f;
        f=*a;

        if(*a != (type)c )
        {
            tout<<"Function t_rational_constructor failed on 1 check with args=("<<*a<<','<<*b<<") \n";
            return true;
        }

        if(*b != (type)e )
        {
            tout<<"Function t_rational_constructor failed on 2 check with args=("<<*a<<','<<*b<<") \n";
            return true;
        }

        if(f != c )
        {
            tout<<"Function t_rational_constructor failed on 3 check with args=("<<*a<<','<<*b<<") \n";
            return true;
        }

        if( (*a)/(*b) != (type)d )
        {
            tout<<"Function t_rational_constructor failed on 4 check with args=("<<*a<<','<<*b<<") \n";
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
bool t_rational_operator_double_bool(rational<type> a)
{
    try
    {
        double manually = (double)a.numerator()/(double)a.denominator();
        double orig = (double)a;
        double delta = std::abs(orig - manually);

        // Really epsilon can depend on how near numerator and denominator
        // to double's max absolute value.
        double epsilon =
            2*std::max(std::abs(orig), std::abs(manually))*
                std::numeric_limits<double>::epsilon();

        if(delta > epsilon)
        {
            tout
                << "Function t_rational_operator_double_bool failed on 1 check with: "
                << "\n\ttype = " << typeid(type).name()
                << "\n\ta = " << a
                << "\n\tmanually = " << manually
                << "\n\torig = " << orig
                << "\n\tdelta = " << delta
                << "\n\tepsilon = " << epsilon
                << "\n\tdelta - epsilon = " << delta - epsilon
                << "\n";
            return true;
        }

        if( (bool)a.numerator() != (bool)a ) //Считаем, что знаменатель !=0
        {
            tout<<"Function t_rational_operator_double_bool failed on 2 check with args=("<<a<<") \n";
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
bool t_rational_is_integer(rational<type> a)
{
    try
    {
        if(a.is_integer()==bool(a.numerator()%a.denominator()))
        {
            tout<<"Function t_rational_is_integer failed on 1 check with args=("<<a<<") \n";
            return true;
        }

        a.normalize();

        if(!a.is_integer()&&( a.is_unit()||a.is_opposite_unit() ) )
        {
            tout<<"Function t_rational_is_integer failed on 2 check with args=("<<a<<") \n";
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
bool t_rational_is_unit(rational<type> a)
{
    try
    {
        if(!a.is_unit()&&a.is_integer()&&(a.numerator()/a.denominator()==(type)1))
        {
            tout<<"Function t_rational_is_unit failed on 1 check with args=("<<a<<") \n";
            return true;
        }

        if(!a.is_opposite_unit()&&a.is_integer()&&(a.numerator()/a.denominator()==(type)(-1) ))
        {
            tout<<"Function t_rational_is_unit failed on 2 check with args=("<<a<<") \n";
            return true;
        }

        a.normalize();

        if(!a.is_unit()&&(a.numerator()==a.denominator()==(type)1))
        {
            tout<<"Function t_rational_is_unit failed on 3 check with args=("<<a<<") \n";
            return true;
        }

        if(!a.is_opposite_unit()&&(a.numerator()==a.denominator()*(type)(-1) )&&(a.denominator()==(type)1))
        {
            tout<<"Function t_rational_is_unit failed on 4 check with args=("<<a<<") \n";
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
bool t_rational_swap(rational<type> a, rational<type> b)
{
    try
    {
        rational<type> c=a, d=b;

        c.swap(d);


        if(c!=b)
        {
            tout<<"Function t_rational_swap failed on 1 check with args=("<<a<<','<<b<<") \n";
            return true;
        }

        if(d!=a)
        {
            tout<<"Function t_rational_swap failed on 2 check with args=("<<a<<','<<b<<") \n";
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

template <typename type1, typename type2>
bool t_rational_copy(rational<type1> a, type2 b)
{
    try
    {
        rational<type1> c=b, d=a;
        d=b;

        if((type2)c!=b)
        {
            tout<<"Function t_rational_copy failed on first check with args=("<<a<<','<<b<<")\n";
            return true;
        }

        if((type2)d!=b)
        {
            tout<<"Function t_rational_copy failed on 2 check with args=("<<a<<','<<b<<")\n";
            return true;
        }

        if(c!=d)
        {
            tout<<"Function t_rational_copy failed on 3 check with args=("<<a<<','<<b<<")\n";
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
bool t_rational_mul_div(rational<type1> ta, rational<type2> tb)
{
    try
    {
        rational<type1> a=ta;
        rational<type2> b=tb;
        if((a*b).numerator()*a.denominator()*b.denominator()!=
            (a*b).denominator()*a.numerator()*b.numerator() )
        {
            tout<<"Function t_rational_plus_minus failed on 1 check with args=("<<ta<<','<<tb<<")\n";
            return true;
        }

        if((b*a).numerator()*b.denominator()*a.denominator()!=
            (b*a).denominator()*b.numerator()*a.numerator() )
        {
            tout<<"Function t_rational_plus_minus failed on 2 check with args=("<<ta<<','<<tb<<")\n";
            return true;
        }

        if(!is_null(b))
        {
            if((a/b).numerator()*a.denominator()*b.numerator()!=
                (a/b).denominator()*a.numerator()*b.denominator() )
            {
                tout<<"Function t_rational_plus_minus failed on 3 check with args=("<<ta<<','<<tb<<")\n";
                return true;
            }

            if(a%b>=b)
            {
                tout<<"Function t_rational_plus_minus failed on 4 check with args=("<<ta<<','<<tb<<")\n";
                return true;
            }

            if(a/b+a%b!=a/b)
            {
                tout<<"Function t_rational_plus_minus failed on 5 check with args=("<<ta<<','<<tb<<")\n";
                return true;
            }

            if( (a/b-a%b)%b!=0)
            {
                tout<<"Function t_rational_plus_minus failed on 6 check with args=("<<ta<<','<<tb<<")\n";
                return true;
            }
        }

        if(!is_null(a))
        {
            if((b/a).numerator()*b.denominator()*a.numerator()!=
                (b/a).denominator()*b.numerator()*a.denominator() )
            {
                tout<<"Function t_rational_plus_minus failed on 7 check with args=("<<ta<<','<<tb<<")\n";
                return true;
            }

            if(b%a>=a)
            {
                tout<<"Function t_rational_plus_minus failed on 8 check with args=("<<ta<<','<<tb<<")\n";
                return true;
            }

            if(b/a+b%a!=b/a)
            {
                tout<<"Function t_rational_plus_minus failed on 9 check with args=("<<ta<<','<<tb<<")\n";
                return true;
            }

            if( (b/a-b%a)%a!=0)
            {
                tout<<"Function t_rational_plus_minus failed on 10 check with args=("<<ta<<','<<tb<<")\n";
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
bool t_rational_ifloor_iceil_frac_abs(rational<type> ta)
{
    try
    {
        rational<type> a=ta;

        if((a-ifloor(a)>=unit(a))||(a-ifloor(a)<0))
        {
            tout<<"Function t_rational_ifloor_iceil_frac failed on first check with args=("<<ta<<")\n";
            return true;
        }

        if((iceil(a)-a>=unit(a))||(iceil(a)-a<0))
        {
            tout<<"Function t_rational_ifloor_iceil_frac failed on 2 check with args=("<<ta<<")\n";
            return true;
        }

        if(frac(a)!=a-ifloor(a))
        {
            tout<<"Function t_rational_ifloor_iceil_frac failed on 3 check with args=("<<ta<<")\n";
            return true;
        }

        if(Arageli::abs(a)!=a.sign()*a)
        {
            tout<<"Function t_rational_ifloor_iceil_frac failed on 4 check with args=("<<ta<<")\n";
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
