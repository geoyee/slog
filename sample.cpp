#define _ENABLE_SLOG

#include <iostream>
#include <vector>
#include "slog.h"

using ints = std::vector<int>;

int get(ints vec, int index)
{
    for (int i = 0; i < 10000000; ++i)
        ;
    return vec.at(index);
}

class IntVec
{
private:
    ints _vec;

public:
    IntVec() {}
    void init(ints vec)
    {
        _vec = vec;
    }
    int get(int index, int add)
    {
        return _vec.at(index + add);
    }
};

int get2(ints vec, int index)
{
    SENTRY

    return vec.at(index);

    SLEAVE(-1)
};

int main(int argc, char *argv[])
{
    ints vec = {1, 2, 3, 4, 5};
    int a = SFUNCTION(get, vec, 2);
    printf("a = %d\n", a);

    IntVec iv;
    SCFUNCTION(iv, IntVec::init, vec);
    printf("init vec\n");
    int c = SCFUNCTION(iv, IntVec::get, 3, 4);
    printf("c = %d\n", c);

    int d = get2(vec, -2);
    printf("d = %d\n", d);

    return 0;
}