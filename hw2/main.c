//
// Created by Kelei Gong on 9/12/15.
//

#include "main.h"

#include <stdio.h>
#include <stdlib.h>

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
    }
    else
    {
        rec_off = 0;
        fseek( fp, rec_off, SEEK_SET );
        fread( &rec_siz, sizeof( int ), 1, fp );
        buf = malloc( rec_siz + 1 );
        fread( buf, 1, rec_siz, fp );
        buf[ rec_siz ] = '\0';
    }

}








