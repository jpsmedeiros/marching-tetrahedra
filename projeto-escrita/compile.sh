mpic++ Decimate.cpp Isosurface.cpp Gyroid.cpp math3D.cpp LinkedList.cpp main.cpp -o main.o && mpirun --hostfile hostfile -np $1 main.o $2 $3
