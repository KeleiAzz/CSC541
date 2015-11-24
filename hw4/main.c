//
// Created by Kelei Gong on 11/11/15.
//

//#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { /* B-tree node */
    int n; /* Number of keys in node */
    int *key; /* Node's keys */
    long *child; /* Node's child subtree offsets */
} btree_node;

struct Node {
    long data;
    int layer;
    struct Node* next;
} *front,*rear,*temp,*front1;
void enq(long data, int layer);
int deq(btree_node *node, int order, FILE *inp);
void empty();
//void display();
void create();
int qsize = 0;
void find( int key, int order, FILE *inp);
long search_node(long off, int key, int order, FILE *inp, long *path, int *node_in_path, int *flag);
int read_node(btree_node *node, long off, int order, FILE *inp);
long search_in_node(btree_node *node, int key, int num_of_int, long off, int *flag);
void add(int key, int order, FILE *inp);
void write_to_file(btree_node *node, int num_of_int, long off, FILE *inp, int order);
void add_to_non_leaf(int key, int node_in_path, long* path, long new_off, int order, FILE *inp);
int is_leaf(btree_node *node);
void printout(FILE *inp, int order);
int cmpfunc(const void * a, const void * b);
char** parse_command(char *input);
int phrase_counter(char **command);
int* calculatePhraseLength(char **command, int phrase_count);

int main(int argc, char *argv[]) {
    int order = 5; /* B-tree order */
    btree_node *node = (btree_node *) malloc(sizeof(btree_node)); /* Single B-tree node */
    node -> n = 0;
    node -> key = (int *) calloc((size_t) (order - 1), sizeof( int ) );
    node -> child = (long *) calloc((size_t) order, sizeof( long ) );

    FILE *fp; /* Input file stream */
    long root; /* Offset of B-tree root node */

    /* If file doesn't exist, set root offset to unknown and create * file,
     * otherwise read the root offset at the front of the file */
    char *filename = argv[1];
    order = atoi(argv[2]);
    fp = fopen( filename, "r+b" );
    if ( fp == NULL )
    {
        root = (long) sizeof(long);
//        root = 10;
        fp = fopen( filename, "w+b" );
        fwrite( &root, sizeof( long ), 1, fp );
        fwrite( &(node -> n), sizeof( int ), 1, fp );
        fwrite( node -> key, sizeof( int ), (size_t) (order - 1), fp );
        fwrite( node -> child, sizeof( long ), (size_t) order, fp );
    }
    while(1){
        char input[150];
        fgets(input, 150, stdin);
        char **command = parse_command(input);
        int phrase_count = phrase_counter(command);
        int *phraseLength = calculatePhraseLength(command, phrase_count);
        if(phrase_count == 2 || phrase_count == 3){
            char key_char[phraseLength[1] + 1];
            strncpy(key_char, command[1], phraseLength[1]);
            key_char[phraseLength[1]] = '\0';
            int pk = atoi(key_char);
            if(phraseLength[0] == 3){
                add( pk, order, fp);
            }else{
                find(pk, order, fp);
            }

        }else{
            if(phraseLength[0] == 5) printout(fp, order);
            else{
                break;
            }
        }
//        break;
    }
    fclose(fp);
    return 0;
}

void find( int key, int order, FILE *inp)
{
    long root;
    fseek(inp, 0, SEEK_SET);
    fread(&root, sizeof(long), 1, inp);
    int node_in_path = 1;
    long *path = malloc(sizeof(long));
    int *flag = malloc(4);
    *flag = -1;
    long off_to_add = search_node(root, key, order, inp, path, &node_in_path, flag);
    if(*flag == 0){
        printf("Entry with key=%d does not exist\n", key);
    } else{
        printf("Entry with key=%d exists\n", key);
    }
}


long search_node(long off, int key, int order, FILE *inp, long *path, int *node_in_path, int *flag)
{
    *path = off;
    btree_node *node = (btree_node *) malloc(sizeof(btree_node));
    node -> key = (int *) calloc((size_t) (order - 1), sizeof( int ) );
    node -> child = (long *) calloc((size_t) order, sizeof( long ) );
    int num_of_int = read_node(node, off, order, inp);
    int i;

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
    free( node -> key);
    free(node -> child);
    free(node);
    return off;
}

int read_node(btree_node *node, long off, int order, FILE *inp)
{
    fseek(inp, off, SEEK_SET);
    fread(&(node ->n), sizeof(int), 1, inp);
    fread(node -> key, sizeof(int), (size_t) (order - 1), inp);
    fread(node -> child, sizeof(long), (size_t) order, inp);
    int i;
    if( *(node -> child) == 0){
        for(i = 0; i < order; i++){
            *(node -> child + i) = 0;
        }
    }
    return node -> n;
}

long search_in_node(btree_node *node, int key, int num_of_int, long off, int *flag) {
    int i;
    for(i = 0; i < node -> n; i++) {
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
    for(i = 0; i < node -> n - 1; i++) {
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
    if(*(node -> key + node -> n - 1) < key) {
        if(*(node -> child + node -> n) == 0)
        {
            *flag = 0;
            return off;
        }
        else{
            *flag = 2;
            return *(node -> child + node -> n);
        }
    }
    *flag = 0;
    return off;
}

void add(int key, int order, FILE *inp)
{
    long root;
    fseek(inp, 0, SEEK_SET);
    fread(&root, sizeof(long), 1, inp);
    int node_in_path = 1;
    long *path = malloc(sizeof(long));
    int *flag = malloc(4);
    *flag = -1;
    long off_to_add = search_node(root, key, order, inp, path, &node_in_path, flag);

    if(*flag == 1) {
        printf("Entry with key=%d already exists\n", key);
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
            int *tmp = realloc(node -> key, order * sizeof(int));
            *(tmp + num_of_int) = key;
            qsort(tmp, (size_t) order, sizeof(int), cmpfunc);
            int i;
            for(i = 0; i < order; i++){
            }
            int split = 1 + ((order-1)/2);
            int *left = malloc( split * sizeof(int));
            for(i = 0; i < split; i++){
                *(left+i) = *(tmp+i);
            }
            int *right = malloc((order-split-1)* sizeof(int));
            for(i = 0; i < order - split - 1; i++)
            {
                *(right+i) = *(tmp+split+1+i);
            }
            int to_promote = *(tmp + split);
            for(i = 0; i < order - 1; i++ ){
                if(i < split){
                    *(node -> key + i) = *(left + i);
                }else{
                    *(node -> key + i) = 0;
                }
            }

            node -> n = split;
            write_to_file(node, 1, off_to_add, inp, order);
            btree_node *new_node = (btree_node *) malloc(sizeof(btree_node));
            new_node -> n = order-split-1;
            new_node -> key = (int *)realloc(right, (order - 1)*sizeof(int));
            new_node -> child = (long *) calloc((size_t) order, sizeof( long ) );
            fseek(inp, 0, SEEK_END);
            long new_off = ftell(inp);
            write_to_file(new_node, 1, -1, inp, order);
            add_to_non_leaf(to_promote, node_in_path, path, new_off, order, inp);
        }
        else {
            *(node -> key + num_of_int) = key;
            node -> n++;
            qsort(node -> key, (size_t) node -> n, sizeof(int), cmpfunc);
            write_to_file(node, num_of_int, off_to_add, inp, order);
        }
    }
    free(path);
    free(flag);
}


void write_to_file(btree_node *node, int num_of_int, long off, FILE *inp, int order)
{
    if(off == -1){
        fseek(inp, 0, SEEK_END);
        fwrite( &(node -> n), sizeof( int ), 1, inp );
        fwrite( node -> key, sizeof( int ), (size_t) (order - 1), inp );
        fwrite( node -> child, sizeof( long ), (size_t) order, inp );
    }else{
        fseek(inp, off, SEEK_SET);
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

        if(num_of_int == order - 1){
            int *tmp = malloc( order * sizeof(int));
            for(i = 0; i < num_of_int; i++){
                *(tmp + i) = *(target_node -> key + i);
            }
            *(tmp + num_of_int) = key;
            qsort(tmp, (size_t) order, sizeof(int), cmpfunc);

            int split = 1 + ((order-1)/2);
            int *left = malloc( split * sizeof(int));
            for(i = 0; i < split; i++){
                *(left+i) = *(tmp+i);
            }
            int *right = malloc((order-split-1)* sizeof(int));
            for(i = 0; i < order - split - 1; i++)
            {
                *(right+i) = *(tmp+split+1+i);
            }
            for(i = 0; i < order - 1; i++){
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
                        position = -10;
                        j++;
                    }
                    else{
                        *(off_list+j) = *(target_node -> child + i);
                        j++;
                        i++;
                    }
                }
            }
            btree_node *new_node = (btree_node *) malloc(sizeof(btree_node));
            new_node -> n = order-split-1;
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
            write_to_file(new_node, 1, -1, inp, order);
            int to_promote = *(tmp + split);
            node_in_path--;
            add_to_non_leaf(to_promote, node_in_path, path, eof, order, inp);
        }else{

            *(target_node -> key + num_of_int) = key;
            target_node -> n = target_node -> n + 1;
            qsort(target_node -> key, (size_t) target_node -> n, sizeof(int), cmpfunc);

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
                        position = -10;
                        j++;
                    }
                    else{
                        *(tmp+j) = *(target_node -> child + i);
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

void printout(FILE *inp, int order){
    long root;
    fseek(inp, 0, SEEK_SET);
    fread(&root, sizeof(long), 1, inp);
    create();
    enq(root, 1);
    int cur_layer = 0;
    int node_layer;
    btree_node *node = (btree_node *) malloc(sizeof(btree_node));
    node -> key = (int *) calloc((size_t) (order - 1), sizeof( int ) );
    node -> child = (long *) calloc((size_t) order, sizeof( long ) );
    int i;
    while(qsize > 0){
        node_layer = deq(node, order, inp);
        if(cur_layer != node_layer){
            printf(" %d: ", node_layer);
            cur_layer = node_layer;
        }
        for(i = 0; i <= node -> n; i++){
            if(*(node -> child + i) != 0) enq(*(node -> child + i), node_layer + 1);

        }
        for(i = 0; i < node -> n; i++){
            if( i < node ->n -1){
                printf("%d,", *(node -> key + i));
            }else{
                printf("%d ", *(node -> key + i));
            }

        }
        if(front != NULL && front->layer != cur_layer) printf("\n");
    }
    printf("\n");
}

int cmpfunc(const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

void create()
{
    front = rear = NULL;
}

void enq(long data, int layer)
{
    if (rear == NULL)
    {
        rear = malloc(1*sizeof(struct Node));
        rear->next = NULL;
        rear->data = data;
        rear->layer = layer;
        front = rear;
    }
    else
    {
        temp=malloc(1*sizeof(struct Node));
        rear->next = temp;
        temp->data = data;
        temp->layer = layer;
        temp->next = NULL;

        rear = temp;
    }
    qsize++;
}

int deq(btree_node *node, int order, FILE *inp)
{
    front1 = front;
    int layer;
    layer = front1->layer;
    read_node(node, front1->data, order, inp);
    if (front1 == NULL)
    {
        return -1;
    }
    else
    if (front1->next != NULL)
    {
        front1 = front1->next;
        free(front);
        front = front1;
    }
    else
    {
        free(front);
        front = NULL;
        rear = NULL;
    }
    qsize--;
    return layer;
}
char** parse_command(char *input)
{
    char **command = malloc(sizeof(char *) * 3);
    char * p;
    p = strtok (input," ");
    command[0] = p;

    int i = 1;
    while(i < 3)
    {
        p = strtok (NULL," ");
        command[i] = p;
        i++;
    }

    return command;
}
int phrase_counter(char **command) {
    int i, phrase_count = 0;
    for(i = 0; i < 3; i++) {
        if(command[i]) {
            phrase_count++;
        }
    }
    return phrase_count;
}

int* calculatePhraseLength(char **command, int phrase_count)
{
    int j, k, *field_length = malloc(sizeof(int) * phrase_count);
    for(j = 0; j < phrase_count; j++) {
        k = (int) strlen(command[j]);

        if(j == phrase_count - 1) {
            field_length[j] = k - 1;
        }
        else {
            field_length[j] = k;
        }
    }
    return field_length;
}
