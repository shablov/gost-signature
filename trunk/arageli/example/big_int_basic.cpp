#include <iostream>
#include <iomanip>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;
using std::cin;
using std::endl;

void test(big_int a, big_int b, int echo = 1)
{
    if (echo)
    {
        cout << "a = " << a << endl;
        cout << "b = " << b << endl;
        cout << "length(a) = " << a.length() << endl;
        cout << "length(b) = " << b.length() << endl;
    }

    /*
    if (a <= 10000)
    if (echo) cout << a << "! = " << factorial(a) << endl;
    if (b <= 10000)
    if (echo) cout << b << "! = " << factorial(b) << endl;
    */

    big_int sum = a + b;
    big_int sub = a - b;
    big_int mul = a * b;
    big_int quo = a / b;
    big_int rem = a % b;
    //  big_int sqr = sqrt(a);

    if (echo) cout
        << "a+b     = " << sum << endl
        << "a-b     = " << sub << endl
        << "a*b     = " << mul << endl
        << "a/b     = " << quo << endl
        << "a%b     = " << rem << endl
        /*<< "sqrt(a) = " << sqr << endl*/;

    int error_count = 0;
    if (sum - b != a)
    {
        error_count++;
        cout << "error in addition \n";
    }
    if (sub + b != a)
    {
        error_count++;
        cout << "error in subtraction \n";
    }
    /*
    if (absolute_value(rem) > absolute_value(b) || a != b * quo + rem)
    {
        error_count++;
        cout << "error in division \n";
    }
    */
    /*
    if (sqr * sqr > a || (sqr + 1) * (sqr + 1) <= a)
    {
        error_count++;
        cout << "error in rooting \n";
    }
    */
    if (!error_count) cout << "ok\n";

}


int main()
{

    big_int a, b;

    cout
        << "Big arithmetics test\n"
        << "First of all, three special examples:\n";

    cout << "1-st" << endl;
    a = "100000000000000000000";
    b = "10000000000";
    test(a,b);

    cout << "2-nd" << endl;

    a = "88324001366761939746549495863516861064950065520957783943261528711385488"
        "25453005411168543038149209596356801605156065539368956745893369425384794"
        "78322831400288752675392703699495044198922023126453845881027767106658208"
        "72314333788431511433864242605473215830341951617902145503437983966122064"
        "932382766743322898389102160022195175260593675382";

    b = "36894823947562488362989727081483887800365382823662491";

    test(a,b,0);

    cout << "3-rd" << endl;

    a = "533324585685356541505558501104902242558042747327816503847822350303085910"
        "734027004147511443822571546330186490118760568255822929147341847563701615"
        "861962525483803479415309944879599591619922861967041623319146338305396338"
        "432948262213617625676564357028812775660522537721706893674787299338890104"
        "4275204880340763055855393748002806711757961433758136310906685845030008416";

    b = "492795842627912932954814370012435141701919872711274673189442383997743460"
        "695103452642552188307998619408058929346014856379838443669625449085032539";

    test(a,b,0);

    cout << "Now enter any numbers. For exit enter 0 0 \n";

    for(;;)
    {
        cout << '\n';
        cout << "a: ";
        cin >> a;
        cout << "b: ";
        cin >> b;
        cout << '\n';

        if (a == 0 && b == 0)break;

        test(a, b);
    }
    return 0;
}
