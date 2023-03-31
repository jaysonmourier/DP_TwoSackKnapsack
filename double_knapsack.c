/*
    Clément De Miribel
    Jayson Mourier
*/

#include <stdio.h>
#include <stdio.h>

#define P 10
#define Q 20
#define N 8

int resolve(int *state, object *obj);
int max(int a, int b, int c);

typedef struct {
    int w;
    int p;
} object;

static object sacA[P];
static object sacB[Q];

int
main(void)
{
    int state[2] = {P, Q};
    object objects[N] = {
        {3, 15},
        {5, 30},
        {1, 12},
        {4, 20},
        {2, 15},
        {8, 34},
        {12, 25},
        {7, 43},
    };

    for(int k = 0; k < N; ++k) resolve(state, objects);

    return 0;
}

int resolve(int *state, object *obj)
{
    int val = 0;
    
    if((state[0] >= obj->p) && (state[1] >= obj->p))
    {
        val += max(obj->w + resolve(state[0] - obj->p, obj), obj->w + resolve(state[0], state[1] - obj->p), resolve(state, obj));
    }

    return 0;
}

int max(int a, int b, int c) {
   int max_val = a;
   
   if (b > max_val) {
      max_val = b;
   }
   
   if (c > max_val) {
      max_val = c;
   }
   
   return max_val;
}