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
long search_node(long off, int key, int order, FILE *inp, long *path, int *node_in_path, int *flag);
int read_node(btree_node *node, long off, int order, FILE *inp);
long search_in_node(btree_node *node, int key, int num_of_int, int off, int *flag);
void add(long off, int key, int order, FILE *inp);
void write_to_file(btree_node *node, int num_of_int, long off, FILE *inp, int order);
int is_leaf(btree_node *node);

int main(int argc, char *argv[])
{
    int order = 4; /* B-tree order */
    btree_node node; /* Single B-tree node */
    node.n = 0;
    node.key = (int *) calloc((size_t) (order - 1), sizeof( int ) );
    node.child = (long *) calloc((size_t) order, sizeof( long ) );
    FILE *fp; /* Input file stream */
    long root; /* Offset of B-tree root node */
    fp = fopen( "index.bin", "r+b" ); /* If file doesn't exist, set root offset to unknown and create * file, otherwise read the root offset at the front of the file */
    if ( fp == NULL )
    {
        root = (long) sizeof(long);
//        root = 10;
        fp = fopen( "index.bin", "w+b" );
        fwrite( &root, sizeof( long ), 1, fp );
        fwrite( &node.n, sizeof( int ), 1, fp );
        fwrite( node.key, sizeof( int ), (size_t) (order - 1), fp );
        fwrite( node.child, sizeof( long ), (size_t) order, fp );
        add(root, 2, 4, fp);
        fclose(fp);
    }
    else {
        fread(&root, sizeof(long), 1, fp);
    }
    return 0;
}



long search_node(long off, int key, int order, FILE *inp, long *path, int *node_in_path, int *flag)
{
//    *path = malloc(sizeof(long));
//    int node_in_path = 1;
    printf("search\n");
    *path = off;
    btree_node *node = (btree_node *) malloc(sizeof(btree_node));
    node -> key = (int *) calloc((size_t) (order - 1), sizeof( int ) );
    node -> child = (long *) calloc((size_t) order, sizeof( long ) );
    int num_of_int = read_node(node, off, order, inp);
//    int flag = -1;
    *flag = -1;
    while(*flag != 0)
    {
        off = search_in_node(node, key, num_of_int, off, flag);
        if(off != 0 && *flag == 2)
        {
            *node_in_path += 1;
            long *tmp = realloc(path, *node_in_path * sizeof(int));
            *(tmp + *node_in_path - 1) = off;
            path = tmp;
        }
        if(*flag == 1)
        {
            break;
        }
    }
    return off;
}

int read_node(btree_node *node, long off, int order, FILE *inp)
{
//    btree_node node;
    printf("read node\n");
    fseek(inp, off, SEEK_SET);
//    int num_of_int;
//    fread(&num_of_int, sizeof(int), 1, inp);
    fread(&(node ->n), sizeof(int), 1, inp);
//    node -> key = (int *) calloc( order - 1, sizeof( int ) );
//    node -> child = (int *) calloc( order, sizeof( int ) );
    fread(node -> key, sizeof(int), (size_t) (order - 1), inp);
    fread(node -> child, sizeof(long), (size_t) order, inp);
    return node -> n;
}

long search_in_node(btree_node *node, int key, int num_of_int, int off, int *flag)
{
    printf("search in node\n");
    int i;
    for(i = 0; i < num_of_int; i++)
    {
        if(*(node -> key + i) == key)
        {
            *flag = 1;
            return off;
        }
    }
    if(*(node -> key) > key)
    {
        if(*(node -> child) == 0)
        {
            *flag = 0;
            return off;
        }
        else{
            *flag = 2;
            return *(node -> child);
        }
    }
    for(i = 0; i < num_of_int - 1; i++)
    {
        if( *(node -> key + i) < key && *(node -> key + i + 1) >key )
        {
            if(*(node -> child + i + 1) == 0)
            {
                *flag = 0;
                return off;
            }
            else{
                *flag = 2;
                return *(node -> child + i + 1);
            }
        }
    }
    if(*(node -> key + num_of_int - 1) > key)
    {
        if(*(node -> child + num_of_int) == 0)
        {
            *flag = 0;
            return off;
        }
        else{
            *flag = 2;
            return *(node -> child + num_of_int);
        }
    }
    *flag = 0;
    return off;
}

void add(long off, int key, int order, FILE *inp)
{
    printf("add\n");
    int node_in_path = 1;
    long *path = malloc(sizeof(long));
    int *flag = malloc(4);
    *flag = -1;
    long off_to_add = search_node(off, key, order, inp, path, &node_in_path, flag);
    if(*flag == 1)
    {
        return;
    }
    else
    {
        btree_node *node = (btree_node *) malloc(sizeof(btree_node));
        node -> key = (int *) calloc((size_t) (order - 1), sizeof( int ) );
        node -> child = (long *) calloc((size_t) order, sizeof( long ) );
        int num_of_int = read_node(node, off_to_add, order, inp);
        if(num_of_int == order - 1)
        {
            //split and promote
        }
        else
        {
            *(node -> key + num_of_int) = key;
            node -> n++;
            write_to_file(node, num_of_int, off_to_add, inp, order);
        }
    }
}


void write_to_file(btree_node *node, int num_of_int, long off, FILE *inp, int order)
{
    printf("write to file\n");
    fseek(inp, off, SEEK_SET);
//    fwrite( &num_of_int, sizeof(int), 1, inp);
    fwrite( &(node -> n), sizeof( int ), 1, inp );
    fwrite( node -> key, sizeof( int ), (size_t) (order - 1), inp );
    fwrite( node -> child, sizeof( long ), (size_t) order, inp );
}

int is_leaf(btree_node *node)
{
    if(node -> child == 0) return 1;
    return 0;
}