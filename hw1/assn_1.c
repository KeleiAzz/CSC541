#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int* inMemorySequential(int *key, int *seek, int key_len, int seek_len);
int inMemoryBinary(int *key, int seek, int lo, int hi);
int* onDiskSequential(FILE *keyFile, int *seek, int key_len, int seek_len);
int onDiskBinary(FILE *keyFile, int seek, int lo, int hi);
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
    int *result = (int *)malloc(seek_len*sizeof(int));

    // Start recording time
    gettimeofday( &start_time, NULL );
    
    // Use different function 
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
        for( int i = 0; i < seek_len; i++)
        {
            result[i] = inMemoryBinary(key, seek[i], 0, key_len - 1);
        }
    }
    else if (strcmp( argv[1], "--disk-lin") == 0)
    {
        keyFile = fopen( argv[2], "rb" );
        int key_len = numOfInt(keyFile);
        result = onDiskSequential(keyFile, seek, key_len, seek_len);
        fclose(keyFile);
        
    }
    else if (strcmp( argv[1], "--disk-bin") == 0)
    {
        keyFile = fopen( argv[2], "rb" );
        int key_len = numOfInt(keyFile);
        for( int i = 0; i < seek_len; i++)
        {
            result[i] = onDiskBinary(keyFile, seek[i], 0, key_len - 1);
        }
        fclose(keyFile);
    }
    else
    {
        printf("Parameter error\n");
    }
    
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
    printf("Time: %lf\n", (end_time.tv_sec - start_time.tv_sec)+(end_time.tv_usec-start_time.tv_usec)/1000000.0);
    return 0;
};

int* inMemorySequential(int *key, int *seek, int key_len, int seek_len)
{
    int *s = (int *)malloc(seek_len*sizeof(int));
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

int inMemoryBinary(int *key, int seek, int lo, int hi)
{
    int mid;
    if( lo > hi)
    {
        return 0;
    }
    mid = ( lo + hi ) / 2;
    if( key[mid] == seek )
    {
        return 1;
    }
    else if( seek > key[mid] )
    {
        return inMemoryBinary(key, seek, mid + 1, hi);
    }
    else if( seek < key[mid] )
    {
        return inMemoryBinary(key, seek, lo, mid - 1);
    }
}

int* onDiskSequential(FILE *keyFile, int *seek, int key_len, int seek_len)
{
    int *s = (int *)malloc(seek_len*sizeof(int));
    int i, j, tmp;
    for(i = 0; i < seek_len; i++){
        for(int j = 0; j < key_len; j++){
            fseek( keyFile, j * 4, SEEK_SET);
            fread( &tmp, 4, 1, keyFile);
            if(tmp == seek[i]){
                s[i] = 1;
                break;
            }
            s[i] = 0;
        }
    }
    return s;
}

int onDiskBinary(FILE *keyFile, int seek, int lo, int hi)
{
    int mid, tmp;
    if( lo > hi)
    {
        return 0;
    }
    mid = ( lo + hi ) / 2;

    fseek( keyFile, mid * sizeof(int), SEEK_SET);
    fread( &tmp, sizeof(int), 1, keyFile);

    if( tmp == seek )
    {
        return 1;
    }
    else if( seek > tmp )
    {
        return onDiskBinary(keyFile, seek, mid + 1, hi);
    }
    else if( seek < tmp )
    {
        return onDiskBinary(keyFile, seek, lo, mid - 1);
    }
}

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
