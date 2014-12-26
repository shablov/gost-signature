#include <iostream>
#include <arageli/arageli.hpp>

using namespace Arageli;
using std::cout;


int main ()
{
    const big_int seed = 1298347343;
    big_int x = seed;
    unsigned int y, z;
    y = x/big_int("1000000000");
    int i;
    for (i = 0; i < y; ++i)
    {
        z = (x/big_int("100000000")) % 10;
        switch (z)
        {
            case 0:
                goto K3;
                break;
            case 1:
                goto K4;
                break;
            case 2:
                goto K5;
                break;
            case 3:
                goto K6;
                break;
            case 4:
                goto K7;
                break;
            case 5:
                goto K8;
                break;
            case 6:
                goto K9;
                break;
            case 7:
                goto K10;
                break;
            case 8:
                goto K11;
                break;
            case 9:
                goto K12;
                break;
        }
    K3:
        if (x < big_int("5000000000")) x += big_int("5000000000");
    K4:
        x = ((x * x) / 100000) % big_int("10000000000");
    K5:
        x = (big_int("1001001001") * x) % big_int("10000000000");
    K6:
        if (x < big_int("100000000"))
        {
            x += big_int("9814055677");
        }
        else
        {
            x = big_int("10000000000") - x;
        }
    K7:
        x = 100000 * (x % 100000) + x / 100000;
    K8:
        x = (big_int("1001001001") * x) % big_int("10000000000");
    K9:
        // figure reduction
        int j;
        for(j = 0; j < 10; ++j)
        {
            if ((x / pow(10,j)) % 10) x -= pow(10,j);
        }
    K10:
        if (x < 100000)
        {
            x = x * x + 99999;
        }
        else
        {
            x = x - 99999;
        }
    K11:
        if (x < big_int("1000000000"))
        {
            x *= 10;
            goto K11;
        }
    K12:
        x = ((x * (x - 1)) / 100000) % big_int("10000000000");
    }

    cout << x << '\n';

    return 0;
}
