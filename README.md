# AutoTyper

模拟键盘输入

用途：对于禁用粘贴的窗口，可从指定文件中读取内容并模拟键盘输入，以实现粘贴/手敲代码的效果

> acwing上有些题没买课无法使用，但是在AC Saber测试模式的题谱中仍开放了测评，故可借此白嫖(x)；但每次手打着实头大，所以有了此脚本，起源于acwing中某帖

## 使用方式

### Ubuntu

#### X11服务器

方案一：使用现成的xdotool工具

1. 安装xdotool `sudo apt install xdotool`
2. 执行`autotype.sh`即可（需赋予可执行权限`chmod +d autytype.sh`

存在缺陷
- 由于xdotool似乎不能很好的输出文件中的换行，shell中进行了分行处理
- shell中对文件内容的操作僵硬，不能很好的适配各种编辑器的优化操作

方案二：C++使用Xlib和XTest库

1. 安装Xlib和XTest， `sudo apt install libx11-dev libxtst-dev`
2. 执行`make`构建
3. 运行`./AutoTyper > log.txt`

TODO
- 对象化，增加复用
- 指定窗口
- 增加对编辑器的优化（换行后自动tab，括号自动成对等）

#### Wayland服务器
似乎可以用uinput，兼容性更加

TODO

### Windows

姑且来自 https://www.acwing.com/blog/content/44823/ 尚没有测试完善


### Python
