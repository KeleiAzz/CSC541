//
// Created by Kelei Gong on 10/25/15.
//

#include "random.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    int *output = malloc(244444 * sizeof(int));
    int i;
    for(i = 0; i < 244444; i++)
    {
        output[i] = rand();
//        printf("%d\n", output[i]);
    }
    FILE *out = fopen("inputt.bin", "wb");
    fwrite(output, 244444, sizeof(int), out);
    return 0;
}