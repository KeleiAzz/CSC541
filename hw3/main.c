//
// Created by Kelei Gong on 10/15/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

int numOfInt(FILE *inp);
int cmpfunc(const void * a, const void * b);
int readLeftMost(int *ptr, int buffer_size);
void setLeftMost(int *ptr, int buffer_size);
void basicMerge(FILE *inp);
void multiMerge(FILE *inp);

int main(int argc, char *argv[])
{
    struct timeval start_time, end_time;
    FILE *inp = fopen("input.bin", "r");
    gettimeofday( &start_time, NULL );
    basicMerge(inp);
    gettimeofday( &end_time, NULL );
    printf("Time: %lf\n", (end_time.tv_sec - start_time.tv_sec)+(end_time.tv_usec-start_time.tv_usec)/1000000.0);
    return 0;
}

void basicMerge(FILE *inp)
{
    FILE *outfile = fopen( "sort.bin", "w");
    int len = numOfInt(inp); // calculate number of int to sort

    int *input = malloc(1000 * sizeof(int));
    int *output = malloc(1000 * sizeof(int));

    //----- Create runs -----
    int num_of_runs = len / 1000;
    if( num_of_runs * 1000 < len) //If len can't be divided by 1000 equally, add an addition run
    {
        num_of_runs += 1;
    }
    int i;
    FILE *fp;
    char *filename = malloc(13 * sizeof(char));
    for( i = 1; i <= num_of_runs - 1; i++ ) // The first num_of_runs-1 runs are sure to have 1000 int
    {
        fread(input, sizeof(int), 1000, inp);
        qsort(input, 1000, sizeof(int), cmpfunc);
        sprintf(filename, "input.bin.%03d", i);
        fp = fopen(filename, "w");
        fwrite(input, sizeof(int), 1000, fp);
        fclose(fp);
    }
    fread(input, sizeof(int), (size_t) (len-1000*(num_of_runs - 1)), inp); // The last run may have less than 1000 int
    printf("%d\n", (len-1000*(num_of_runs - 1)));
    qsort(input, (size_t) (len-1000*(num_of_runs - 1)), sizeof(int), cmpfunc);
    sprintf(filename, "input.bin.%03d", num_of_runs);
    fp = fopen(filename, "w");
    fwrite(input, sizeof(int), (size_t) (len-1000*(num_of_runs - 1)), fp);
    fclose(fp);

    int num_to_buffer = 1000 / num_of_runs; // number of int to buffer for each run.

    FILE *files[num_of_runs];
    for( i = 1; i <= num_of_runs; i++ )
    {
        sprintf(filename, "input.bin.%03d", i);
        files[i-1] = fopen(filename, "r");
    }

    int *ptr[num_of_runs];
    for( i = 0; i < num_of_runs; i++ )
    {
        fread(input + i * num_to_buffer, sizeof(int), (size_t) num_to_buffer, files[i]);
        ptr[i] = input + i * num_to_buffer;
    }
    int tmp;
    int min;
    int min_index = 0;
    int w = 0;
    int out_index = 0;
    int empty_file = 0;
    while(w < len)
    {
        min = INT_MAX;
        min_index = 0;
        for( i = 0; i < num_of_runs; i++)
        {
            tmp = readLeftMost(ptr[i], num_to_buffer);
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
                if(!feof(files[i]))
                {
                    fread(ptr[i], sizeof(int), (size_t) num_to_buffer, files[i]);
                    tmp = readLeftMost(ptr[i], num_to_buffer);
//                    if(tmp == -1)
//                    {
//                        empty_file++;
//                        files[i] = NULL;
//                    }
                    if (tmp != -1 && tmp < min) {
                        min = tmp;
                        min_index = i;
                    }
                }

            }
        }
        setLeftMost(ptr[min_index], num_to_buffer); // When find the smallest int, put it into output buffer
        output[out_index] = min;
        out_index++;
        if( out_index >= 1000) // When the output buffer is full, reset the out_index and append to output file
        {
            out_index = 0;
            fwrite(output, sizeof(int), 1000, outfile);
        }
        w++;
    }
//    printf("%d empty\n", empty_file);
    if(out_index != 0)
    {
        fwrite(output, sizeof(int), (size_t) out_index, outfile);
    }
    fclose(outfile);
}

void merge()
{

}

void multiMerge(FILE *inp)
{

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

int readLeftMost(int *ptr, int buffer_size)
{
    int i;
    int res;
    for( i = 0; i < buffer_size; i++)
    {
        if( *(ptr + i) != -1)
        {
            res = *(ptr + i);
            return res;
        }
    }
    return -1;
}

void setLeftMost(int *ptr, int buffer_size)
{
    int i;
    for(i = 0; i < buffer_size; i++)
    {
        if( *(ptr + i) != -1)
        {
            *(ptr + i) = -1;
            break;
        }
    }
}
