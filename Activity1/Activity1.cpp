#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (size != 4) {
        if (rank == 0)
            cout << "Run the program with 4 processes!" << endl;
        MPI_Finalize();
        return 0;
    }

    int data[16]; 
    int divisor = 2; 

    if (rank == 0) {
        cout << "Data: ";
        for (int i = 0; i < 16; i++) {
            data[i] = i + 1; 
            cout << data[i] << " ";
        }
        cout << endl;
        cout << "Divisor: " << divisor << endl;
    }

    int local_data[4];
    MPI_Scatter(data, 4, MPI_INT, local_data, 4, MPI_INT, 0, MPI_COMM_WORLD);

    int local_count = 0;
    for (int i = 0; i < 4; i++) {
        if (local_data[i] % divisor == 0)
            local_count++;
    }

    cout << "Process " << rank << " local count = " << local_count << endl;

    int global_count;
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Total multiples of " << divisor << " = " << global_count << endl;
    }

    MPI_Finalize();
    return 0;
}