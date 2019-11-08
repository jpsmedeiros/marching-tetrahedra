#include <iostream>
#include "Gyroid.h"
#include "Decimate.h"
#include <mpi.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>

using namespace std;

int process_id;
int n_processes;
int method;
int input_res;

void process_images() {
    Gyroid surface;
    int x0 = -10;
    float h = 2 * abs(x0) / (n_processes);
    float xMin = x0 + (h * process_id);
    float xMax = xMin + h;
    double res = input_res / cbrt (n_processes);
    decimate(surface, xMin, xMax, xMin, xMax, xMin, xMax, -1, res);
}

void method1() {
    double startTime = 0, endTime = 0;

    if (process_id == 0) {
        startTime = MPI_Wtime();
    } else {
        process_images();
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (process_id == 0) {
        endTime = MPI_Wtime();
        sleep(1);
        cout << "\n Tempo decorrido: " << endTime - startTime << "\n";
    }
}

void method2() {
    if (process_id == 0) {
        int n_arrays = 0;
        // Recebe a quantidade de arrays de quatro pontos flutuantes que vai receber do processo
        for (int i = 1; i < n_processes; i++){
            MPI_Recv(&n_arrays, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            cout << "N Arrays: " << n_arrays << " process:" << i << "\n";
        }
    } else {
        process_images();
    }

}

void process_method(int method) {
    if (method == 1)
        return method1();
    return method2();
}

void mpi_setup(int ierr) {
    if (ierr != 0) {
        cout << "\nFatal error!\nMPI_Init returned nonzero ierr.\n";
        exit (1);
    }
    MPI_Comm_size (MPI_COMM_WORLD, &n_processes);
    MPI_Comm_rank (MPI_COMM_WORLD, &process_id);
}

int main (int argc, char * argv[])
{
    input_res = atoi(argv[1]);
    method = atoi(argv[2]);
    mpi_setup(MPI_Init(&argc, &argv));
    process_method(method);
    MPI_Finalize();

    return 0;
}
