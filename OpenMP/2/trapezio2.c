#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double Local_trap(double a, double b, int n);
double f(double x);

int main(int argc, char* argv[]){
    double global_result = 0.0;     /* Store result in global_result */
    double a, b;                    /* Left and right endpoints */
    int n;                          /* Total number of trapezoids */
    int thread_count;

    thread_count = strtol(argv[1], NULL, 10);
    printf("Enter a, b, and n\n");
    scanf("%lf %lf %d", &a, &b, &n);

    # pragma omp parallel num_threads(thread_count) reduction(+: global_result)
    {
        global_result += Local_trap(a, b, n);
    }
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.14e\n", a, b, global_result);

    return 0;
} /* main */

double Local_trap(double a, double b, int n){
    double h, x, my_result;
    double local_a, local_b;
    int i, local_n;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    h = (b-a)/n;
    local_n = n/thread_count;
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    my_result = (f(local_a) + f(local_b)) / 2.0;

    # pragma omp parallel for num_threads(thread_count) reduction(+: my_result)
        for (i = 1; i <= local_n - 1; i++)
            my_result += f(local_a + i*h);
    my_result = my_result*h;

    return my_result;
} /* Trap */

double f(double x){
    return x*x;
}