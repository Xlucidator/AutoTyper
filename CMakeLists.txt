cmake_minimum_required(VERSION 3.12)
project(AutoTyper)

### C++ Standard
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")

### Output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

### OS Dectect & Configure Links
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Building on Windows")
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(STATUS "Using Microsoft Visual Studio")

        add_compile_options(/W4)  # 高警告等级
        set(LIBRARIES User32.lib)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        message(STATUS "Using MinGW")

        add_compile_options(-Wall -Wextra -pedantic)
        set(LIBRARIES User32)
    endif()

elseif(UNIX OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring for Ubuntu/Linux")
    
    add_compile_options(-Wall -Wextra -pedantic)
    find_package(X11 REQUIRED)
    set(LIBRARIES X11::X11 X11::Xtst)
endif()

### Includes
include_directories(${CMAKE_SOURCE_DIR}/src)

### Source
file(GLOB_RECURSE SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
message(STATUS "Source file: ${SRC}")

### Execute
add_executable(AutoTyper ${SRC})

### Links
target_link_libraries(AutoTyper ${LIBRARIES})
