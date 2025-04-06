cmake_minimum_required(VERSION 3.12)
project(AutoTyper)

### Add CMake Modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(smart_copy)

### Build Options
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()
message(STATUS "[Option] Build Type: ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -Wall -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s -Wl,--gc-sections")

### C++ Standard
set(CMAKE_CXX_STANDARD 17)

### Output
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

### OS Dectect & Configure Links
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "[OS] Building on Windows")
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(STATUS "[OS] Using Microsoft Visual Studio")

        add_compile_options(/W4)  # high warning level
        set(LIBRARIES User32.lib Imm32.lib)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        message(STATUS "[OS] Using MinGW")

        add_compile_options(-Wall -Wextra -pedantic)
        set(LIBRARIES User32 Imm32)
    endif()

elseif(UNIX OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "[OS] Configuring for Ubuntu/Linux")
    
    add_compile_options(-Wall -Wextra -pedantic)
    find_package(X11 REQUIRED)
    set(LIBRARIES X11::X11 X11::Xtst)
endif()

### Execute
file(GLOB_RECURSE SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
message(STATUS "[files] Source file: ${SRC}")
add_executable(AutoTyper ${SRC})

### Links
target_link_libraries(AutoTyper ${LIBRARIES})

### Public Copy
set(PUBLIC_DIR "${CMAKE_SOURCE_DIR}/public")
set(public_files 
    "${PUBLIC_DIR}/config.ini"
    "${PUBLIC_DIR}/target.cpp"
)
smart_copy_files("${public_files}" ON)

### Install
set(VERSION "0.1.1")
set(INSTALL_DIR "bin/v${VERSION}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE PATH "Installation directory" FORCE)
install(TARGETS AutoTyper RUNTIME DESTINATION ${INSTALL_DIR})
install(FILES ${public_files} DESTINATION ${INSTALL_DIR})
message(STATUS "[install] Files will be installed to: ${CMAKE_SOURCE_DIR}/${INSTALL_DIR}")