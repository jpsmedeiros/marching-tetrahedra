#PBS -S  /bin/bash
#PBS -N MARCHING-TETRAHEDRA
#PBS -q  linux.q
#PBS -l nodes=8:ppn=2

cd $PBS_O_WORKDIR

# OPENMPI 1.10 + GCC 5.3
source /hpc/modulos/bash/openmpi-1.10-gcc53.sh
mpic++ Decimate.cpp Isosurface.cpp Gyroid.cpp math3D.cpp LinkedList.cpp main.cpp -o main.o
mpiexec -np ${PBS_NP} -hostfile  ${PBS_NODEFILE} $1 main.o $2 $3 $4
