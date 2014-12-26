#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::cin;
using std::endl;
using std::boolalpha;


namespace
{

void the_first_simple_example ()
{
    cout << "*************************************\n";
    cout << "A simple example with sparse_polynom.\n";

    sparse_polynom<rational<> >
        f = "1/7*x^13-5*x^8-7*x^5+10",
        g = "-11*x^3+22/17*x^2-x";

    do
    {
        cout
            << "\nf(x) = " << f
            << "\ng(x) = " << g
            << "\nf(x)*g(x) = " << f*g
            << "\nf(x)+g(x) = " << f + g
            << "\nf(x)/g(x) = " << f/g
            << "\nf(x)%g(x) = " << f%g
            << "\nGCD(f(x), g(x)) = " << gcd(f, g)
            << "\ndividing is valid: " << boolalpha << ((f/g)*g + f%g == f);

        cout << "\n\nPlease, input two polynomials (f and g).\nInput f = 0 and g = 0 for exit.";
        cout << "\nf = "; cin >> f;
        cout << "g = "; cin >> g;
    }while(!is_null(f) || !is_null(g));
}


class GF2
{
private:

    unsigned char val;

public:

    GF2 () :
        val(0)
    {}

    GF2 (int x) :
        val(x&1)
    {}

    operator int () const
    {
        return val;
    }

    GF2 operator- ()
    {
        return val;
    }

    GF2& operator+= (const GF2& b)
    {
        val = (val+b)&1;
        return *this;
    }

    GF2& operator-= (const GF2& b)
    {
        val = (val-b)&1;
        return *this;
    }

    GF2& operator*= (const GF2& b)
    {
        val = (val*b)&1;
        return *this;
    }

    GF2& operator/= (const GF2& b)
    {
        val = (val/b)&1;
        return *this;
    }
};

inline GF2 operator+ (const GF2& a, const GF2& b)
{
    return GF2(int(a) + int(b));
}

inline GF2 operator- (const GF2& a, const GF2& b)
{
    return GF2(int(a) - int(b));
}

inline GF2 operator* (const GF2& a, const GF2& b)
{
    return GF2(int(a) * int(b));
}

inline GF2 operator/ (const GF2& a, const GF2& b)
{
    return GF2(int(a) / int(b));
}

inline std::istream& operator>> (std::istream& in, GF2& a)
{
    int b;
    in >> b;
    if(!in.bad() && !in.fail())
        a = b;
    return in;
}

inline std::ostream& operator<< (std::ostream& out, GF2& a)
{
    return out << int(a);
}

// The following several functions are necessary to correct printing of
// polynomial with coefficient from GF2.

inline bool is_negative (const GF2& a)
{
    return false;
}

inline bool is_opposite_unit (const GF2& a)
{
    return false;
}


void over_GF2 ()
{
    cout << "****************************************\n";
    cout << "Manipulations with sparse_polynom<GF2>.\n";

    typedef sparse_polynom<GF2> SPB;
    SPB
        f = "x^13-x^8-x^5+1",
        g = "x^3+x^2-x";

    do
    {
        cout
            << "\nf(x) = " << f
            << "\ng(x) = " << g
            << "\nf(x)*g(x) = " << f*g
            << "\nf(x)+g(x) = " << f + g
            << "\nf(x)/g(x) = " << f/g
            << "\nf(x)%g(x) = " << f%g
            << "\nGCD(f(x), g(x)) = " << gcd(f, g)
            << "\ndividing is valid: " << boolalpha << ((f/g)*g + f%g == f);

        cout << "\n\nPlease, input two polynomials (f and g).\nInput f = 0 and g = 0 for exit.";
        cout << "\nf = "; cin >> f;
        cout << "g = "; cin >> g;
    }while(!is_null(f) || !is_null(g));
}


void the_second_simple_example ()
{
    cout << "*******************************************\n";
    cout << "Polynomial over matrices and substitution.\n";

    sparse_polynom<matrix<big_int> >
        f = "(((3,-5),(0,7))*x^8+((0,1),(0,-8))*x^3+((-1,9),(13,2)))";

    matrix<sparse_polynom<big_int> >
        x = "((x,0),(0,x))";

    cout
        << "f(x) = " << f
        << "\nx = " << x
        << "\nthe resulting matrix is \n";

    output_aligned(cout, f.subs(x), "||", "||");

    cin.get();
}

}


int main ()
{
    //the_first_simple_example();
    over_GF2();
    the_second_simple_example();
}
