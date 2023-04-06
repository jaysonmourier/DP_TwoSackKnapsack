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
void find_items_in_sacks(Cube *dp, int *weights, int *values, int n, int capacity1, int capacity2, int *sack1, int *sack2, int *sack1_count, int *sack2_count);


int
main(void)
{
    int values[] = {30, 20, 5, 27, 4, 40, 7, 8, 14};
    int weights[] = {3, 1, 4, 3, 2, 5, 1, 4, 2};
    int n = sizeof(values) / sizeof(values[0]);
    int capacity1 = 6;
    int capacity2 = 9;

    int result = 0;
    
    Cube *c = max_value_two_sacks(values, weights, n, capacity1, capacity2, &result);

    print_3D(c);    

    printf("Le poids total des objets sélectionnés dans les deux sacs est : %d\n", result);

    int sack1[n];
    int sack2[n];
    int sack1_count, sack2_count;

    find_items_in_sacks(c, weights, values, n, capacity1, capacity2, sack1, sack2, &sack1_count, &sack2_count);

    printf("Objets dans le sac 1 : ");
    for (int i = 0; i < sack1_count; ++i)
    {
        printf("%d ", sack1[i] + 1);
    }
    printf("\n");

    printf("Objets dans le sac 2 : ");
    for (int i = 0; i < sack2_count; ++i)
    {
        printf("%d ", sack2[i] + 1);
    }
    printf("\n");


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

void find_items_in_sacks(Cube *dp, int *weights, int *values, int n, int capacity1, int capacity2, int *sack1, int *sack2, int *sack1_count, int *sack2_count)
{
    int i = n;
    int j = capacity1;
    int k = capacity2;
    *sack1_count = 0;
    *sack2_count = 0;

    while (i > 0 && (j > 0 || k > 0))
    {
        if (weights[i - 1] <= j && get(dp, i - 1, j - weights[i - 1], k) + values[i - 1] == get(dp, i, j, k))
        {
            sack1[(*sack1_count)++] = i - 1;
            j -= weights[i - 1];
        }
        else if (weights[i - 1] <= k && get(dp, i - 1, j, k - weights[i - 1]) + values[i - 1] == get(dp, i, j, k))
        {
            sack2[(*sack2_count)++] = i - 1;
            k -= weights[i - 1];
        }
        i--;
    }
}
