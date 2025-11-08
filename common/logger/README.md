# log

#### 介绍
基于C++ spdlog日志库的二次封装<br>


 **支持以下功能** ：
1. 颜色控制台输出
2. 旋转日志文件输出     （存储多个文件，循环覆盖） 
3. 日期日志文件输出     （采用日期文件记录，在每天的指定时间生成一个日志文件）
4. 日期旋转日志文件输出  （采用日期文件记录，在每天的指定时间生成一个日志文件， 存储多个文件，循环覆盖）
4. 同步/异步输出模式   
6. 多logger输出     （ps: 可以形成多个日志文件，指定logger输出，方便多模块功能的日志记录）
<br><br>


#### 目录介绍
 **spdlog** ：spdlog日志库源码<br>
 **src** ：基于spdlog源码二次封装的代码<br>
 **test** ：测试代码<br>

#### 下载源码
```
git clone https://gitee.com/for_ten_years/log.git
cd log
git submodule update --init
```

#### windows编译静态库
```
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```


#### linux编译静态库
```
mkdir build
cd build
cmake ..
make
```

