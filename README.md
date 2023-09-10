# slog

一个简单的头文件，用于捕获函数执行的异常并记录日志。主要在 GDAL 的 CPLErr 中使用，也可以单独使用。

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
```

## 示例输出

示例程序为[sample.cpp](./sample.cpp)，执行输出如下：

```shell
~ 2023/09/10 23:03:30
  [Function]    get(vec, 2)
  [Location]    E:\slog\sample.cpp (37)
  [Success]     It takes 13.001000 ms
a = 3
~ 2023/09/10 23:03:30
  [Function]    IntVec::init(vec)
  [Location]    E:\slog\sample.cpp (41)
init vec
  [Success]     It takes 0.000000 ms
~ 2023/09/10 23:03:30
  [Function]    IntVec::get(7)
  [Location]    E:\slog\sample.cpp (42)
  [Failure]     vector::_M_range_check: __n (which is 7) >= this->size() (which is 5)
b = 0
```

## 依赖

1. [\*GDAL](https://github.com/OSGeo/gdal)

_\*为非必须项_
