//
// Created by Kelei Gong on 9/12/15.
//

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define isascii(c)  ((c & ~0x7F) == 0)
char** parse_command(char *input);
int*calculatePhraseLength(char **command, int phrase_count);
int phrase_counter(char **command);
char** parseRecord(char *record);
void writeRecord(FILE *fp, char **fields, int pk, long offset);


typedef struct { int key; /* Record's key */ long off; /* Record's offset in file */ } index_S;
typedef struct { int siz; /* Hole's size */ long off; /* Hole's offset in file */ } avail_S;

int main(int argc, char *argv[])
{
    char *buf; /* Buffer to hold student record */
    FILE *fp; /* Input/output stream */
    long rec_off; /* Record offset */
    int rec_siz; /* Record size, in characters */

/* If student.db doesn't exist, create it, otherwise read * its first record */

    if ( ( fp = fopen( "student.db", "r+b" ) ) == NULL)
    {
        fp = fopen( "student.db", "w+b" );
//        int x[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
        char *y = "712412913|Ford|Rob|Phi";

        fwrite(y, sizeof(char), 22, fp);
        fclose(fp);
    }
    else
    {   while(1){

        char input[150];
        fgets(input, 150, stdin);
        int total_length = (int) strlen(input);
        char **command = parse_command(input);
        int phrase_count = phrase_counter(command);
        int *phraseLength = calculatePhraseLength(command, phrase_count);
        int i;
        for(i = 0; i < phrase_count; i++)
        {
            printf("=-=-=%d\n", phraseLength[i]);
        }


            if(phrase_count == 3)
            {
                char key_char[phraseLength[1] + 1];
                strncpy(key_char, command[1], phraseLength[1]);
                key_char[phraseLength[1]] = '\0';
                printf("----%s\n", key_char);
                int pk = atoi(key_char);
                printf("%d\n", pk);

                char record[phraseLength[2] + 1];
                strncpy(record, command[2], phraseLength[2]);
                record[phraseLength[2]] = '\0';
                printf("%s ---record\n", record);

                char **fields = parseRecord(record);

                for(i = 0; i < 3; i++)
                {
                    printf("--%s--\n", fields[i]);
                }
                writeRecord(fp,fields, pk, 0);
            }
            else if(phrase_count == 1)
            {
                break;
            }

        }
        fclose(fp);
//        char act[phraseLength[0] + 1], lname[phraseLength[1]- phraseLength[0]], major[total_length- phraseLength[1]];
//        strncpy(fname, &input[0], position[0]);


//        rec_off = 0;
//        fseek( fp, rec_off, SEEK_SET );
//        fread( &rec_siz, sizeof( int ), 1, fp );
//        buf = malloc( rec_siz + 1 );
//        fread( buf, 1, rec_siz, fp );
//        buf[ rec_siz ] = '\0';
    }
}

void writeRecord(FILE *fp, char **fields, int pk, long offset)
{

    fseek(fp, offset, SEEK_END);
    int length = (int) (sizeof(int) * 2 + sizeof(char) * (3 + strlen(fields[0]) + strlen(fields[1]) + strlen(fields[2])));
    fwrite( &length, sizeof(int), 1, fp);
    fwrite(&pk, sizeof(int), 1, fp);
//    fseek(fp, sizeof(int), SEEK_SET);
    fwrite("|", sizeof(char), 1, fp);
//    fseek(fp, 0, SEEK_END);
    fwrite(fields[0], sizeof(char), strlen(fields[0]), fp);
    fwrite("|", sizeof(char), 1, fp);
    fwrite(fields[1], sizeof(char), strlen(fields[1]), fp);
    fwrite("|", sizeof(char), 1, fp);
    fwrite(fields[2], sizeof(char), strlen(fields[2]), fp);
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
            printf("---%s\n", command[i]);
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

void get_command()
{
    char act[5];
    int id;
    char record[100];
    int i;
    printf("\n");
    scanf("%s %d %*d|%s",act, &id, record);
    printf("%s %d %d|%s\n", act, id, id, record);
    printf("size of record: %lu\n", sizeof(record));
//        int i;
    int position[2];
    int j = 0, end;
    for(i = 0; i < sizeof(record); i++)
    {
        if (record[i])
        {
            if( record[i] == '|' )
            {
                position[j++] = i;
                printf("%d %c\n", position[j-1], record[i]);
            }
        }
        else
        {
            end = i;
            printf("%d\n", end);
            break;
        }
    }

    char fname[position[0] + 1], lname[position[1]-position[0]], major[end-position[1]];
    strncpy(fname, &record[0], position[0]);
    fname[position[0]] = '\0';
    printf("%s\n", fname);

}