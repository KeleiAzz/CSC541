//
// Created by Kelei Gong on 9/12/15.
//

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
//        printf("%s\n", sizeof(*y));
        fwrite(y, sizeof(char), 22, fp);
        fclose(fp);
    }
    else
    {
        int id;
        char record[100];
        int i;
        printf("\n");
        scanf("%d %*d|%s", &id, record);
        printf("%d %d|%s\n", id, id, record);
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

        char text[10];
//        gets(text);


        rec_off = 0;
        fseek( fp, rec_off, SEEK_SET );
        fread( &rec_siz, sizeof( int ), 1, fp );
        buf = malloc( rec_siz + 1 );
        fread( buf, 1, rec_siz, fp );
        buf[ rec_siz ] = '\0';
    }

}









