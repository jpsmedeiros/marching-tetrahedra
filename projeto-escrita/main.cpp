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
    int x0 = -15;
    float h = 2 * abs(x0) / (n_processes - 1);
    float xMax = x0 + h;
    decimate(surface, // superficie que vamos renderizar
            x0, xMax, // limites eixo x
            x0, -x0, // limites eixo y
            x0, -x0, // limites eixo z
            -1, input_res, list, // isolevel, resolução e lista de resultados
            process_id, n_processes); // dados do MPI com offset para adaptar pro metodo 1 automaticamente
}

void method1() {
    double startTime = 0, endTime = 0;
    startTime = MPI_Wtime();
    process_images(NULL);
    endTime = MPI_Wtime();
    cout << "\n Tempo decorrido: " << endTime - startTime << "\n";
}

void print_method2(float chunk_recv_matrix[][4], int size, int process){
    char type;
    for (int i = 0; i < size; i++) {
        type = 's';
        if (chunk_recv_matrix[i][3] > 0) type = 'n';
         cout << type << "," << chunk_recv_matrix[i][0] << ',' << chunk_recv_matrix[i][1] << ',' << chunk_recv_matrix[i][2] << ',' << process -1 << '\n';
    }
}

void method2() {
    double startTime = 0, endTime = 0;
    if (process_id == 0) {
        startTime = MPI_Wtime();
        int n_arrays = 0, i = 1, j = 0, diff_last_chunk = 0, n_arrays_sum = 0;
        float chunk_recv_matrix[chunk_size][4];
        float number_of_chunks = 0;

        for (i = 1; i < n_processes; i++) {
            MPI_Recv(&n_arrays, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // pega o número de pontos que vai enviar
            n_arrays_sum += n_arrays;
            number_of_chunks = n_arrays / chunk_size; // número de vezes que vai receber dados
            diff_last_chunk = n_arrays % chunk_size; // diferença pra quando for enviar o último pacote de dados (se houver diferença)
            for (j = 0; j < number_of_chunks; j++) {
                MPI_Recv(&chunk_recv_matrix, chunk_size * 4, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                print_method2(chunk_recv_matrix, chunk_size, i);
            }
            if (diff_last_chunk > 0) {
                MPI_Recv(&chunk_recv_matrix, chunk_size * 4, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                print_method2(chunk_recv_matrix, diff_last_chunk, i);
            }
        }
        endTime = MPI_Wtime();
        cout << "\n Tempo decorrido: " << endTime - startTime << "\n";
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
    if (method != 1) {
        mpi_setup(MPI_Init(&argc, &argv));
    } else {
        n_processes = 2;
        process_id = 1;
    }
    process_method(method);
    if (method != 1) {
        MPI_Finalize();
    }

    return 0;
}
