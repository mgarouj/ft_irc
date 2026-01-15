#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void)
{
    char* a;
    char* ab;
    char *ss;
    for (int i = 0; i < 100000000; i++)
    {
        printf("%d \n", i);
        a = malloc(INTMAX_MAX - INTMAX_MAX / 2);
    }

    while (1)
    {
        a = malloc(500000);
        while (1){
            printf("ss\n");
             ab = malloc(500000);
            ss = ab;
            ab = a;
        }

    }


    while (1);
}