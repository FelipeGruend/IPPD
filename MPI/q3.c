#include <stdio.h>
#include <mpi.h>

int is_primo(int n) {
  int div = n - 1;
  while(div > 1) {
    if(n % div == 0) 
      return 0;

    div--;
  }

  return 1;
}

int calculate_primo(int order) {
  int i;
  int primo = 0;
  while(order > 0) {
    if(primo < 1) {
      primo++;
    }
    else {
      primo++;
      while(!is_primo(primo))
        primo++;
    }
    order--;
  }

  return primo;
}

int main() {
  int world_rank;
  int comm_sz;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  comm_sz--;

  if (world_rank == 0) {
      int n;
      scanf("%d", &n);
      int chunk_size = n/comm_sz;
      int chunk_sizes[comm_sz];
      int i;
      for(i = 0; i < comm_sz; ++i) {
        chunk_sizes[i] = chunk_size;
      }
      int last_chunk = n % comm_sz;
      for(last_chunk; last_chunk > 0; --last_chunk) {
        chunk_sizes[last_chunk - 1] += 1;
      }
      for(i = 1; i <= comm_sz; ++i) {
        MPI_Send(&chunk_sizes[i - 1], 1, MPI_INT, i, i, MPI_COMM_WORLD);
      }
      for(i = 0; i < n; ++i) {
        int rank = i % comm_sz + 1;
        int order = i + 1;
        MPI_Send(&order, 1, MPI_INT, rank, rank, MPI_COMM_WORLD);
      }
  }
  else {
    int chunk_size;
    MPI_Recv(&chunk_size, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    int i;
    int order;
    for(i = 0; i < chunk_size; ++i) {
      MPI_Recv(&order, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      int primo = calculate_primo(order);
      printf("Primo nº %d: %d\n", order, primo);
    }
  }

  MPI_Finalize();

  return 0;
}