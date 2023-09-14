# slog

[![CMake on multiple platforms](https://github.com/geoyee/slog/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/geoyee/slog/actions/workflows/cmake-multi-platform.yml)

一个用于 C++14 的简单捕获函数执行异常并记录日志的头文件，基于 CPLError 扩展。

## 使用

```cpp
#define _ENABLE_SLOG
#include "slog.h"

/* 普通函数 */
// int b = func(vec, a);
int b = SFUNCTION(func, vec, a);

/* 成员函数 */
// Class cls
// cls.func(a, b, c);
SCFUNCTION(cls, Class::func, a, b, c);

/* 在函数中 */
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
~ 2023/09/14 21:59:51
  [Function]    get(vec, 2)
  [Location]    \home\slog\sample.cpp (45)
  [Success]     It takes 18.142000 ms
a = 3
~ 2023/09/14 21:59:51
  [Function]    IntVec::init(vec)
  [Location]    \home\slog\sample.cpp (49)
  [Success]     It takes 0.000000 ms
init vec
~ 2023/09/14 21:59:51
  [Function]    IntVec::get(3, 4)
  [Location]    \home\slog\sample.cpp (51)
  [Failure]     vector::_M_range_check: __n (which is 7) >= this->size() (which is 5)
c = 0
- 2023/09/14 21:59:51
  [Function]    get2
  [Location]    \home\slog\sample.cpp (35)
  [Failure]     vector::_M_range_check: __n (which is 18446744073709551614) >= this->size() (which is 5)
d = -1
```
