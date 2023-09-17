#define _ENABLE_SLOG

#include <iostream>
#include <vector>
#include "slog.h"

using reals = std::vector<double>;

double get(reals vec, int index)
{
    for (int i = 0; i < 10000000; ++i)
        ;
    return vec.at(index);
}

class RealVec
{
private:
    reals _vec;

public:
    RealVec() {}
    void init(reals vec)
    {
        _vec = vec;
    }
    double get(int index, int add)
    {
        return _vec.at(index + add);
    }
};

double get2(reals vec, int index)
{
    SENTRY

    return vec.at(index);

    SLEAVE(std::numeric_limits<double>::quiet_NaN())
};

void change(int *a, int b)
{
    VALIDATE_ARGUMENT0(a, __FUNCTION__);
    VALIDATE_ARGUMENT0(b, __FUNCTION__);

    *a = b;
};

int main(int argc, char *argv[])
{
    reals vec = {1.52, 2.33, -3.14, 0.44, 90.18};
    double a = SFUNC_RUN(get, vec, 2);
    printf("a = %g\n", a);

    auto new_get = SFUNC_DEC(get);
    double b = new_get(vec, 20);
    printf("b = %g\n", b);

    RealVec rv;
    SFUNC_MEM_RUN(rv, RealVec::init, vec);
    printf("init vec\n");
    double c = SFUNC_MEM_RUN(rv, RealVec::get, 3, 4);
    printf("c = %g\n", c);

    auto new_mem_get = SFUNC_MEM_DEC(rv, RealVec::get);
    double d = new_mem_get(1, 3);
    printf("d = %g\n", d);

    double e = get2(vec, -2);
    printf("e = %g\n", e);

    int *f = nullptr;
    change(f, 3);

    return 0;
}