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
int createRuns(FILE *inp);
void merge(int start, int runs_to_merge, FILE *outfile, char inf[]);
void sift(int *heap, int i, int n);
void heapify(int *heap, int n);
void replacementMerge(FILE *inp);
void heap_sort(int *heap, int n);


int main(int argc, char *argv[])
{
    struct timeval start_time, end_time;
    FILE *inp = fopen("input.bin", "r");
    gettimeofday( &start_time, NULL );
    basicMerge(inp);
    gettimeofday( &end_time, NULL );
    printf("Time: %lf\n", (end_time.tv_sec - start_time.tv_sec)+(end_time.tv_usec-start_time.tv_usec)/1000000.0);

    gettimeofday( &start_time, NULL );
    multiMerge(inp);
    gettimeofday( &end_time, NULL );
    printf("Time: %lf\n", (end_time.tv_sec - start_time.tv_sec)+(end_time.tv_usec-start_time.tv_usec)/1000000.0);

    gettimeofday( &start_time, NULL );
    replacementMerge(inp);
    gettimeofday( &end_time, NULL );
    printf("Time: %lf\n", (end_time.tv_sec - start_time.tv_sec)+(end_time.tv_usec-start_time.tv_usec)/1000000.0);

    return 0;
}

void basicMerge(FILE *inp)
{
    FILE *outfile = fopen( "sort.bin", "w");
    int num_of_runs = createRuns(inp);
    merge(1, num_of_runs, outfile, "input.bin.%03d");
    fclose(outfile);
}

int createRuns(FILE *inp)
{
    int len = numOfInt(inp); // calculate number of int to sort

    int *input = malloc(1000 * sizeof(int));
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
//    printf("%d\n", (len-1000*(num_of_runs - 1)));
    qsort(input, (size_t) (len-1000*(num_of_runs - 1)), sizeof(int), cmpfunc);
    sprintf(filename, "input.bin.%03d", num_of_runs);
    fp = fopen(filename, "w");
    fwrite(input, sizeof(int), (size_t) (len-1000*(num_of_runs - 1)), fp);
    fclose(fp);
    free(input);
    return num_of_runs;
}

void merge(int start, int runs_to_merge, FILE *outfile, char inf[])
{
    int *output = malloc(1000 * sizeof(int));
    int *input = malloc(1000 * sizeof(int));
    int num_to_buffer = 1000 / runs_to_merge;
    char *filename = malloc(13 * sizeof(char));
    FILE *files[runs_to_merge];
    int i;
    for( i = 0; i < runs_to_merge; i++ )
    {
        sprintf(filename, inf, i + start);
        files[i] = fopen(filename, "r");
    }
    int *ptr[runs_to_merge];
    for( i = 0; i < runs_to_merge; i++ )
    {
        fread(input + i * num_to_buffer, sizeof(int), (size_t) num_to_buffer, files[i]);
        ptr[i] = input + i * num_to_buffer;
    }
    int tmp;
    int min;
    int min_index = 0;
    int out_index = 0;
    int empty_file = 0;
    while(empty_file < runs_to_merge)
    {
        min = INT_MAX;
        min_index = -1;
        for( i = 0; i < runs_to_merge; i++)
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
                if(files[i] != NULL && !feof(files[i]))
                {
                    fread(ptr[i], sizeof(int), (size_t) num_to_buffer, files[i]);
                    tmp = readLeftMost(ptr[i], num_to_buffer);
                    if(tmp == -1)
                    {
                        empty_file++;
                        files[i] = NULL;
                    }
                    else if ( tmp < min) {
                        min = tmp;
                        min_index = i;
                    }
                }
                else if(files[i] != NULL)
                {
                    empty_file++;
                    files[i] = NULL;
                }

            }
        }
        if( min_index != -1)
        {
            setLeftMost(ptr[min_index], num_to_buffer); // When find the smallest int, put it into output buffer
            output[out_index] = min;
            out_index++;
            if( out_index >= 1000) // When the output buffer is full, reset the out_index and append to output file
            {
                out_index = 0;
                fwrite(output, sizeof(int), 1000, outfile);
            }
        }
    }

    if(out_index != 0)
    {
        fwrite(output, sizeof(int), (size_t) out_index, outfile);
        printf("happens\n");
    }
    free(input);
    free(output);
}

void multiMerge(FILE *inp)
{
    int num_of_runs = createRuns(inp);

    // Create super runs
    int super_runs = num_of_runs / 15;
    if( super_runs * 15 < num_of_runs)
    {
        super_runs++;
    }
    char *filename = malloc(19 * sizeof(char));
    int i;
    for( i = 0; i < super_runs - 1; i++ )
    {
        sprintf(filename, "input.bin.super.%03d", i + 1);
        FILE *super_run_file = fopen(filename, "w");
        merge( 1 + i * 15, 15, super_run_file, "input.bin.%03d");
        fclose(super_run_file);
    }
    sprintf(filename, "input.bin.super.%03d", super_runs);
    FILE *super_run_file = fopen(filename, "w");
    merge( 1 + (super_runs-1) * 15, num_of_runs - 15 * (super_runs - 1), super_run_file, "input.bin.%03d");
    fclose(super_run_file);

    FILE *outfile = fopen("sort_multi.bin", "w");
    merge(1, 17, outfile, "input.bin.super.%03d");
}

void replacementMerge(FILE *inp)
{
    int len = numOfInt(inp);
    int num_output = 0;
    int *heap = malloc(750 * sizeof(int));
    int *buffer = malloc(250 * sizeof(int));
    int *output = malloc(1000 * sizeof(int));
    int out_index = 0;
    int buffer_index = 0;
    int num_in_heap = 750;
    char *filename = malloc(16 * sizeof(char));
    int file_index = 1;
    sprintf(filename, "input.bin.re.%03d", file_index);
    FILE *current_file = fopen(filename, "w");


    fread(heap, sizeof(int), 750, inp);
    fread(buffer, sizeof(int), 250, inp);

    int buffered = 0;

    int tmp = INT_MAX, i;
    for ( i = 0; i < 750; ++i) {
        if(tmp > heap[i])
        {
            tmp = heap[i];
        }
    }
    heapify(heap, 750);
    printf("%d %d %d\n", heap[0], heap[1], tmp);
    int buffer_availabe = 1;
    int secondary_heap_p = 750;
    while(1 <= len)
    {
        if(buffer_availabe && buffer_index == 250 && buffered == len - 1000)
        {
            buffer_availabe = 0;
            secondary_heap_p = num_in_heap;
        }
        if(buffer_availabe)
        {
            if(num_in_heap > 0)
            {
                output[out_index] = heap[0];
                out_index++;
                if(out_index == 1000)
                {
                    fwrite(output, sizeof(int), 1000, current_file);
                    num_output += 1000;
                    out_index = 0;
                }
                if(heap[0] <= buffer[buffer_index])
                {
                    heap[0] = buffer[buffer_index];
                    buffer_index++;
                    if(buffer_index == 250 && buffered < len - 1000)
                    {
                        fread(buffer, sizeof(int), 250, inp);
                        buffer_index = 0;
                        buffered += 250;
                    }
                    heapify(heap, num_in_heap);
                }
                else
                {
                    heap[0] = heap[num_in_heap-1];
                    heap[num_in_heap-1] = buffer[buffer_index];
                    buffer_index++;
                    if(buffer_index == 250 && buffered < len - 1000)
                    {
                        fread(buffer, sizeof(int), 250, inp);
                        buffer_index = 0;
                        buffered += 250;
                    }
                    num_in_heap--;
                    heapify(heap, num_in_heap);
                }

            }
            else
            {
                // Current run ends
                if(out_index > 0)
                {
                    fwrite(output, sizeof(int), (size_t) out_index, current_file);
                    num_output += out_index;
                    out_index = 0;
                }
                fclose(current_file);
                file_index++;
                sprintf(filename, "input.bin.re.%03d", file_index);
                current_file = fopen(filename, "w");
                num_in_heap = 750;
                heapify(heap, num_in_heap);
            }
        }
        else
        {
            if(num_in_heap > 0)
            {
                output[out_index] = heap[0];
                out_index++;
                if(out_index == 1000)
                {
                    fwrite(output, sizeof(int), 1000, current_file);
                    num_output += 1000;
                    out_index = 0;
                }
                heap[0] = heap[num_in_heap-1];
                num_in_heap--;
                heapify(heap, num_in_heap);
            }
            else
            {
                if(out_index > 0)
                {
                    fwrite(output, sizeof(int), (size_t) out_index, current_file);
                    num_output += out_index;
                    out_index = 0;
                }
                fclose(current_file);
                file_index++;
                sprintf(filename, "input.bin.re.%03d", file_index);
                current_file = fopen(filename, "w");
//                heap_sort(&heap[secondary_heap_p], 750-secondary_heap_p);
                qsort(&heap[secondary_heap_p], (size_t) (750-secondary_heap_p), sizeof(int), cmpfunc);
                fwrite(&heap[secondary_heap_p], sizeof(int), (size_t) (750-secondary_heap_p), current_file);
                fclose(current_file);
                break;
            }
        }

    }
    if(out_index > 0)
    {
        fwrite(output, sizeof(int), (size_t) out_index, current_file);
        fclose(current_file);
        printf("opps\n");
    }
    FILE *outfile = fopen("sort_re.bin", "w");
    merge(1, file_index, outfile, "input.bin.re.%03d");
    printf("%d buffered\n", buffered);
}



void sift(int *heap, int i, int n)
{
    int j, k, lg, tmp;
    while(i * 2 + 1 < n)
    {
        j = i * 2 + 1;
        k = j + 1;
        if(k < n && heap[k] < heap[j])
        {
            lg = k;
        }
        else
        {
            lg = j;
        }
        if(heap[i] < heap[lg])
        {
            return ;
        }
        tmp = heap[i];
        heap[i] = heap[lg];
        heap[lg] = tmp;
        i = lg;
    }
}

void heapify(int *heap, int n)
{
    int i = n / 2;
    for(i; i >= 0; i--)
    {
        sift(heap, i, n);
    }
//    while(n-1 > 0)
//    {
//        int tmp = heap[n-1];
//        heap[n-1] = heap[0];
//        heap[0] = tmp;
//        sift(heap, 0, n-1);
//        n--;
//    }
}

void heap_sort(int *heap, int n)
{
    int i = n / 2;
    for(i; i >= 0; i--)
    {
        sift(heap, i, n);
    }
    while(n-1 > 0)
    {
        int tmp = heap[n-1];
        heap[n-1] = heap[0];
        heap[0] = tmp;
        sift(heap, 0, n-1);
        n--;
    }
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
