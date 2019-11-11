#include <iostream>
#include "Gyroid.h"
#include "Decimate.h"
#include <mpi.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include "LinkedList.h"

using namespace std;

int process_id;
int n_processes;
int method;
int input_res;
int chunk_size;

void process_images(LinkedList* list) {
    Gyroid surface;
    int x0 = -10;
    float h = 2 * abs(x0) / (n_processes);
    float xMin = x0 + (h * process_id);
    float xMax = xMin + h;
    double res = input_res / cbrt (n_processes);
    decimate(surface, xMin, xMax, -1, res, list);
}

void method1() {
    double startTime = 0, endTime = 0;

    if (process_id == 0) {
        startTime = MPI_Wtime();
    } else {
        process_images(NULL);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (process_id == 0) {
        endTime = MPI_Wtime();
        //sleep(1);
        //cout << "\n Tempo decorrido: " << endTime - startTime << "\n";
    }
}

void print_method2(float chunk_recv_matrix[][4], int size){
    char type;
    for (int i = 0; i < size; i++) {
        type = 's';
        if (chunk_recv_matrix[i][3] > 0) type = 'n';
        cout << type << "," << chunk_recv_matrix[i][0] << ',' << chunk_recv_matrix[i][1] << ',' << chunk_recv_matrix[i][2] << '\n';
    }
}

void method2() {
    if (process_id == 0) {
        int n_arrays = 0, i = 1, j = 0, diff_last_chunk = 0;
        float chunk_recv_matrix[chunk_size][4];
        float number_of_chunks = 0;

        for (i = 1; i < n_processes; i++) {
            MPI_Recv(&n_arrays, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // pega o número de pontos que vai enviar
            number_of_chunks = n_arrays / chunk_size; // número de vezes que vai receber dados
            diff_last_chunk = n_arrays % chunk_size; // diferença pra quando for enviar o último pacote de dados (se houver diferença)
            for (j = 0; j < number_of_chunks; j++) {
                MPI_Recv(&chunk_recv_matrix, chunk_size * 4, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                print_method2(chunk_recv_matrix, chunk_size);
            }
            if (diff_last_chunk > 0) {
                MPI_Recv(&chunk_recv_matrix, chunk_size * 4, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                print_method2(chunk_recv_matrix, diff_last_chunk);
            }
        }
    } else {
        LinkedList* list = new LinkedList();
        process_images(list);
        MPI_Send(&list->length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        list->send(chunk_size, 4);
        delete list;
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
    chunk_size = atoi(argv[3]);
    mpi_setup(MPI_Init(&argc, &argv));
    process_method(method);
    MPI_Finalize();

    return 0;
}
