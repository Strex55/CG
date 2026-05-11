cmake_minimum_required(VERSION 3.20)

project(Lab_3_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FREEGLUT_DIR "C:/vcpkg/packages/freeglut_x64-windows")
set(OPENGL_DIR "C:/vcpkg/packages/opengl_x64-windows")

add_executable(Lab_3_2
        Lab_3_2.cpp
)

target_include_directories(Lab_3_2 PRIVATE
        "${FREEGLUT_DIR}/include"
        "${OPENGL_DIR}/include"
)

target_compile_definitions(Lab_3_2 PRIVATE FREEGLUT_LIB_PRAGMAS=0)

target_link_libraries(Lab_3_2 PRIVATE
        "${FREEGLUT_DIR}/lib/freeglut.lib"
        opengl32
)

add_custom_command(TARGET Lab_3_2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/vcpkg/packages/freeglut_x64-windows/bin/freeglut.dll"
        $<TARGET_FILE_DIR:Lab_3_2>
)