#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char* argv[]){
    double factor = 1.0;
    double sum = 0.0;
    double pi_approx = 0.0;

    int thread_count = strtol(argv[1], NULL, 10);
    int n = 0;

    printf("Escolha a precisão (númeor de iterações):\n");
    scanf("%d", &n);

    # pragma omp parallel for num_threads(thread_count) reduction (+: sum) private(factor)
        for(int k = 0; k < n; k++){
            if (k % 2 == 0)
                factor = 1.0;
            else
                factor = -1.0;
            sum += factor/(2*k+1);
        }

    pi_approx = 4.0*sum;

    printf("%.14e\n", pi_approx);
}