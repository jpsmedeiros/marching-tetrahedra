mpic++ Decimate.cpp Isosurface.cpp Gyroid.cpp math3D.cpp main.cpp -o main.o && mpirun --hostfile hostfile -np $1 main.o
