#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    typedef rational<> Q;

    matrix<Q> A, B;
    vector<Q> c, d, res;
    Q alpha;
    int beta;

    A = "((-1/2, 3/4), (-2/3, 5), (1/7, -5/2))";
    B = "((3/4, 1/6, -7/8), (5/2, 2/5, -9/10))";

    c = "(1/4, -4/15, 5)";
    d = "(-2/3, -1, 4)";

    alpha = Q(1, 120);
    beta = -2;

    res = ((A*B)*c - d*alpha)/beta;
    cout << "Result:" << endl;
    output_aligned(cout, res);

    return 0;
}
