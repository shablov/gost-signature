/*****************************************************************************

    test/universal.hpp

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
// Description : Tests without class specification.

#ifndef universalH
#define universalH

#include "stdafx.hpp"

using namespace Arageli;

//Проверка оператора присваивания и конструктора копирования
//Выполняется копирование *tb=*ta
template <typename type>
bool t_copy(type *ta, type *tb)
{
    try
    {
        type arg1=*ta, arg2=*tb,
            tem1=*ta, //Конструктор копирования
            tem2;
        tem2=*ta;//Оператор присваивания

        *tb=*ta;//Копирование
        if((*ta)!=(*tb))
        {
            tout<<"Function t_copy failed on first check with args=("<<arg1<<','<<arg2<<")\n";
            return true;
        }

        if(tem1!=*tb)
        {
            tout<<"Function t_copy failed on second check with args=("<<arg1<<','<<arg2<<")\n";
            return true;
        }

        if(tem2!=*tb)
        {
            tout<<"Function t_copy failed on third check with args=("<<arg1<<','<<arg2<<")\n";
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

//Проверка операторов сравнения
template <typename type>
bool t_compares(type *ta, type *tb)
{
    try
    {
        type arg1=*ta, arg2=*tb;
        bool
            bigger_ab, less_ab, boe_ab, loe_ab,
            equal, not_equal,
            bigger_ba, less_ba, boe_ba, loe_ba;

        int cmp_rez;

        bigger_ab=(*ta)>(*tb) ,
            less_ab=(*ta)<(*tb),
            boe_ab=(*ta)>=(*tb),
            loe_ab=(*ta)<=(*tb);
        equal=*ta==*tb, not_equal=*ta!=*tb;
        bigger_ba=*tb>*ta, less_ba=*tb<*ta, boe_ba=*tb>=*ta, loe_ba=*tb<=*ta;

        cmp_rez=cmp(*ta, *tb);
        int cmp_rez_inv = cmp(*tb, *ta);

        if(cmp_rez != -cmp_rez_inv)
        {
            tout
                << "Function t_compares failed on 0 check with:"
                << "\n\targ1 = " << arg1
                << "\n\targ2 = " << arg2
                << "\n\tless_ab = " << less_ab
                << "\n\tbigger_ba = " << bigger_ba
                << "\n\tloe_ab = " << loe_ab
                << "\n\tboe_ba = " << boe_ba
                << "\n\tcmp_rez = " << cmp_rez
                << "\n\tcmp_rez_inv = " << cmp_rez_inv
                << "\n\t";
        }

        if(equal)
        {
            if(bigger_ab || less_ab || bigger_ba || less_ba)
            {
                tout<<"Function t_compares failed on 1 check with args=("<<(arg1)<<','<<(arg2)<<")\n";
                return true;
            }

            if(!(boe_ab && loe_ab && boe_ba && loe_ba))
            {
                tout<<"Function t_compares failed on 2 check with args=("<<arg1<<','<<arg2<<")\n";
                return true;
            }

            if(cmp_rez||not_equal)
            {
                tout<<"Function t_compares failed on 3 check with args=("<<arg1<<','<<arg2<<")\n";
                return true;
            }
        }
        else
        {
            if(!not_equal)
            {
                tout<<"Function t_compares failed on 4 check with args=("<<arg1<<','<<arg2<<")\n";
                return true;
            }

            if(bigger_ab)
            {
                if(less_ab || bigger_ba || loe_ab || boe_ba)
                {
                    tout
                        << "Function t_compares failed on 5 check with:"
                        << "\n\targ1 = " << arg1
                        << "\n\targ2 = " << arg2
                        << "\n\tless_ab = " << less_ab
                        << "\n\tbigger_ba = " << bigger_ba
                        << "\n\tloe_ab = " << loe_ab
                        << "\n\tboe_ba = " << boe_ba
                        << "\n";

                    return true;
                }

                if(!(less_ba && boe_ab && loe_ba))
                {
                    tout<<"Function t_compares failed on 6 check with args=("<<arg1<<','<<arg2<<")\n";
                    return true;
                }

                if(cmp_rez!=1)
                {
                    tout<<"Function t_compares failed on 7 check with args=("<<arg1<<','<<arg2<<")\n";
                    return true;
                }
            }
            else
            {
                if(!(less_ab && bigger_ba && loe_ab && boe_ba))
                {
                    tout<<"Function t_compares failed on 8 check with args=("<<arg1<<','<<arg2<<")\n";
                    return true;
                }

                if(less_ba || boe_ab || loe_ba)
                {
                    tout<<"Function t_compares failed on 9 check with args=("<<arg1<<','<<arg2<<")\n";
                    return true;
                }

                if(cmp_rez!=-1)
                {
                    tout<<"Function t_compares failed on 10 check with args=("<<arg1<<','<<arg2<<")\n";
                    return true;
                }
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

//Проверка +, -
template <typename type1, typename type2>
bool t_plus_minus( type1 ta, type2 tb)
{
    try
    {
        type1 a=ta;
        type2 b=tb;

        if(a!=(ta))
        {
            tout<<"Function t_plus_minus failed on 1 check with args=("<<(ta)<<','<<(tb)<<")\n";
            return true;
        }

        if(b!=(tb))
        {
            tout<<"Function t_plus_minus failed on 2 check with args=("<<ta<<','<<tb<<")\n";
            return true;
        }

        if(b+a!=a+b)
        {
            tout<<"Function t_plus_minus failed on 5 check with args=("<<ta<<','<<tb<<")\n";
            return true;
        }

        if(-b+a!=a-b)
        {
            tout<<"Function t_plus_minus failed on 6 check with args=("<<ta<<','<<tb<<")\n";
            return true;
        }

        if( (((a-b)+b)!=a)||(((b-a)+a)!=b) )
        {
            tout<<"Function t_plus_minus failed on 8 check with args=("<<ta<<','<<tb<<")\n";
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

//Проверка конструктора копирования и оператора присваивания
template <typename type>
bool t_copy_constructor(type *a)
{
    try
    {
        type b=(*a), c;
        c=*a;

        if(b!=(*a))
        {
            tout<<"Function t_copy_constructor failed on 1 check with args=("<<(*a)<<")\n";
            return true;
        }

        if(c!=(*a))
        {
            tout<<"Function t_copy_constructor failed on 2 check with args=("<<(*a)<<")\n";
            return true;
        }

        if(c != b)
        {
            tout<<"Function t_copy_constructor failed on 3 check with args=("<<(*a)<<")\n";
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
bool t_opposite(type a)
{
    try
    {
        type b=a;
        b.opposite();
        if(!is_null(a+b))
        {
            tout<<"Function t_opposite failed on 1 check with args=("<<(a)<<")\n";
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
bool t_inverse(type a)
{
    try
    {
        type b=a;
        b.inverse();
        type c=a*b;
        if(!is_unit(c))
        {
            tout
                << "Function t_inverse failed on 1 check with"
                << "\n\ta = " << a
                << "\n\tb = " << b
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
bool t_sign(type a)
{
    try
    {
        if(a*a.sign()<0)
        {
            tout<<"Function t_sign failed on 1 check with args=("<<(a)<<")\n";
            return true;
        }

        if((a.sign()==0)&&(a!=0))
        {
            tout<<"Function t_sign failed on 2 check with args=("<<(a)<<")\n";
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


//Проверка *, /
template <typename type1, typename type2>
bool t_mul_div( type1 ta, type2 tb)
{
    try
    {
        type1 a=ta;
        type2 b=tb;

        if(a!=(ta))
        {
            tout<<"Function t_mul_div failed on 1 check with args=("<<(ta)<<','<<(tb)<<")\n";
            return true;
        }

        if(b!=(tb))
        {
            tout<<"Function t_mul_div failed on 2 check with args=("<<ta<<','<<tb<<")\n";
            return true;
        }

        if(b*a!=a*b)
        {
            tout<<"Function t_mul_div failed on 3 check with args=("<<ta<<','<<tb<<")\n";
            return true;
        }

        if(!is_null(b))
        {
//            if( (a/b)*b!=a )
            if( (b*a)/b!=a )
            {
                tout<<"Function t_mul_div failed on 4 check with args=("<<ta<<','<<tb<<")\n";
                return true;
            }

        }

        if(!is_null(a))
        {
            if( (b/a)*a!=b )
//            if( (a*b)/a!=b )
            {
                tout<<"Function t_mul_div failed on 5 check with args=("<<ta<<','<<tb<<")\n";
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
    catch(...)
    {
        tout << "EXCEPTION: ";
        tout<<"Function t_mul_div failed with args=("<<ta<<','<<tb<<")\n";

        return true;
    }
}

template <typename type>
bool t_swap(type a, type b)
{
    try
    {
        type c=a, d=b;

        c.swap(d);


        if(c!=b)
        {
            tout<<"Function t_swap failed on 1 check with args=("<<a<<','<<b<<") \n";
            return true;
        }

        if(d!=a)
        {
            tout<<"Function t_swap failed on 2 check with args=("<<a<<','<<b<<") \n";
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
