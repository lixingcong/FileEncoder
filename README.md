转换文本编码，并进行正则替换

## 效果

测试文本 gbk.txt 编码为GB2312
```
STR("hello"), STR("world")
DDD("hello"), FFF("world")
```

输出结果 utf8.txt 编码为UTF8
```
MYSTR("hello"), MYSTR("world")
DDD("hello"), FFF("world")
```

## 编译

要求C++11，GCC4.9以上，MSVC为VS2013以上

|依赖|Windows|Linux|
|--|--|--|
|libiconv|[libiconv-win-build](https://github.com/kiyolee/libiconv-win-build)|```./configure --host=x86_64-linux-gnu --prefix=/path/to/install --disable-shared --enable-static```|

将静态库\(\*.a或者\*.lib\)放置到lib目录下，include复制到include下。

Linux

```
mkdir build && cd build
cmake ..
make
```

Windows

```
md build && cd build
cmake ..
# 使用Visual Studio打开sln文件编译
```

## 运行

Linux

```
# 用shell会有美元转义，因此单引号
./FileEncoder gbk.txt utf8.txt gb2312 utf-8 '(STR)(\(\".*?\"\)):MYSTR$2'
```

Windows

```
# 建议写入到1.cmd文件运行，用PowerShell会有美元转义
.\FileEncoder.exe gbk.txt utf8.txt gb2312 utf-8 "(STR)(\(\"".*?\""\)):MYSTR$2"
```

正则表达式替换不是必要的，可以省略该参数，只进行文件编码转换

```
./FileEncoder gbk.txt utf8.txt gb2312 utf-8
```

正则表达式替换可以先后进行多个，以逗号和分号间隔

```
# 先替换abc为xyz，再替换123为456
./FileEncoder gbk.txt utf8.txt gb2312 utf-8 'abc:xyz;123:456'
```
