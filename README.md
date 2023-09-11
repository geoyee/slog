# slog

一个简单的头文件，用于捕获函数执行的异常并记录日志。

## 使用

```cpp
#define _DEBUG
#include "slog.h"

/* 普通函数 */
// int b = func(vec, a);
int b = SL(func, vec, a);

/* 成员函数 */
// Class cls
// cls.func(a, b, c);
SLC6(cls, Class::func, a, b, c);

/* 在函数中 */
float func(float a, float b)
{
  SSF
    return a / b;
  SEF(0.0)  // 失败返回的默认值
}
```

## 示例输出

示例程序为[sample.cpp](./sample.cpp)，执行输出如下：

```shell
~ 2023/09/11 22:39:02
  [Function]    get(vec, 2)
  [Location]    E:\dataFiles\github\slog\sample.cpp (45)
  [Success]     It takes 12.000000 ms
a = 3
~ 2023/09/11 22:39:02
  [Function]    IntVec::init(vec)
  [Location]    E:\dataFiles\github\slog\sample.cpp (49)
  [Success]     It takes 0.000000 ms
init vec
~ 2023/09/11 22:39:02
  [Function]    IntVec::get(7)
  [Location]    E:\dataFiles\github\slog\sample.cpp (51)
  [Failure]     vector::_M_range_check: __n (which is 7) >= this->size() (which is 5)
b = 0
- 2023/09/11 22:39:02
  [Function]    get2
  [Location]    E:\dataFiles\github\slog\sample.cpp (35)
  [Failure]     vector::_M_range_check: __n (which is 18446744073709551614) >= this->size() (which is 5)
e = -1
```

## 依赖

1. [\*GDAL](https://github.com/OSGeo/gdal)

_\*为非必须项_
