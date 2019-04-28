## 多线程pthread库的安装心得

by czl

2019/04/28

因为视觉识别比较慢的原因，我们加入了基于C语言的双线程工作

虽然threads库已出，但是教程较少，所以还是选择了pthread库

> * windows10 VS2017环境下库的安装

点击“项目”-“管理 ** 程序包”搜索并安装pthread库即可

> * 还需要一些依赖项

网上教程十分复杂，亲测发现根本不用这么复杂。只需要两个文件：pthreadVC2.lib和pthreadVC2.dll

将lib文件放入cpp文件同目录下，将dll文件放入生成的exe同目录下即可。

由于使用了OpenCV，所以我们的依赖项文件是针对×64的

如果要使用基于32位程序（默认）或是其他什么的可以访问[某镜像网站](http://www.mirrorservice.org/sites/sourceware.org/pub/pthreads-win32)

选择prebuilt-dll-2-9-1-release文件夹，在里面找到32位的dll和lib文件并做同样操作即可。

> * demo已附上

----------------------------

### One More Thing

pthread.h中会报错：C2011 “timespec”:“struct”类型重定义

* * 解决方法：

双击错误提示进入定义文件，在该文件比较前面的位置

```C++

#if !defined( PTHREAD_H )

#define PTHREAD_H

```
下面加上

```C++

#define HAVE_STRUCT_TIMESPEC

```
即可
