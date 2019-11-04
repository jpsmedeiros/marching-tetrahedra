#include <iostream>
#include "Gyroid.h"
#include "Decimate.h"
#include <mpi.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>

using namespace std;

int main (int argc, char * argv[])
{
    int process_id;
    int ierr;
    int n_processes;
    double startTime = 0, endTime = 0;

    ierr = MPI_Init (&argc, &argv);
    if (ierr != 0)
    {
        cout << "\n";
        cout << "Fatal error!\n";
        cout << "MPI_Init returned nonzero ierr.\n";
        exit (1);
    }
    ierr = MPI_Comm_size (MPI_COMM_WORLD, &n_processes);
    ierr = MPI_Comm_rank (MPI_COMM_WORLD, &process_id);

    if ( process_id == 0)
        startTime = MPI_Wtime();

    Gyroid surface;
    int x0 = -10;
    int h = -x0 / n_processes; // todo: virar float
    int xMin = x0 + (h * process_id);
    int xMax = xMin + h;
    int input_res = atoi(argv[1]);
    double res = input_res / cbrt (n_processes);
    decimate(surface, xMin, xMax, xMin, xMax, xMin, xMax, -1, res);

    MPI_Barrier(MPI_COMM_WORLD);

    if (process_id == 0){
        endTime = MPI_Wtime();
        sleep(1);
        cout << "\n Tempo decorrido: " << endTime - startTime << "\n";
    }

    MPI_Finalize ();

    if (process_id == 0)
    {
        cout << "\n";
        cout << "Normal end of execution.\n";
        cout << "\n";
    }

    return 0;
}
