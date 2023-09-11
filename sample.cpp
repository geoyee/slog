#define _SLOG_DEBUG

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
    int get(int index)
    {
        return _vec.at(index);
    }
};

int get2(ints vec, int index)
{
    SSF

        return vec.at(index);

    SEF(-1)
};

int main(int argc, char *argv[])
{
    ints vec = {1, 2, 3, 4, 5};
    int a = SL(get, vec, 2);
    printf("a = %d\n", a);

    IntVec iv;
    SLC6(iv, IntVec::init, vec);
    printf("init vec\n");
    int d = SLC6(iv, IntVec::get, 7);
    printf("b = %d\n", d);

    int e = get2(vec, -2);
    printf("e = %d\n", e);

    return 0;
}