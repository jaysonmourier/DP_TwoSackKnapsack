// https://developers.google.com/optimization/pack/multiple_knapsack?hl=fr#python

#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct {
    int n;
    int m;
    int z;
    int *value;
} Cube;

Cube *init(int n, int m, int z);
void set(Cube *c, int value, int i, int j, int k);
int get(Cube *c, int i, int j, int k);
void print_3D(Cube *c);

Cube *max_value_two_sacks(int *values, int *weights, int n, int capacity1, int capacity2, int *result);

int
main(void)
{
    int values[] = {48, 30, 120};
    int weights[] = {10, 30, 1};
    int n = sizeof(values) / sizeof(values[0]);
    int capacity1 = 10;
    int capacity2 = 30;

    int result = 0;
    
    Cube *c = max_value_two_sacks(values, weights, n, capacity1, capacity2, &result);

    print_3D(c);    

    printf("Le poids total des objets sélectionnés dans les deux sacs est : %d\n", result);

    return 0;
}

Cube *init(int n, int m, int z)
{
    Cube *tmp = (Cube *) malloc(1 * sizeof(Cube));

    if(tmp == NULL)
    {
        fprintf(stderr, "Erreur: impossible d'allouer la mémoire demandée.\n");
        exit(1);
    }

    tmp->value = (int *) calloc(n * m * z, sizeof(Cube));

    if(tmp->value == NULL)
    {
        fprintf(stderr, "Erreur: impossible d'allouer la mémoire demandée.\n");
        exit(1);
    }

    tmp->n = n;
    tmp->m = m;
    tmp->z = z;

    return tmp;
}

void set(Cube *c, int value, int i, int j, int k)
{
    *(c->value + j + i * c->m + k * c->n * c->m) = value;
}

int get(Cube *c, int i, int j, int k)
{
    return *(c->value + j + i * c->m + k * c->n * c->m);
}

void print_3D(Cube *c)
{
    int n = c->n;
    int m = c->m;
    int z = c->z;

    for(int k = 0; k < z; ++k)
    {
        for(int i = 0; i < n; ++i)
        {
            for(int j = 0; j < m; ++j)
            {
                fprintf(stdout, "%3d ", *(c->value + j + i * m + k * n * m));
            }
            putchar('\n');
        }
        putchar('\n');
    }
}

Cube *max_value_two_sacks(int *values, int *weights, int n, int capacity1, int capacity2, int *result)
{
    Cube *dp = init(n + 1, capacity1 + 1, capacity2 + 1);
    
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 0; j <= capacity1; ++j)
        {
            for (int k = 0; k <= capacity2; ++k)
            {
                if (weights[i - 1] <= j && weights[i - 1] <= k)
                {
                    set(dp, MAX(get(dp, i - 1, j, k), 
                                MAX(values[i - 1] + get(dp, i - 1, j - weights[i - 1], k), 
                                    values[i - 1] + get(dp, i - 1, j, k - weights[i - 1]))), i, j, k);
                }
                else if (weights[i - 1] <= j)
                {
                    set(dp, MAX(get(dp, i - 1, j, k), values[i - 1] + get(dp, i - 1, j - weights[i - 1], k)), i, j, k);
                }
                else if (weights[i - 1] <= k)
                {
                    set(dp, MAX(get(dp, i - 1, j, k), values[i - 1] + get(dp, i - 1, j, k - weights[i - 1])), i, j, k);
                }
                else
                {
                    set(dp, get(dp, i - 1, j, k), i, j, k);
                }
            }
        }
    }
    
    *result = get(dp, n, capacity1, capacity2);

    return dp;
}