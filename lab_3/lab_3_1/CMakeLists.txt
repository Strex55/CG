cmake_minimum_required(VERSION 3.20)

project(Lab3OpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FREEGLUT_DIR "C:/vcpkg/packages/freeglut_x64-windows")
set(OPENGL_DIR "C:/vcpkg/packages/opengl_x64-windows")

add_executable(lab_3_1
        lab_3_1.cpp
)

target_include_directories(lab_3_1 PRIVATE
        "${FREEGLUT_DIR}/include"
        "${OPENGL_DIR}/include"
)

target_link_libraries(lab_3_1 PRIVATE
        "${FREEGLUT_DIR}/lib/freeglut.lib"
        opengl32
)

add_custom_command(TARGET lab_3_1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/vcpkg/packages/freeglut_x64-windows/bin/freeglut.dll"
        $<TARGET_FILE_DIR:lab_3_1>
)