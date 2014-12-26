#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::endl;


int main ()
{
    vector<int> a = "(3)";
    int x = a[0];

    cout << "a = " << a << endl;
    cout << "x = " << x << endl;

    // Let's insert new entries
    a.push_front(1);
    a.push_back(5);
    a.insert(1, 2, x);
    cout << "a (after inserting new entries) = " << a << endl;

    // Now let's change the middle entry
    int m_index = (a.size())/2;
    a[m_index] = -1;
    cout << "a (after changing the middle entry) = " << a << endl;

    // Remove all entries equaled to @x@
    a.remove(x);
    cout << "a (after removing all entries equaled to x) = "
        << a << endl;

    // Swap the first and last entries
    a.swap_els(0, a.size() - 1);
    cout<<"\na (after swapping the first and the last entries) = "
        << a << endl;

    return 0;
}

