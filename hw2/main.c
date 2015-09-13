//
// Created by Kelei Gong on 9/12/15.
//

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define isascii(c)  ((c & ~0x7F) == 0)
char** parse_command(char *input);
int* calculate_field_length(char **command, int phrase_count);
int phrase_counter(char **command);


int main(int argc, char *argv[])
{
    char *buf; /* Buffer to hold student record */
    FILE *fp; /* Input/output stream */
    long rec_off; /* Record offset */
    int rec_siz; /* Record size, in characters */

/* If student.db doesn't exist, create it, otherwise read * its first record */

    if ( ( fp = fopen( "student.db", "r+b" ) ) == NULL )
    {
        fp = fopen( "student.db", "w+b" );
        int x[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
        char *y = "712412913|Ford|Rob|Phi";

        fwrite(y, sizeof(char), 22, fp);
        fclose(fp);
    }
    else
    {
        char record[100];
        fgets(record, 100, stdin);

        char **command = parse_command(record);
        int phrase_count = phrase_counter(command);
        int *fields_length = calculate_field_length(command, phrase_count);
        int i;
        for(i = 0; i < phrase_count; i++)
        {
            printf("=-=-=%d\n", fields_length[i]);
        }



        rec_off = 0;
        fseek( fp, rec_off, SEEK_SET );
        fread( &rec_siz, sizeof( int ), 1, fp );
        buf = malloc( rec_siz + 1 );
        fread( buf, 1, rec_siz, fp );
        buf[ rec_siz ] = '\0';
    }
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

int* calculate_field_length(char **command, int phrase_count)
{

    int i, j, k, *field_length = malloc(sizeof(int) * phrase_count);
    for(j = 0; j < phrase_count; j++)
    {
        k = 0;
        for(i = 0; i < 100; i++)
        {
//            printf("---%c\n", command[0][i]);
            if(isascii(command[j][i]) & command[j][i] != '\n' & command[j][i] != ' ' )
            {
                if(command[j][i] == '\0') break;
                printf("---%c\n", command[j][i]);
                k++;

            }
        }
        printf("%d-----\n", k);
        field_length[j] = k;
    }
    return field_length;
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