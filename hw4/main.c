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
long search_in_node(btree_node *node, int key, int num_of_int, long off, int *flag);
void add(int key, int order, FILE *inp);
void write_to_file(btree_node *node, int num_of_int, long off, FILE *inp, int order);
void add_to_non_leaf(int key, int node_in_path, long* path, long new_off, int order, FILE *inp);
int is_leaf(btree_node *node);
int cmpfunc(const void * a, const void * b);

int main(int argc, char *argv[]) {
    int order = 4; /* B-tree order */
    btree_node node; /* Single B-tree node */
    node.n = 0;
    node.key = (int *) calloc((size_t) (order - 1), sizeof( int ) );
    node.child = (long *) calloc((size_t) order, sizeof( long ) );
    FILE *fp; /* Input file stream */
    long root; /* Offset of B-tree root node */

    /* If file doesn't exist, set root offset to unknown and create * file,
     * otherwise read the root offset at the front of the file */
    fp = fopen( "index.bin", "r+b" );
    if ( fp == NULL )
    {
        root = (long) sizeof(long);
//        root = 10;
        fp = fopen( "index.bin", "w+b" );
        fwrite( &root, sizeof( long ), 1, fp );
        fwrite( &node.n, sizeof( int ), 1, fp );
        fwrite( node.key, sizeof( int ), (size_t) (order - 1), fp );
        fwrite( node.child, sizeof( long ), (size_t) order, fp );
        add( 2, 4, fp);
        long *path = malloc(sizeof(long));
        int *node_in_path = malloc(sizeof(int));
        *node_in_path = 0;
        int *flag = malloc(sizeof(int));
        *flag = -1;
        long off = search_node(root, 2, 4, fp, path, node_in_path, flag);
        printf("search: %ld\n", off);
        add( 3, order, fp);
        add( 4, order, fp);
        add( 5, order, fp);
        add( 6, order, fp);
        add( 7, order, fp);
        add( 8, order, fp);
        add( 1, order, fp);
        add( 9, order, fp);
        add( 10, order, fp);
        add( 11, order, fp);
        add( 12, order, fp);
        add( 13, order, fp);
        add( 14, order, fp);
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
        read_node(node, off, order, inp);
        if(off != 0 && *flag == 2) {
            *node_in_path += 1;
            long *tmp = realloc(path, *node_in_path * sizeof(int));
            *(tmp + *node_in_path - 1) = off;
            path = tmp;
        }
        if(*flag == 1) {
            break;
        }
    }
    printf("Get offset: %li\n", off);
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

long search_in_node(btree_node *node, int key, int num_of_int, long off, int *flag) {
    printf("search in node\n");
    int i;
    for(i = 0; i < num_of_int; i++) {
        if(*(node -> key + i) == key)
        {
            *flag = 1;
            return off;
        }
    }
    if(*(node -> key) > key) {
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
    for(i = 0; i < num_of_int - 1; i++) {
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
    if(*(node -> key + num_of_int - 1) < key) {
        if(*(node -> child + num_of_int) == 0)
        {
            *flag = 0;
            return off;
        }
        else{
            *flag = 2;
            printf("%li\n", *(node -> child + num_of_int));
            return *(node -> child + num_of_int);
        }
    }
    *flag = 0;
    return off;
}

void add(int key, int order, FILE *inp)
{
//    printf("add\n");
    long root;
    fseek(inp, 0, SEEK_SET);
    fread(&root, sizeof(long), 1, inp);
    int node_in_path = 1;
    long *path = malloc(sizeof(long));
    int *flag = malloc(4);
    *flag = -1;
    long off_to_add = search_node(root, key, order, inp, path, &node_in_path, flag);
    printf("In add: node in path: %d\n", node_in_path);
    if(*flag == 1) {
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
            int *tmp = realloc(node -> key, order * sizeof(int));
            *(tmp + num_of_int) = key;
            qsort(tmp, (size_t) order, sizeof(int), cmpfunc);
            int i;
            for(i = 0; i < order; i++){
                printf("%d\n", tmp[i]);
            }
            int split = 1 + ((order-1)/2);
//            printf("split %d\n", split);
            int *left = malloc( split * sizeof(int));
            for(i = 0; i < split; i++){
                *(left+i) = *(tmp+i);
                printf("\t%d\n", *(left+i));
            }
            int *right = malloc((order-split-1)* sizeof(int));
            for(i = 0; i < order - split - 1; i++)
            {
                *(right+i) = *(tmp+split+1+i);
                printf("\t%d\n", *(right+i));
            }
            node -> key = left;
            node -> n = split;
            write_to_file(node, 1, off_to_add, inp, order);
            btree_node *new_node = (btree_node *) malloc(sizeof(btree_node));
            new_node -> n = order-split-1;
//            new_node -> key = (int *) calloc((size_t) (order - 1), sizeof( int ) );
            new_node -> key = (int *)realloc(right, (order - 1)*sizeof(int));
            new_node -> child = (long *) calloc((size_t) order, sizeof( long ) );
            fseek(inp, 0, SEEK_END);
            long new_off = ftell(inp);
            write_to_file(new_node, 1, -1, inp, order);
            int to_promote = *(tmp + split);
            add_to_non_leaf(to_promote, node_in_path, path, new_off, order, inp);
        }
        else {
            *(node -> key + num_of_int) = key;

            node -> n++;
            qsort(node -> key, (size_t) node -> n, sizeof(int), cmpfunc);
            write_to_file(node, num_of_int, off_to_add, inp, order);
        }
    }
}


void write_to_file(btree_node *node, int num_of_int, long off, FILE *inp, int order)
{
    printf("write to file\n");
    if(off == -1){
        fseek(inp, 0, SEEK_END);
        fwrite( &(node -> n), sizeof( int ), 1, inp );
        fwrite( node -> key, sizeof( int ), (size_t) (order - 1), inp );
        fwrite( node -> child, sizeof( long ), (size_t) order, inp );
    }else{
        fseek(inp, off, SEEK_SET);
//    fwrite( &num_of_int, sizeof(int), 1, inp);
        fwrite( &(node -> n), sizeof( int ), 1, inp );
        fwrite( node -> key, sizeof( int ), (size_t) (order - 1), inp );
        fwrite( node -> child, sizeof( long ), (size_t) order, inp );
    }

}

void add_to_non_leaf(int key, int node_in_path, long* path, long new_off, int order, FILE *inp){
    if(node_in_path == 1){ // no parent node, means current is root, thus will generate new root
        btree_node *new_root = (btree_node *) malloc(sizeof(btree_node));
        new_root -> n = 1;
        new_root -> key = (int *)calloc((size_t) (order - 1),sizeof(int));
        new_root -> child = (long *) calloc((size_t) order, sizeof( long ) );
        *(new_root -> key) = key;
        *(new_root -> child) = *path;
        *(new_root -> child + 1) = new_off;
        fseek(inp, 0, SEEK_END);
        long eof = ftell(inp);
        rewind(inp);
        fwrite(&eof, sizeof(long), 1, inp);
        write_to_file(new_root, 1, -1, inp, order);
    }else{
        btree_node *target_node = (btree_node *) malloc(sizeof(btree_node));
        target_node -> key = (int *)calloc((size_t) (order - 1),sizeof(int));
        target_node -> child = (long *) calloc((size_t) order, sizeof( long ) );
        int num_of_int = read_node(target_node, *(path+node_in_path-2), order, inp);
        int position = 0;
        int i;
        for(i = 0; i < num_of_int; i++){
            if(*(target_node -> key + i) < key){
                position++;
            }
        }
        printf("position to add %d, new off %li\n", position, new_off);
        if(num_of_int == order - 1){
            //split and promote
            int *tmp = malloc( order * sizeof(int));
            for(i = 0; i < num_of_int; i++){
                *(tmp + i) = *(target_node -> key + i);
            }
            *(tmp + num_of_int) = key;
            qsort(tmp, (size_t) order, sizeof(int), cmpfunc);
//            int i;
            for(i = 0; i < order; i++){
                printf("%d\n", tmp[i]);
            }
            int split = 1 + ((order-1)/2);
//            printf("split %d\n", split);
            int *left = malloc( split * sizeof(int));
            for(i = 0; i < split; i++){
                *(left+i) = *(tmp+i);
                printf("\t%d\n", *(left+i));
            }
            int *right = malloc((order-split-1)* sizeof(int));
            for(i = 0; i < order - split - 1; i++)
            {
                *(right+i) = *(tmp+split+1+i);
                printf("\t%d\n", *(right+i));
            }
//            target_node -> key = left;
            for(i = 0; i < order - 1; i++){
//                printf("target node key %d\n", *(target_node -> key +i));
                if(i < split){
                    *(target_node -> key + i) = *(left + i);
                }else{
                    *(target_node -> key + i) = 0;
                }

            }
            target_node -> n = split;
            //TODO need to update child
            long *off_list = malloc((num_of_int+2) * sizeof(long));
            if(position == num_of_int){
                off_list = target_node -> child;
                *(off_list+num_of_int+1) = new_off;
            }else{
                i = 0;
                int j = 0;
                while(i < num_of_int + 1)
                {
                    if(i == position + 1){
                        *(off_list+j) = new_off;
                        printf("``%li\n", new_off);
                        j++;
                    }
                    else{
                        *(off_list+j) = *(target_node -> child + i);
                        printf("``%li\n", *(off_list+j));
                        j++;
                        i++;
                    }
                }
            }
            for(i = 0; i < num_of_int + 2; i++){
                printf("off list %li\n", *(off_list+i));
            }
//            for(i = 0; i < order; i++){
//                *(target_node -> child + i) = 0;
//            }
            btree_node *new_node = (btree_node *) malloc(sizeof(btree_node));
            new_node -> n = order-split-1;
//            new_node -> key = (int *) calloc((size_t) (order - 1), sizeof( int ) );
            new_node -> key = (int *)realloc(right, (order - 1)*sizeof(int));
            new_node -> child = (long *) calloc((size_t) order, sizeof( long ) );

            for(i = 0; i < order-split; i++){
                *(new_node -> child + i ) = *(off_list + i + split + 1);
            }
            for(i = 0; i < order; i++ ){
                if(i > split){
                    *(target_node -> child + i) = 0;
                }else{
                    *(target_node -> child + i) = *(off_list + i);
                }
            }


            write_to_file(target_node, 1, *(path+node_in_path-2), inp, order);

            fseek(inp, 0, SEEK_END);
            long eof = ftell(inp);
            //TODO need to update child
            write_to_file(new_node, 1, -1, inp, order);
            int to_promote = *(tmp + split);
            printf("%d to promote", to_promote);
            node_in_path--;
            add_to_non_leaf(to_promote, node_in_path, path, eof, order, inp);
//            printf("to be implemented\n");
        }else{
            *(target_node -> key + num_of_int) = key;
            target_node -> n = target_node -> n + 1;
            qsort(target_node -> key, (size_t) target_node -> n, sizeof(int), cmpfunc);
            // need to update child

            if(position == num_of_int)
            {
                *(target_node -> child + position + 1) = new_off;
            }
            else{
                long *tmp = malloc((num_of_int+2) * sizeof(long));
                i = 0;
                int j = 0;
                while(i < num_of_int + 1)
                {
                    if(i == position + 1){
                        *(tmp+j) = new_off;
                        printf("``%li\n", new_off);
                        j++;
                    }
                    else{
                        *(tmp+j) = *(target_node -> child + i);
                        printf("``%li\n", *(tmp+j));
                        j++;
                        i++;
                    }
                }
                target_node -> child = tmp;
            }


            write_to_file(target_node, num_of_int, *(path+node_in_path-2), inp, order);
            node_in_path--;
        }

    }
}

void print(FILE *inp){
    long root;
    fseek(inp, 0, SEEK_SET);
    fread(&root, sizeof(long), 1, inp);

}

int is_leaf(btree_node *node)
{
    if(node -> child == 0) return 1;
    return 0;
}
int cmpfunc(const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}