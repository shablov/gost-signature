/*
    Note for Microsoft Visual Studio users.

    To run this example directly from MSVS IDE you should
    set correct value for working directory path.
    In MSVS 2005 IDE go to adv_simple_method Property Pages ->
    Configuration Properties -> Debugging.
    Set Working Directory field to "..\..\..\example" (without quotes).
*/

#include <iostream>
#include <fstream>
#include <arageli/arageli.hpp>

using namespace Arageli;
using ctrl::simplex_method::adv_simplex_method_alg_slog;
using std::ofstream;
using std::cout;

void SolveFromFile()
{
    ofstream outfile("../example/adv_simplex_method.task.output1.txt");

    adv_simplex_method_alg_slog<ofstream>
        ctrl_sm_slog(outfile);

    simplex_method::adv_simplex_method_alg
    <
        rational<>,
        adv_simplex_method_alg_slog<ofstream>
    >
        sm(ctrl_sm_slog);

    try
    {
        sm.LoadTaskFromFile("../example/adv_simplex_method.task.txt");
    }
    catch(exception& e)
    {
        ARAGELI_EXCEPT_LOC(e);
        cout
            << "\nSomething wrong happened while reading the file."
            << "\nDetailes:\n" << e;
        return;
    }

    sm.SimplexDriver();
}

void SolveFromInput()
{
    ofstream outfile("../example/adv_simplex_method.task.output2.txt"); // output file

    adv_simplex_method_alg_slog<ofstream>
        ctrl_sm_slog(outfile); // controller

    simplex_method::adv_simplex_method_alg
    <
        rational<>,
        adv_simplex_method_alg_slog<ofstream>
    >
        sm(ctrl_sm_slog);

    int i = 0;

    matrix<rational<> > A = "((1,-1,1,-1,1,-1),(2,1,-2,-3,2,3),(3,2,-1,-4,3,2))";
    vector<rational<> > c = "(1,1,1,-2,1,-2)";
    vector<rational<> > lower_bounds(9), upper_bounds(9);
    vector<bool> is_lower_bounds_inf(9); // flags for lower bound infinity
    vector<bool> is_upper_bounds_inf(9); // flags for upper bound infinity
    for (i = 0; i < 9; i++)
    {
        is_lower_bounds_inf[i] = false;
        if (i < 3)
            is_upper_bounds_inf[i] = false;
        else
            is_upper_bounds_inf[i] = true;
    }

    lower_bounds[0] = upper_bounds[0] = 7;
    lower_bounds[1] = upper_bounds[1]  = 0;
    lower_bounds[2] = upper_bounds[2]  = 10;
    for (i = 3; i < 9; i++)
        lower_bounds[i] = upper_bounds[i] = 0;

    // order is important!!!
    sm.SetConstraintMatrix(A);
    sm.SetObjFuncCoeffs(c);
    sm.SetOptDirection(simplex_method::DIR_MAXIMIZE);
    sm.SetBounds(lower_bounds, upper_bounds);
    sm.SetIsBoundsInf(is_lower_bounds_inf, is_upper_bounds_inf);

    sm.SimplexDriver();
}


int main()
{
    SolveFromFile();
    SolveFromInput();
    return 0;
}
