cmake_minimum_required(VERSION 3.10)
project("imagerepair")

link_directories(/usr/local/lib)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_C_FLAGS "-O2")
set(CMAKE_CXX_FLAGS "-O2")

include_directories(
    /usr/local/include
    ../source/include
    ../../mvlib/include
    )

add_executable(imagerepair
    ../source/main.cpp
    ../../mvlib/mvpng/mv_graphics_png.c
    ../../mvlib/mvtools/mv_graphics_tools_swapredblue.cpp

    )


target_link_libraries(imagerepair
    png16
    z)

