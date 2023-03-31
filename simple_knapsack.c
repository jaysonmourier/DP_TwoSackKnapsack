#include <stdio.h>
#include <stdlib.h>

#ifndef __WIN32__
// sys/time.h n'est pas disponible sur windows
#include <sys/time.h>
#endif

// Pour simplifier la comparaison, j'utilise une directive de préprocesseur
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// Cette variable me permet de compter les opérations de la fonction simple_knapsack
static int n_op = 0;

int *simple_knapsack(int W, int ut[], int wt[], int n) __attribute__((malloc));

int main(void)
{
    // Tableau des poids de chaque objet
    int pt[] = {3, 5, 1, 4, 2, 3, 4, 5, 3, 1};
    // Tableau des utilités de chaque objet
    int wt[] = {15, 30, 12, 20, 15, 18, 36, 23, 21, 6};
    // Poid total du sac
    int P = 10;
    // Le nombre d'objets, calculé dynamiquement
    int N = sizeof(pt) / sizeof(pt[0]);

#ifndef __WIN32__
    // Calculer le temps d'exécution de la fonction simple_knapsack
    struct timeval t1, t2;
    double elapsed_time;
    gettimeofday(&t1, NULL);
#endif

    int *K = simple_knapsack(P, wt, pt, N);

#ifndef __WIN32__
    gettimeofday(&t2, NULL);
    elapsed_time = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
#endif

    if(K == NULL) return 1;

    // On affiche la matrice résultante de simple_knapsack
    for(int i = 0; i < (N+1); ++i)
    {
        for(int j = 0; j < (P+1); ++j)
        {
            printf("%2d ", *(K + j + i * (P + 1)));
        }
        putchar('\n');
    }

    printf("the best value is : %d\n", *(K + P + N * (P + 1)));
    printf("n_op = %d\n", n_op);

#ifndef __WIN32__
    printf("elapsed time : %f\n", elapsed_time);
#endif
    
    free(K);
    return 0;
}

int *simple_knapsack(int W, int ut[], int wt[], int n)
{
    /*
        K est une matrice de dimension (n+1, W+1)
        Pour accéder à un élément de K, j'utilise la formule suivante : j + i * ncol
        ncol = W+1

        L'utilisation de calloc permet d'initialiser les valeurs à zéro
    */
    int *K = (int*) calloc((n+1)*(W+1), sizeof(int));

    if(K == NULL) return NULL;

    int ncol = W+1;

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0) continue;
            else if (wt[i-1] <= w) {
                *(K + w + i * ncol) = MAX(ut[i-1] + *(K + (w-wt[i-1]) + (i-1) * ncol), *(K + w + (i-1) * ncol));
                n_op++;
            }
            else {
                *(K + w + i * ncol) = *(K + w + (i-1) * ncol);
                n_op++;
            }
        }
    }

    return K;
}