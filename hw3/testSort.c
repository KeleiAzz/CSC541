//
// Created by Kelei Gong on 10/16/15.
//

//#include "testSort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
int numOfInt(FILE *inp);

int cmpfunc(const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

int main()
{
    FILE *outfile = fopen( "sortt.bin", "r");
    FILE *example = fopen("sort_example.bin", "r");
    int len = numOfInt(outfile);
    printf("%d\n", len);
    int *allall = malloc(len * sizeof(int));
    int *sorted = malloc(len * sizeof(int));

    fread(allall, sizeof(int), (size_t) len, outfile);
    rewind(outfile);
    fread(sorted, sizeof(int), (size_t) len, outfile);
    qsort(sorted, len, sizeof(int), cmpfunc);
    char *filename = malloc(16 * sizeof(char));
    int i, j, num = 0;
    for(i = 0; i < len-1; i++)
    {
        if(allall[i] != sorted[i])
            {
            printf("Wrong!!! %d %d \n", allall[i], sorted[i]);
            }
        if(allall[i+1] < allall[i])
        {
                printf("Wrong!!! %d %d \n", allall[i], allall[i+1]);
        }
    }
//    for(j = 1; j <= 168; j++)
//    {
//        sprintf(filename, "input.bin.re.%03d", j);
//        FILE *re = fopen(filename, "r");
//        len = numOfInt(re);
//        num += len;
//        int *all = malloc(len * sizeof(int));
////        int *sorted = malloc(len * sizeof(int));
//        printf("%d\n", len);
//        fread(all, sizeof(int), (size_t) len, outfile);
//        for(i = 0; i < len-1; i++)
//        {
////            if(all[i] != sorted[i])
////            {
//////            printf("Wrong!!! %d %d \n", all[i], sorted[i]);
////            }
//            if(all[i+1] < all[i])
//            {
////                printf("Wrong!!! %d %d \n", all[i], all[i+1]);
//            }
//        }
//        printf("total %d\n", num);
//    }

    return 0;
}


int numOfInt(FILE *inp){
    fseek(inp, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(inp);
    rewind(inp);
    return (int) (len/4);
};