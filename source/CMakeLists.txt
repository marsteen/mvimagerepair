cmake_minimum_required(VERSION 3.10)
project("fillholes")

link_directories(../../mvlib/lib)
include_directories(./source/include  ./source)


message("System: ${CMAKE_SYSTEM_NAME}")

#find_package(PkgConfig REQUIRED QUIET)
#pkg_search_module(
#    OpenGL
#    REQUIRED
#      OpenGL
#    IMPORTED_TARGET
#    )


#
# OPENGL
#
#find_package(OpenGL REQUIRED COMPONENTS OpenGL)
#if (OPENGL_FOUND)
#    message("found OPENGL")
#    include_directories($(OPENGL_INCLUDE_DIRS))
#    link_libraries(${OPENGL_LIBRARIES})
#endif()




#
# PNG
#
find_package(PNG REQUIRED)
if (PNG_FOUND)
    message("found PNG")
    include_directories($(PNG_INCLUDE_DIRS))
    link_libraries(${PNG_LIBRARIES})
endif()


include_directories(
    ./include
    ../../mvlib/include
)

add_executable(fillholes
    main.cpp
)



target_link_libraries(fillholes
        mv_graphics_png
        mv_graphics_tools
        png
)

#if (CMAKE_SYSTEM_NAME STREQUAL "MSYS")
#    #
#    # Windows
#    #
#    target_link_libraries(mvshader
#        opengl32)
#elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
#    #
#    # Linux
#    #
#    target_link_libraries(mvshader
#        SDL2 
#        GL
#        GLEW)
#elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
#    #
#    # Mac OS
#    #
#    # *TODO*
#endif()


