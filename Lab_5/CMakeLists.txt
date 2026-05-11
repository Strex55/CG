cmake_minimum_required(VERSION 3.16)

project(Lab_5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Lab_5 Lab_5.cpp)

set(FREEGLUT_DIR "C:/vcpkg/packages/freeglut_x64-windows")

target_include_directories(Lab_5 PRIVATE
    "${FREEGLUT_DIR}/include"
)

target_link_libraries(Lab_5 PRIVATE
    "${FREEGLUT_DIR}/lib/freeglut.lib"
    opengl32
    glu32
    gdiplus
)

add_custom_command(TARGET Lab_5 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FREEGLUT_DIR}/bin/freeglut.dll"
    $<TARGET_FILE_DIR:Lab_5>
)

add_custom_command(TARGET Lab_5 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/textures"
    "$<TARGET_FILE_DIR:Lab_5>/textures"
)