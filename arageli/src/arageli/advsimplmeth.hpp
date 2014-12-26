/*****************************************************************************

    advsimplmeth.hpp

    This file is a part of the Arageli library.

    Copyright (C) 2006 Alexey Polovinkin

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

// Sergey Lyalin to Alexey Polovinkin:
// Please, read TODOs and remove this comment.
// In addition, read TODOs in ctrl_slog.hpp where I've placed slog-controller.

/**
    \file advsimplmeth.hpp
    \brief One variant of simplex method.

    WARNING! Some names in this file do not fit the Coding Guidelines.

    TODO: ADD WHOLE FILE DESCRIPTION HERE
*/


#ifndef _ARAGELI_advsimplmeth_hpp_
#define _ARAGELI_advsimplmeth_hpp_

#include "config.hpp"

#include <vector>
#include <string>

#include "vector.hpp"
#include "matrix.hpp"
#include "gauss.hpp"


namespace Arageli
{

namespace ctrl
{
namespace simplex_method
{

struct adv_simplex_method_alg_idler
{
    template <typename T1, typename T2, typename T3>
    void before_iter
    (
        const T1&, // basic matrix B
        const T1&, // non basic matrix N
        const T2&, // numbers of basic variables
        const T2&, // numbers of non basic variables
        const T3&  // number of iterations
    ) const
    {}

    // T - vector of reduced costs
    template <typename T>
    void print_reduced_costs (const T&) const
    {}

    template <typename T>
    void print_vars_values
    (
        const T&, // values of basic variables
        const T&  // values of non basic variables
    ) const
    {}

    template <typename T1, typename T2, typename T3>
    void print_non_basic_pivot_var
    (
        const T1&, // pivot non basic variable number
        const T2&, // pivot non basic variable status
        const T3&  // reduced cost of corresponding pivot non basic variable
    ) const
    {}

    template <typename T1>
    void print_basic_pivot_var
    (
        const T1& // number of pivot basic variable
    ) const
    {}

    template <typename T1, typename T2>
    void print_non_basic_var_status_changed
    (
        const T1&, // pivot non basic variable number
        const T2&, // old status of pivot non basic variable
        const T2&  // new status of pivot non basic variable
    ) const
    {}

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    void task_info
    (
        const T1&, // number of auxiliary variables
        const T1&, // number of structural variables
        const T2&, // names of variables
        const T3&, // constraint matrix A
        const T4&, // vector of flags: is lower bound equal infinity
        const T4&, // vector of flags: is upper bound equal infinity
        const T5&, // lower bounds of variables
        const T5&  // upper bounds of variables
    ) const
    {}

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    void art_basis_method_info
    (
        const T1&, // new objective function coeeficients
        const T2&, // number of auxiliary variables
        const T3&, // names of auxiliry variables
        const T4&, // vector of flags: is lower bounds of auxiliary variables equal infinity
        const T4&, // vector of flags: is upper bound of auxiliary variables equal infinity
        const T5&, // new lower bounds of auxiliary variables
        const T5&  // new upper bounds of auxiliary variables
    ) const
    {}

    void after_iter () const
    {}

    void print_unbounded_sol_info () const
    {}

    void print_nofeasible_exists_info () const
    {}

    template <typename T1, typename T2, typename T3, typename T4>
    void print_solution
    (
        const T1&, // number of structural variables
        const T2&, // numbers of basic variables
        const T2&, // numbers of non basic variables
        const T3&, // values of basic variables
        const T3&, // values of non basic variables
        const T4&  // names of variables
    ) const
    {}

    void art_basis_successful_info () const
    {}
};

} // namespace simplex_method
} // namespace ctrl

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace simplex_method
{

// TODO: Comment the following enums. Rename to fit coding guidelines.

enum solvingResult
{
    RES_OPTIMAL_FOUND,
    RES_SOLUTION_NOT_BOUNDED,
    RES_NO_FEASIBLE_EXISTS
};

enum presolvingResult
{
    RES_PRIMAL_FEASIBLE_FOUND,
    RES_PRIMAL_FEASIBLE_NOT_FOUND
};

enum varType
{
    VT_FREE,
    VT_LOWER_BOUNDED,
    VT_UPPER_BOUNDED,
    VT_DOUBLE_BOUNDED,
    VT_FIXED
};

enum varStatus
{
    VS_FREE,
    VS_ON_LOWER_BOUND,
    VS_ON_UPPER_BOUND,
    VS_FIXED
};

enum iterResult
{
    IRES_ITERATION_SUCCESSFUL,
    IRES_SOLUTION_NOT_BOUNDED,
    IRES_NON_BASIC_VAR_STATUS_CHANGED,
    IRES_OPTIMAL_FOUND
};

enum optDirection
{
    DIR_MAXIMIZE,
    DIR_MINIMIZE
};


// WARNING! MACRO!
#define ARAGELI_LP_READ_ERROR 100


// TODO: Comment this.
template
<
    typename T,
    typename Ctrler =
        Arageli::ctrl::simplex_method::adv_simplex_method_alg_idler
>
class adv_simplex_method_alg
{
    /*
        WARNING!
        Sergey Lyalin to Alexey Polovinkin:

        As I have already said I think the loader should be totally
        separated from the main code of this advansed simplex method.
        Because the user shouldn't care about the loader if he doesn't want
        to load a task from a file, but pass the task as a set of vectors
        and matrices of coefficients.

        So, I don't see the functionality that can be used by the user
        to pass the task in such way. I think the necessity of the store
        operation before task solving and the load operation after solving
        is a hard restriction for Arageli.
    */

    typedef typename vector<T>::size_type size_type;

    matrix<T> m_A;
    vector<T> m_C;
    vector<T> m_LowerBounds;
    vector<T> m_UpperBounds;
    vector<bool> m_IsLowerBoundsInf;
    vector<bool> m_IsUpperBoundsInf;
    vector<int> m_BasicVarNums;
    vector<int> m_NonBasicVarNums;
    vector<varType> m_VarsType;
    vector<varStatus> m_VarsStatus;

    Ctrler m_Ctrler;

    vector<T> m_BasicVars;
    vector<T> m_NonBasicVars;
    int m_NumAuxVars;
    int m_NumStructVars;

    matrix<T> m_B;
    matrix<T> m_Binv;
    vector<T> m_D;
    matrix<T> m_Amod;
    matrix<T> m_N;

    int m_PivotNonBasicVarNum;
    int m_PivotBasicVarNum;
    T m_CurrCost;

    std::vector<std::string> m_VarNames;

    /// The name of the criterion function.
    std::string m_ObjFuncName;

    void ComputeReducedCosts();
    void ComputeVarsValues();
    void ChooseNonBasicVar();
    iterResult ChooseBasicVar();
    void ChangeBasis();

    iterResult MakeSimplexStep();

    void InitInitialData();

    solvingResult SolveProblem();

    presolvingResult ArtificialBasisMethod();

    bool IsSolutionPrimalFeasible();
    bool IsSolutionDualFeasible();

    optDirection m_OptDirection;

    int m_IterNum;

    void AllocateData();
    void CheckVarTypes();

public:

    // TODO Comment each of the following declarations.

    adv_simplex_method_alg(Ctrler& ctrler = Ctrler()) :
        m_Ctrler(ctrler)
    {}

    int LoadTaskFromFile(char *filename);    // Sergey Lyalin: Отделить от основного кода! Пусть будет отдельная функция в отдельном файле.

    solvingResult SimplexDriver();
    vector<T> GetSolution();

    void SetConstraintMatrix (const matrix<T>& A);

    void SetNumAuxVars(int num_aux_vars)
    {
        m_NumAuxVars = num_aux_vars;
    }

    void SetNumStructVars (int num_struct_vars)
    {
        m_NumStructVars = num_struct_vars;
    }

    void SetObjFuncCoeffs (const vector<T>& C);

    void SetBounds
    (
        const vector<T>& lower_bounds,
        const vector<T>& upper_bounds
    );

    void SetIsBoundsInf
    (
        const vector<bool>& is_lower_bounds_inf,
        const vector<bool>& is_upper_bounds_inf
    );

    void SetOptDirection (optDirection opt_dir)
    {
        m_OptDirection = opt_dir;
    }
};


} // namespace simplex_method
} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_advsimplmeth
    #include "advsimplmeth.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_advsimplmeth
#endif

#endif    // #ifndef _ARAGELI_advsimplmeth_hpp_
