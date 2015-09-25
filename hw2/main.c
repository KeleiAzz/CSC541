//
// Created by Kelei Gong on 9/12/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int key; /* Record's key */ long off; /* Record's offset in file */ } index_S;
typedef struct { int siz; /* Hole's size */ long off; /* Hole's offset in file */ } avail_S;
char** parse_command(char *input);
int*calculatePhraseLength(char **command, int phrase_count);
int phrase_counter(char **command);
long writeRecord(FILE *fp, char record[], long offset);
char* readRecord(FILE *fp, long offset);
void sortIndex(index_S *index, int n);
int findRecordOffset(index_S *index, int pk, int lo, int hi);
void removeIndex(index_S *index, int record_index, int record_counter);
void saveIndex(index_S *index, int index_length);
void saveAvail(avail_S *avail, int length);
index_S* readIndex();
avail_S* readAvail();
int indexLength();
int availLength();
int findSuitableHole(avail_S *avail, int avail_length, int record_length);
int bestFit(const void *a, const void *b);
int worstFit(const void *a, const void *b);
void updateAvail(avail_S *avail, int record_length, int hole_index, int avail_counter, int method);

int main(int argc, char *argv[])
{
    int method = 1;

    if( argv[1])
    {
        ;
    }
    else
    {
        printf("Wrong argument, please use --first-fit or --best-fit or --worst-fit\n");
        exit(1);
    }

    if( strcmp( argv[1], "--first-fit") == 0 )
    {
        method = 1;
    }
    else if( strcmp( argv[1], "--best-fit") == 0 )
    {
        method = 2;
    }
    else if( strcmp( argv[1], "--worst-fit") == 0 )
    {
        method = 3;
    }
    else
    {
        printf("Wrong argument, please use --first-fit or --best-fit or --worst-fit\n");
        exit(1);
    }
    FILE *fp; /* Input/output stream */
    int record_counter, avail_counter = 0;
    index_S *index = malloc(sizeof(index_S));
    avail_S *avail = malloc(sizeof(avail_S));
/* If student.db doesn't exist, create it, otherwise read * its first record */

    if ( ( fp = fopen( argv[2], "r+b" ) ) == NULL)
    {
        fp = fopen( argv[2], "w+b" );
        fclose(fp);
        fp = fopen( argv[2], "r+b" );
        record_counter = 0;
        avail_counter = 0;
    }
    else
    {
        index = readIndex();
        record_counter = indexLength();
        avail = readAvail();
        avail_counter = availLength();
    }
    long offset = 0;

    while(1){
        char input[150];
        fgets(input, 150, stdin);
        char **command = parse_command(input);
        int phrase_count = phrase_counter(command);
        int *phraseLength = calculatePhraseLength(command, phrase_count);
        int i;
        if(phrase_count == 3)  //Add a record
        {
            char key_char[phraseLength[1] + 1];
            strncpy(key_char, command[1], phraseLength[1]);
            key_char[phraseLength[1]] = '\0';
            int pk = atoi(key_char);
            if( record_counter == 0 | findRecordOffset(index, pk, 0, record_counter - 1) == -1)
            {
                int hole_index = findSuitableHole(avail, avail_counter, 4 + phraseLength[2]);
                if(hole_index == -1)
                {
                    offset = -1;

                    //no need to update avail list
                }
                else
                {
                    offset = avail[hole_index].off;
                    updateAvail(avail, 4 + phraseLength[2], hole_index, avail_counter, method);
                }
                char record[phraseLength[2] + 1];
                strncpy(record, command[2], phraseLength[2]);
                record[phraseLength[2]] = '\0';
                index_S *temp;
                temp = realloc(index, (record_counter + 1) * sizeof(index_S));
                if (temp == NULL) {
                    exit(EXIT_FAILURE);
                } else {
                    index = temp;
                }
                offset = writeRecord(fp, record, offset);
                index[record_counter].key = pk;
                index[record_counter].off = offset;
                record_counter++;
                sortIndex(index, record_counter);
//                offset += sizeof(int) * 1 + phraseLength[2];
            }
            else
            {
                printf("Record with SID=%d exists\n", pk);
            }
        }
        else if(phrase_count == 2) //find or delete
        {
            if(phraseLength[0] == 4) //find
            {
                char key_char[phraseLength[1] + 1];
                strncpy(key_char, command[1], phraseLength[1]);
                key_char[phraseLength[1]] = '\0';
                int pk = atoi(key_char);
                int record_index = findRecordOffset(index, pk, 0, record_counter - 1);
                if( record_index == -1)
                {
                    printf("No record with SID=%d exists\n", pk);
                }
                else
                {
                    char *record = readRecord(fp, index[record_index].off);
                    printf("%s\n", record);
                }

            }
            if(phraseLength[0] == 3)  //delete
            {
                char key_char[phraseLength[1] + 1];
                strncpy(key_char, command[1], phraseLength[1]);
                key_char[phraseLength[1]] = '\0';
                int pk = atoi(key_char);
                int record_index = findRecordOffset(index, pk, 0, record_counter - 1);
                if(record_index == -1)
                {
                    printf("No record with SID=%d exists\n", pk);
                }
                else
                {
                    int record_length;
                    fseek(fp, index[record_index].off, SEEK_SET);
                    fread(&record_length, sizeof(int), 1, fp);
//                    printf("%d record lentgh\n",record_length);
                    avail_S *temp;
                    temp = realloc(avail, (avail_counter + 1) * sizeof(avail_S));
                    if (temp == NULL) {
                        exit(EXIT_FAILURE);
                    } else {
                        avail = temp;
                    }
                    avail[avail_counter].off = index[record_index].off;
                    avail[avail_counter].siz = record_length;
                    avail_counter++;
                    if(method == 2)
                    {
                        qsort(avail, (size_t) avail_counter, sizeof(avail_S), bestFit);
                    }
                    if(method == 3)
                    {
                        qsort(avail, (size_t) avail_counter, sizeof(avail_S), worstFit);
                    }
                    removeIndex(index, record_index, record_counter);
                    record_counter--;

                }
            }
        }
        else if(phrase_count == 1) //end
        {
            break;
        }

    }
    printf("Index:\n");
    int i;
    for(i = 0; i < record_counter; i++)
    {
        printf( "key=%d: offset=%ld\n", index[i].key, index[i].off );
    }
    int hole_space = 0;
    printf("Availability:\n");
    for(i = 0; i < avail_counter; i++)
    {
        printf("size=%d: offset=%ld\n", avail[i].siz, avail[i].off);
        hole_space += avail[i].siz;
    }
    printf("Number of holes: %d\n", avail_counter);
    printf("Hole space: %d\n", hole_space);
    saveIndex(index, record_counter);
    saveAvail(avail, avail_counter);
    fclose(fp);
    return 0;
}

long writeRecord(FILE *fp, char record[], long offset)
{
    if(offset == -1)
    {
        fseek(fp, 0, SEEK_END);
        offset = ftell(fp);
    }
    else
    {
        fseek(fp, offset, SEEK_SET);
    }
//    fseek(fp, 0, SEEK_END);
    int length = (int) (sizeof(int) * 1 + sizeof(char) * (strlen(record)));
    fwrite( &length, sizeof(int), 1, fp);
//    fwrite(&pk, sizeof(int), 1, fp);
    fwrite(record, sizeof(char), (size_t) (strlen(record)), fp);
    return offset;

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

int phrase_counter(char **command)
{
    int i, phrase_count = 0;
    for(i = 0; i < 3; i++)
    {
        if(command[i])
        {
//            printf("---%s\n", command[i]);
            phrase_count++;
        }
    }
    return phrase_count;
}

int* calculatePhraseLength(char **command, int phrase_count)
{

    int j, k, *field_length = malloc(sizeof(int) * phrase_count);
    for(j = 0; j < phrase_count; j++)
    {
        k = (int) strlen(command[j]);

        if(j == phrase_count - 1)
        {
            field_length[j] = k - 1;
        }
        else
        {
            field_length[j] = k;
        }
//        printf("%d--%s---\n", field_length[j], command[j]);
    }
    return field_length;
}

char* readRecord(FILE *fp, long offset)
{
    fseek(fp, offset, SEEK_SET);
    int length;
    fread(&length, sizeof(int), 1, fp);
    fseek(fp, offset + 4, SEEK_SET);
    char *record = malloc((size_t) (length - 4));
//    record[0] = 'K';
    fread(record, sizeof(char), (size_t) (length - 4), fp);
//    char **fields;
//    fields = parseRecord(record);
    return record;
}

void sortIndex(index_S *index, int n)
{
    int j,i;
    index_S temp;
    for(i=1;i<n;i++)
    {
        for(j=0;j<n-i;j++)
        {
            if(index[j].key >index[j+1].key)
            {
                temp = index[j];
                index[j] =index[j+1];
                index[j+1] = temp;
            }
        }
    }
}

int findRecordOffset(index_S *index, int pk, int lo, int hi)
{
    int mid;
    if( lo > hi)
    {
        return -1;
    }
    mid = ( lo + hi ) / 2;
    if( index[mid].key == pk )
    {
        return mid;
    }
    else if( pk > index[mid].key )
    {
        return findRecordOffset(index, pk, mid + 1, hi);
    }
    else if( pk < index[mid].key )
    {
        return findRecordOffset(index, pk, lo, mid - 1);
    }
}

void removeIndex(index_S *index, int record_index, int record_counter)
{
    int i;
    for(i = record_index; i < record_counter - 1; i++) index[i] = index[i + 1];
    index_S *temp = realloc(index, (record_counter - 1) * sizeof(index_S) );
    if (temp == NULL && record_counter > 1) {
        exit(EXIT_FAILURE);
    }
    index = temp;
}

void saveIndex(index_S *index, int index_length)
{
    FILE *out = fopen( "index.bin", "wb" );
    fwrite( index, sizeof( index_S ), (size_t) index_length, out );
    fclose( out );
}

index_S* readIndex()
{
    FILE *in = fopen("index.bin", "rb");
    fseek(in, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(in);
    rewind(in);
    index_S *index = malloc( len);
    fread( index, sizeof(index_S), len/sizeof(index_S), in);
    fclose(in);
    return index;
}

int indexLength()
{
    FILE *in = fopen("index.bin", "rb");
    fseek(in, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(in);
    fclose(in);
    return (int) (len/sizeof(index_S));
}

void saveAvail(avail_S *avail, int length)
{
    FILE *out = fopen( "avail.bin", "wb" );
    fwrite( avail, sizeof( avail_S ), (size_t) length, out );
    fclose( out );
}

avail_S* readAvail()
{
    FILE *in = fopen("avail.bin", "rb");
    fseek(in, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(in);
    rewind(in);
    avail_S *avail = malloc( len);
    fread( avail, sizeof(avail_S), len/sizeof(avail_S), in);
    fclose(in);
    return avail;
}

int availLength()
{
    FILE *in = fopen("avail.bin", "rb");
    fseek(in, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(in);
    fclose(in);
    return (int) (len/sizeof(avail_S));
}

int bestFit(const void *a, const void *b)
{
    avail_S *holeA = (avail_S *)a;
    avail_S *holeB = (avail_S *)b;
    return ( holeA->siz - holeB->siz);
}

int worstFit(const void *a, const void *b)
{
    avail_S *holeA = (avail_S *)a;
    avail_S *holeB = (avail_S *)b;
    return ( holeB->siz - holeA->siz);
}

int findSuitableHole(avail_S *avail, int avail_length, int record_length)
{
    int i;
    for( i = 0; i < avail_length; i++ )
    {
        if(avail[i].siz > record_length) return i;
    }
    return -1;
}

void updateAvail(avail_S *avail, int record_length, int hole_index, int avail_counter, int method)
{

    int i;
    int hole_size = avail[hole_index].siz;
    long offset = avail[hole_index].off;
    for(i = hole_index; i < avail_counter - 1; i++) avail[i] = avail[i + 1];
    if(hole_size == record_length)
    {
        avail_S *temp = realloc(avail, (avail_counter - 1) * sizeof(avail_S) );
        if (temp == NULL && avail_counter > 1) {
            exit(EXIT_FAILURE);
        }
        avail = temp;
    }
    else
    {
        avail[avail_counter - 1].off = offset + record_length;
        avail[avail_counter - 1].siz = hole_size - record_length;
    }
    if(method == 2)
    {
        qsort(avail, (size_t) avail_counter, sizeof(avail_S), bestFit);
    }
    if(method == 3)
    {
        qsort(avail, (size_t) avail_counter, sizeof(avail_S), worstFit);
    }

}