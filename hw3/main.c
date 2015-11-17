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
void basicMerge(char *inp, char *output);
void multiMerge(char *inp, char *output_file);
int createRuns(char *inp);
void merge(int start, int runs_to_merge, FILE *outfile, char inf[]);
void sift(int *heap, int i, int n);
void heapify(int *heap, int n);
void replacementMerge(char *inp, char *output_file);
void heap_sort(int *heap, int n);


int main(int argc, char *argv[])
{
    struct timeval start_time, end_time;
    FILE *inp;
    char *s = strrchr(argv[2], '/');
    char *filename;
    if (!s) {
        filename = strdup(argv[2]);
//        printf("%s\n", filename);
    }
    else {
        filename = strdup(s + 1);
//        printf("%s\n", filename);
//        return 0;
    }
    if( strcmp( argv[1], "--basic") == 0 )
    {
        gettimeofday( &start_time, NULL );
//        inp = fopen(argv[2], "r");
        basicMerge(filename, argv[3]);
//        fclose(inp);
        gettimeofday( &end_time, NULL );
        printf("Time: %lf\n", (end_time.tv_sec - start_time.tv_sec)+(end_time.tv_usec-start_time.tv_usec)/1000000.0);
    }
    else if( strcmp( argv[1], "--multistep") == 0 )
    {
        gettimeofday( &start_time, NULL );
//        inp = fopen(argv[2], "r");
        multiMerge(filename, argv[3]);
//        fclose(inp);
        gettimeofday( &end_time, NULL );
        printf("Time: %lf\n", (end_time.tv_sec - start_time.tv_sec)+(end_time.tv_usec-start_time.tv_usec)/1000000.0);
    }
    else if( strcmp( argv[1], "--replacement") == 0 )
    {
        gettimeofday( &start_time, NULL );
//        inp = fopen(argv[2], "r");
        replacementMerge(filename, argv[3]);
//        fclose(inp);
        gettimeofday( &end_time, NULL );
        printf("Time: %lf\n", (end_time.tv_sec - start_time.tv_sec)+(end_time.tv_usec-start_time.tv_usec)/1000000.0);

    }
    else
    {
        printf("Wrong argument\n");
        exit(1);
    }

    return 0;
}

void basicMerge(char *inpfile, char *output)
{
//    FILE *inpfile = fopen( inp, "r");
    FILE *outfile = fopen( output, "w");
    int num_of_runs = createRuns(inpfile);
    char *name_pattern = malloc((strlen(inpfile) + 4) * sizeof(char));
    strcpy(name_pattern, inpfile);
    strcat(name_pattern, ".%03d");
    merge(0, num_of_runs, outfile, name_pattern);
    fclose(outfile);
}

int createRuns(char *inpfile)
{
    FILE *inp = fopen( inpfile, "r");
    int len = numOfInt(inp); // calculate number of int to sort
    if(len < 1000)
    {
        int *input = malloc(len * sizeof(int));
        fread(input, sizeof(int), len, inp);
        qsort(input, len, sizeof(int), cmpfunc);
        char *filename = malloc((strlen(inpfile) + 4) * sizeof(char));
        strcpy(filename, inpfile);
        strcat(filename, ".000");
        FILE *fp = fopen(filename, "w");
        fwrite(input, sizeof(int), len, fp);
        fclose(fp);
        free(filename);
        free(input);
        return 1;
    }
    int *input = malloc(1000 * sizeof(int));
    //----- Create runs -----
    int num_of_runs = len / 1000;
    if( num_of_runs * 1000 < len) //If len can't be divided by 1000 evenly, add an addition run
    {
        num_of_runs += 1;
    }
    int i;
    FILE *fp;
    char *filename = malloc((strlen(inpfile) + 4) * sizeof(char));
    for( i = 1; i <= num_of_runs - 1; i++ ) // The first num_of_runs-1 runs are sure to have 1000 int
    {
        fread(input, sizeof(int), 1000, inp);
        qsort(input, 1000, sizeof(int), cmpfunc);
//        heap_sort(input, 1000);
        char *index = malloc(4 * sizeof(char));
        sprintf(index, ".%03d", i - 1);
        strcpy(filename, inpfile);
        strcat(filename, index);

        fp = fopen(filename, "w");
        fwrite(input, sizeof(int), 1000, fp);
        fclose(fp);
    }
    fread(input, sizeof(int), (size_t) (len-1000*(num_of_runs - 1)), inp); // The last run may have less than 1000 int
//    printf("%d\n", (len-1000*(num_of_runs - 1)));
    qsort(input, (size_t) (len-1000*(num_of_runs - 1)), sizeof(int), cmpfunc);
//    heap_sort(input, )
    char *index = malloc(4 * sizeof(char));
    sprintf(index, ".%03d", num_of_runs - 1);
    strcpy(filename, inpfile);
    strcat(filename, index);
//    sprintf(filename, "input.bin.%03d", num_of_runs - 1);
    fp = fopen(filename, "w");
    fwrite(input, sizeof(int), (size_t) (len-1000*(num_of_runs - 1)), fp);
    fclose(fp);
    fclose(inp);
    free(input);
    free(index);
    free(filename);
    return num_of_runs;
}

void merge(int start, int runs_to_merge, FILE *outfile, char inf[])
{
    if(runs_to_merge == 1)
    {
        char *filename = malloc(13 * sizeof(char));
        sprintf(filename, inf, 0);
        FILE *file = fopen(filename, "r");
//        fread(input + i * num_to_buffer, sizeof(int), (size_t) num_to_buffer, files[i]);
        int len = numOfInt(file);
        int *input = malloc(len * sizeof(int));
        fread(input, sizeof(int), len, file);
//        FILE *output = fopen(outfile, "w");
        fwrite(input, sizeof(int), len, outfile);
        return ;
    }
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
//        printf("happens\n");
    }
    free(input);
    free(output);
}

void multiMerge(char *inpfile, char *output_file)
{
//    FILE *inp = fopen(inpfile, "r");
    int num_of_runs = createRuns(inpfile);

    // Create super runs
    int super_runs = num_of_runs / 15;
    if( super_runs * 15 < num_of_runs)
    {
        super_runs++;
    }
    char *filename = malloc((strlen(inpfile) + 10) * sizeof(char));
    int i;
    for( i = 0; i < super_runs - 1; i++ )
    {
        char *index = malloc(10 * sizeof(char));
        sprintf(index, ".super.%03d", i);
        strcpy(filename, inpfile);
        strcat(filename, index);

//        sprintf(filename, "input.bin.super.%03d", i);
        FILE *super_run_file = fopen(filename, "w");
        char *name_pattern = malloc((strlen(inpfile) + 4) * sizeof(char));
        strcpy(name_pattern, inpfile);
        strcat(name_pattern, ".%03d");
        merge( 0 + i * 15, 15, super_run_file, name_pattern);
        free(name_pattern);
        free(index);
        fclose(super_run_file);
    }
    char *index = malloc(10 * sizeof(char));
    sprintf(index, ".super.%03d", super_runs - 1);
    strcpy(filename, inpfile);
    strcat(filename, index);
//    sprintf(filename, "input.bin.super.%03d", super_runs - 1);
    FILE *super_run_file = fopen(filename, "w");
    char *name_pattern = malloc((strlen(inpfile) + 4) * sizeof(char));
    strcpy(name_pattern, inpfile);
    strcat(name_pattern, ".%03d");
    merge( 0 + (super_runs-1) * 15, num_of_runs - 15 * (super_runs - 1), super_run_file, name_pattern);
    fclose(super_run_file);
    free(name_pattern);
    free(index);
    free(filename);

    FILE *outfile = fopen(output_file, "w");
    char *name_pattern_super = malloc((strlen(inpfile) + 10) * sizeof(char));
    strcpy(name_pattern_super, inpfile);
    strcat(name_pattern_super, ".super.%03d");
    merge(0, super_runs, outfile, name_pattern_super);
    free(name_pattern_super);
}

void replacementMerge(char *inpfile, char *output_file)
{
    FILE *inp = fopen(inpfile, "r");
    int len = numOfInt(inp);
    if(len < 1000)
    {
        int *input = malloc(len * sizeof(int));
        fread(input, sizeof(int), len, inp);
        qsort(input, len, sizeof(int), cmpfunc);
        char *filename = malloc((strlen(inpfile) + 4) * sizeof(char));
        strcpy(filename, inpfile);
        strcat(filename, ".000");
        FILE *fp = fopen(filename, "w");
        fwrite(input, sizeof(int), len, fp);
        fclose(fp);
        free(filename);
        free(input);
        FILE *outfile = fopen(output_file, "w");
        char *name_pattern = malloc((strlen(inpfile) + 4) * sizeof(char));
        strcpy(name_pattern, inpfile);
        strcat(name_pattern, ".%03d");
        merge(0, 1, outfile, name_pattern);
        return ;
    }


    int num_output = 0;
    int *heap = malloc(750 * sizeof(int));
    int *buffer = malloc(250 * sizeof(int));
    int *output = malloc(1000 * sizeof(int));
    int out_index = 0;
    int buffer_index = 0;
    int num_in_heap = 750;
    char *filename = malloc((strlen(inpfile) + 4) * sizeof(char));
    int file_index = 1;
    char *index = malloc(4 * sizeof(char));
    sprintf(index, ".%03d", file_index - 1);
    strcpy(filename, inpfile);
    strcat(filename, index);
//    sprintf(filename, "input.bin.%03d", file_index - 1);

    FILE *current_file = fopen(filename, "w");

    fread(heap, sizeof(int), 750, inp);
    fread(buffer, sizeof(int), 250, inp);

    int buffered = 0;

    heapify(heap, 750);
    int buffer_availabe = 1;
    int secondary_heap_p = 750;
    int final_remain = 0;
    while(1)
    {
        if(buffer_availabe && buffer_index == final_remain && buffered == len - 1000)
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
                    if(buffer_index == 250 && len - 1000 - buffered > 250)
                    {
                        fread(buffer, sizeof(int), 250, inp);
                        buffer_index = 0;
                        buffered += 250;
                    }
                    else if(buffer_index == 250 && len - 1000 - buffered <= 250)
                    {
                        fread(buffer, sizeof(int), (size_t) (len - 1000 - buffered), inp);
                        buffer_index = 0;
                        final_remain = len - 1000 - buffered;
                        buffered = len - 1000;
                    }
                    heapify(heap, num_in_heap);
                }
                else
                {
                    heap[0] = heap[num_in_heap-1];
                    heap[num_in_heap-1] = buffer[buffer_index];
                    buffer_index++;
                    if(buffer_index == 250 && len - 1000 - buffered > 250)
                    {
                        fread(buffer, sizeof(int), 250, inp);
                        buffer_index = 0;
                        buffered += 250;
                    }
                    else if(buffer_index == 250 && len - 1000 - buffered <= 250)
                    {
                        fread(buffer, sizeof(int), (size_t) (len - 1000 - buffered), inp);
                        buffer_index = 0;
                        final_remain = len - 1000 - buffered;
                        buffered = len - 1000;
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
                sprintf(index, ".%03d", file_index - 1);
                strcpy(filename, inpfile);
                strcat(filename, index);
//                sprintf(filename, "input.bin.%03d", file_index - 1);
                current_file = fopen(filename, "w");
                num_in_heap = 750;
                heapify(heap, num_in_heap);
            }
        }
        else
        {
//            printf("%d buffered", buffered);
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
                sprintf(index, ".%03d", file_index - 1);
                strcpy(filename, inpfile);
                strcat(filename, index);
//                sprintf(filename, "input.bin.%03d", file_index - 1);
                current_file = fopen(filename, "w");
//                heap_sort(&heap[secondary_heap_p], 750-secondary_heap_p);
                qsort(&heap[secondary_heap_p], (size_t) (750-secondary_heap_p), sizeof(int), cmpfunc);
//                heap_sort(&heap[secondary_heap_p], 750-secondary_heap_p);
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
//        printf("opps\n");
    }
    FILE *outfile = fopen(output_file, "w");
    char *name_pattern = malloc((strlen(inpfile) + 4) * sizeof(char));
    strcpy(name_pattern, inpfile);
    strcat(name_pattern, ".%03d");
    merge(0, file_index, outfile, name_pattern);
//    printf("%d buffered\n", buffered);
}



void sift(int *heap, int i, int n)
{
    int j, k, sm, tmp;
    while(i * 2 + 1 < n)
    {
        j = i * 2 + 1;
        k = j + 1;
        if(k < n && heap[k] < heap[j])
        {
            sm = k;
        }
        else
        {
            sm = j;
        }
        if(heap[i] < heap[sm])
        {
            return ;
        }
        tmp = heap[i];
        heap[i] = heap[sm];
        heap[sm] = tmp;
        i = sm;
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
    int j = 1;
    while(n-1 > 0)
    {
//        int tmp = heap[n-1];
//        heap[n-1] = heap[0];
//        heap[0] = tmp;
        heapify(&heap[j], n - 1);
        j++;
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
