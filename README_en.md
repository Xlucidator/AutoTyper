# AutoTyper

**切换 [中文](README.md)**

Some problems on AcWing require purchasing courses to use, but they are still available for evaluation in the AC Saber test mode. Thus, you can use for free by leveraging this feature (x). However, since it’s a hassle to type everything out each time instead of copying and pasting, I was inspired by a post that had tried writing the tool. It may be a fuss, but just for fun.

By simulating peripheral input (currently only the keyboard), the content of a specified file is input into the current window, serving the purpose of pasting; it is especially designed for windows where pasting is not allowed (like AC Saber).

### How to Build

#### Dependency

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
# Linux: cmake .. -DCMAKE_BUILD_TYPE=Release

cmake --build .                     # <=> make
cmake --build . --target install    # <=> make install
cmake --build . --target clean      # <=> make clean
```

Default：`CMAKE_INSTALL_PREFIX = ${CMAKE_SOURCE_DIR}`，`CMAKE_BUILD_TYPE = Release`

#### Makefile [Deprecated]

for convenience，target to Linux or MinGW

```powershell
make 		 # build
make install # install to ./bin/$(VERSION)  can set VERSION before install to control subdir name
make clean 	 # clean
make cleanall
```

### How to Use

Run the app

```shell
## Command line is recommended
AutoTyper.exe > log.txt  # windows
AutoTyper > log.txt      # linux
	# It is recommended to redirect the stdio to log.txt
	# you can check the actual configuration parameters at the beginning of the log
## Or you can just double click the executable file
```

- `config.ini` and the application must be in the same directory, the `srcfile` path should be correctly specified (default is `target.cpp` in the same directory as the application).
- After running, switch to the target output window and activate the cursor within 3 seconds. Make sure the input method is set to English (Chinese input is not supported yet).
- During operation, long pressing ESC will terminate the output.

Configuration

Parameters can be configured in `config.ini`. If not present or incorrectly recognized, default values will be used.

- `io` Input/output related
  - `srcfile` Input file, should be an absolute or relative path to the application’s path
    - **Note**: Only files written by vscode are supported, indentation should be spaces.
  - `target` Target window text editor for output mode switching. If correctly set, it will override the fit configuration.
    - acsaber: For acsaber editor (the vinegar editor)
    - vscode: For vscode editor
    - gedit: For Ubuntu gedit editor
  - notepad: For Windows text editor
  
- `type` Typing related parameters configuration
  - `interval` Output speed, character sleep time in microseconds
- `fit` Adaptation for target window text editor functionality (to mitigate input impact)
  - `view_space_as_tab` The editor views a certain number of spaces as '\t'. When backspacing, it directly deletes 4 (not 8) spaces.
  - `brackets_autopair` The editor auto-pairs brackets.
  - `space_autofill` The editor inherits the indentation (number of spaces) from the previous line and will adjust indentation based on bracket opening and closing.

### Additional Notes

1. The Windows version originally came from https://www.acwing.com/blog/content/44823/
2. On Ubuntu X11, there is a ready-made tool `xdotool`. After installation, it can be used; however, it doesn’t handle line breaks well from the file. Therefore, a simple shell script wrapping `xdotool` is available in `other/autotype.sh`. Further adaptation to various text editors is tricky and rigid, so I abandoned it.
3. Wayland server may be compatible with `uinput` and might offer better compatibility. A Python implementation could be more convenient; however, I don’t know much about it at the moment.
4. The current version is the object-oriented, multi-file version for self-entertainment. The old single-file version is also placed in `other/AutoTyper_xxx.cpp` and can also be compiled directly.
   - Under MSVC, you can compile like this: `cl AutoTyper_win.cpp /Fe:Autotyper.exe /link User32.lib`

TODO
- [ ] Support specifying output window directly
- [ ] Handle compatibility between '\t' and spaces in input file indentation
- [ ] Support command-line argument configuration
- [ ] Support Chinese output
- [ ] Add output devices (depending on actual need)