#define _DEBUG

#include <iostream>
#include <vector>
#include "slog.h"

using ints = std::vector<int>;

int get(ints vec, int index)
{
    return vec.at(index);
}

int main(int argc, char *argv[])
{
    ints vec = {1, 2, 3, 4, 5};
    int a = SL(get, vec, 2);
    printf("a = %d\n", a);
    int b = SL(get, vec, 7);
    printf("b = %d\n", b);

    return 0;
}