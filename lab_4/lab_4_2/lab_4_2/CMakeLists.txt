cmake_minimum_required(VERSION 3.20)

project(lab_4_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab_4_2
    lab_4_2.cpp
)

set(FREEGLUT_ROOT "C:/vcpkg/packages/freeglut_x64-windows")

target_include_directories(lab_4_2 PRIVATE
    "${FREEGLUT_ROOT}/include"
)

target_link_libraries(lab_4_2 PRIVATE
    "${FREEGLUT_ROOT}/lib/freeglut.lib"
    opengl32
    glu32
)

add_custom_command(TARGET lab_4_2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FREEGLUT_ROOT}/bin/freeglut.dll"
        $<TARGET_FILE_DIR:lab_4_2>
)