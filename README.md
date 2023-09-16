# slog

[![CMake on multiple platforms](https://github.com/geoyee/slog/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/geoyee/slog/actions/workflows/cmake-multi-platform.yml)

一个用于 C++14 的简单捕获函数执行异常并记录日志的头文件，基于 CPLError 扩展。

## 使用

```cpp
#define _ENABLE_SLOG
#include "slog.h"

/* 装饰函数 */
// int b = func(vec, a);
auto new_func = SFUNC_DEC(func);
int b = new_func(vec, a);

/* 装饰成员函数，使用bind */
// Class cls
// cls.func(a, b, c);
// auto new_func = SFUNC_BIND_DEC(
//   std::bind(&Class::func, &cls, std::placeholders::_1, std::placeholders::_2));
// new_func(a, b);

/* 运行函数 */
// int b = func(vec, a);
int b = SFUNC_RUN(func, vec, a);

/* 运行成员函数 */
// Class cls
// cls.func(a, b, c);
SFUNC_MEM_RUN(cls, Class::func, a, b);

/* 写在函数中 */
float func(float a, float b)
{
  SENTRY
    return a / b;
  SLEAVE(0.0)  // 失败返回的默认值
}
```

## 示例输出

示例程序为[sample.cpp](./sample.cpp)，执行输出如下：

```shell
~ 2023/09/16 11:06:31
  [Function]    get(vec, 2)
  [Location]    E:\slog\sample.cpp (45)
  [Success]     It takes 13.000000 ms
a = -3.14
~ 2023/09/16 11:06:31
  [Function]    get(...)
  [Location]    E:\slog\sample.cpp (48)
  [Failure]     vector::_M_range_check: __n (which is 20) >= this->size() (which is 5)
b = nan
~ 2023/09/16 11:06:31
  [Function]    RealVec::init(vec)
  [Location]    E:\slog\sample.cpp (53)
  [Success]     It takes 0.000000 ms
init vec
~ 2023/09/16 11:06:31
  [Function]    RealVec::get(3, 4)
  [Location]    E:\slog\sample.cpp (55)
  [Failure]     vector::_M_range_check: __n (which is 7) >= this->size() (which is 5)
c = nan
- 2023/09/16 11:06:31
  [Function]    get2
  [Location]    E:\slog\sample.cpp (35)
  [Failure]     vector::_M_range_check: __n (which is 18446744073709551614) >= this->size() (which is 5)
e = nan
```

## 参考

1. [\*GDAL\CPL](https://github.com/OSGeo/gdal/tree/master/port)
2. [if_constexpr14](https://github.com/Garcia6l20/if_constexpr14)
