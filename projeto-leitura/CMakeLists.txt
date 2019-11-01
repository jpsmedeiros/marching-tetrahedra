cmake_minimum_required(VERSION 3.0)
project(projeto-leitura)

add_executable(projeto-leitura
    common.h
    main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(projeto-leitura ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
