/*****************************************************************************

    intcount_barvinok.cpp

    This file is a part of the Arageli library.

    An implementation of all algorithms in this file
    have been done by Ekaterina Schukina.

    An integration of the implemented algorithms
    into Arageli and extention to not bodily polytopes
    by Sergey Lyalin.

    Copyright (C) 2006 Ekaterina Shchukina
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
    \file intcount_barvinok.cpp
    \brief The algorithm that counts all integer points in polytope.

    This file contains implementation for the Barvinok algorithm for counting
    integer points in polytope.
*/

//TODO: This file needs to rewrite with new approaches.

//#define ARAGELI_INTERNAL_BARVINOK_DEBUG_OUTPUT

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_INTCOUNT_BARVINOK)


#include <string>
#include <list>
#include <iostream>
#include <fstream>

#ifdef ARAGELI_INTERNAL_BARVINOK_DEBUG_OUTPUT
    #include <ctime>
#endif

#include "big_int.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "gauss.hpp"
#include "lll.hpp"
#include "skeleton.hpp"
#include "rational.hpp"
#include "solve_linsys.hpp"
#include "intconvex.hpp"
#include "cone.hpp"

#include "intcount_barvinok.hpp"


namespace Arageli
{


namespace _Internal
{


// +++++++ Original declaration part ++++++++

template <typename T1, typename T2>
T1 C_n_k(const T1 n,const T2 k);

template <typename T, typename R, typename MT, typename VT, typename VR>
class UniCone
{
public:
    UniCone(const MT& a,VR& v, int s);
    void Dual();
    bool check_lyambda(const VT& lyambda);
    void GetLatticePoint();
    T t_to_s(const VT& lyambda);
    R GetValue(const T& min_num);
    void GetRationalFuncInDVariables(std::ofstream& out);
    void GetRationalFuncInOneVariable(std::ofstream& out);
    friend std::ostream& operator<< (std::ostream&,UniCone);
private:
    MT generators;
    VR vertex;
    VT lattice_point;
    int sign;
    VT koeff_degree_s_num;
    VT koeff_degree_s_den;
};

template <typename T, typename R, typename MT, typename VT, typename VR>
class SimplicialCone
{
public:
    SimplicialCone(const MT& a, VR& v, int s,T i);
    bool Decompose(std::list<UniCone<T, R, MT, VT, VR>*>& UniCones); //true - разложили, false - индекс не уменьшился
    void Dual();
    T GetIndex();
    friend std::ostream& operator<< (std::ostream&, SimplicialCone);
private:
    MT generators;
    T Index;
    int sign;
    VR vertex;
};


template <typename T, typename R, typename MT, typename VT, typename VR>
class Cone
{
public:
    Cone(const MT& a, VR& v);
    void Dual();
    void Delone(std::list<SimplicialCone<T, R, MT, VT, VR>*>& Simplicial);
    friend std::ostream& operator<< (std::ostream&,Cone);
private:
    MT generators;
    VR vertex;
};


template <typename T, typename R, typename MT, typename VT, typename VR>
class Polytope
{
public:
    Polytope(const MT& a);
    R barvinok_algorithm(bool points = false);
    int GetSupportCones(std::list<Cone<T, R, MT, VT, VR>*>& SupportCones);
    friend std::ostream& operator<< (std::ostream&,Polytope);
private:
    MT a, f, q, e;
};


// +++++++ Original implementation part ++++++++


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4018)
#endif

template <typename VT, typename T>
bool add_one (VT& koeff, T max)
{
    bool flag = false;
    for(size_t i = (koeff.size()-1); i >= 0; i--)
        if (koeff(i) == max && i == koeff.size()-1)
        {
            koeff(i) = (-1)*max;
            koeff(i-1)++;
            flag = true;
            if (koeff(i-1) <= max)
                break;
        }
        else if ((koeff(i) < max)&&(i == koeff.size()-1))
        {
            koeff(i)++;
            if (koeff(i) == max)
                flag = true;
            break;
        }
        else if ((koeff(i) > max)&&(i!=0))
        {
            koeff(i) = (-1)*max;
            koeff(i-1)++;
            flag = true;
                if (koeff(i-1) <= max)
                break;
        }
        else if ((koeff(i) > max)&&(i == 0))
            return false;

    if (!flag)
    {

        size_t count_neudovl = 0;
        for(size_t i = 0; i < koeff.size(); i++)
            if (abs(koeff(i)) == max)
                break;
            else count_neudovl++;

        if (count_neudovl == koeff.size())
            add_one(koeff,max);
    }
    return true;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

template <typename T, typename R, typename VT, typename VR, typename MR>
bool enumeration_step(VR &lyambda,const MR& a)
{
    size_t d = a.ncols();
    VT koeff_row(d);
    double norm_lyambda = pow(double(std::abs(det(a))),double(1.0/d));
    size_t max = 1;
    for(;;)
    {

        if (!add_one(koeff_row,max))
        {
            max++;

            for(size_t i = 0; i < d-1; i++)
                koeff_row(i)= T(0)-max;
        }

        for(size_t i = 0; i < lyambda.size(); i++)
            lyambda(i) = R(0);
        size_t count_udovl = 0;
        for(size_t i = 0; i < d; i++)
        {
            for(size_t j = 0; j < d; j++)
                lyambda(i)+= koeff_row(j)*a(j,i);

            if (double(std::abs(lyambda(i))) > norm_lyambda)
                break;
            else
                count_udovl++;
        }

        if (count_udovl == a.ncols())
            return true;
    }
    return false;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4018)
#endif

template <typename T1, typename T2>
T1 C_n_k(const T1 n, const T2 k)
{
    if ((n==0)&&(k==0))
        return 1;
    else if (k > n)
        return 0;
    else
    {
        T1 den(1),num(1);
        for(T1 i = 1; i < k+1; i++)
            den*=i;
        for(T1 i = n; i > n-k; i--)
            num*=i;
        ARAGELI_ASSERT_1(is_divisible(num, den));
        return num/den;
    }
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

template <typename T, typename R, typename MT, typename VT, typename VR>
UniCone<T, R, MT, VT, VR>::UniCone(const MT& a,VR& v, int s) :
    generators(a),
    vertex(v)
{
    sign = s;
}

template <typename T, typename R, typename MT, typename VT, typename VR>
void UniCone<T, R, MT, VT, VR>::Dual()
{
    MT f,q,e;
    skeleton(generators,f,q,e);
    generators = f;
}

template <typename T, typename R, typename MT, typename VT, typename VR>
std::ostream& operator<< (std::ostream& out, UniCone<T, R, MT, VT, VR> c)
{
    out << "unimodular cone: " << std::endl
        << "vertex = " << c.vertex << std::endl
        << "sign = ";
    if (c.sign == 1)
        out << "+ \n";
    else
        out << "- \n";
    output_aligned(out,c.generators);
    out << std::endl;

    return out;
}

template <typename T, typename R, typename MT, typename VT, typename VR>
void UniCone<T, R, MT, VT, VR>::GetRationalFuncInOneVariable(std::ofstream& out)
{

    out << "(" << koeff_degree_s_num(0);
    for(size_t i = 1; i < koeff_degree_s_num.size(); i++)
    {
        if (koeff_degree_s_num(i) == 1)
            out << "+s^" << i;
        else if (koeff_degree_s_num(i) == -1)
            out << "-s^" << i;
        else if ((koeff_degree_s_num(i) > 0)&&(koeff_degree_s_num(i)!=1))
            out << "+" << koeff_degree_s_num(i) << "*s^" << i;
        else if ((koeff_degree_s_num(i) < 0)&&(koeff_degree_s_num(i)!=-1))
            out << koeff_degree_s_num(i) << "*s^" << i;
    }

    out << ")/(" << std::endl;

    if (koeff_degree_s_den(0)!=0)
        out << koeff_degree_s_den(0);
    for(size_t i = 1; i < koeff_degree_s_den.size(); i++)
    {
        if (koeff_degree_s_den(i) == 1)
            out << " + s^" << i;
        else if (koeff_degree_s_den(i)!= 0)
            out << " + " << koeff_degree_s_den(i) << "*s^" << i;
    }
    out << ")" << std::endl;
}

template <typename T, typename R, typename MT, typename VT, typename VR>
void UniCone<T, R, MT, VT, VR>::GetRationalFuncInDVariables(std::ofstream& out)
{
    size_t d = generators.ncols();
    if (sign == 1)
        out << "+";
    else
        out << "-";
    //write x^(lattice_point) (numerator function)
    int count_null = 0;
    for(size_t i = 0; i < d; i++)
    {
        if (lattice_point(i) == T(0))
            count_null++;
        else if (lattice_point(i) == T(1))
            out << "x" << i;
        else
            out << "x" << i << "^(" << lattice_point(i) << ")";
        //point (0,0,...,0)
        if (count_null == d)
            out << "1";
    }
    out << "/(";
    //write all d generators
    for(size_t i = 0; i < d; i++)
    {
        out << "(1 - ";
        for(size_t j = 0; j < d; j++)
        {
            if (generators(i,j) == T(0))
                ;
            else if (generators(i,j) == T(1))
                out << "x" << j;
            else
                out << "x" << j << "^(" << generators(i,j) << ")";
        }
        out << ")";
    }
    out << ")";
}

template <typename T, typename R, typename MT, typename VT, typename VR>
bool UniCone<T, R, MT, VT, VR>::check_lyambda(const VT& lyambda)
{

    for(size_t i = 0; i < generators.nrows(); i++)
    {
        big_int sum = 0;
        for(size_t j = 0; j < generators.ncols(); j++)
            sum+= lyambda(j)*generators(i,j);
        if (sum == 0)
            return false;
    }
    return true;
}

template <typename T, typename R, typename MT, typename VT, typename VR>
void UniCone<T, R, MT, VT, VR>::GetLatticePoint()
{
    typedef typename MT::template other_element_type<R>::type MR;

    size_t d = generators.ncols();

    lattice_point.resize(d);

    bool flag = true;
    for(size_t i = 0; i < vertex.size(); i++)
        if (!vertex(i).is_integer())
        {
            flag = false;
            break;
        }
        else
            lattice_point(i) = iceil(vertex(i));

    if (!flag)
    {
        VR koeff_vertex(vertex.size());
        generators.transpose();
        koeff_vertex = inverse(MR(generators))*vertex;
        VR res_koeff(vertex.size());
        for(size_t i = 0; i < koeff_vertex.size(); i++)
            koeff_vertex(i) = iceil(koeff_vertex(i));
        res_koeff = generators*koeff_vertex;
        for(size_t i = 0; i < res_koeff.size(); i++)
            lattice_point(i) = iceil(res_koeff(i));
        generators.transpose();
    }

}

template <typename T, typename R, typename MT, typename VT, typename VR>
T UniCone<T, R, MT, VT, VR>::t_to_s(const VT& lyambda)
{

    size_t d = generators.ncols();

    koeff_degree_s_num.resize(d+1);

    for(size_t i = 0; i < d; i++)
        koeff_degree_s_num(0)+= lattice_point(i)*lyambda(i);

    koeff_degree_s_den.resize(d+1);
    for(size_t i = 0; i < generators.nrows(); i++)
    {

        T degree_t(0);
        for(size_t j = 0; j < d; j++)
            degree_t += lyambda(j)*generators(i,j);

        if (degree_t < 0)
        {
            degree_t *= -1;
            koeff_degree_s_num(0) += degree_t;
        }
        else
            sign *= -1;

        if (i==0)
            for(size_t j = 0; j < d+1; j++)
                koeff_degree_s_den(j) = C_n_k(degree_t,j+1);
        else
        {

            VT current_koeff(d+1);
            for(size_t j = 0; j < d+1; j++)
                current_koeff(j) = C_n_k(degree_t,j+1);

            VT new_koeff(d+1,T(0));
            for(size_t j = 0; j < d+1; j++)
                for (size_t k = 0; k < j+1; k++)
                    for(size_t l = 0; l < j+1; l++)
                        if ((k+l) == j)
                            new_koeff(j)+=koeff_degree_s_den(k)*current_koeff(l);

            koeff_degree_s_den = new_koeff;
        }
    }

    return koeff_degree_s_num(0);
}



template <typename T, typename R, typename MT, typename VT, typename VR>
R UniCone<T, R, MT, VT, VR>::GetValue(const T& min_num)
{

    koeff_degree_s_num(0)-=min_num;
    T degree_t;
    degree_t = koeff_degree_s_num(0);
    size_t d = generators.ncols();
    koeff_degree_s_num.resize(d+1);

    for(size_t i = 0; i < d+1; i++)
        koeff_degree_s_num(i) = sign*C_n_k(degree_t,i);
    VR c(d+1);
    c(0) = R(koeff_degree_s_num(0),koeff_degree_s_den(0));
    for(size_t k = 1; k < d+1; k++)
    {
        c(k) = koeff_degree_s_num(k);
        for(size_t j = 1; j < k+1; j++)
            for(size_t l = 0; l < k; l++)
                if ((j+l) == k)
                    c(k)-=koeff_degree_s_den(j)*c(l);
        c(k)/= koeff_degree_s_den(0);
    }
    return c.at(d);
}

template <typename T, typename R, typename MT, typename VT, typename VR>
SimplicialCone<T, R, MT, VT, VR>::SimplicialCone (const MT& a, VR &v, int s, T i) :
    generators(a),
    vertex(v)
{
    sign = s;
    Index = i;
}

template <typename T, typename R, typename MT, typename VT, typename VR>
void SimplicialCone<T, R, MT, VT, VR>::Dual()
{
    MT f,q,e;
    skeleton(generators,f,q,e);
    generators = f;
    Index = det_int(generators);
}

template <typename T, typename R, typename MT, typename VT, typename VR>
std::ostream& operator<< (std::ostream& out, SimplicialCone<T, R, MT, VT, VR> c)
{
    out
        << "simplicial cone: " << std::endl
        << "vertex = " << c.vertex << std::endl
        << "sign = ";
    if (c.sign == 1)
        out << "+ \n";
    else
        out << "- \n";
    output_aligned(out,c.generators);
    out << std::endl;

    return out;
}

template <typename T, typename R, typename MT, typename VT, typename VR>
bool SimplicialCone<T, R, MT, VT, VR>::Decompose
(std::list<UniCone<T, R, MT, VT, VR>*>& UniCones)
{
    std::list<SimplicialCone<T, R, MT, VT, VR>*> NonUni;
    typedef typename MT::template other_element_type<R>::type MR;
    size_t m = generators.nrows();
    size_t d = generators.ncols();
    if (m!=d)
    {
        //std::cerr << "there is not simplicial cone " << std::endl;
        return false;
    }

    if (std::abs(Index)==T(1))
        UniCones.push_back(new UniCone<T, R, MT, VT, VR>(generators,vertex,1));
    else
        if (Index==T(0))
        {
            //std::cerr << "matrix of cones is singular" << std::endl;
            return false;
        }
        else
            NonUni.push_back(new SimplicialCone<T, R, MT, VT, VR>(generators,vertex,sign,Index));

    while(!NonUni.empty())
    {
        SimplicialCone* K(NonUni.back());
        NonUni.pop_back();
        MR Q(K->generators);

        Q.transpose();
        Q = inverse(Q);
        MR H;

        lll_reduction(Q,H);
        Q.transpose();

        VR max_el(d, R(0));
        for(size_t i = 0; i < d; i++)
        for(size_t j = 0; j < d; j++)
            if (std::abs(Q(i,j)) > max_el(i))
                max_el(i) = std::abs(Q(i,j));

        size_t min_index = 0;
        for(size_t i = 1; i < d; i++)
            if (max_el(i) < max_el(min_index))
                min_index = i;

        K->generators.transpose();

        VT z(d);
        R current_entries;
        for(size_t k = 0; k < d; k++)
        {
            current_entries = 0;
            for(size_t j = 0; j < d; j++)
                current_entries+= K->generators(k,j)*Q(min_index,j);
            z(k) = iceil(current_entries);
        }

        size_t count_otr = 0;
        for(size_t j = 0; j < d; j++)
            if (Q(min_index,j) <= T(0))
                count_otr++;
        if (count_otr == d)
            z*= -1;
        K->generators.transpose();

        vector< MT > MatrixForCone(d,K->generators);
        VT DetOfCone(d);
        bool flag = true;
        for(size_t j = 0; j < d; j++)
        {
            MatrixForCone(j).insert_row(j,z);

            MatrixForCone(j).erase_row(j+1);

            if (rank_int(MatrixForCone(j)) == d)
            {
                DetOfCone(j) = det_int(MatrixForCone(j));
                if (std::abs(DetOfCone(j)) >= std::abs(K->Index))
                {
                    flag = false;
                    break;
                }
            }
            else
                DetOfCone(j) = T(0);
        }

        if (!flag)
        {
            vector< R > lyambda(d);
            //!!!!enumeration_step
            enumeration_step<T, R, VT>(lyambda,Q);
            K->generators.transpose();
            //z = (K->generators)*lyambda
            VT z(d);
            R current_entries;
            for(size_t k = 0; k < d; k++)
            {
                current_entries = 0;
                for(size_t j = 0; j < d; j++)
                    current_entries+= K->generators(k,j)*lyambda(j);
                z(k) = iceil(current_entries);
            }

            size_t count_otr = 0;
            for(size_t j = 0; j < d; j++)
                if (lyambda(j) < T(0))
                    count_otr++;
            if (count_otr == d)
                z*= -1;
            K->generators.transpose();
            flag = true;
            for(size_t j = 0; j < d; j++)
            {
                MatrixForCone(j).insert_row(j,z);

                MatrixForCone(j).erase_row(j+1);
                if (rank_int(MatrixForCone(j))==d)
                {
                    DetOfCone(j) = det_int(MatrixForCone(j));
                    if (std::abs(DetOfCone(j)) >= std::abs(K->Index))
                        flag = false;
                    //std::cerr << "not smaller index! \n";
                    return false;
                }
                else
                    DetOfCone(j) = T(0);
            }

        }

        for(size_t j = 0; j < d; j++)
        {
            if (std::abs(DetOfCone(j)) == T(1))
            {
                if (DetOfCone(j)*K->Index>0)
                    UniCones.push_back(new UniCone<T, R, MT, VT, VR>(MatrixForCone(j),vertex,K->sign));
                else
                    UniCones.push_back(new UniCone<T, R, MT, VT, VR>(MatrixForCone(j),vertex,-K->sign));
            }
            else
            {
                if (DetOfCone(j) != T(0))
                {
                    if (DetOfCone(j)*K->Index>0)
                        NonUni.push_back(new SimplicialCone<T, R, MT, VT, VR>(MatrixForCone(j),vertex,K->sign,DetOfCone(j)));
                    else
                        NonUni.push_back(new SimplicialCone<T, R, MT, VT, VR>(MatrixForCone(j),vertex,-K->sign,DetOfCone(j)));
                }
            }
        }
        delete K;
    }

    return true;
}


template <typename T, typename R, typename MT, typename VT, typename VR>
Cone<T, R, MT, VT, VR>::Cone(const MT& a, VR& v) :
    generators(a),
    vertex(v)
{}

template <typename T, typename R, typename MT, typename VT, typename VR>
void Cone<T, R, MT, VT, VR>::Dual()
{
    MT f,q,e;
    skeleton(generators,f,q,e);
    generators = f;
}

template <typename T, typename R, typename MT, typename VT, typename VR>
std::ostream& operator<< (std::ostream& out, Cone<T, R, MT, VT, VR> c)
{
    out << std::endl;
    out
        << "cone: " << std::endl
        << "vertex: " << c.vertex << std::endl;
    output_aligned(out, c.generators);

    return out;
}

template <typename T, typename R, typename MT, typename VT, typename VR>
void Cone<T, R, MT, VT, VR>::Delone (std::list<SimplicialCone<T, R, MT, VT, VR>*>& Simplicial)
{
    size_t ncols = generators.ncols();
    size_t nrows = generators.nrows();
    if (ncols == nrows)
        Simplicial.push_back
        (
            new SimplicialCone<T, R, MT, VT, VR>
            (
                generators,
                vertex,
                1,
                det_int(generators)
            )
        );
    else
    {
        generators.insert_col(ncols, T(0));

        for(size_t i = 0; i < nrows; i++)
        for(size_t j = 0; j < ncols; j++)
            generators(i,ncols) += generators(i,j)*generators(i,j);
        MT f,q,e;
        skeleton(generators,f,q,e);
        generators.erase_col(ncols);

        for (size_t i = 0; i < f.nrows(); i++)
        {
            if (f(i, f.ncols()-1) < 0)
            {
                MT b(0,ncols);
                for(size_t j = 0; j < q.ncols(); j++)
                    if (q(i,j) == T(0))
                        b.insert_row(b.nrows(),generators.copy_row(j));
                Simplicial.push_back
                (
                    new SimplicialCone<T, R, MT, VT, VR>
                    (
                        b,
                        vertex,
                        1,
                        det_int(b)
                    )
                );
            }
        }
    }
}


template <typename T, typename R, typename MT, typename VT, typename VR>
void barvinok_lyambda (VT& lyambda,std::list<UniCone<T, R, MT, VT, VR>*>& Uni)
{
    //g(tau)=(1,tau,tau^2,...,tau^d-1)
    //(g(tau),bij)!=0
    //l принадлежит {g(0),g(1),...,g(m)}, m=d(d-1)|I|+1
    //|I|-count of unimodular cones
    size_t d = lyambda.size();
    size_t I = Uni.size();
    size_t m = d*(d-1)*I+1;
    for(size_t i = 0; i < m; i++)
    {
        lyambda.at(0) = 1;
        lyambda.at(1) = i;
        for(size_t j = 2; j < d; j++)
            lyambda.at(j) = i*lyambda.at(j-1);
        size_t count_udovl = 0;
        for(typename std::list<UniCone<T, R, MT, VT, VR>*>::iterator j = Uni.begin(); j!=Uni.end(); ++j)
            if (!(*j)->check_lyambda(lyambda))
                break;
            else
                count_udovl++;
        if (count_udovl == I)
            break;
    }
}

template <typename T, typename R, typename MT, typename VT, typename VR>
void random_lyambda(VT& lyambda,std::list<UniCone<T, R, MT, VT, VR>*>& Uni)
{

    for(size_t i = 0; i < lyambda.size(); i++)
        lyambda.at(i) = big_int::random_in_range(T(18))-9;
    for(;;)
    {
        size_t count_udovl = 0;
        for(typename std::list<UniCone<T, R, MT, VT, VR>*>::iterator i = Uni.begin(); i!= Uni.end(); ++i)
            if (!(*i)->check_lyambda(lyambda))
                break;
            else
                count_udovl++;
        if (count_udovl == Uni.size())
            break;
        else
        {
            size_t add = big_int::random_in_range(lyambda.size()-1);
            lyambda.at(add)++;
        }
    }
}

template <typename T, typename R, typename MT, typename VT, typename VR>
Polytope<T, R, MT, VT, VR>::Polytope(const MT& a) :
    a(a)
{}

template <typename T, typename R, typename MT, typename VT, typename VR>
std::ostream& operator<< (std::ostream& out, Polytope<T, R, MT, VT, VR> p)
{
    out << "polytope: (Ax <= b) as (b|-A)" << std::endl;
    output_aligned(out, p.a);
    out << std::endl;

    return out;
}

//return:
//5 - not bodily polytope
//4 - all inequalities crosses in one point
//3 - empty polytope (actually polytope without integer points)
//2 - unbounded polytope
//1 - all good
template <typename T, typename R, typename MT, typename VT, typename VR>
int Polytope<T, R, MT, VT, VR>::GetSupportCones(std::list<Cone<T, R, MT, VT, VR>*>& SupportCones)
{

    if(f.nrows() == 0)
    {
        //std::cout << "Empty polytope!" << std::endl;
        return 3;
    }
    else if(!e.is_empty())
    {
        // Cone has a nonzero subspace.

        for(std::size_t i = 0; i < f.nrows(); ++i)
            if(!is_null(f(i, 0)))
                return 5;    // polyhedron has a nonzero subspace

        return 3;    // polyhedron is empty
    }
    else if(f.nrows() == 1)
    {
        if(is_null(f(0, 0)))
            return 2;    // unbounded polyhedron
        else if(is_unit(f(0, 0)))
            return 4;    // one-pointed polyhedron
        else
            return 3;    // polyhedron without integer points
    }
    else
    {
        bool has_reces = false, has_posit = false;
        for(size_t i = 0; i < f.nrows(); i++)
        {
            if(is_null(f(i, 0)))
                has_reces = true;
            else
                has_posit = true;

            if(has_reces && has_posit)break;
        }

        if(has_reces)
            if(has_posit)
                return 2;    // unbounded polyhedron
            else
                return 3;    // polyhedron is empty

        //if q(i,j) == 0, that
        //rows number i in matrix a (inequalities) corresponding
        //rows number j in matrix f (generators)
        for (size_t i = 0; i < f.nrows(); i++)
        {
            MT b(0,a.ncols()-1);
            for(size_t j = 0; j < q.ncols(); j++)
            {
                if (q(i,j) == T(0))
                {
                    VT one_row(a.copy_row(j));
                    one_row.erase(0);
                    //insert corresponding generators
                    b.insert_row(b.nrows(),one_row);
                }
            }
            //getting support cones for vertex i
            //get vertex: rows of f without first entries / (first entries)
            VR vertex(f.ncols()-1);
            for(size_t j = 1; j < f.ncols(); j++)
                vertex(j-1) = R(f(i,j),f(i,0));
            SupportCones.push_back(new Cone<T, R, MT, VT, VR>(b, vertex));
        }

        // A**=A,
    }
    return 1;
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4288)
#endif

//return count lattice points in bodily polyhedron
template <typename T, typename R, typename MT, typename VT, typename VR>
R Polytope<T, R, MT, VT, VR>::barvinok_algorithm(bool points)
{
#ifdef ARAGELI_INTERNAL_BARVINOK_DEBUG_OUTPUT

    std::time_t time_begin = std::clock();
    std::ofstream out("intcount_barvinok.debug.output.txt");
    out << "Arageli Debug Output File for Barvinok algorithm.\nIf you don't want this file is being generated, comment definition of macro ARAGELI_INTERNAL_BARVINOK_DEBUG_OUTPUT\n\n";

#else

    std::ofstream out;    // just a black hole

#endif

    std::list<Cone<T, R, MT, VT, VR>*> SupportCones;
    std::list<SimplicialCone<T, R, MT, VT, VR>*> SimplicialCones;
    std::list<UniCone<T, R, MT, VT, VR>*> UniCones;

    //use algorithm Motzkin - Burger
    //for double description of polytope
    skeleton(a,f,q,e);

    // Check if a polyhedron is bodily and
    // reduce space dimention to make it bodily.

    vector<std::size_t, false> redcols;

    for(std::size_t i = 0; i < q.ncols(); ++i)
    {
        bool is_reducing = true;
        for(std::size_t j = 0; j < q.nrows(); ++j)
            if(!is_null(q(j, i)))
            {
                is_reducing = false;
                break;
            }

        if(is_reducing)
            redcols.push_back(i);
    }

    if(!redcols.is_empty()/* && a.ncols() > 1*/)
    {
        matrix<T> p;
        vector<T, false> p0;
        vector<T, false> b = -a.take_col(0);    // extract b from (-b|A)

        if
        (
            solve_linsys_integer
            (
                a.copy_rows(redcols),
                b.copy_subvector(redcols),
                p0, p
            ) == SLR_EMPTY
        )
        {
            return 0;
        }

        //ARAGELI_ASSERT_1(!p.is_square());
        MT new_a;

        if(!p.is_square())
        {
            new_a = a*p;
            new_a.insert_col(0, a*p0 - b);
            a = new_a;
            skeleton(a,f,q,e);
        }
        else
        {
            new_a = a;
            new_a.insert_col(0, -b);
            a = new_a;
        }

        //return GetSupportCones(SupportCones);    // WARNING! Recursion!
    }


    //get support cones for polytope (with skeleton)
    switch(GetSupportCones(SupportCones))
    {
        case 2:
        case 5:
        {
            out << "unbounded polytope!\n";
            // Build a convex hull of all integer points of polyhedron.
            // WARNING! There is more efficient way to do this than here.
            cone<T> poly(a, fromineq);
            matrix<T, false> intineq;
            poly.normalize_all();
            matrix<std::size_t, false> tr;
            triangulate_simple_1(poly.relation(), poly.dim(), tr, poly.basis_matrix().nrows());
            //output_aligned(std::cout << "poly.min_generatrix() = \n", poly.min_generatrix());
            //output_aligned(std::cout << "\ntr = \n", tr);
            //std::cout << std::endl;
            intconvex_triangulation(poly.min_generatrix(), tr, intineq/*, intconvex_triangulation_simpleslog()*/);
            //output_aligned(std::cout << "\nintineq = \n", intineq);

            MT f, q, e;
            skeleton(intineq, f, q, e);
            for(std::size_t i = 0; i < e.nrows(); ++i)
                if(!is_null(e(i, 0)))
                    return -1;    // the polyhedron has infinite integer points
            for(std::size_t i = 0; i < f.nrows(); ++i)
                if(!is_null(f(i, 0)))
                    return -1;    // the polyhedron has infinite integer points

            return 0;    // the polyhedron has no integer points
        }
        case 3:
            out << "empty polytope!\n";
            return 0;
        case 4:
            out << "#points: \n" << "1\n"
                << "one-pointed polytope\n";
            return R(1);
    }

    out << "triangulation Delone...";
    for(typename std::list<Cone<T, R, MT, VT, VR>*>::iterator i = SupportCones.begin(); i != SupportCones.end(); ++i)
        (*i)->Delone(SimplicialCones);
    out << "done.\n";

    out << "decompose simplicial cones on unimodular cones...";
    for(typename std::list<SimplicialCone<T, R, MT, VT, VR>*>::iterator i = SimplicialCones.begin(); i != SimplicialCones.end(); ++i)
        if (!(*i)->Decompose(UniCones))
        {
            out << "not smaller index! not decomposition!" << std::endl;
            return -1;
        }
    out << "done.\n";

    out << "dualizing unimodular cones...";
    for(typename std::list<UniCone<T, R, MT, VT, VR>*>::iterator i = UniCones.begin(); i != UniCones.end(); ++i)
        (*i)->Dual();
    out << "done.\n";

    VT lyambda(a.ncols()-1);
    random_lyambda(lyambda,UniCones);

    out << "get lattice point for every unimodular cones...";
    for(typename std::list<UniCone<T, R, MT, VT, VR>*>::iterator i = UniCones.begin(); i != UniCones.end(); ++i)
        (*i)->GetLatticePoint();
    out << "done.\n";

#ifdef ARAGELI_INTERNAL_BARVINOK_DEBUG_OUTPUT

    //output rational functions if options [-points] in command line
    if (points == true)
    {
        std::ofstream out_points("intcount_barvinok.points.debug.output.txt");
        for(typename std::list<UniCone<T, R, MT, VT, VR>*>::iterator i = UniCones.begin(); i != UniCones.end(); ++i)
            (*i)->GetRationalFuncInDVariables(out_points);
        out_points.close();
    }

#endif

    out << "transformation of variables: t to s ...";
    //and find min degree of s in numerator
        typename std::list<UniCone<T, R, MT, VT, VR>*>::iterator i = UniCones.begin();
    T min_num_degree = (*i)->t_to_s(lyambda);
    ++i;
    while(i != UniCones.end())
    {
        T current_degree = (*i)->t_to_s(lyambda);
        if (current_degree < min_num_degree)
            min_num_degree = current_degree;
        ++i;
    }
    out << "done.\n";

    out << "get value for rational function's for every unimodular cones...";
    R value = 0;
    for(typename std::list<UniCone<T, R, MT, VT, VR>*>::iterator i = UniCones.begin(); i != UniCones.end(); ++i)
        value += (*i)->GetValue(min_num_degree);
    out << "done.\n";

#ifdef ARAGELI_INTERNAL_BARVINOK_DEBUG_OUTPUT

    std::time_t time_end = std::clock();

    out
        << "#points: \n" << value << std::endl
        << "#cones: \n" << SupportCones.size() << std::endl
        << "#simplicial_cones: \n" << SimplicialCones.size() << std::endl
        << "#uni_cones: \n" << UniCones.size() << std::endl
        << "lyambda = " << lyambda << std::endl
        << "#time: (sec)\n" << (double(time_end) - time_begin)/CLOCKS_PER_SEC << std::endl;

#endif

    //delete all cones
    for(typename std::list<UniCone<T, R, MT, VT, VR>*>::iterator i = UniCones.begin(); i != UniCones.end(); ++i)
        delete (*i);
    for(typename std::list<SimplicialCone<T, R, MT, VT, VR>*>::iterator i = SimplicialCones.begin(); i != SimplicialCones.end(); ++i)
        delete (*i);
    for(typename std::list<Cone<T, R, MT, VT, VR>*>::iterator i = SupportCones.begin(); i != SupportCones.end(); ++i)
        delete (*i);

    return value;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

}


template <typename A, typename T, typename Ctrler>
void intcount_barvinok (const A& a, T& res, Ctrler ctrler)
{
    _Internal::Polytope<T, rational<T>, matrix<T>, vector<T>, vector<rational<T> > > polytope(a);
    res = polytope.barvinok_algorithm(true);
}



} // namesapce Arageli


#endif
