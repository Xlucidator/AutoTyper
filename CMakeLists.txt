cmake_minimum_required(3.12)
project(AutoTyper)

### C++ Standard
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")

### OS Dectect & Configure Links
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Building on Windows")
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(STATUS "Using Microsoft Visual Studio")

        add_compile_options(/W4)  # 高警告等级
        target_link_libraries(AutoTyper User32.lib)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        message(STATUS "Using MinGW")

        add_compile_options(-Wall -Wextra -pedantic)
        # target_link_libraries(AutoTyper user32)
    endif()

elseif(UNIX OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring for Ubuntu/Linux")
    
    add_compile_options(-Wall -Wextra -pedantic)
    find_package(X11 REQUIRED)
    if(NOT TARGET X11::X11)
        find_library(XTEST_LIB XTest REQUIRED)
        find_library(X11_LIB X11 REQUIRED)
        set(LINUX_LIBRARIES ${X11_LIB} ${XTEST_LIB})
    else()
        set(LINUX_LIBRARIES X11::X11 X11::Xtst)
    endif()
    target_link_libraries(AutoTyper ${LINUX_LIBRARIES})
endif()

### Includes
include_directories(${CMAKE_SOURCE_DIR}/src)

### Source
file(GLOB_RECURSE SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
message(STATUS "Source file: ${SRC}")

### Execute
add_executable(AutoTyper ${SRC})

### Links