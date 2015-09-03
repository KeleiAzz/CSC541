#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int* inMemorySequential(int *key, int *seek, int key_len, int seek_len);
void inMemoryBinary();
void onDiskSequential();
void onDiskBinary();
int numOfInt(FILE *inp);
int* readIntoMemory(FILE *inp, int len);

int main(int argc, char *argv[])
{
    struct timeval start_time, end_time;

    FILE *keyFile, *seekFile;

    // Read the seek.db into memeory
    seekFile = fopen( argv[3], "rb");
    int seek_len = numOfInt(seekFile);
    int *seek = readIntoMemory( seekFile, seek_len );
    fclose(seekFile);

    gettimeofday( &start_time, NULL );
    int *result;
    if( strcmp( argv[1], "--mem-lin") == 0 )
    {
        keyFile = fopen( argv[2], "rb" );
        int key_len = numOfInt(keyFile);
        int *key = readIntoMemory(keyFile, key_len);
        fclose(keyFile);
        result = inMemorySequential(key, seek, key_len, seek_len);
    }
    else if (strcmp( argv[1], "--mem-bin") == 0)
    {
        keyFile = fopen( argv[2], "rb" );
        int key_len = numOfInt(keyFile);
        int *key = readIntoMemory(keyFile, key_len);
        fclose(keyFile);
        result = inMemoryBinary(key, seek, key_len, seek_len);
    }
    else if (strcmp( argv[1], "--disk-lin") == 0)
    {

    }
    else if (strcmp( argv[1], "--disk-bin") == 0)
    {

    }
    else
    {
        printf("Parameter error\n");
    }
    
    
    // int *seek = readIntoMemory(seekFile, seek_len);
    gettimeofday( &end_time, NULL );

    
    int k;
    for(k = 0; k < seek_len; k++){
        if(result[k]){
            printf("%12d: Yes\n", seek[k]);
        }
        else{
            printf("%12d: No\n", seek[k]);
        }
    }
    // int *num = readIntoMemory("key.db");
    // for(int i = 0; i < 100; i++){
    //     printf("%d\n", num[i]);
    // }
    
    // printf( "Seconds: %ld\n", tm.tv_sec );
    // printf( "Microseconds: %d\n", tm.tv_usec );
    printf("Time: %lf\n", (end_time.tv_sec - start_time.tv_sec)+(end_time.tv_usec-start_time.tv_usec)/1000000.0);
    return 0;
};

int* inMemorySequential(int *key, int *seek, int key_len, int seek_len)
{
    int *s = (int *)malloc(seek_len*sizeof(int));
    // int s[seek_len];
    int i, j;
    for(i = 0; i < seek_len; i++){
        for(int j = 0; j < key_len; j++){
            if(key[j] == seek[i]){
                s[i] = 1;
                break;
            }
            s[i] = 0;
        }
    }
    return s;
};

// void inMemoryBinary(char keyFileName[], char seekFileName[])
// {

// }

int* readIntoMemory(FILE *inp, int len){
    int *num = (int *)malloc(len*sizeof(int));
    fread( num, sizeof(int), len, inp);
    fclose(inp);
    return num;
}

int numOfInt(FILE *inp){
    fseek(inp, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(inp);
    rewind(inp);
    return len/4;
};
