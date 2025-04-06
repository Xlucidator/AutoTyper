# AutoTyper

**switch [English](README_en.md)**

AcWing上有些题没买课无法使用，但是在AC Saber测试模式的题谱中仍开放了测评，故可借此白嫖(x)；但由于无法粘贴每次都要手打的话还是头大，看某贴后受启发于是尝试写了。有些小题大作，就图一乐了。

通过模拟外设输入（目前只用了键盘），将指定文件内容输入当前窗口，即起到粘贴作用；特针对无法禁用粘贴的窗口（比如AC Saber）

### 构建方法

#### 依赖

```bash
## Linux: Xlib, XTest
sudo apt install libx11-dev libxtst-dev
## Windows: Compiler option - MinGW or MSVC
```

#### CMake

```bash
mkdir build && cd build
cmake -G <Generator> .. -DCMAKE_BUILD_TYPE=<BuildType>
# Windows-MinGW: cmake -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Release
# Windows-MSVC : cmake -G "Visual Studio 17 2022" .. -DCMAKE_BUILD_TYPE=Release
# Linux:         cmake .. -DCMAKE_BUILD_TYPE=Release

cmake --build .                     # <=> make
cmake --build . --target install    # <=> make install
cmake --build . --target clean      # <=> make clean
```

默认参数：`CMAKE_INSTALL_PREFIX = ${CMAKE_SOURCE_DIR}`，`CMAKE_BUILD_TYPE = Release`

#### Makefile [Deprecated]

省事版，针对Linux或MinGW

```powershell
make 		 # build
make install # install to ./bin/$(VERSION)  can set VERSION before install to control subdir name
make clean 	 # clean
make cleanall
```

### 使用方式

#### 运行

图型界面可双击图标运行，Linux不会生成窗口，Windows可配置是否生成执行命令行窗口

命令行推荐重定向stdio到log.txt，志开头处可查看实际配置参数值

```shell
AutoTyper.exe > log.txt # windows
AutoTyper > log.txt 	# linux
```

- `config.ini`和应用程序需在同目录，`srcfile`路径指定应正确(默认为应用同目录下的target.cpp)
- 运行后，请在3s内切换到目标输出窗口并激活光标，同时注意检查输入法切至英文（尚不支持中文输入,Windows和Linux端都尚未搞定输入法模式检测，晕，是感觉很简单一个事情）
- 运行过程中，长按ESC可终止输出

#### 配置

`config.ini`中可配置参数，如无或识别错误则将使用默认参数

- `io` 输入输出相关
  
  - `srcfile` 输入文件，应为绝对路径或相对应用的路径；<font color="red">注意</font> ，仅适配了由vscode写的文件，缩进均存为空格
  - `target` 目标窗口文本编辑器，用于切换适配的输出方式；如正确设置则强制覆盖fit中的配置
    - acsaber : 针对acsaber编辑器（就是这盘醋）
    - vscode : vscode编辑器
    - gedit : ubuntu gedit编辑器
  - notepad : windows 文本编辑器
  
- `type` 打字相关参数配置
  - `interval` 输出速度，字符间睡眠时间，单位us
  - `hide_window` Windows下GUI，是否隐层双击exe后生成的窗口
  
- `fit` 对目标窗口文本编辑器功能适配（从而抵消输入影响）
  - <font color="red">注意</font> ：调整仅针对正确的C++语法和相对规范的写法，否则请将`target`设为notepad即关闭所有`fit`选项进行纯输出
  - `view_space_as_tab` 编辑器将符合数量的空格视为'\t'，退格时直接删去4个(未适配8个)空格
  - `brackets_autopair` 编辑器会自动生成匹配的关闭括号
  - `brace_nextline_pair` 编辑器对'{'不直接进行匹配，后接换行时才会缩进并在下一行补上'}'
    - 针对AC Saber： '{'后如果加几个空白字符，则再接换行就不会补'}'
    - 方案1：用栈记录不同效果的'{'，对于生成换行补全的'{'，在遇到'}'时进行处理
      - 输出结果与输入文件一般情况一致；不一致的情况如下
        ```cpp
        // 文件中
        while (true) {
        false; } /* comment here */ 
        
        // AC Saber输出结果
        while (true) {
        false;
        } /* comment here */ 
        ```
    - 方案2：对于末尾空白的'{'，额外输出空格，即可破坏编辑器的换行补全 ；输出结果基本上必然与输入文件不一致，空白字符结尾的'{'后必然会多一个空格；**当前默认采用方案2** 
  - `space_autofill` 编辑器会继承上行的缩进(空格数)，会根据括号开闭决定换行后是否增加缩进

### 其他说明

1. windows版最初来源于 https://www.acwing.com/blog/content/44823/
2. ubuntu x11下其实有现成的工具 `xdotool`，安装后即可使用；不过它似乎不能很好的输出文件中的换行，所以 `other/autotype.sh`中有其简单的封装后的shell脚本，但进一步对各种文本编辑器的适配就比较僵硬不好写了，故放弃
3. wayland服务器或许可以用uinput，兼容性应该更好；Python实现或许会更方便些；暂不了解了
4. 目前版本为自娱自乐面向对象化后的多文件版本，单文件旧版同样放置于 `other/AutoTyper_xxx.cpp`中，也可直接编译
   - MSVC环境下还可这样编译 `cl AutoTyper_win.cpp /Fe:Autotyper.exe /link User32.lib`

TODO

- [ ] 支持直接指定窗口输出
- [ ] 输入文件中缩进的'\t'和空格兼容
- [ ] 支持命令行参数配置
- [ ] 检测输入法
- [ ] 支持中文输出
- [ ] 增加输出设备(看具体需求了)
