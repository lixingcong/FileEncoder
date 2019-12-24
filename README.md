## 小脚本

|依赖|Windows|Linux|
|--|--|--|
|libiconv|[libiconv-win-build](https://github.com/kiyolee/libiconv-win-build)|```./configure --host=x86_64-linux-gnu --prefix=/tmp/SYS --disable-shared --enable-static```|

将静态库\(\*.a或者\*.lib\)放置到lib目录下，include复制到include下。

```
mkdir build && cd build
cmake ..
```
