/*****************************************************************************

    advsimplmeth.cpp

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

// Sergey Lyalin to Alexey Polovinkin: Please, read TODOs and remove this line.

/**
    \file advsimplmeth.cpp
    \brief The advsimplmeth.hpp file stuff implementation.

    TODO: ADD ADDITIONAL FILE DESCRIPTION HERE
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_advsimplmeth)

#include <fstream>
#include <map>
#include <cmath>

#include "exception.hpp"
#include "factory.hpp"
#include "advsimplmeth.hpp"


namespace Arageli
{
namespace simplex_method
{


template<typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::AllocateData()
{
    m_A.resize(m_NumAuxVars, m_NumAuxVars + m_NumStructVars);
    m_C.resize(m_NumAuxVars + m_NumStructVars);
    m_B.resize(m_NumAuxVars, m_NumAuxVars);
    m_N.resize(m_NumAuxVars, m_NumStructVars);
    m_Binv.resize(m_NumAuxVars, m_NumAuxVars);
    m_Amod.resize(m_NumAuxVars, m_NumAuxVars + m_NumStructVars);
    m_LowerBounds.resize(m_NumAuxVars + m_NumStructVars);
    m_UpperBounds.resize(m_NumAuxVars + m_NumStructVars);
    m_IsLowerBoundsInf.resize(m_NumAuxVars + m_NumStructVars);
    m_IsUpperBoundsInf.resize(m_NumAuxVars + m_NumStructVars);
    m_VarsType.resize(m_NumAuxVars + m_NumStructVars);
    m_VarsStatus.resize(m_NumAuxVars + m_NumStructVars);
    m_VarNames.resize(m_NumAuxVars + m_NumStructVars);
}


template<typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::CheckVarTypes()
{
    for (size_type i = 0; i < m_NumAuxVars + m_NumStructVars; i++)
    {
        if (m_IsLowerBoundsInf[i])
        {
            if (!m_IsUpperBoundsInf[i])
            {
                m_VarsType[i] = VT_UPPER_BOUNDED;
            }
            else
            {
                m_VarsType[i] = VT_FREE;
            }
        }
        else if (m_IsUpperBoundsInf[i])
        {
            m_VarsType[i] = VT_LOWER_BOUNDED;
        }
        else if (m_LowerBounds[i] != m_UpperBounds[i])
        {
            m_VarsType[i] = VT_DOUBLE_BOUNDED;
        }
        else
        {
            m_VarsType[i] = VT_FIXED;
        }
    }
}


template<typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::SetConstraintMatrix (const matrix<T>& A)
{
    int i = 0, j = 0;
    SetNumAuxVars(A.nrows());
    SetNumStructVars(A.ncols());
    AllocateData();
    for (i = 0; i < m_NumAuxVars; i++)
        m_A(i, i) = unit<T>();
    for (j = m_NumAuxVars; j < m_NumAuxVars + m_NumStructVars; j++)
        for (i = 0; i < m_NumAuxVars; i++)
            m_A(i, j) = A(i, j - m_NumAuxVars);

}


template<typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::SetObjFuncCoeffs(const vector<T>& c)
{
    m_C.resize(m_NumAuxVars + m_NumStructVars);
    for (int i = 0; i < m_NumStructVars; i++)
        m_C[i + m_NumAuxVars] = c[i];
}


template <typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::SetBounds
(const vector<T>& lower_bounds, const vector<T>& upper_bounds)
{
    m_LowerBounds.assign(lower_bounds);
    m_UpperBounds.assign(upper_bounds);
}


template <typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::SetIsBoundsInf
(
    const vector<bool>& is_lower_bounds_inf,
    const vector<bool>& is_upper_bounds_inf
)
{
    m_IsLowerBoundsInf.assign(is_lower_bounds_inf);
    m_IsUpperBoundsInf.assign(is_upper_bounds_inf);
    for (int i = 0; i < m_NumAuxVars + m_NumStructVars; i++)
    {
        if (m_IsLowerBoundsInf[i])
            m_LowerBounds[i] = null<T>();
        if (m_IsUpperBoundsInf[i])
            m_UpperBounds[i] = null<T>();
    }
    CheckVarTypes();
}


template <typename T, typename Ctrler>
solvingResult adv_simplex_method_alg<T, Ctrler>::SolveProblem()
{
    iterResult curr_step_res;

    do
    {
        curr_step_res = MakeSimplexStep();
    }while
    (
        curr_step_res == IRES_ITERATION_SUCCESSFUL ||
        curr_step_res == IRES_NON_BASIC_VAR_STATUS_CHANGED
    );

    switch (curr_step_res)
    {
        case IRES_SOLUTION_NOT_BOUNDED:
            return RES_SOLUTION_NOT_BOUNDED;
        case IRES_OPTIMAL_FOUND:
            return RES_OPTIMAL_FOUND;
    }
}


template <typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::InitInitialData()
{
    int i = 0;
    m_IterNum = 0;
    m_BasicVarNums.resize(m_NumAuxVars);
    m_NonBasicVarNums.resize(m_NumStructVars);
    m_BasicVars.resize(m_NumAuxVars);
    m_NonBasicVars.resize(m_NumStructVars);
    m_D.resize(m_NumStructVars);

    if (m_OptDirection == DIR_MAXIMIZE)
        m_C = - m_C;

    for (i = 0; i < m_NumAuxVars; i++)
        m_BasicVarNums[i] = i;
    for (i = 0; i < m_NumStructVars; i++)
        m_NonBasicVarNums[i] = m_NumAuxVars + i;
    for (i = m_NumAuxVars; i < m_NumAuxVars + m_NumStructVars; i++)
        m_A.mult_col(i, -unit<T>());

    m_B.assign_diag(m_NumAuxVars, unit<T>());
    m_Binv.assign_diag(m_NumAuxVars, unit<T>());

    m_A.copy_cols(m_NonBasicVarNums, m_N);
    m_Amod.assign(m_N);

    for(i = m_NumAuxVars; i < m_NumAuxVars + m_NumStructVars; i++)
        switch (m_VarsType[i])
        {
            case VT_FREE:
                m_VarsStatus[i] = VS_FREE;
                break;
            case VT_LOWER_BOUNDED:
                m_VarsStatus[i] = VS_ON_LOWER_BOUND;
                break;
            case VT_UPPER_BOUNDED:
                m_VarsStatus[i] = VS_ON_UPPER_BOUND;
                break;
            case VT_DOUBLE_BOUNDED:
                if (std::abs(m_LowerBounds[i]) < std::abs(m_UpperBounds[i]))
                    m_VarsStatus[i] = VS_ON_LOWER_BOUND;
                else
                    m_VarsStatus[i] = VS_ON_UPPER_BOUND;
                break;
            case VT_FIXED:
                m_VarsStatus[i] = VS_FIXED;
                break;
        }

    ComputeVarsValues();
    ComputeReducedCosts();
}


template <typename T, typename Ctrler>
iterResult adv_simplex_method_alg<T, Ctrler>::MakeSimplexStep()
{
    m_Ctrler.before_iter(m_B, m_N, m_BasicVarNums, m_NonBasicVarNums, ++m_IterNum);

    ComputeVarsValues();
    m_Ctrler.print_vars_values(m_BasicVars, m_NonBasicVars);
    ComputeReducedCosts();
    m_Ctrler.print_reduced_costs(m_D);

    if (!IsSolutionDualFeasible())
    {
        ChooseNonBasicVar();

        m_Ctrler.print_non_basic_pivot_var
        (
            m_PivotNonBasicVarNum,
            m_VarsStatus[m_PivotNonBasicVarNum],
            m_CurrCost
        );
    }
    else
        return IRES_OPTIMAL_FOUND;

    iterResult curr_iter_res = ChooseBasicVar();

    if (curr_iter_res == IRES_ITERATION_SUCCESSFUL)
    {
        ChangeBasis();
        m_Ctrler.print_basic_pivot_var(m_PivotBasicVarNum);
    }
    else if (curr_iter_res == IRES_NON_BASIC_VAR_STATUS_CHANGED)
    {
        varStatus curr_status = m_VarsStatus[m_PivotNonBasicVarNum];

        varStatus new_status =
        (
            curr_status == VS_ON_LOWER_BOUND ?
            VS_ON_UPPER_BOUND :
            VS_ON_LOWER_BOUND
        );

        m_Ctrler.print_non_basic_var_status_changed
        (
            m_PivotNonBasicVarNum,
            curr_status,
            new_status
        );
    }

    m_Ctrler.after_iter();
    return curr_iter_res;
}


template <typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::ComputeVarsValues()
{
    for(size_type i = 0; i < m_NumStructVars; i++)
        switch (m_VarsStatus[m_NonBasicVarNums[i]])
        {
            case VS_FREE:
                m_NonBasicVars[i] = 0;
                break;
            case VS_ON_LOWER_BOUND:
                m_NonBasicVars[i] = m_LowerBounds[m_NonBasicVarNums[i]];
                break;
            case VS_ON_UPPER_BOUND:
                m_NonBasicVars[i] = m_UpperBounds[m_NonBasicVarNums[i]];
                break;
            case VS_FIXED:
                m_NonBasicVars[i] = m_LowerBounds[m_NonBasicVarNums[i]];
                break;
        }

    m_BasicVars = - m_Binv * (m_N * m_NonBasicVars);
}


template <typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::ComputeReducedCosts()
{
    size_type i = 0, j = 0;
    vector<T> cb(m_NumAuxVars);
    vector<T> cn(m_NumStructVars);
    for (i = 0; i < m_NumAuxVars; i++)
        cb[i] = m_C[m_BasicVarNums[i]];
    for (i = 0; i < m_NumStructVars; i++)
        cn[i] = m_C[m_NonBasicVarNums[i]];

    m_D = cn - cb * m_Amod;
}


template <typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::ChooseNonBasicVar()
{
    size_type i = 0;

    for(i = 0; i < m_NumStructVars; i++)
        if
        (
            (
                m_VarsStatus[m_NonBasicVarNums[i]] == VS_FREE &&
                m_D[i] != 0
            ) ||
            (
                m_VarsStatus[m_NonBasicVarNums[i]] == VS_ON_LOWER_BOUND &&
                m_D[i] < 0
            ) ||
            (
                m_VarsStatus[m_NonBasicVarNums[i]] == VS_ON_UPPER_BOUND &&
                m_D[i] > 0
            )
        )
        {
            m_PivotNonBasicVarNum = m_NonBasicVarNums[i];
            m_CurrCost = m_D[i];
            return;
        }
}


template <typename T, typename Ctrler>
iterResult adv_simplex_method_alg<T, Ctrler>::ChooseBasicVar()
{
    typename matrix<T>::size_type i = 0, min_index = -1;
    T var_interval, curr_theta, min_theta;
    bool is_var_interval_inf = false;
    if (m_VarsType[m_PivotNonBasicVarNum] == VT_DOUBLE_BOUNDED)
        var_interval =
            m_UpperBounds[m_PivotNonBasicVarNum] -
            m_LowerBounds[m_PivotNonBasicVarNum];
    else
        is_var_interval_inf = true;

    min_theta = -unit<T>();
    int pivot_col_number = -1;
    for (i = 0; i < m_NumStructVars; i++)
        if (m_NonBasicVarNums[i] == m_PivotNonBasicVarNum)
        {
            pivot_col_number = i;
            break;
        }

    if (m_CurrCost < 0)
    {
        for (i = 0; i < m_NumAuxVars; i++)
        {
            if (m_Amod(i, pivot_col_number) == 0)
                continue;
            else if (m_Amod(i, pivot_col_number) < 0)
            {
                if (m_IsUpperBoundsInf[m_BasicVarNums[i]]) continue;
                curr_theta =
                    (m_UpperBounds[m_BasicVarNums[i]] - m_BasicVars[i]) /
                    (-m_Amod(i, pivot_col_number));
            }
            else
            {
                if (m_IsLowerBoundsInf[m_BasicVarNums[i]])
                    continue;
                curr_theta =
                    (m_LowerBounds[m_BasicVarNums[i]] - m_BasicVars[i]) /
                    (-m_Amod(i, pivot_col_number));
            }
            if (curr_theta < min_theta || min_theta == -unit<T>())
            {
                min_theta = curr_theta;
                min_index = i;
            }
        }
    }
    else if (m_CurrCost > 0)
    {
        for (i = 0; i < m_NumAuxVars; i++)
        {
            if (m_Amod(i, pivot_col_number) == 0)
                continue;
            else if (m_Amod(i, pivot_col_number) > 0)
            {
                if (m_IsUpperBoundsInf[m_BasicVarNums[i]])
                    continue;
                curr_theta =
                    (m_BasicVars[i] - m_UpperBounds[m_BasicVarNums[i]]) /
                    (-m_Amod(i, pivot_col_number));
            }
            else
            {
                if (m_IsLowerBoundsInf[m_BasicVarNums[i]])
                    continue;
                curr_theta =
                    (m_BasicVars[i] - m_LowerBounds[m_BasicVarNums[i]]) /
                    (-m_Amod(i, pivot_col_number));
            }
            if (curr_theta < min_theta || min_theta == -unit<T>())
            {
                min_theta = curr_theta;
                min_index = i;
            }
        }
    }

    if (min_index == -1)
    {
        return IRES_SOLUTION_NOT_BOUNDED;
    }
    else if
    (
        min_theta >
            (
                m_UpperBounds[m_PivotNonBasicVarNum] -
                m_LowerBounds[m_PivotNonBasicVarNum]
            )
        &&
        !m_IsUpperBoundsInf[m_PivotNonBasicVarNum] &&
        !m_IsLowerBoundsInf[m_PivotNonBasicVarNum]
    )
    {
        if (m_VarsStatus[m_PivotNonBasicVarNum] == VS_ON_LOWER_BOUND)
        {
            m_VarsStatus[m_PivotNonBasicVarNum] = VS_ON_UPPER_BOUND;
        }
        else if (m_VarsStatus[m_PivotNonBasicVarNum] == VS_ON_UPPER_BOUND)
        {
            m_VarsStatus[m_PivotNonBasicVarNum] = VS_ON_LOWER_BOUND;
        }
        return IRES_NON_BASIC_VAR_STATUS_CHANGED;
    }
    else
    {
        m_PivotBasicVarNum = m_BasicVarNums[min_index];
        return IRES_ITERATION_SUCCESSFUL;
    }
}


template <typename T, typename Ctrler>
void adv_simplex_method_alg<T, Ctrler>::ChangeBasis()
{
    int pivot_basic_col_num = -1, pivot_non_basic_col_num = -1;
    size_type i = 0;
    for (i = 0; i < m_NumAuxVars; i++)
        if (m_BasicVarNums[i] == m_PivotBasicVarNum)
        {
            pivot_basic_col_num = i;
            break;
        }
    for (i = 0; i < m_NumStructVars; i++)
        if (m_NonBasicVarNums[i] == m_PivotNonBasicVarNum)
        {
            pivot_non_basic_col_num = i;
            break;
        }

    if (m_VarsType[m_PivotBasicVarNum] == VT_FIXED)
    {
        m_VarsStatus[m_PivotBasicVarNum] = VS_FIXED;
    }
    else
    {
        if
        (
            (
                m_CurrCost < null<T>() &&
                // TODO Use is_positive instead comparing with 0.
                m_Amod(pivot_basic_col_num, pivot_non_basic_col_num) > null<T>()
            ) ||
            (
                m_CurrCost > null<T>() &&
                // TODO Use is_negative instead comparing with 0.
                m_Amod(pivot_basic_col_num, pivot_non_basic_col_num) < null<T>()
            )
        )
        {
            m_VarsStatus[m_PivotBasicVarNum] = VS_ON_LOWER_BOUND;
        }
        else
        {
            m_VarsStatus[m_PivotBasicVarNum] = VS_ON_UPPER_BOUND;
        }
    }

    m_B.assign_col(pivot_basic_col_num, m_A.copy_col(m_PivotNonBasicVarNum));
    m_N.assign_col(pivot_non_basic_col_num, m_A.copy_col(m_PivotBasicVarNum));

    m_BasicVarNums[pivot_basic_col_num] = m_PivotNonBasicVarNum;
    m_NonBasicVarNums[pivot_non_basic_col_num] = m_PivotBasicVarNum;

    //m_Binv = inverse(m_B);
    //m_Binv.assign(m_B);
    m_Binv = inverse(m_B);

    m_Amod = m_Binv * m_N;

}


template <typename T, typename Ctrler>
presolvingResult adv_simplex_method_alg<T, Ctrler>::ArtificialBasisMethod()
{
    vector<T> c_copy = m_C;
    size_type i = 0;

    for (i = m_NumAuxVars; i < m_NumAuxVars + m_NumStructVars; i++)
    {
        m_C[i] = null<T>();
    }

    vector<T>
        lower_bounds_copy(m_NumAuxVars),
        upper_bounds_copy(m_NumAuxVars);

    vector<bool>
        is_lower_bounds_inf_copy(m_NumAuxVars),
        is_upper_bounds_inf_copy(m_NumAuxVars);

    vector<varType> vars_type_copy(m_NumAuxVars);

    for (i = 0; i < m_NumAuxVars; i++)
    {
        lower_bounds_copy[i] = m_LowerBounds[i];
        upper_bounds_copy[i] = m_UpperBounds[i];
        is_lower_bounds_inf_copy[i] = m_IsLowerBoundsInf[i];
        is_upper_bounds_inf_copy[i] = m_IsUpperBoundsInf[i];
        vars_type_copy[i] = m_VarsType[i];
    }

    for (i = 0; i < m_NumAuxVars; i++)
    {
        if
        (
            (
                m_VarsType[i] == VT_LOWER_BOUNDED ||
                m_VarsType[i] == VT_DOUBLE_BOUNDED ||
                m_VarsType[i] == VT_FIXED
            )
            &&
            m_BasicVars[i] < m_LowerBounds[i]
        )
        {
            m_VarsType[i] = VT_UPPER_BOUNDED;
            m_UpperBounds[i] = m_LowerBounds[i];
            //m_LowerBounds[i] = -Inf<T>();
            m_LowerBounds[i] = null<T>();
            m_IsLowerBoundsInf[i] = true;
            m_IsUpperBoundsInf[i] = false;
            m_C[i] = -unit<T>();
        }
        else if
        (
            (
                m_VarsType[i] == VT_UPPER_BOUNDED ||
                m_VarsType[i] == VT_DOUBLE_BOUNDED ||
                m_VarsType[i] == VT_FIXED
            )
            &&
            m_BasicVars[i] > m_UpperBounds[i]
        )
        {
            m_VarsType[i] = VT_LOWER_BOUNDED;
            m_LowerBounds[i] = m_UpperBounds[i];
            //m_UpperBounds[i] = Inf<T>();
            m_UpperBounds[i] = null<T>();
            m_IsLowerBoundsInf[i] = false;
            m_IsUpperBoundsInf[i] = true;
            m_C[i] = unit<T>();
        }
    }

    m_Ctrler.art_basis_method_info
    (
        m_C, m_NumAuxVars, m_VarNames,
        m_IsLowerBoundsInf, m_IsUpperBoundsInf,
        m_LowerBounds, m_UpperBounds
    );

    solvingResult art_basis_res = SolveProblem();

    if (art_basis_res != RES_OPTIMAL_FOUND)
        return RES_PRIMAL_FEASIBLE_NOT_FOUND;
    else
    {
        // check for all constraints
        for (i = 0; i < m_NumAuxVars; i++)
        {
            if (m_BasicVarNums[i] >= m_NumAuxVars)
                continue;

            if
            (
                vars_type_copy[m_BasicVarNums[i]] == VT_LOWER_BOUNDED &&
                m_BasicVars[i] < lower_bounds_copy[i]
            )
            {
                return RES_PRIMAL_FEASIBLE_NOT_FOUND;
            }
            else
                continue;

            if
            (
                vars_type_copy[m_BasicVarNums[i]] == VT_UPPER_BOUNDED &&
                m_BasicVars[i] > upper_bounds_copy[i]
            )
            {
                return RES_PRIMAL_FEASIBLE_NOT_FOUND;
            }
            else
                continue;

            if
            (
                vars_type_copy[m_BasicVarNums[i]] == VT_DOUBLE_BOUNDED &&
                (m_BasicVars[i] < lower_bounds_copy[i] || m_BasicVars[i] > upper_bounds_copy[i])
            )
            {
                return RES_PRIMAL_FEASIBLE_NOT_FOUND;
            }
            else
                continue;

            if
            (
                vars_type_copy[m_BasicVarNums[i]] == VT_FIXED &&
                m_BasicVars[i] != lower_bounds_copy[i]
            )
            {
                return RES_PRIMAL_FEASIBLE_NOT_FOUND;
            }
            else
                continue;
        }

        m_IterNum = 0;
        m_C.assign(c_copy);
        for (i = 0; i < m_NumAuxVars; i++)
        {
            m_LowerBounds[i] = lower_bounds_copy[i];
            m_UpperBounds[i] = upper_bounds_copy[i];
            m_IsLowerBoundsInf[i] = is_lower_bounds_inf_copy[i];
            m_IsUpperBoundsInf[i] = is_upper_bounds_inf_copy[i];
            m_VarsType[i] = vars_type_copy[i];
        }
        for (i = 0; i < m_NumStructVars; i++)
        {
            if (m_NonBasicVarNums[i] < m_NumAuxVars)
            {
                switch (m_VarsType[m_NonBasicVarNums[i]])
                {
                case VT_FIXED:
                    m_VarsStatus[m_NonBasicVarNums[i]] = VS_FIXED;
                    break;
                case VT_LOWER_BOUNDED:
                    m_VarsStatus[m_NonBasicVarNums[i]] = VS_ON_LOWER_BOUND;
                    break;
                case VT_UPPER_BOUNDED:
                    m_VarsStatus[m_NonBasicVarNums[i]] = VS_ON_UPPER_BOUND;
                    break;
                case VT_DOUBLE_BOUNDED:
                    if (m_NonBasicVars[i] == m_LowerBounds[m_NonBasicVarNums[i]])
                        m_VarsStatus[m_NonBasicVarNums[i]] = VS_ON_LOWER_BOUND;
                    else
                        m_VarsStatus[m_NonBasicVarNums[i]] = VS_ON_UPPER_BOUND;
                }
            }
        }
        return RES_PRIMAL_FEASIBLE_FOUND;
    }
}


template <typename T, typename Ctrler>
solvingResult adv_simplex_method_alg<T, Ctrler>::SimplexDriver()
{
    InitInitialData();

    m_Ctrler.task_info
    (
        m_NumAuxVars, m_NumStructVars, m_VarNames, m_A,
        m_IsLowerBoundsInf, m_IsUpperBoundsInf,
        m_LowerBounds, m_UpperBounds
    );

    if (IsSolutionPrimalFeasible() && IsSolutionDualFeasible())
        return RES_OPTIMAL_FOUND;

    presolvingResult art_basis_res = RES_PRIMAL_FEASIBLE_FOUND;

    if (!IsSolutionPrimalFeasible())
    {
        art_basis_res = ArtificialBasisMethod();
    }
    if (art_basis_res == RES_PRIMAL_FEASIBLE_NOT_FOUND)
    {
        m_Ctrler.print_nofeasible_exists_info();
        return RES_NO_FEASIBLE_EXISTS;
    }
    else
    {
        m_Ctrler.art_basis_successful_info();
    }

    solvingResult sol_res = SolveProblem();

    if (sol_res == RES_OPTIMAL_FOUND)
    {
        m_Ctrler.print_solution
        (
            m_NumStructVars, m_BasicVarNums, m_NonBasicVarNums,
            m_BasicVars, m_NonBasicVars, m_VarNames
        );
        return RES_OPTIMAL_FOUND;
    }
    else if (sol_res == RES_SOLUTION_NOT_BOUNDED)
    {
        m_Ctrler.print_unbounded_sol_info();
        return RES_SOLUTION_NOT_BOUNDED;
    }

    // WARNING! The function must return a value!
}


template<typename T, typename Ctrler>
bool adv_simplex_method_alg<T, Ctrler>::IsSolutionPrimalFeasible()
{
    for (int i = 0; i < m_NumAuxVars; i++)
    {
        if
        (
            m_BasicVars[i] < m_LowerBounds[m_BasicVarNums[i]] ||
            m_BasicVars[i] > m_UpperBounds[m_BasicVarNums[i]]
        )
            return false;
    }

    return true;
}


template<typename T, typename Ctrler>
bool adv_simplex_method_alg<T, Ctrler>::IsSolutionDualFeasible()
{
    for (int i = 0; i < m_NumStructVars; i++)
    {
        switch (m_VarsStatus[m_NonBasicVarNums[i]])
        {

            // TODO Use is_null, is_positive, is_negative instead
            // explicit comparing with 0.

            case VS_FREE:
                if (m_D[i] != null<T>()) return false;
                break;
            case VS_ON_LOWER_BOUND:
                if (m_D[i] < null<T>()) return false;
                break;
            case VS_ON_UPPER_BOUND:
                if (m_D[i] > null<T>()) return false;
        }
    }

    return true;
}


template<typename T, typename Ctrler>
vector<T> adv_simplex_method_alg<T, Ctrler>::GetSolution()
{
    vector<T> solution(m_NumStructVars);
    int i = 0, j = 0;
    for (i = 0; i < m_NumAuxVars; i++)
    {
        if (m_BasicVarNums[i] >= m_NumAuxVars)
        {
            solution[m_BasicVarNums[i] - m_NumAuxVars] = m_BasicVars[i];
        }
    }
    for (i = 0; i < m_NumStructVars; i++)
    {
        if (m_NonBasicVarNums[i] >= m_NumAuxVars)
        {
            solution[m_NonBasicVarNums[i] - m_NumAuxVars] = m_NonBasicVars[i];
        }
    }

    return solution;
}


template <typename T, typename Ctrler>
int adv_simplex_method_alg<T, Ctrler>::LoadTaskFromFile(char* filename)
{
    // TODO: Make lines shorter where they are greater than 80 characters.
    // TODO: Attempt to divide this whole function into smaller ones.

    std::fstream lp_file(filename);
    if(!lp_file)
    {
        // Temporary we use a common exception class.
        invalid_argument e;
        ARAGELI_EXCEPT_LOC_DESC
        (
            e,
            std::string("Cannot open the file ") + filename + "."
        );
        throw e;
    }
    std::string curr_string, curr_token, curr_var_name;
    std::string curr_constraint_name;
    bool is_opt_direction_defined = false;
    bool is_obj_func_name_defined = false;
    bool is_var_name_defined = false;
    bool is_lower_bound_defined = false;
    bool is_curr_constraint_name_defined = false;
    T coeff = 0;

    int num_unnamed_constraint = 0;

    enum
    {
        none,
        obj_func,
        constraint_sect,
        bounds_sect
    }
        stage = none;

    enum
    {
        none_,
        sign,
        coeff_value,
        var_name,
        constraint_type,
        right_side_value
    }
        token_type = none_;

    enum
    {
        plus,
        minus
    }
        coeff_sign = plus;

    enum
    {
        leq,
        eq,
        geq
    }
        ineq_type;

    std::map<std::string, T> constraint_coeffs;
    std::map<std::string, T> right_side_values;

    std::map<std::string, T> lower_bounds;
    std::map<std::string, T> upper_bounds;
    std::map<std::string, bool> is_lower_bounds_inf;
    std::map<std::string, bool> is_upper_bounds_inf;
    std::map<std::string, T> obj_func_coeff;

    std::map<std::string, std::string> constraint_types;

    std::map<std::string, std::map<std::string, T> > all_constraint_coeffs;

    int i = 0, j = 0;
    //bool is_str_stream_empty = false;

    bool is_coeff_plus_inf = false;
    bool is_coeff_minus_inf = false;

    while (1)
    {
start:
        std::getline(lp_file, curr_string);
        is_curr_constraint_name_defined = false;
        if (Arageli::stricmp(curr_string.c_str(), "end") == 0)
            break;
        if (curr_string == "")
            continue;
        std::istringstream str_stream(curr_string);

        if (stage == bounds_sect)
        {
            coeff_sign = plus;
            token_type = none_;
            is_var_name_defined = false;
            is_lower_bound_defined = false;
        }

        while (str_stream >> curr_token)
        {
            //str_stream >> curr_token;
            switch (stage)
            {
            case none:
                if (curr_token.find("\\") != curr_token.npos)
                    goto start;
                if
                (
                    Arageli::stricmp(curr_token.c_str(), "minimize") == 0 ||
                    Arageli::stricmp(curr_token.c_str(), "minimum") == 0 ||
                    Arageli::stricmp(curr_token.c_str(), "min") == 0
                )
                {
                    m_OptDirection = DIR_MINIMIZE;
                    is_opt_direction_defined = true;
                    stage = obj_func;
                }
                else if
                (
                    Arageli::stricmp(curr_token.c_str(), "maximize") == 0 ||
                    Arageli::stricmp(curr_token.c_str(), "maximum") == 0 ||
                    Arageli::stricmp(curr_token.c_str(), "max") == 0
                )
                {
                    m_OptDirection = DIR_MAXIMIZE;
                    is_opt_direction_defined = true;
                    stage = obj_func;
                }


                break;
            case obj_func:
                if (!is_opt_direction_defined) return ARAGELI_LP_READ_ERROR;
                if (token_type == none_ && !is_obj_func_name_defined)
                {
                    coeff = unit<T>();
                    if (curr_token.find(":") == curr_token.length() - 1)
                    {
                        m_ObjFuncName = curr_token.substr(0, curr_token.length() - 1);
                        is_obj_func_name_defined = true;
                        continue;
                    }
                    else
                    {
                        m_ObjFuncName = "Z";
                        is_obj_func_name_defined = true;
                    }
                }

                if (curr_token == "+" || curr_token == "-")
                {
                    if (token_type == sign || token_type == coeff_value)
                        return ARAGELI_LP_READ_ERROR;
                    if (curr_token == "+")
                        coeff_sign = plus;
                    else
                        coeff_sign = minus;
                    token_type = sign;
                    coeff = unit<T>();
                    continue;
                }

                if
                (
                    (curr_token[0] >= '0' && curr_token[0] <= '9') ||
                    curr_token[0] == '+' ||
                    curr_token[0] == '-'
                )
                {
                    if (token_type == coeff_value || token_type == var_name)
                        return ARAGELI_LP_READ_ERROR;
                    coeff = T(curr_token.c_str());    // WARNING! It restricts type T.
                    token_type = coeff_value;
                    continue;
                }

                if (Arageli::stricmp(curr_token.c_str(), "subject") == 0)
                {
                    str_stream >> curr_token;
                    if (Arageli::stricmp(curr_token.c_str(), "to") == 0)
                    {
                        stage = constraint_sect;
                        token_type = none_;
                        continue;
                    }
                }

                if (Arageli::stricmp(curr_token.c_str(), "such") == 0)
                {
                    str_stream >> curr_token;
                    if (Arageli::stricmp(curr_token.c_str(), "that") == 0)
                    {
                        stage = constraint_sect;
                        token_type = none_;
                        continue;
                    }
                }

                if
                (
                    Arageli::stricmp(curr_token.c_str(), "s.t.") == 0 ||
                    Arageli::stricmp(curr_token.c_str(), "st.") == 0 ||
                    Arageli::stricmp(curr_token.c_str(), "st") == 0
                )
                {
                    stage = constraint_sect;
                    token_type = none_;
                    continue;
                }

                if
                (
                    (curr_token[0] >= 'a' && curr_token[0] <= 'z') ||
                    (curr_token[0] >= 'A' && curr_token[0] <= 'Z') ||
                    curr_token[0] == '_'
                )
                {
                    if (token_type == var_name) return ARAGELI_LP_READ_ERROR;
                    obj_func_coeff[curr_token] = (coeff_sign == plus) ? coeff : -coeff;
                    token_type = var_name;
                    continue;
                }

                return ARAGELI_LP_READ_ERROR;
                break;
            case constraint_sect:
                if (token_type == none_)
                {
                    coeff = unit<T>();
                    coeff_sign = plus;
                    if (curr_token.find(":") == curr_token.length() - 1)
                    {
                        curr_constraint_name = curr_token.substr(0, curr_token.length() - 1);
                        is_curr_constraint_name_defined = true;
                        continue;
                    }
                    else if (!is_curr_constraint_name_defined)
                    {
                        char row_num[10];
                        std::ostringstream itoa_stream;
                        itoa_stream << num_unnamed_constraint++;
                        //itoa(num_unnamed_constraint++, row_num, 10);
                        curr_constraint_name = "r." + std::string(itoa_stream.str());
                    }
                }

                if (curr_token == "+" || curr_token == "-")
                {
                    if (token_type == sign || token_type == coeff_value)
                        return ARAGELI_LP_READ_ERROR;

                    if (curr_token == "+")
                        coeff_sign = plus;
                    else
                        coeff_sign = minus;
                    if (token_type != constraint_type)
                        token_type = sign;
                    coeff = unit<T>();
                    continue;
                }

                if (curr_token[0] >= '0' && curr_token[0] <= '9')
                {
                    if (token_type == coeff_value || token_type == var_name)
                        return ARAGELI_LP_READ_ERROR;
                    if (token_type != constraint_type)
                    {
                        coeff = T(curr_token.c_str());    // WARNING! It restricts type T.
                        token_type = coeff_value;
                    }
                    else
                    {
                        right_side_values[curr_constraint_name] =
                            (coeff_sign == plus) ?
                            T(curr_token.c_str()) :
                            -T(curr_token.c_str());    // WARNING! It restricts type T.
                        all_constraint_coeffs[curr_constraint_name] = constraint_coeffs;
                        constraint_coeffs.clear();
                        token_type = none_;
                    }
                    continue;
                }

                if
                (
                    Arageli::stricmp(curr_token.c_str(), "bound") == 0 ||
                    Arageli::stricmp(curr_token.c_str(), "bounds") == 0
                )
                {
                    stage = bounds_sect;
                    token_type = none_;
                    continue;
                }

                if
                (
                    (curr_token[0] >= 'a' && curr_token[0] <= 'z') ||
                    (curr_token[0] >= 'A' && curr_token[0] <= 'Z') ||
                    curr_token[0] == '_'
                )
                {
                    if (token_type == var_name)
                        return ARAGELI_LP_READ_ERROR;
                    constraint_coeffs[curr_token] = (coeff_sign == plus) ? coeff : -coeff;
                    token_type = var_name;
                    continue;
                }

                if (curr_token[0] == '+' || curr_token[0] == '-')
                {
                    if (token_type == constraint_type)
                    {
                        right_side_values[curr_constraint_name] = T(curr_token.c_str());    // WARNING! It restricts type T.
                        all_constraint_coeffs[curr_constraint_name] = constraint_coeffs;
                        constraint_coeffs.clear();
                    }
                    else if (token_type == none_)
                    {
                        coeff = T(curr_token.c_str());    // WARNING! It restricts type T. And so on...
                        token_type = coeff_value;
                    }
                    else
                    {
                        return ARAGELI_LP_READ_ERROR;
                    }
                    continue;
                }

                if (curr_token == "=" || curr_token == "<=" || curr_token == ">=")
                {
                    if (token_type != var_name)
                        return ARAGELI_LP_READ_ERROR;
                    coeff_sign = plus;
                    token_type = constraint_type;
                    if (curr_token == "=")
                        ineq_type = eq;
                    else if (curr_token == "<=")
                        ineq_type = leq;
                    else if (curr_token == ">=")
                        ineq_type = geq;
                    constraint_types[curr_constraint_name] = curr_token;
                    continue;
                }



                return ARAGELI_LP_READ_ERROR;
                break;
            case bounds_sect:
                if
                (
                    (curr_token[0] >= 'a' && curr_token[0] <= 'z') ||
                    (curr_token[0] >= 'A' && curr_token[0] <= 'Z') ||
                    curr_token[0] == '_'
                )
                {
                    if
                    (
                        token_type != none_ &&
                        token_type != constraint_type &&
                        !(token_type == var_name && curr_token == "free" )
                    )
                        return ARAGELI_LP_READ_ERROR;
                    if (curr_token == "free")
                    {
                        if (token_type == var_name)
                        {
                            //lower_bounds[curr_var_name] = -Inf<T>();
                            //upper_bounds[curr_var_name] = Inf<T>();
                            lower_bounds[curr_var_name] = null<T>();
                            upper_bounds[curr_var_name] = null<T>();
                            is_lower_bounds_inf[curr_var_name] = true;
                            is_upper_bounds_inf[curr_var_name] = true;
                            token_type = none_;
                            continue;
                        }
                        else
                        {
                            return ARAGELI_LP_READ_ERROR;
                        }
                    }
                    curr_var_name = curr_token;
                    /*
                    if (is_lower_bound_defined)
                        lower_bounds[curr_var_name] = coeff;
                    else
                        lower_bounds[curr_var_name] = null<T>();
                        */
                    token_type = var_name;
                    is_var_name_defined = true;
                    if (is_lower_bound_defined)
                    {
                        lower_bounds[curr_var_name] = coeff;
                        is_lower_bounds_inf[curr_var_name] = false;
                        //upper_bounds[curr_var_name] = Inf<T>();
                        upper_bounds[curr_var_name] = null<T>();
                        is_upper_bounds_inf[curr_var_name] = true;
                    }

                    continue;
                }

                if (curr_token == "+" || curr_token == "-")
                {
                    if (token_type != none_ && token_type != constraint_type)
                        return ARAGELI_LP_READ_ERROR;
                    if (curr_token == "+")
                        coeff_sign = plus;
                    else
                        coeff_sign = minus;
                    token_type = sign;
                    continue;
                }

                if
                (
                    curr_token[0] == '+' ||
                    curr_token[0] == '-' ||
                    (curr_token[0] >= '0' && curr_token[0] <= '9')
                )
                {
                    if
                    (
                        token_type != constraint_type &&
                        token_type != none_
                    )
                        return ARAGELI_LP_READ_ERROR;
                    if (Arageli::stricmp(curr_token.c_str(), "+inf") == 0)
                    {
                        //coeff = Inf<T>();
                        coeff = null<T>();
                        is_coeff_plus_inf = true;
                    }
                    else if (Arageli::stricmp(curr_token.c_str(), "-inf") == 0)
                    {
                        //coeff = -Inf<T>();
                        coeff = null<T>();
                        is_coeff_minus_inf = true;
                    }
                    else
                    {
                        coeff =
                            T(curr_token.c_str()) *
                            (coeff_sign == plus ? unit<T>() : -unit<T>());
                        is_coeff_plus_inf = false;
                        is_coeff_minus_inf = false;
                    }

                    if (token_type == none_)
                    {
                        is_lower_bound_defined = true;
                        continue;
                    }

                    if (token_type == constraint_type)
                    {
                        if (is_var_name_defined && ineq_type == geq)
                        {
                            if (!is_coeff_minus_inf)
                            {
                                lower_bounds[curr_var_name] = coeff;
                                is_lower_bounds_inf[curr_var_name] = false;
                            }
                            else
                            {
                                lower_bounds[curr_var_name] = null<T>();
                                is_lower_bounds_inf[curr_var_name] = true;
                            }
                            //upper_bounds[curr_var_name] = Inf<T>();
                            upper_bounds[curr_var_name] = null<T>();
                            is_upper_bounds_inf[curr_var_name] = true;
                            continue;
                        }

                        if
                        (
                            is_var_name_defined &&
                            ineq_type == leq &&
                            !is_lower_bound_defined
                        )
                        {
                            lower_bounds[curr_var_name] = null<T>();
                            is_lower_bounds_inf[curr_var_name] = false;
                            if (!is_coeff_plus_inf)
                            {
                                upper_bounds[curr_var_name] = coeff;
                                is_upper_bounds_inf[curr_var_name] = false;
                            }
                            else
                            {
                                upper_bounds[curr_var_name] = null<T>();
                                is_upper_bounds_inf[curr_var_name] = true;
                            }
                            continue;
                        }

                        if (is_var_name_defined && ineq_type == leq && is_lower_bound_defined)
                        {
                            if (!is_coeff_plus_inf)
                            {
                                upper_bounds[curr_var_name] = coeff;
                                is_upper_bounds_inf[curr_var_name] = false;
                            }
                            else
                            {
                                upper_bounds[curr_var_name] = null<T>();
                                is_upper_bounds_inf[curr_var_name] = true;
                            }
                            continue;
                        }

                        if
                        (
                            is_var_name_defined &&
                            ineq_type == eq &&
                            !is_lower_bound_defined
                        )
                        {
                            lower_bounds[curr_var_name] = upper_bounds[curr_var_name] = coeff;
                            is_lower_bounds_inf[curr_var_name] = false;
                            is_upper_bounds_inf[curr_var_name] = false;
                            continue;
                        }

                        return ARAGELI_LP_READ_ERROR;
                    }

                    token_type = coeff_value;

                    continue;
                }

                if (curr_token == "<=" || curr_token == "=" || curr_token == ">=")
                {
                    if
                    (
                        (curr_token == ">=" || curr_token == "=") &&
                        token_type != var_name
                    )
                        return ARAGELI_LP_READ_ERROR;
                    if (curr_token == "<=")
                    {
                        ineq_type = leq;
                    }
                    else if (curr_token == "=")
                        ineq_type = eq;
                    else
                        ineq_type = geq;
                    token_type = constraint_type;
                    continue;
                }
                break;
            };
        }
    }

    std::vector<std::string> var_names;

    typename std::map<std::string, T>::iterator iter, iter1;

    typename std::map<std::string, std::map<std::string, T> >::iterator _iter;
    for
    (
        _iter = all_constraint_coeffs.begin();
        _iter != all_constraint_coeffs.end();
        _iter++
    )
    {
        for
        (
            iter = _iter->second.begin();
            iter != _iter->second.end();
            iter++
        )
        {
            if (find(var_names.begin(), var_names.end(), iter->first) == var_names.end())
                var_names.push_back(iter->first);
        }
    }

    sort(var_names.begin(), var_names.end());
    std::map<std::string, int> var_indexes;

    for (i = 0; i < static_cast<int>(var_names.size()); i++)
        var_indexes[var_names[i]] = i;


    m_NumAuxVars = all_constraint_coeffs.size();
    m_NumStructVars = var_names.size();

    AllocateData();

    for (i = 0; i < m_NumAuxVars; i++)
        m_A(i, i) = unit<T>();
    for
    (
        _iter = all_constraint_coeffs.begin(), i = 0;
        _iter != all_constraint_coeffs.end();
        _iter++, i++
    )
    {
        m_VarNames[i] = _iter->first;
        for (iter = _iter->second.begin(); iter != _iter->second.end(); iter++)
        {
            m_A(i, m_NumAuxVars + var_indexes[iter->first]) = iter->second;
        }
    }

    for (i = 0; i < m_NumStructVars; i++)
        m_VarNames[m_NumAuxVars + i] = var_names[i];

    for (iter = obj_func_coeff.begin(); iter != obj_func_coeff.end(); iter++)
        m_C[m_NumAuxVars + var_indexes[iter->first]] = iter->second;

    // set default values
    for (i = 0; i < m_NumAuxVars + m_NumStructVars; i++)
    {
        m_LowerBounds[i] = null<T>();
        m_IsLowerBoundsInf[i] = false;
        m_UpperBounds[i] = null<T>();
        m_IsUpperBoundsInf[i] = true;
    }

    std::map<std::string, std::string>::iterator ctype_iter;
    for
    (
        i = 0, ctype_iter = constraint_types.begin();
        ctype_iter != constraint_types.end();
        ctype_iter++, i++
    )
    {
        if (ctype_iter->second == "<=")
        {
            //m_LowerBounds[i] = -Inf<T>();
            m_LowerBounds[i] = null<T>();
            m_IsLowerBoundsInf[i] = true;
            m_UpperBounds[i] = right_side_values[ctype_iter->first];
            m_IsUpperBoundsInf[i] = false;
        }
        else if (ctype_iter->second == "=")
        {
            m_LowerBounds[i] = m_UpperBounds[i] = right_side_values[ctype_iter->first];
            m_IsLowerBoundsInf[i] = m_IsUpperBoundsInf[i] = false;
        }
        else if (ctype_iter->second == ">=")
        {
            m_LowerBounds[i] = right_side_values[ctype_iter->first];
            m_IsLowerBoundsInf[i] = false;
            //m_UpperBounds[i] = Inf<T>();
            m_UpperBounds[i] = null<T>();
            m_IsUpperBoundsInf[i] = true;
        }
    }

    std::map<std::string, bool>::iterator is_inf_iter1, is_inf_iter2;
    for
    (
        iter = lower_bounds.begin(), iter1 = upper_bounds.begin(), is_inf_iter1 = is_lower_bounds_inf.begin(), is_inf_iter2 = is_upper_bounds_inf.begin();
        iter != lower_bounds.end();
        iter++, iter1++, is_inf_iter1++, is_inf_iter2++
    )
    {
        m_IsLowerBoundsInf[m_NumAuxVars + var_indexes[iter->first]] = is_inf_iter1->second;
        m_LowerBounds[m_NumAuxVars + var_indexes[iter->first]] = iter->second;
        m_IsUpperBoundsInf[m_NumAuxVars + var_indexes[iter->first]] = is_inf_iter2->second;
        m_UpperBounds[m_NumAuxVars + var_indexes[iter1->first]] = iter1->second;
    }

    CheckVarTypes();

    return 0;
}


} // namespace simplex_method
} // namespace Arageli


#if ARAGELI_DEBUG_LEVEL > 3

#include <iostream>
#include "rational.hpp"
#include "ctrl_slog.hpp"

namespace Arageli
{

template class simplex_method::adv_simplex_method_alg
<
    rational<>,
    ctrl::simplex_method::adv_simplex_method_alg_slog<std::ostream>
>;

template class simplex_method::adv_simplex_method_alg
<
    rational<>,
    ctrl::simplex_method::adv_simplex_method_alg_idler
>;

// WARNING!
// The following explicit specialization cannot be compiled
// due to restrictions in the loader code. The restriction is that
// the type should be constructable from a string.

/*
template class simplex_method::adv_simplex_method_alg
<
    double,
    ctrl::simplex_method::adv_simplex_method_alg_slog<std::ostream>
>;

template class simplex_method::adv_simplex_method_alg
<
    double,
    ctrl::simplex_method::adv_simplex_method_alg_idler
>;
*/

}

#endif

#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
