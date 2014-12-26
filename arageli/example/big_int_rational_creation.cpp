#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::cin;
using std::endl;
using std::setprecision;


int main ()
{
    // Default constructor
    big_int zero;
    cout << "By default, constructor creates zero = "
        << zero << endl;

    // Define the big_int by a string
    big_int big_number =
        "101100111000111100001111100000111111000000";
    cout << "Really big integer: " << big_number << endl;

    // Input from a keyboard is analogous to defining by a string
    big_int number_from_input;
    cout << "Input a big integer: ";
    cin >> number_from_input;
    cout << number_from_input << endl;

    // Now create a rational number
    rational<> rat_zero;
    cout << "For rational numbers zero = "
        << rat_zero << endl;
    cout << "Its numerator is "
        << rat_zero.numerator() << endl;
    cout << "Its demonimator is "
        << rat_zero.denominator() << endl;

    // Rational number can be defined by a string
    rational<> fraction = "705/32768";

    // Input from a keabord is easy
    rational<> rational_number_from_input;
    cout << "Input any rational number: ";
    cin >> rational_number_from_input;
    cout << rational_number_from_input << endl;

    // You can assign integer numbers to rational variables
    rational<> rational_integer_number = number_from_input;
    cout << "The following rational number is whole: "
        << rational_integer_number << endl;

    // You can specify numerator and denominator
    rational<> number_pi(22, 7);
    cout << "Pi approximately is " << number_pi << endl;
    // You can assign integer and rational numbers to floating point variables
    double floating_point_pi = number_pi;
    cout << "The following is rough approximation of pi: "
        << setprecision(14) << floating_point_pi << endl;
    // pi = 3.141592653589793238462643383279502884197
    rational<> pi(355, 113);
    cout << "More precisely Pi is " << pi << " ~ "
        << setprecision(14) << double(pi) << endl;

    // You can get an access to numerator and denominator
        // of a rational number and you can change them.
        // After that perhaps the fraction must be reduced
    fraction.numerator() += number_from_input;
    cout << "Reducing the fraction: " <<  fraction << " = ";
        fraction.normalize();
    cout << fraction << endl;

    return 0;
}
