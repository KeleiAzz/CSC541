#include <stdio.h>
#include <sys/time.h> 
// FILE *inp; /* Input file stream */ 
// int k1, k2, k3; /* Keys to read */ 
// inp = fopen( "key.db", "rb" ); 
// fread( &k1, sizeof( int ), 1, inp ); 
// fseek( inp, 2 * sizeof( int ), SEEK_SET ); 
// fread( &k2, sizeof( int ), 1, inp ); 
// fseek( inp, -2 * sizeof( int ), SEEK_END ); 
// fread( &k3, sizeof( int ), 1, inp );


int main(int argc, const char *argv[])
{   
    struct timeval tm; 
    
    FILE *inp; /* Input file stream */
    int key[5000], seek[10000]; /* Keys to read */
    inp = fopen( "seek.db", "rb" );
    fread( &seek, sizeof( int ), 10000, inp );
    
    gettimeofday( &tm, NULL ); 
    printf( "Seconds: %ld\n", tm.tv_sec ); 
    printf( "Microseconds: %d\n", tm.tv_usec );
    
    inp = fopen( "key.db", "rb" );
    fread( &key, sizeof( int ), 5000, inp );
    
    gettimeofday( &tm, NULL );
    printf( "Seconds: %ld\n", tm.tv_sec ); 
    printf( "Microseconds: %d\n", tm.tv_usec );
    
    int s[10000];
    for(int i = 0; i < 10000; i++){
        for(int j = 0; j < 5000; j++){
            if(key[j] == seek[i]){
                s[i]=1;
            }
        }
    }
    gettimeofday( &tm, NULL ); 
    printf( "Seconds: %ld\n", tm.tv_sec ); 
    printf( "Microseconds: %d\n", tm.tv_usec );
    
    for(int i = 0; i< 10000; i++){
        printf("%d\n", s[i]);
    }
    return 0;
};
