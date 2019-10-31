#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

int is_prime(int x){

    if(x == 1)
        return 1;

    for(int i = 2; i < x; i++){
        if(x % i == 0)
         return 1;
    }

    return 0;
}

int main(int argc, char *argv[]){

    if (argc < 2){
        printf("Passe como argumento o número até o qual se deseja!\n");
        return 1;
    }
    
    int n_prime = atoi(argv[1]); // numero até o qual se deseja verificar os primos
    int *prime = 0;              // primo atual

    int     comm_sz;                // número de processos
    int     my_rank;                // ranking do processo

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Para os processos diferentes de zero
    if (my_rank != 0){

        // Se o numero desejado for 0, apenas retorna
        if((n_prime == 0) || (n_prime == 1)){
            MPI_Finalize();
            return 0;
        }

        //  Cada processo começa com o seu próprio my_rank
        // e itera o i de acordo com o numero de processos -1(pois o zero é usado para controle)
        int iteration = comm_sz-1;
        int i;

        // Se o valor de i é primo ou é o valor máximo desejado, envia
        for(i = my_rank; i < n_prime; i+=iteration){
            if(is_prime(i) || (i == n_prime)){
                *prime = i;
                MPI_Send(prime, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

    }else{

        if((n_prime == 0) || (n_prime == 1)){
            printf("%d\n", n_prime);
            MPI_Finalize();
            return 0;
        }
        int i = 1;
        
        // Laço para ficar recebendo dos outros processos até que o número máximo seja enviado
        while(*prime < n_prime){
            printf("oi");
            MPI_Recv(prime, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d\n", *prime);
            
            if(i == comm_sz){
                i = 1;
            }else{
                i++;
                i = i % comm_sz;
            }
        }
    }

    MPI_Finalize();

    return 0;
}