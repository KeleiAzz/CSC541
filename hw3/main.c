//
// Created by Kelei Gong on 10/15/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numOfInt(FILE *inp);
int cmpfunc(const void * a, const void * b);
int readLeftMost(int *ptr);
void setLeftMost(int *ptr);

int main(int argc, char *argv[])
{
    FILE *inp = fopen( "input.bin", "r");
    int len = numOfInt(inp);
    printf("%d\n", len);
    int *input = malloc(1000 * sizeof(int));
    int *output = malloc(1000 * sizeof(int));
    int num_of_runs = len / 1000;
    int i;
    FILE *fp;
    char *filename = malloc(13 * sizeof(char));
//    char *output_filename = "input.bin.001";
//    printf("%s\n", output_filename);
//    for( i = 1; i <= num_of_runs; i++ )
//    {
//        fread(input, sizeof(int), 1000, inp);
//        qsort(input, 1000, sizeof(int), cmpfunc);
////        char *output_file = malloc(13 * sizeof(char));
//        sprintf(filename, "input.bin.%03d", i);
//        fp = fopen(filename, "w");
//        fwrite(input, sizeof(int), 1000, fp);
//        fclose(fp);
////        free(filename);
//    }
    int num_to_buffer = 1000 / num_of_runs;
    int *buffer = malloc(num_to_buffer * sizeof(int));
    FILE *files[250];
    for( i = 1; i <= num_of_runs; i++ )
    {
        sprintf(filename, "input.bin.%03d", i);
        files[i-1] = fopen(filename, "r");
    }

    int *ptr[250];
    for( i = 0; i < num_of_runs; i++ )
    {
//        fseek(files[i-1], (i-1)*4* sizeof(int), SEEK_SET);
        fread(input + i * 4, sizeof(int), 4, files[i]);
        ptr[i] = input + i * 4;
//        printf("%d\n",  *ptr[i]);
    }
    printf("\n");
    int tmp;
    int min = input[0];
    int min_index = 0;
    int w = 0;
    int out = 0;
    while(w < 1000)
    {
        min = readLeftMost(ptr[0]);
        min_index = 0;
//        printf("%d\n", min);
        for( i = 0; i < num_of_runs; i++)
        {
            tmp = readLeftMost(ptr[i]);
//            printf("%d\n", tmp);

            if( tmp != -1)
            {
                if( tmp < min)
                {
                    min = tmp;
                    min_index = i;
                }
            }
            else
            {
                printf("!!!!\n");
                fread(ptr[i], sizeof(int), 4, files[i]);
                tmp = readLeftMost(ptr[i]);
                if( tmp < min)
                {
                    min = tmp;
                    min_index = i;
                }
            }
        }
        setLeftMost(ptr[min_index]);
        if(readLeftMost(ptr[0]) == -1)
        {
            fread(ptr[i], sizeof(int), 4, files[0]);
        }
        output[out] = min;
        out++;
        if( out >= 1000)
        {
            out = 0;
            for(i = 0; i < 1000; i++)
            {
                printf("%d,", output[i]);
            }
        }

//        printf("%d, %d\n", min, min_index);
//        break;
        w++;
    }


    for(i = 0; i < num_of_runs; i++)
    {
//        printf("%d,", *ptr[i]);
    }
    return 0;
}


int numOfInt(FILE *inp){
    fseek(inp, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(inp);
    rewind(inp);
    return (int) (len/4);
};

int cmpfunc(const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

int readLeftMost(int *ptr)
{
    int i;
    int res;
    for( i = 0; i < 4; i++)
    {
        if( *(ptr + i) != -1)
        {
            res = *(ptr + i);
//            printf("%d\n", res);
//            *(ptr + i) = -1;
            return res;
        }
    }
    return -1;
}

void setLeftMost(int *ptr)
{
    int i;
    for(i = 0; i < 4; i++)
    {
        if( *(ptr + i) != -1)
        {
//            printf("%d\n", *(ptr + i));
            *(ptr + i) = -1;
//            printf("%d\n", *(ptr + i));
            break;
        }
    }
}

void readIntoMem(FILE *inp, int* input)
{

}