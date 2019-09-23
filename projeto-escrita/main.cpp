#include <iostream>
#include "Gyroid.h"
#include "Decimate.h"
# include <mpi.h>
using namespace std;

int main (int argc, char * argv[])
{
    int id;
    int ierr;
    int p;

    ierr = MPI_Init (&argc, &argv);
    if (ierr != 0)
    {
        cout << "\n";
        cout << "Fatal error!\n";
        cout << "MPI_Init returned nonzero ierr.\n";
        exit (1);
    }
    ierr = MPI_Comm_size (MPI_COMM_WORLD, &p);
    ierr = MPI_Comm_rank (MPI_COMM_WORLD, &id);
    if ( id == 0 )
    {
        cout << "\n";
        cout << "P" << id << ": Master process:\n";
        cout << "P" << id << ": The number of processes is " << p << "\n";
        cout << "\n";
    }

    Gyroid surface;
    int x0 = -10;
    int h = -x0 / p;
    int xMin = x0 + (h * id);
    int xMax = xMin + h;
    decimate(surface, xMin, xMax, xMin, xMax, xMin, xMax, -1, 10);

    MPI_Finalize ();

    if (id == 0)
    {
        cout << "\n";
        cout << "P" << id << ": Normal end of execution.\n";
        cout << "\n";
    }

    return 0;
}
