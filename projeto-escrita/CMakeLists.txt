cmake_minimum_required(VERSION 3.0)
project(projeto-escrita)
find_package(MPI REQUIRED)
add_executable(projeto-escrita
    Array3D.h
    Decimate.h
    Decimate.cpp
    Gyroid.h
    Gyroid.cpp
    Isosurface.h
    Isosurface.cpp
    main.cpp
    math3D.h
    math3D.cpp
    LinkedList.h LinkedList.cpp)

target_link_libraries(projeto-escrita
        PRIVATE
        MPI_C)