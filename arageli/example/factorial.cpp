#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;
using std::boolalpha;


int main ()
{
    // There is a universal function to compute factorial
    // of any positive integer number -- factorial.

    // If you are sure that the result of computation fits
    // some built-in integer type, say int, use it as an input.
    // So the following is correct if and only if 5! is
    // representable as int:
    int res_int = factorial(5);
    cout << "5! = " << res_int << endl;

    // If you suppose that the result will exceed any
    // built-in integer type, use big_int to correct factorial
    // computation:
    big_int res_big = factorial(big_int(50));
    cout << "50! = " << res_big << endl;

    // For small inputs (as 5 above) you can use big_int too,
    // but factorial with int takes less time than factorial
    // with big_int due to the big_int overhead.


    // Actually there are several functions to compute factorial
    // of a positive integer number. Let's consider two of them:
    // factorial_successive_multiplication based on simple
    // multiplication sequence from 2 to a given number,
    // and the algorithm based on 2s isolation --
    // factorial_even_odd_multiplication.

    big_int n = 20000;

    // Compute factorial of n with two algorithms and measure
    // how long it takes.

    cout
        << "Factorial of " << n << " is being computed.\n"
        << "It can take a long time; please wait." << endl;

    timer tm_fsm;
    big_int res_fsm = factorial_successive_multiplication(n);
    tm_fsm.stop();

    timer tm_feom;
    big_int res_feom = factorial_even_odd_multiplication(n);
    tm_feom.stop();

    // The results should be the same:
    cout
        << "Both the algorithms give the same result: "
        << boolalpha << (res_fsm == res_feom) << '\n'
        << "The result contains " << nbits(res_fsm) << " bits." << endl;

    // The difference is that the first algorithm relay only on
    // multiplication and increment but the second requires
    // division, shift and other operations.

    // But for big numbers (such as our n) the second one takes
    // much less time than the first one:
    cout
        << "factorial_successive_multiplication took "
        << tm_fsm.time() << " seconds,\n"
        << "factorial_even_odd_multiplication took "
        << tm_feom.time() << " seconds.\n"
        << "The second algorithm is " << tm_fsm.time()/tm_feom.time()
        << " times as fast than the first one." << endl;

    // Function factorial just calls the fastest function for
    // a given number.
}
