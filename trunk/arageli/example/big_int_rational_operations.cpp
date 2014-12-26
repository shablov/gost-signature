#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;
using std::setprecision;


int main ()
{
    // Operations with big_int can be performed
    // as with standard C++ types.
    // You can mix in one operation operands of different types
    big_int day_seconds = (big_int(60) * 60) * 24;
    cout << "A day contains " << day_seconds <<" seconds"<< endl;
    big_int year_seconds = day_seconds * 365;
    cout << "A year contains " << year_seconds <<" seconds"<< endl;
    big_int leap_year_seconds = year_seconds + day_seconds;
    cout << "But a leap year contains " << leap_year_seconds
        << " seconds" << endl;

    if((big_int("12345678987654321") % big_int(111)).is_null())
    {
        cout << "12345678987654321 is divisible by 111" << endl;
        cout << "The quatient is "
            << big_int("12345678987654321") / big_int(111) << endl;
    }
    else
        cout << "12345678987654321 is not divisible over 111" << endl;

    big_int first_big_number  = power(big_int(23), 32);
    big_int second_big_number = power(big_int(32), 23);
    if(first_big_number < second_big_number)
        cout << "We stated that 23^32 < 32^23" << endl;
    else
        if(first_big_number > second_big_number)
            cout << "We stated that 32^23 < 23^32" << endl;
        else
            if(first_big_number == second_big_number)
                cout << "Too good to be true!" << endl;
            else
                cout << "It's very lovely!" << endl;

    cout << "2^64 = " << (big_int(1) << 64) << endl;

    // Working with rationals is easy too
    rational<> sqrt_2 = "1/2";
    for(int i = 0; i < 10; i++)
        sqrt_2 = 1 / (2 + sqrt_2);
    // You can also use (sqrt_2 += 2).inverse();
    sqrt_2 += 1;
    cout << "sqrt(2) is approx. " << sqrt_2 << endl;
    rational<> two = square(sqrt_2);
    if(two.is_integer())
        cout << "It couldn't be further from the truth";
    cout << "(" << sqrt_2 << ")^2 = " << two << endl;
    cout
        << two << " is approx. "
        << setprecision(16) << double(two) << endl;

    return 0;
}
