# 使用文档

### SINFO

`SINFO(eErrClass, err_no, ...)`

宏函数，用于打印调试信息，与 CPLError 使用方法相同。

返回值：无

参数：

- ` eErrClass`: 错误类型，可选值为 `CPLErr`中的值
- ` err_no`: 错误码，可选值为 `CPLErrorNum`中的值
- `...`: 变长参数，错误信息，使用占位符和其他参数实现

例子：

```cpp
SINFO(CE_Fatal, CPLE_AppDefined, "[Fatal] Unknown exception");
```

输出：

```
[Fatal] Unknown exception
```

### NaN

`NaN<typename>()`

模板函数，用于获取不同类型的 `NaN` 值。

返回值：与`typename`相同类型的`NaN`值

参数：无

例子：

```cpp
double d = NaN<double>();
std::cout << "d = " << d << std::endl;
```

输出：

```
d = nan
```

### VALIDATE_ARGUMENT0

`VALIDATE_ARGUMENT0(arg, func)`

宏函数，用于判断输入参数`arg`是否为 `NaN`，如果是则打印错误信息，与 CPLError 中的`VALIDATE_POINTER0`使用方法相同。

返回值：无

参数：

- `arg`: 需要进行判断的参数
- `func`: 当前的函数名

例子：

```cpp
void func(double *d)
{
    VALIDATE_ARGUMENT0(d, "func");
    ...
}

double *d = nullptr;
func(d);
```

输出：

```
- [2020-12-30 16:00:00]
  [Function]  func
  [Location]  slog/test/test.cpp (10)
  [Failure]   Argument 'd' is NaN
```

### VALIDATE_ARGUMENT1

`VALIDATE_ARGUMENT0(arg, func, ret)`

宏函数，用于判断输入参数`arg`是否为 `NaN`，如果是则打印错误信息，并返回默认值`ret`，与 CPLError 中的`VALIDATE_POINTER1`使用方法相同。

返回值：`ret`

参数：

- `arg`: 需要进行判断的参数
- `func`: 当前的函数名
- `ret`: 判断失败后的默认返回值

例子：

```cpp
double func(double *d)
{
    VALIDATE_ARGUMENT1(d, "func", 0.0);
    ...
}

double *d = nullptr;
double r = func(d);
std::cout << "r = " << r << std::endl;
```

输出：

```
- [2020-12-30 16:00:00]
  [Function]  func
  [Location]  slog/test/test.cpp (10)
  [Failure]   Argument 'd' is NaN
r = 0
```

### SENTRY & SLEAVE

`SENTRY`
`SLEAVE(ret)`

一对宏定义，用于在函数入口和出口进行包裹，进行异常捕获，并打印相关信息。

返回值：`ret`

参数：

- `ret`: 捕捉异常后的默认返回值

例子：

```cpp
int func(int i)
{
    SENTRY;
    std::vector<int> v = {1, 2, 3, 4, 5};
    return v.at(i);
    SLEAVE(233);
}

int r = func(-1);
std::cout << "r = " << r << std::endl;
```

输出：

```
- [2020-12-30 16:00:00]
  [Function]  func
  [Location]  slog/test/test.cpp (10)
  [Failure]   vector::_M_range_check: __n (which is 18446744073709551614) >= this->size() (which is 5)
r = 233
```

### SFUNC_DEC

`SFUNC_DEC(func)`

宏函数，用于对普通函数进行装饰，为该函数增加异常捕获和打印相关信息的功能。

返回值：`std::function`

参数：

- `func`: 待装饰的函数

例子：

```cpp
int func(int i)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    return v.at(i);
}

auto new_func = SFUNC_DEC(func);
int r = new_func(-1);
```

输出：

```
~ [2020-12-30 16:00:00]
  [Function]  func(...)
  [Location]  slog/test/test.cpp (10)
  [Failure]   vector::_M_range_check: __n (which is 18446744073709551614) >= this->size() (which is 5)
```

### SFUNC_MEM_DEC

`SFUNC_MEM_DEC(obj, func)`

宏函数，用于对成员函数进行装饰，为该成员函数增加异常捕获和打印相关信息的功能。

返回值：`std::function`

参数：

- `obj`: 成员函数的对象
- `func`: 待装饰的成员函数

例子：

```cpp
class A
{
public:
    int func(int i)
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        return v.at(i);
    }
};

A *a = new A();
auto new_func = SFUNC_MEM_DEC(a, A::func);
int r = new_func(-1);
```

输出：

```
~ [2020-12-30 16:00:00]
  [Function]  A::func(...)
  [Location]  slog/test/test.cpp (10)
  [Failure]   vector::_M_range_check: __n (which is 18446744073709551614) >= this->size() (which is 5)
```

### SFUNC_RUN

`SFUNC_RUN(func, ...)`

宏函数，与`SFUNC_DEC`相似，用于运行普通函数，为普通函数增加进行异常捕获，并打印相关信息等功能。

返回值：与`func`返回值相同

参数：

- `func`: 待运行的函数
- `...`: 变长参数，`func`函数的输入参数

例子：

```cpp
int func(int i)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    return v.at(i);
}

int r = SFUNC_RUN(func, -1);
```

输出：

```
~ [2020-12-30 16:00:00]
  [Function]  func(-1)
  [Location]  slog/test/test.cpp (10)
  [Failure]   vector::_M_range_check: __n (which is 18446744073709551614) >= this->size() (which is 5)
```

### SFUNC_MEM_RUN

`SFUNC_MEM_RUN(obj, func, ...)`

宏函数，与`SFUNC_MEM_DEC`相似，用于运行成员函数，为成员函数增加进行异常捕获，并打印相关信息等功能。

返回值：与`obj.func`返回值相同

参数：

- `obj`: 成员函数的对象
- `func`: 待运行的成员函数
- `...`: 变长参数，`func`函数的输入参数

例子：

```cpp
class A
{
public:
    int func(int i)
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        return v.at(i);
    }
};

A *a = new A();
int r = SFUNC_MEM_RUN(a, A::func, -1);
```

输出：

```
~ [2020-12-30 16:00:00]
  [Function]  A::func(-1)
  [Location]  slog/test/test.cpp (10)
  [Failure]   vector::_M_range_check: __n (which is 18446744073709551614) >= this->size() (which is 5)
```

### \_ENABLE_SLOG

`_ENABLE_SLOG`

宏定义，用于控制是否启用其他宏函数的异常捕获或打印。

返回值：无

参数：无

例子：

```cpp
#define _ENABLE_SLOG
#include "slog.h"

...
```

输出：无
