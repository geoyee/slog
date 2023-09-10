# slog

A simple C++ exception capture and log library for `CPLError` based on GDAL, only one header file.

## how to use

```cpp
#define _DEBUG
#include "slog.h"

// int b = func(vec, a);
int b = SL(func, vec, a);
```

## output of sample

```shell
~ 2023/09/10 18:41:21
  [Function]    get(vec, 2)
  [Location]    E:\slog\sample.cpp (17)
  [Success]     It takes 0.000000 ms
a = 3
~ 2023/09/10 18:41:21
  [Function]    get(vec, 7)
  [Location]    E:\slog\sample.cpp (19)
  [Failure]     vector::_M_range_check: __n (which is 7) >= this->size() (which is 5)
b = 0
```
