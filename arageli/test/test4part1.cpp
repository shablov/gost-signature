/*****************************************************************************

    test/test4part1.cpp

    This file is a part of the Arageli library.

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

#include "stdafx.hpp"

#pragma warning (disable : 4503)


using namespace Arageli;
using namespace Arageli::simplex_method;
using namespace Arageli::ctrl;
using namespace Arageli::ctrl::simplex_method;
using Arageli::vector;

using std::ostream;
using std::cout;
using std::cin;
using std::ofstream;
using std::ifstream;
using std::cerr;
using std::endl;
using std::string;


struct the_second_test_Shevchenko_ctrl_idler
{
    template <typename A, typename B, typename C>
    void preamble (const A& a, const B& b, const C& c) const {}

    basis_artificial_idler ctrl_for_basis_artificial () const
    { return basis_artificial_idler(); }

    primal_row_iters_idler ctrl_for_primal_row_iters () const
    { return primal_row_iters_idler(); }

    template <typename T, typename X>
    void primal_opt (const T& res, const X& x_opt) const {}

    gomory1_iters_idler ctrl_for_primal_task_gomory1_iters () const
    { return gomory1_iters_idler(); }

    gomory1_iters_idler ctrl_for_dual_task_gomory1_iters () const
    { return gomory1_iters_idler(); }

    dual_col_iters_idler ctrl_for_dual_col_iters () const
    { return dual_col_iters_idler(); }

    template <typename T, typename X>
    void primal_opt_int (const T& res, const X& x_opt_int) const {}

    template
    <
        typename Da, typename Db, typename Dc,
        typename T, typename Bsuba, typename Bsubc
    >
    void dual
    (
        const Da& da, const Db& db, const Dc& dc,
        const T& res_offset,
        const Bsuba& bsuba, const Bsubc& bsubc
    ) const {}

    template <typename T, typename Y>
    void dual_y_opt (const T& res, const Y& y_opt) const {}

    template
    <
        typename Invbsuba, typename Bsubc,
        typename Y, typename U
    >
    void y_to_u_opt
    (
        const Invbsuba& invbsuba, const Bsubc& bsubc,
        const Y& y_opt, const U& u_opt
    ) const {}

    template <typename T, typename Y>
    void dual_y_opt_int (const T& res, const Y& y_opt) const {}

    template
    <
        typename Invbsuba, typename Bsubc,
        typename Y, typename U
    >
    void y_to_u_opt_int
    (
        const Invbsuba& invbsuba, const Bsubc& bsubc,
        const Y& y_opt_int, const U& u_opt_int
    ) const {}

};


// Решается задача целочисленного линейного программирования
// и двойственная к ней симплек методом с отсечением Гомори.
// Ограничения на исходную задачу: в векторе b не должно быть
// отрицательных компонент; и прямая, и двойственная задачи
// должные иметь целочисленное решение.
template
<
    typename T1, bool REFCNT1,
    typename T2, bool REFCNT2,
    typename T3, bool REFCNT3,
    typename Ctrler
>
void the_second_test_Shevchenko
(
    const matrix<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b,
    const vector<T3, REFCNT3>& c,
    Ctrler ctrler
)
{
    typedef matrix<T1, false> Matrix;
    typedef vector<T1, false> Vector;
    typedef vector<size_t, false> Basis;

    ctrler.preamble(a, b, c);
    Matrix q;
    row_table_create(a, b, q);
    Basis basis;
    result_kind rk = basis_artificial
        (q, basis, ctrler.ctrl_for_basis_artificial());
    ARAGELI_ASSERT_0(rk == rk_found);

    //Matrix bsuba;
    //fill_submatrix_col(a, basis-1, bsuba);
    //fill_subvector(c, basis-1, bsubc);
    Basis first_basis = basis;    // сохраняем на будущее

    // Решаем прямую задачу прямым строчечным симплекс методом.

    row_table_place_c(c, q);
    row_table_pivot_basis_c(q, basis);
    rk = primal_row_iters(q, basis, ctrler.ctrl_for_primal_row_iters());
    ARAGELI_ASSERT_0(rk == rk_found);

    Vector x_opt;
    row_table_extract_solution(q, basis, x_opt);
    ctrler.primal_opt(q(0, 0), x_opt);

    // Находим решение прямой задачи в целых числах.

    Matrix t;
    Basis nonbasis;
    row_to_col_table(q, basis, t, nonbasis);
    gomory1_iters
        (t, nonbasis, ctrler.ctrl_for_primal_task_gomory1_iters());
    Vector x_opt_int;
    col_table_extract_solution(t, a.ncols(), x_opt_int);
    ctrler.primal_opt_int(t(0, 0), x_opt_int);

    // Строим двойственную задачу.

    Matrix bsuba, da;
    Vector bsubc, db, dc;
    T1 primal_res_offset;
    primal_to_dual_standard_discr
        (a, b, c, first_basis, da, db, dc, primal_res_offset, bsuba, bsubc);
    cout << "***** " << det(bsuba) << " *******";
    ctrler.dual(da, db, dc, primal_res_offset, bsuba, bsubc);

    // Решаем двойственную задачу столбцовым двойственным симплекс методом.

    col_table_create_by_standard(da, db, dc, t, nonbasis);
    t(0, 0) = primal_res_offset;
    rk = dual_col_iters
        (t, nonbasis, ctrler.ctrl_for_dual_col_iters());
    ARAGELI_ASSERT_0(rk == rk_found);
    Vector y_opt;
    col_table_extract_solution(t, da.ncols(), y_opt);
    ctrler.dual_y_opt(-t(0, 0), y_opt);

    Matrix invbsuba = inverse(bsuba);
    Vector u_opt = (y_opt + bsubc)*inverse(bsuba);
    ctrler.y_to_u_opt(invbsuba, bsubc, y_opt, u_opt);

    // Решаем двойственную задачу в целых числах.

    gomory1_iters
        (t, nonbasis, ctrler.ctrl_for_dual_task_gomory1_iters());
    Vector y_opt_int;
    col_table_extract_solution(t, da.ncols(), y_opt_int);
    ctrler.dual_y_opt_int(-t(0, 0), y_opt_int);

    Vector u_opt_int = (y_opt_int + bsubc)*inverse(bsuba);
    ctrler.y_to_u_opt_int(invbsuba, bsubc, y_opt_int, u_opt_int);

    cout << "\n*****************************************\n";
    cout
        << "\nOptimal point: " << x_opt
        << "\nOptimal integer point: " << x_opt_int
        << "\nOptimal point for dual: " << u_opt
        << "\nOptimal integer point for dual: " << u_opt_int << '\n';
    cout << "\n*****************************************\n";
}


struct the_second_test_Shevchenko_ctrl_latexlog
{
    ostream& stream;

    the_second_test_Shevchenko_ctrl_latexlog(ostream& stream_a) : stream(stream_a) {}

    template <typename A, typename B, typename C>
    void preamble (const A& a, const B& b, const C& c) const
    {
        stream << "Дана задача линейного программирования:\n$$\\max cx$$\n"
            "$$\\left\\{ \\begin{tabular}{l}$Ax = b,$\\\\ $x \\ge 0,$ \\end{tabular}\\right.$$"
            "\nгде\n";
        stream << "$$A = ";
        output_latex(stream, a, true);
        stream << ",$$\n$$b = ";
        output_latex(stream, b, true);
        stream << "^T,$$\n$$c = ";
        output_latex(stream, c, true);
        stream << ".$$\nТребуется решить эту задачу при $x \\in R^{" << a.ncols()
            << "}$ и для $x \\in Z^{" << a.ncols() << "}$."
            " Построить двойственную и также "
            "решить для этих двух случаев.\\par";
        stream << endl;
    }

    basis_artificial_latexlog<ostream> ctrl_for_basis_artificial () const
    { return basis_artificial_latexlog<ostream>(stream, false, false); }

    primal_row_iters_latexlog<ostream> ctrl_for_primal_row_iters () const
    { return primal_row_iters_latexlog<ostream>(stream, false, false); }

    template <typename T, typename X>
    void primal_opt (const T& res, const X& x_opt) const
    {
        stream
            << "Итак, оптимальный вектор для прямой задачи $x = " << x_opt
            << "^T$, значение функции $" << res << "$.\n";
    }

    gomory1_iters_latexlog<ostream> ctrl_for_primal_task_gomory1_iters () const
    { return gomory1_iters_latexlog<ostream>(stream, false, false); }

    gomory1_iters_latexlog<ostream> ctrl_for_dual_task_gomory1_iters () const
    { return gomory1_iters_latexlog<ostream>(stream, false, false); }

    dual_col_iters_latexlog<ostream> ctrl_for_dual_col_iters () const
    { return dual_col_iters_latexlog<ostream>(stream, false, false); }

    template <typename T, typename X>
    void primal_opt_int (const T& res, const X& x_opt_int) const
    {
        stream
            << "Оптимальный вектор для прямой задачи в целых числах $x = " << x_opt_int
            << "^T$, значение функции $" << res << "$.\n";
    }

    template
    <
        typename Da, typename Db, typename Dc,
        typename T, typename Bsuba, typename Bsubc
    >
    void dual
    (
        const Da& da, const Db& db, const Dc& dc,
        const T& res_offset,
        const Bsuba& bsuba, const Bsubc& bsubc
    ) const
    {
        stream
            << "Двойственная задача:\n$$\\min ub$$\n"
            "$$uA \\ge c,$$\n";
        stream << "где $u = (u_1";
        for(size_t i = 2; i <= db.size(); ++i)
            stream << ", u_{" << i << "}";
        stream << ")$. Требуется решить эту задачу при $u \\in R^{" << da.nrows()
            << "}$ и для $x \\in Z^{" << da.nrows() << "}$."
            " Введём переменные незязки $y = uB - c_B \\ge 0$, где\n$$B = ";
        output_latex(stream, bsuba, true);
        stream << ",$$\n$$c_B = ";
        output_latex(stream, bsubc, true);
        stream << " $$ \nТогда исходную двойственную задачу можно переписать в новых"
            " переменных так:\n$$\\min c'x$$\n"
            "$$\\left\\{ \\begin{tabular}{l}$A'y^T \\le b',$\\\\ $y \\ge 0,$ \\end{tabular}\\right.$$"
            "\nгде\n";
        stream << "$$A' = ";
        output_latex(stream, da, true);
        stream << ",$$\n$$b' = ";
        output_latex(stream, db, true);
        stream << "^T,$$\n$$c' = ";
        output_latex(stream, dc, true);
        stream << "$$Довесок к целевой функции $" << res_offset << "$. Введением"
            " слабых переменных приведём задачу к каноническому виду.";
    }

    template <typename T, typename Y>
    void dual_y_opt (const T& res, const Y& y_opt) const
    {
        stream
            << "Таким образом, найден оптимальный вектор"
            " для двойственной задачи $y = " << y_opt
            << "^T$, значение функции $" << res << "$.\n";
    }

    template
    <
        typename Invbsuba, typename Bsubc,
        typename Y, typename U
    >
    void y_to_u_opt
    (
        const Invbsuba& invbsuba, const Bsubc& bsubc,
        const Y& y_opt, const U& u_opt
    ) const
    {
        stream <<
            "Восстановим значения оригинальных переменных $u$:\n"
            "$$u = (y + c_B)B^{-1} = \\left(y + ";
        output_latex(stream, bsubc, true);
        stream << "^T\\right)";
        output_latex(stream, invbsuba, true);
        stream << " = ";
        output_latex(stream, u_opt, true, eep_alone, false);
        stream << "^T.$$\n";
    }

    template <typename T, typename Y>
    void dual_y_opt_int (const T& res, const Y& y_opt) const
    {
        stream
            << "Таким образом, найден оптимальный целочисленный вектор"
            " для двойственной задачи $y = " << y_opt
            << "^T$, значение функции $" << res << "$.\n";
    }

    template
    <
        typename Invbsuba, typename Bsubc,
        typename Y, typename U
    >
    void y_to_u_opt_int
    (
        const Invbsuba& invbsuba, const Bsubc& bsubc,
        const Y& y_opt_int, const U& u_opt_int
    ) const
    {
        stream <<
            "Восстановим значения оригинальных переменных $u$:\n"
            "$$u = (y + c_B)B^{-1} = \\left(y + ";
        output_latex(stream, bsubc, true);
        stream << "^T\\right)";
        output_latex(stream, invbsuba, true);
        stream << " = ";
        output_latex(stream, u_opt_int, true, eep_alone, false);
        stream << "^T.$$\n";
    }

};


template <typename A, typename B, typename C>
void task_from_file (const char* filename, A& a, B& b, C& c)
{
    ifstream task(filename);

    if(!task)
    {
        cout << "\nERROR. Can't open file '" << filename << "'\n";
        exit(1);
    }

    task >> a >> b >> c;
}

template <typename A, typename B, typename C>
void task_from_file (const string& filename, A& a, B& b, C& c)
{ task_from_file(filename.c_str(), a, b, c); }


template <typename A, typename B, typename C, typename Basis>
void task_basis_from_file (const char* filename, A& a, B& b, C& c, Basis& basis)
{
    ifstream task(filename);

    if(!task)
    {
        cout << "\nERROR. Can't open file '" << filename << "'\n";
        exit(1);
    }

    task >> a >> b >> c >> basis;
}


template <typename A, typename B, typename C>
void print_task (const A& a, const B& b, const C& c)
{
    output_aligned(cout << "\nA =\n", a);
    cout << "b = " << b << "\nc = " << c << '\n';
    cout << "A*x = b, x >= 0.\n";
}


template <typename A, typename B>
void print_task (const A& a, const B& b)
{
    output_aligned(cout << "\nA =\n", a);
    cout << "b = " << b << '\n';
    cout << "A*x = b, x >= 0.\n";
}


void test4_1 (const std::string& file_with_problem)
{
    ofstream out("the_second_test_Shevchenko2.output.tex");

    typedef rational<> T;
    matrix<T, false> a;
    vector<T, false> b, c;

    task_from_file(file_with_problem.c_str(), a, b, c);

    the_second_test_Shevchenko
        (a, b, c, the_second_test_Shevchenko_ctrl_latexlog(out));
}


template <typename A, typename AA, typename B, typename Det, typename Basis>
void partial_rref_int_order
(
    const A& a,
    AA& aa, B& b, Det& det,
    const Basis& basis
)
{
    AA q; Basis basis_out;
    rref_order(a, aa, q, basis, basis_out, det);
    aa *= std::abs(det);
    // B = *b|N
    b = aa;
    output_aligned(cout << "\n|det(B)|*B^(-1)*(b|A) = \n", b);
    b.erase_cols(basis);
}


template <typename A1, typename A2>
void create_vector_order (const A1& a1, A2& a2, size_t n)
{
    a2 = a1;
    for(size_t i = 0; i < n; ++i)
        if(std::find(a1.begin(), a1.end(), i) == a1.end())
            a2.push_back(i);
}


void test4_2 ()
{
    // Применение алгоритма Моцкина-Бургера для области нашей задачи ЗЛП.

    typedef big_int T;
    typedef matrix<T, false> Matrix;
    typedef vector<T, false> Vector;

    Matrix a;
    Vector b, c;

    task_from_file("../../../../samples/old-samples/Shevchenko_course-3_v-896.task.txt", a, b, c);
    print_task(a, b, c);

    Vector basis = "(1, 3, 4)";

    //a.swap_cols(2, 3);
        rational<> det;

    {
        Matrix ab = a;
        ab.insert_col(0, b);

        matrix<rational<> > newa, bn;
        partial_rref_int_order(matrix<rational<> >(ab), newa, bn, det, basis);
        a = bn;
        output_aligned(cout << "PARTIAL_RREFINT(b|A) = \n", newa);
        output_aligned(cout << "\nbn = \n", bn);
    }

    //Vector nonbasis;
    //basis_to_nonbasis(basis, nonbasis, 0, a.ncols());
    //cout << "\nbasis = " << basis << '\n';
    //Matrix bmat = a.take_cols(basis);
    //Matrix Bm1N = a;

    a.opposite();
    //a.insert_col(0, b+1);
    a.mult_col(0, -1);
    Matrix ba_orig = a;
    for(int i = 0; i < a.ncols(); ++i)
    {
        Vector v(a.ncols());
        v[i] = 1;
        a.insert_row(i/*a.nrows()*/, v);
    }


    output_aligned(cout << "\nA =\n", a);
    //output_aligned(cout << "\nB =\n", bmat);
    //cout << "\nnonbasis = " << nonbasis;

    Matrix f, q, e;
    skeleton(a, f, q, e, ctrl::make_skeleton_slog(cout, false));

    matrix<rational<> > ff = f;
    for(int i = 0; i < ff.nrows(); ++i)
        if(!is_null(ff(i, 0)))ff.div_row(i, safe_reference(ff(i, 0)));

    output_aligned(cout << "\nff = \n", ff);

    //ff.erase_col(0);
    matrix<rational<> > xbdelta = transpose(matrix<rational<> >(ba_orig)*transpose(ff));
    output_aligned(cout << "\nxbdelta = \n", xbdelta);
    matrix<rational<> > xb = xbdelta/std::abs(det);
    output_aligned(cout << "\nxb = \n", xb);

    matrix<rational<> > pre_x = xb;

    for(size_t i = 0; i < ff.ncols(); ++i)
        pre_x.insert_col(pre_x.ncols(), ff.copy_col(i));

    output_aligned(cout << "\nx preorder = \n", pre_x);
    matrix<rational<> > x;

    Vector order;
    create_vector_order(basis, order, pre_x.ncols());
    Vector invorder;
    vec_inverse_permutation(order, invorder);

    mat_col_copy_order(pre_x, x, invorder);
    x.erase_col(0);

    output_aligned(cout << "\nx =\n", x);

    cout << "\n******* TRIANGULATION *********\n";
    output_aligned(cout << "\nQ from Skeleton = \n", q);

    matrix<int> tr;
    //triangulate_simple_1(q, tr);

    //output_aligned(cout << "\nsimplexes: \n", tr);
    cout << "This code cannot triangulate because a modification in base algorithm. Sorry.";

    //{
    //    typedef matrix<rational<> > MR;

    //    MR xN, v, q;

    //    xN = ff;
    //    xN.erase_col(0);

    //    MR xB = - MR(Bm1N)*transpose(xN);
    //    for(size_t i = 0; i < xB.nrows(); ++i)
    //        for(size_t j = 0; j < xB.ncols(); ++j)
    //            xB(i, j) += b[i];

    //    xB = transpose(xB);

    //    output_aligned(cout << "\nxN =\n", xN);
    //    output_aligned(cout << "\nxB =\n", xB);

    //}

    //output_aligned(cout << "\nff = \n", ff);
    //output_aligned(cout << "\nfff =\n", matrix<double>(ff));

    //output_aligned(cout << "\nB^(-1) = \n", inverse(matrix<rational<T> >(bmat)));
    //output_aligned(cout << "\n(b'|-A') =\n", ba_orig);
    //output_aligned(cout << "\nB^(-1)*(b'|-A') =\n", inverse(matrix<rational<T> >(bmat))*(ba_orig));
    //matrix<rational<T> > xout = transpose(inverse(matrix<rational<T> >(bmat))*(ba_orig)*transpose(f));
    //output_aligned(cout << "\n(B^(-1)*(b'|-A')*X')^T =\n", xout);

    //xout.insert_col(0, f.copy_col(0));
    //for(int i = 1; i < f.ncols(); ++i)
    //    xout.insert_col(xout.ncols(), f.copy_col(i));

    //output_aligned(cout << "\nX = \n", xout);
    //for(int i = 0; i < xout.nrows(); ++i)
    //    if(!is_null(xout(i, 0)))xout.div_row(i, safe_reference(xout(i, 0)));

    //output_aligned(cout << "\nX normalized =\n", xout);

}



void find_in_bounding_box
(
    const matrix<big_int> simplex, const big_int& det,
    Arageli::vector<big_int>& row_coefs, Arageli::vector<big_int>& res
)
{

    matrix<rational<> > rsimplex = simplex;
    vector<rational<> > up(rsimplex.ncols() - 1, -10000000), bot(rsimplex.ncols() - 1, 10000000);
    vector<rational<> > recup(rsimplex.ncols() - 1, 0), recbot(rsimplex.ncols() - 1, 0);
    for(size_t i = 0; i < rsimplex.nrows(); ++i)
    {
        if(rsimplex(i, 0) >= 1)
        {
            if(rsimplex(i, 0) > 1)rsimplex.div_row(i, safe_reference(rsimplex(i, 0)));

            for(size_t j = 0; j < up.size(); ++j)
            {
                rational<> curup = rsimplex(i, j+1), curbot = rsimplex(i, j+1);
                //std::cerr << "\nup = " << up << ", bot = " << bot;
                if(up[j] < curup)up[j] = curup;
                if(bot[j] > curbot)bot[j] = curbot;
            }
        }
        else if(rsimplex(i, 0) == 0)
        {
            // рецессивное направление:

            for(size_t j = 0; j < up.size(); ++j)
            {
                rational<> curup = rsimplex(i, j+1), curbot = rsimplex(i, j+1);
                if(recup[j] < curup)recup[j] = curup;
                if(recbot[j] > curbot)recbot[j] = curbot;
            }

        }
    }



    std::cerr << "\nup = " << up + recup << ", bot = " << bot + recbot << "\n";

    vector<big_int> rup = floor(up + recup), rbot = ceil(bot + recbot);

    std::cerr << "\nrup = " << rup << ", rbot = " << rbot << "\n";
    std::cerr << "Number of potential points: " << product(rup - bot + 1) << "\n";

    matrix<big_int> ainv = det*inverse(matrix<rational<> >(simplex));

    //output_aligned(cout << "\nSIMPLEX\n", ainv);

    vector<big_int> point = rbot;
    point.push_front(1);    // always

    for(;;)
    {
        vector<big_int> z = point*ainv;
        if(z == 0){ cerr << "ERROR1"; return; }
        if(Arageli::allcmp_vec_by_val(z, 0, std::greater_equal<big_int>()))
        {
            bool good = false;
            for(size_t i = 0; i < simplex.nrows(); ++i)
                if(simplex(i, 0) > 1 && z[i] != 0){ good = true; break; }

            if(good)
            {
                row_coefs = z;
                res = point;
                return;
            }
        }

        for(size_t i = 1; i < point.size(); ++i)
            if(++point[i] > rup[i-1])
                point[i] = rbot[i-1];
            else break;

        std::cerr << point;

        vector<big_int> tpoint = point;
        tpoint.erase(0);
        if(tpoint == rbot)break;
    }

    row_coefs.resize(0);
}


void picking_coefs
(
    const matrix<big_int> simplex, const big_int& det,
    Arageli::vector<big_int>& row_coefs, Arageli::vector<big_int>& res
)
{
    Arageli::vector<big_int> col_0 = simplex.copy_col(0), col_0_orig = col_0;
    std::replace(col_0.begin(), col_0.end(), big_int(), det+1);
    Arageli::vector<big_int> ups = det/col_0;
    if(is_null(col_0_orig))
    {
        cout << "ОШИБКА: одни рецессивные направления.#########################################";
        row_coefs.resize(0);
        return;
    }

    row_coefs.assign(col_0.size(), 0);
    row_coefs[0] = 1;

    Arageli::vector<int> nonintegers;
    for(size_t i = 0; i < simplex.nrows(); ++i)
        if(simplex(i, 0) > 1)nonintegers.push_back(i);

    cout << "\ndet = " << det << '\n';

    for(;;)
    {
        //cout << row_coefs << "\n";
        big_int sum = dotprod(row_coefs, col_0_orig);

        for(size_t i = 0; i < row_coefs.size(); ++i)
        {
            if(is_null(col_0_orig[i]))continue;
            ARAGELI_ASSERT_0(sum <= det);
            big_int delta = (det - sum)/col_0_orig[i];
            if(is_null(delta))
            {
                    sum -= row_coefs[i]*col_0_orig[i];
                    ARAGELI_ASSERT_0(!is_negative(sum));
                    row_coefs[i] = 0;
            }
            else
            {
                if(i == 0)
                    row_coefs[i] += delta;
                else
                    ++row_coefs[i];

                break;
            }
        }

        cerr << "\n" << row_coefs;

        if(is_null(row_coefs))break;

        if
        (
            dotprod(col_0_orig, row_coefs) == det &&
            !is_null(row_coefs.copy_subvector(nonintegers))
        )
        {
            // Проверяем систему сравнений.
            res.assign(simplex.ncols(), 0);
            for(size_t i = 0; i < simplex.nrows(); ++i)
                res += simplex.copy_row(i)*row_coefs[i];

            bool is_div = true;
            for(size_t i = 0; i < res.size(); ++i)
                if(!is_divisible(res[i], det)){ is_div = false; break; }

            if(is_div)
            {
                res /= det;
                return;
            }
        }

    }

    row_coefs.resize(0);
}




void solve_mod_2 (vector<big_int> cureq, big_int curb, big_int curmodule, matrix<big_int>& curres)
{
    ARAGELI_ASSERT_0(cureq.size() == 2);
    big_int first = cureq[0], alpha = cureq[1], gamma = curb;
    cout << "\nДвухмерная задача: ";
    for(;;)
    {
        cout << "\n(" << first << ", " << alpha << ") = " << gamma << " (mod " << curmodule << ")" << std::flush;

        if(gamma == 0)break;

        //alpha -= curmodule/2;
        if(first != 1)
        {
            // нормировка по первой переменной:
            big_int invfirst = inverse_mod(first, curmodule);
            cout
                << "\nНормируем по первой переменной: (" << first << "^(-1) = "
                << invfirst << "(mod " << curmodule << "))" << std::flush;
            first = first*invfirst%curmodule;
            alpha = alpha*invfirst%curmodule;
            gamma = gamma*invfirst%curmodule;

            cout << "\n(" << first << ", " << alpha << ") = " << gamma << " (mod " << curmodule << ")" << std::flush;
        }

        if(alpha == 1 || alpha == 0)break;
            // The Algorithm :)

            if(alpha > curmodule/2)
            {
                alpha -= curmodule;
                cout << "\n(" << first << ", " << alpha << ") = " << gamma << " (mod " << curmodule << ")" << std::flush;

                big_int newalpha = prrem(curmodule, -alpha);
                big_int newgamma = prrem(gamma - curmodule, -alpha);
                curmodule = -alpha;
                alpha = newalpha;
                gamma = newgamma;
            }
            else
            {
                big_int newalpha = prrem(-curmodule, alpha);
                gamma = prrem(gamma, alpha);
                curmodule = alpha;
                alpha = newalpha;
            }
    }
}
