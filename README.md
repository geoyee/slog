# slog

[![CMake on multiple platforms](https://github.com/geoyee/slog/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/geoyee/slog/actions/workflows/cmake-multi-platform.yml)

基于 C++14 的简单函数异常捕获和记录头文件。

## 使用

- [使用文档](doc.md)
- [示例](sample.cpp)

示例输出如下：

```
~ 2023/09/17 16:48:52
  [Function]    get(vec, 2)
  [Location]    E:\slog\sample.cpp (53)
  [Success]     It takes 15.000000 ms
a = -3.14
~ 2023/09/17 16:48:52
  [Function]    get(...)
  [Location]    E:\slog\sample.cpp (56)
  [Failure]     vector::_M_range_check: __n (which is 20) >= this->size() (which is 5)
b = nan
~ 2023/09/17 16:48:52
  [Function]    RealVec::init(vec)
  [Location]    E:\slog\sample.cpp (61)
  [Success]     It takes 0.000000 ms
init vec
~ 2023/09/17 16:48:52
  [Function]    RealVec::get(3, 4)
  [Location]    E:\slog\sample.cpp (63)
  [Failure]     vector::_M_range_check: __n (which is 7) >= this->size() (which is 5)
c = nan
~ 2023/09/17 16:48:52
  [Function]    RealVec::get(...)
  [Location]    E:\slog\sample.cpp (66)
  [Success]     It takes 0.000000 ms
d = 90.18
- 2023/09/17 16:48:52
  [Function]    get2
  [Location]    E:\slog\sample.cpp (35)
  [Failure]     vector::_M_range_check: __n (which is 18446744073709551614) >= this->size() (which is 5)
e = nan
- 2023/09/17 16:48:52
  [Function]    change
  [Location]    E:\slog\sample.cpp (44)
  [Failure]     Argument 'a' is NaN
```

## 参考

- [\*GDAL/CPL](https://github.com/OSGeo/gdal/tree/master/port)
- [if_constexpr14](https://github.com/Garcia6l20/if_constexpr14)
