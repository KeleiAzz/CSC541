//
// Created by Kelei Gong on 9/12/15.
//

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define isascii(c)  ((c & ~0x7F) == 0)

typedef struct { int key; /* Record's key */ long off; /* Record's offset in file */ } index_S;
typedef struct { int siz; /* Hole's size */ long off; /* Hole's offset in file */ } avail_S;
char** parse_command(char *input);
int*calculatePhraseLength(char **command, int phrase_count);
int phrase_counter(char **command);
char** parseRecord(char *record);
void writeRecord(FILE *fp, char record[], int pk, long offset);
int* getLengthAndKey(FILE *fp, long offset);
char* readRecord(FILE *fp, long offset);
void sortIndex(index_S *index, int n);
long findRecordOffset(index_S *index, int pk, int lo, int hi);


int main(int argc, char *argv[])
{
    char *buf; /* Buffer to hold student record */
    FILE *fp; /* Input/output stream */
    long rec_off; /* Record offset */
    int rec_siz; /* Record size, in characters */
    index_S *index = malloc(sizeof(index_S));
    avail_S *avail = malloc(sizeof(avail_S));
/* If student.db doesn't exist, create it, otherwise read * its first record */

    if ( ( fp = fopen( "student.db", "r+b" ) ) == NULL)
    {
        fp = fopen( "student.db", "w+b" );
        fclose(fp);
        fp = fopen( "student.db", "r+b" );
    }
    long offset = 0;
    int record_counter = 0;
    while(1){
        char input[150];
        fgets(input, 150, stdin);
        int total_length = (int) strlen(input);
        char **command = parse_command(input);
        int phrase_count = phrase_counter(command);
        int *phraseLength = calculatePhraseLength(command, phrase_count);
        int i;
        if(phrase_count == 3)
        {
            char key_char[phraseLength[1] + 1];
            strncpy(key_char, command[1], phraseLength[1]);
            key_char[phraseLength[1]] = '\0';
//            printf("----%s\n", key_char);
            int pk = atoi(key_char);
//            printf("%d\n", pk);

            if( record_counter == 0 | findRecordOffset(index, pk, 0, record_counter - 1) == -1)
            {
                char record[phraseLength[2] + 1];
                strncpy(record, command[2], phraseLength[2]);

                char try[phraseLength[2] - phraseLength[1] + 1];
                strncpy(try, command[2] + phraseLength[1], phraseLength[2] - phraseLength[1]);
                try[phraseLength[2] - phraseLength[1]] = '\0';
                record[phraseLength[2]] = '\0';
//                printf("%s ---record %s ---try\n", record, try);
                char **fields = parseRecord(record);
                for(i = 0; i < 3; i++)
                {
//                    printf("--%s--\n", fields[i]);
                }

                index_S *temp;
                temp = realloc(index, (record_counter + 1) * sizeof(index_S));
                if (temp == NULL) {
                } else {
                    index = temp;
                }
                index[record_counter].key = pk;
                index[record_counter].off = offset;
                record_counter++;
                sortIndex(index, record_counter);
                writeRecord(fp, try, pk, -1);
                offset += sizeof(int) * 2 + phraseLength[2] - phraseLength[1] + sizeof(char);
//                printf("%ld\n", offset);
            }


        }
        else if(phrase_count == 2)
        {
            if(phraseLength[0] == 4)
            {
                char key_char[phraseLength[1] + 1];
                strncpy(key_char, command[1], phraseLength[1]);
                key_char[phraseLength[1]] = '\0';
//                printf("----%s\n", key_char);
                int pk = atoi(key_char);
//                printf("%d\n", pk);
                rec_off = findRecordOffset(index, pk, 0, record_counter - 1);
                if(rec_off == -1)
                {
                    printf("No record with SID=%d exists\n", pk);
                }
                else
                {
                    char *record = readRecord(fp, rec_off);
                    printf("%d%s\n", pk, record);
                }

            }
            if(phraseLength[0] == 3)
            {
                char key_char[phraseLength[1] + 1];
                strncpy(key_char, command[1], phraseLength[1]);
                key_char[phraseLength[1]] = '\0';
                int pk = atoi(key_char);
                rec_off = findRecordOffset(index, pk, 0, record_counter - 1);
                if(rec_off == -1)
                {
                    printf("No record with SID=%d exists\n", pk);
                }
                else
                {

                }
            }
        }
        else if(phrase_count == 1)
        {
            break;
        }

    }
    printf("Index:\n");
    int i;
    for(i = 0; i < record_counter - 1; i++)
    {
        printf( "key=%d: offset=%ld\n", index[i].key, index[i].off );
    }

//    int *LAK;
//    LAK = getLengthAndKey(fp, 55);
//    int length = LAK[0], pk = LAK[1];
//    char *record = readRecord(fp, 55);
////    int i;
//
////    printf("%s---\n", record);

    fclose(fp);
}

void writeRecord(FILE *fp, char record[], int pk, long offset)
{
    if(offset == -1)
    {
        fseek(fp, 0, SEEK_END);
    }
    else
    {
        fseek(fp, offset, SEEK_SET);
    }
    fseek(fp, 0, SEEK_END);
    int length = (int) (sizeof(int) * 2 + sizeof(char) * (strlen(record) + 1));
    fwrite( &length, sizeof(int), 1, fp);
    fwrite(&pk, sizeof(int), 1, fp);
    fwrite(record, sizeof(char), (size_t) (strlen(record) + 1), fp);
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
//        printf("%d-----\n", k);
        if(j == phrase_count - 1)
            field_length[j] = k - 1;
        else
            field_length[j] = k;
    }
    return field_length;
}

char** parseRecord(char *record)
{
    char **fields = malloc(sizeof(char *) * 3);
    char *p;
    p = strtok(record, "|");

    int i = 0;
    while(i < 3)
    {
        p = strtok(NULL, "|");
        fields[i] = p;
        i++;
    }
    return fields;
}

int* getLengthAndKey(FILE *fp, long offset)
{
    fseek(fp, offset, SEEK_SET);
    int length, pk;
    fread(&length, sizeof(int), 1, fp);
    fread(&pk, sizeof(int), 1, fp);
    int *res = malloc(sizeof(int) * 2);
    res[0] = length;
    res[1] = pk;
    return res;
}

char* readRecord(FILE *fp, long offset)
{
    fseek(fp, offset, SEEK_SET);
    int length;
    fread(&length, sizeof(int), 1, fp);
    fseek(fp, offset + 8, SEEK_SET);
    char *record = malloc((size_t) (length - 8));
//    record[0] = 'K';
    fread(record, sizeof(char), (size_t) (length - 8), fp);
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

long findRecordOffset(index_S *index, int pk, int lo, int hi)
{
    int mid;
    if( lo > hi)
    {
        return -1;
    }
    mid = ( lo + hi ) / 2;
    if( index[mid].key == pk )
    {
        return index[mid].off;
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