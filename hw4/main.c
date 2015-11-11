//
// Created by Kelei Gong on 11/11/15.
//

//#include "main.h"

#include <stdio.h>
#include <stdlib.h>
typedef struct { /* B-tree node */
    int n; /* Number of keys in node */
    int *key; /* Node's keys */
    long *child; /* Node's child subtree offsets */
} btree_node;


int main(int argc, char *argv[])
{
    int order = 4; /* B-tree order */
    btree_node node; /* Single B-tree node */
    node.n = 0;
    node.key = (int *) calloc( order - 1, sizeof( int ) );
    node.child = (long *) calloc( order, sizeof( long ) );
    FILE *fp; /* Input file stream */
    long root; /* Offset of B-tree root node */
    fp = fopen( "index.bin", "r+b" ); /* If file doesn't exist, set root offset to unknown and create * file, otherwise read the root offset at the front of the file */
    if ( fp == NULL )
    {
        root = -1;
        fp = fopen( "index.bin", "w+b" );
        fwrite( &root, sizeof( long ), 1, fp );
    }
    else
    {
        fread( &root, sizeof( long ), 1, fp );
    }


    return 0;
}