# CSC541
##Assignment 1 - In-Memory vs. Disk-Based Searching

###Introduction

The goals of this assignment are two-fold:

To introduce you to random-access file I/O in UNIX using C.
To investigate time efficiency issues associated with in-memory versus disk-based searching strategies.
This assignment uses two "lists" of integer values: a key list and a seek list. The key list is a collection of integers K = (k0, ..., kn-1) representing n keys for a hypothetical database. The seek list is a separate collection of integers S = (s0, ..., sm-1) representing a sequence of m requests for keys to be retrieved from the database.

You will implement two different search strategies to try to locate each si from the seek list:

Linear search. A sequential search of K for a key that matches the current seek value si.
Binary search. A binary search through a sorted list of keys K for a key that matches the current seek value si. The fact that the keys are sorted allows approximately half the remaining keys to be ignored from consideration during each step of the search.
Each of the two searches (linear and binary) will be performed in two different environments. In the first, the key list K will be held completely in memory. In the second, individual elements ki ∈ K will read from disk as they are needed.

###Key and Seek Lists

The key and seek lists are provided to you as binary files. Each binary file contains a sequence of integer values stored one after another in order. You can download examples of both files from the Supplemental Material section of this web page.

Be sure to capture these files as binary data. The example file sizes should be 20,000 bytes for the key file, and 40,000 bytes for the seek file. For simplicity, the remainder of the assignment refers only to key.db and seek.db.

Note. The files we're providing here are meant to serve as examples only. Apart from holding integers, you cannot make any assumptions about the size or the content of the key and seek files we will use to test your program.

###Program Execution

Your program will be named assn_1 and it will run from the command line. Three command line arguments will be specified: a search mode, the name of the key file, and the name of the seek file.

assn_1 search-mode keyfile-name seekfile-name
Your program must support four different search modes.

--mem-lin    Read the key file into memory. Perform a linear search for each seek element si in the seek file.
--mem-bin    Read the key file into memory. Perform a binary search for each seek element si in the seek file.
--disk-lin   Read each ki from the key file as it is needed. Perform a linear search for each seek element si in the seek file.
--disk-bin   Read each ki from the key file as it is needed. Perform a binary search for each seek element si in the seek file.
For example, executing your program as follows

assn_1 --disk-lin key.db seek.db
would search for each element in seek.db using an on-disk linear search within key.db.

###In-Memory Sequential Search

If your program sees the search mode --mem-lin, it will implement an in-memory sequential search of the key list stored in key.db. The program should perform the following steps.

Open and read seek.db into an appropriately-sized integer array S.
Open and read key.db into an appropriately-sized integer array K.
Create a third array of integers called hit of the same size as S. You will use this array to record whether each seek value S[i] exists in K or not.
For each S[i], search K sequentially from beginning to end for a matching key value. If S[i] is found in K, set hit[i]=1. If S[i] is not found in K, set hit[i]=0.
You must record how much time it takes to open and load key.db, and to then determine the presence or absence of each S[i]. This is the total cost of performing the necessary steps in an in-memory sequential search. Be sure to measure only the time needed for these two steps: loading key.db and searching K for each S[i]. Any other processing should not be included.

###In-Memory Binary Search

If your program sees the search mode --mem-bin, it will implement an in-memory binary search of the key list stored in key.db. The keys in key.db are stored in sorted order, so they can be read and searched directly. Your program should perform the following steps.

Open and read seek.db into an appropriately-sized integer array S.
Open and read key.db into an appropriately-sized integer array K.
Create a third array of integers called hit of the same size as S. You will use this array to record whether each seek value S[i] exists in K or not.
For each S[i], use a binary search on K to find a matching key value. If S[i] is found in K, set hit[i]=1. If S[i] is not found, set hit[i]=0.
You must record how much time it takes to open and load key.db, and to then determine the presence or absence of each S[i]. This is the total cost of performing the necessary steps in an in-memory binary search. Be sure to measure only the time needed for these two steps: loading key.db and searching K for each S[i]. Any other processing should not be included.

Recall. To perform a binary search for S[i] in an array K of size n, begin by comparing S[i] to K[n/2].

If S[i] == K[n/2], the search succeeds.
If S[i] < K[n/2], recursively search the lower subarray K[0] ... K[(n/2)-1] for S[i].
Otherwise, recursively search the upper subarray K[(n/2)+1] ... K[n-1] for S[i].
Continue recursively searching for S[i] and dividing the subarray until S[i] found, or until the size of the subarray to search is 0, indicating the search has failed.

###On-Disk Sequential Search

For on-disk search modes, you will not load key.db into an array in memory. Instead, you will search the file directly on disk.

If your program sees the search mode --disk-lin, it will implement an on-disk sequential search of the key list stored in key.db. The program should perform the following steps.

Open and read seek.db into an appropriately-sized integer array S.
Open key.db for reading.
Create a second array of integers called hit of the same size as S. You will use this array to record whether each seek value S[i] exists in K or not.
For each S[i], search key.db sequentially from beginning to end for a matching key value by reading K0 and comparing it to S[i], reading K1 and comparing it to S[i], and so on. If S[i] is found in key.db, set hit[i]=1. If S[i] is not found in key.db, set hit[i]=0.
You must record how much time it takes to to determine the presence or absence of each S[i] in key.db. This is the total cost of performing the necessary steps in an on-disk sequential search. Be sure to measure only the time needed to search key.db for each S[i]. Any other processing should not be included.

Note. If you read past the end of a file in C, its EOF flag is set. Before you can perform any other operations on the file, you must reset the EOF flag. There are two ways to do this: (1) close and re-open the file; or (2) use the clearerr() function to clear the FILE stream's EOF and error bits.

###On-Disk Binary Search

If your program sees the search mode --disk-bin, it will implement an on-disk binary search of the key list stored in key.db. The keys in key.db are stored in sorted order, so they can be read and searched directly. The program should perform the following steps.

Open and read seek.db into an appropriately-sized integer array S.
Open key.db for reading.
Create a second array of integers called hit of the same size as S. You will use this array to record whether each seek value S[i] exists in K or not.
For each S[i], use a binary search on key.db to find a matching key value. If S[i] is found in key.db, set hit[i]=1. If S[i] is not found in key.db, set hit[i]=0.
You must record how much time it takes to to determine the presence or absence of each S[i] in key.db. This is the total cost of performing the necessary steps in an on-disk binary search. Be sure to measure only the time needed to search key.db for each S[i]. Any other processing should not be included.

###Programming Environment

All programs must be written in C or C++, and compiled to run on the remote.eos.ncsu.edu Linux server. Any ssh client can be used to access your Unity account and AFS storage space on this machine. Please see here for more information.

Reading Binary Integers

C's built-in file operations allow you to easily read integer data stored in a binary file. For example, the following code snippet opens a binary integer file for input and reads three integers: the first integer in the file, the third integer from the start of the file, and the second integer from the end of the file.

\#include <stdio.h> FILE *inp; /* Input file stream */ int k1, k2, k3; /* Keys to read */ inp = fopen( "key.db", "rb" ); fread( &k1, sizeof( int ), 1, inp ) fseek( inp, 2 * sizeof( int ), SEEK_SET ); fread( &k2, sizeof( int ), 1, inp ) fseek( inp, -2 * sizeof( int ), SEEK_END ); fread( &k3, sizeof( int ), 1, inp )
Measuring Time

The simplest way to measure execution time is to use gettimeofday() to query the current time at appropriate locations in your program.

\#include <sys/time.h> struct timeval tm; gettimeofday( &tm, NULL ); printf( "Seconds: %d\n", tm.tv_sec ); printf( "Microseconds: %d\n", tm.tv_usec );
Comparing tv_sec and tv_usec for two timeval structs will allow you to measure the amount of time that's passed between two gettimeofday() calls.

Writing Results

Results for each key in S[i], and the total time needed to perform all searching, must be written to the console before your program terminates. The format of your output must conform to the following rules.

Print one line for each S[i] in the order it occurs in seek.db. The line must contain the value of S[i] padded to be twelve characters wide, a colon and a space, and the text Yes if hit[i]==1 (key found) or the text No if hit[i]=0 (key not found). The simplest way to do this is to use a printf statement, for example:
printf( "%12d: Yes", S[i] );
Print the total execution time for your program's search operations as a single line with the label Time: followed by a space, and the execution time in seconds and microseconds. Assuming the execution time is stored in a timeval struct called exec_tm, you can use a printf statement to do this.
printf( "Time: %ld.%06ld", exec_tm.tv_sec, exec_tm.tv_usec );
You can capture your program's results for further examination or validation by redirecting its output to an external file, for example, to a file called output.txt, as follows.

assn_1 --mem-lin key.db seek.db > output.txt
Your assignment will be run automatically, and the output it produces will be compared to known, correct output using diff. Because of this, your output must conform to the above requirements exactly. If it doesn't, diff will report your output as incorrect, and it will be marked accordingly.

###Supplemental Material

In order to help you test your program, we provide an example key.db and seek.db, as well as the output that your program should generate when it processes these files.

key.db, a binary integer key list file containing 5000 keys,
seek.db, a binary integer seek list file containing 10000 keys, and
output.txt, the output your program should generate when it processes key.db and seek.db.
You can use diff to compare output from your program to our output.txt file. Note that the final line containing the execution time most likely won't match, but if your program is running properly and your output is formatted correctly, all of the key searches should produce identical results.

Please remember, as emphasized previously, the files we're providing here are meant to serve as examples only. Apart from holding integers, you cannot make any assumptions about the size or the content of the key and seek files we will use to test your program.

###Hand-In Requirements

Use the submit board Assignment 1 here to submit the following files:

assn_1, a Linux executable of your finished assignment, and
all associated source code files (these can be called anything you want).
There are four important requirements that your assignment must satisfy.

Your executable file must be named exactly as shown above. The program will be run and marked electronically using a script file, so using a different name means the executable will not be found, and subsequently will not be marked.
Your program must be compiled to run on remote.eos.ncsu.edu. If we cannot run your program, we will not be able to mark it, and we will be forced to assign you a grade of 0.
Your program must produce output that exactly matches the format described in the Writing Results section of this assignment. If it doesn't, it will not pass our automatic comparison to known, correct output.
You must submit your source code with your executable prior to the submission deadline. If you do not submit your source code, we cannot MOSS it to check for code similarity. Because of this, any assignment that does not include source code will be assigned a grade of 0.


##Assignment 2 - In-Memory Indexing with Availability Lists

###Introduction

The goals of this assignment are three-fold:

To investigate the use of field delimiters and record sizes for field and record organization.
To build and maintain an in-memory primary key index to improve search efficiency.
To use an in-memory availability list to support the reallocation of space for records that are deleted.
Student File

During this assignment you will build and maintain a simple file of student records. Each record will have four fields: SID (student ID), last (last name), first, (first name) and major (program of study). Fields within a record will be variable-length, and will be delimited by the | character. For example

712412913|Ford|Rob|Phi
represents a student with an SID of 712412913, a last of Ford, a first of Rob, and a major of Phi (Rob Ford is minoring in Ethics).

SID is the primary key for a student record. This means every individual student record will have a unique SID.

Records will be variable-length, and will be stored one after another in a binary data file. Each record will be preceded by an integer that defines the size of its corresponding record.

Note. Read the above description of the record size carefully! It is stored as binary data in a manner similar to how integer data were stored and retrieved in Assignment 1. It is not appropriate to store the size as an ASCII string. For example, if you wanted to read a record at file offset off in a file referenced through a FILE stream fp, it would be done as:

\#include <stdio.h> char *buf; /* Buffer to hold student record */ FILE *fp; /* Input/output stream */ long rec_off; /* Record offset */ int rec_siz; /* Record size, in characters */ /* If student.db doesn't exist, create it, otherwise read * its first record */ if ( ( fp = fopen( "student.db", "r+b" ) ) == NULL ) { fp = fopen( "student.db", "w+b" ); } else { rec_off = 0; fseek( fp, rec_off, SEEK_SET ); fread( &rec_siz, sizeof( int ), 1, fp ); buf = malloc( rec_siz + 1 ); fread( buf, 1, rec_siz, fp ); buf[ rec_siz ] = '\0'; }
Writing a new record uses a similar, reverse procedure. First, convert the record's body into a character buffer with fields delimited by |. Next, seek to the appropriate position in the student file and write an integer representing the buffer's size in bytes, in binary, using fwrite(). Finally, write the buffer to the file with fwrite().

###Program Execution

Your program will be named assn_2 and it will run from the command line. Two command line arguments will be specified: an availability list order, and the name of the student file.

assn_2 avail-list-order studentfile-name
Your program must support three different availability list ordering strategies.

--first-fit  Holes in the availability list will be saved in the order they are added to the list.
--best-fit   Holes in the availability list will be sorted in ascending order of hole size.
--worst-fit  Holes in the availability availability list will be sorted in descending order of hole size.
For example, executing your program as follows

assn_2 --best-fit student.db
would order holes in the availability list in ascending order of hole size, and would use student.db to store records.

Note. If you are asked open an existing student file, you can assume the availability list order specified on the command line matches the order that was used when the student file was first created.

###In-Memory Primary Key Index

In order to improve search efficiency, a primary key index will be maintained in memory as a reference to each record stored in the file. For our records, SID will act as a primary key. This means each entry in your index will have a structure similar to:

typedef struct { int key; /* Record's key */ long off; /* Record's offset in file */ } index_S;
Index entries should be stored in a collection that supports direct access and dynamic expansion. One good choice is a dynamically-expandable array. Index entries must be sorted in ascending key order, with the smallest key is at the front of the index and the largest key is at the end. This will allow you to use a binary search to find a target key in the index.

The index will not be re-built every time the student file is re-opened. Instead, it will be maintained in a permanent form on-disk. As your program exits, you will write your index to disk, saving its contents in an index file. When you re-open the student file, you will load the corresponding index file, immediately reconstructing your primary key index. The index will have exactly the same state as before, and will be ready to use to access records in the student file.

You can use any format you want to store each key–offset pair the index file. The simplest approach is to read and write the entire structure as binary data using fread() and fwrite(), for example

\#include <stdio.h> typedef struct { int key; /* Record's key */ long off; /* Record's offset in file */ } index_S; FILE *out; /* Output file stream */ index_S prim[ 50 ]; /* Primary key index */ out = fopen( "index.bin", "wb" ); fwrite( prim, sizeof( index_S ), 50, out ); fclose( out );
Note. To simplify your program, you can assume the primary key index will never need to store more than 10,000 key–offset pairs.

###In-Memory Availability List

When records are deleted from the file, rather than closing the hole that forms (an expensive operation), we will simply record the size and the offset of the hole in an in-memory availability list. Each entry in your availability list will have a structure similar to:

typedef struct { int siz; /* Hole's size */ long off; /* Hole's offset in file */ } avail_S;
Note. To simplify your program, you can assume the availability list will never need to store more than 10,000 size–offset pairs.

The availability list will not be re-built every time the student file is re-opened. Instead, similar to the primary index, it will be maintained in a permanent form on-disk. As your program exits, you will write your availability list to disk, saving its contents in an availability list file. When you re-open the student file, you will load the corresponding availability list file, immediately reconstructing your availability list.

As noted above, you can assume a consistent availability list order for a given student file. In other words, if you are asked to open an existing student file, the availability list order specified on the command line will always match the order that was being used when the availability list was written to disk.

When new records are added, we will search the availability list for a hole that can hold the new record. If no hole exists, the record is appended to the end of the student file. The order of the entries in the availability list is defined by the availability order specified on the command line when your program is run.

First Fit

If your program sees the availability order --first-fit, it will order entries in the availability list in first in–first out order. New holes are appended to the end of the availability list. When a new record is added, a first-fit strategy is used to search from the front of the availability list until a hole is found that is large enough to hold the new record.

If the hole is larger than the new record, the left-over fragment is saved as a new hole at the end of the availability list.

Best Fit

If your program sees the availability order --best-fit, it will order entries in the availability list sorted in ascending order of hole size. New holes are inserted into the availability list in the proper sorted position. If multiple holes have the same size, the entries for these holes should be sorted in ascending order of hole offset.

When a new record is added, a best-fit strategy is used to search from the front of the availability list until a hole is found that is large enough to hold the new record. Because of how the availability list is ordered, this is the smallest hole that can hold the new record.

If the hole is larger than the new record, the left-over fragment is saved as a new hole at its sorted position in the availability list.

Hint. Use C's qsort() function to sort the availability list.

Worst Fit

If your program sees the availability order --worst-fit, it will order entries in the availability list sorted in descending order of hole size. New holes are inserted into the availability list in the proper sorted position. If multiple holes have the same size, the entries for these holes should be sorted in ascending order of hole offset.

When a new record is added, a worst-fit strategy is used to examine the first entry in the availability list to see if it is large enough to hold the new record. Because of how the availability list is ordered, this is the largest hole that can hold the new record.

If the hole is larger than the new record, the left-over fragment is saved as a new hole at its sorted position in the availability list.

Hint. Use C's qsort() function to sort the availability list.

###User Interface

The user will communicate with your program through a set of commands typed at the keyboard. Your program must support four simple commands:

add key rec
Adds a new record rec with an SID of key to the student file. The format of rec is a |-delimited set of fields (exactly as described in Student File section above), for example
add 712412913 712412913|Ford|Rob|Phi
adds a new record with an SID of 712412913, a last of Ford, a first of Rob, and a major of Phi.
find key
Finds the record with SID of key in the student file, if it exists. The record should be printed in |-delimited format, (exactly as described in Student File section above), for example
find 712412913
should print on-screen
712412913|Ford|Rob|Phi
del key
Delete the record with SID of key from the student file, if it exists.
end
End the program, close the student file, and write the index and availability lists to the corresponding index and availability files.
Add

To add a new record to the student file

Binary search the index for an entry with a key value equal to the new rec's SID. If such an entry exists, then rec has the same primary key as a record already in the student file. Write
Record with SID=key exists
on-screen, and ignore the add request, since this is not allowed. If the user wants to update an already-existing record, they must first delete it, then re-add it.
Search the availability list for a hole that can hold hold rec plus the record size integer that precedes it.
If a hole is found, remove it from the availability list, and write rec's size and body to the hole's offset. If the hole is bigger than rec plus its record size integer, there will be a fragment left at the end of the hole. Add the fragment back to the availability list as a new, smaller hole.
If no appropriately-sized hole exist in the availability list, append rec's size and body to the end of the student file.
Regardless of where rec is written, a new entry must be added to the index holding rec's key and offset, maintaining the index in key-sorted order.
Find

To find a record, binary search the index for an entry with a key value of key. If an entry is found, use it's offset to locate and read the record, then print the record on-screen.

If no index entry with the given key exists, write

No record with SID=key exists
on-screen.

Del

To delete a record, binary search the index for an entry with a key value of key.

If an entry is found, use the entry's offset to locate and read the size of the record. Since the record is being deleted, it will form a hole in the student file. Add a new entry to the availability list containing the new hole's location and size. Remember, the size of the hole is the size of the record being deleted, plus the size of the integer preceding the record. Finally, remove the entry for the deleted record from the index.

If no index entry with the given key exists, print

No record with SID=key exists
on-screen.

End

This command ends the program by closing the student file, and writing the index and availability lists to their corresponding index and availability list files.

###Programming Environment

All programs must be written in C or C++, and compiled to run on the remote.eos.ncsu.edu Linux server. Any ssh client can be used to access your Unity account and AFS storage space on this machine. Please see here for more information.

###Writing Results

When your program ends, you must print the contents of your index and availability lists. For the index entries, print a line containing the text Index: followed by one line for each key–offset pair, using the following format.

printf( "key=%d: offset=%ld\n", index[i].key, index[i].off );
Next, for the availability list entries, print a line containing the text Availability: followed by one line for each size–offset pair, using the following format.

printf( "size=%d: offset=%ld\n", avail[i].siz, avail[i].off );
Finally, you must determine the number of holes hole_n in your availability list, and the total amount of space hole_siz occupied by all the holes (i.e., the sum of the sizes of each hole). These two values should be printed using the following format.

printf( "Number of holes: %d\n", hole_n );
printf( "Hole space: %d\n", hole_siz );
This will allow you to compare the efficiency of different availability list orderings to see whether they offer better or worse performance, in terms of the number of holes they create, and the amount of space they waste within the student file.

Your assignment will be run automatically, and the output it produces will be compared to known, correct output using diff. Because of this, your output must conform to the above requirements exactly. If it doesn't, diff will report your output as incorrect, and it will be marked accordingly.

###Supplemental Material

In order to help you test your program, we provide two input and two output files. The input files contain commands for your program. You can use file redirection to pass them in as though their contents were typed at the keyboard.

input-01.txt, an input file of commands applied to an initially empty student file, and
input-02.txt, an input file of commands applied to the student file produced by input-01.txt.
The output files show what your program should print after each input file is processed.

output-01-first.txt, the output your program should produce after it processes input-01.txt using --first-fit,
output-02-first.txt, the output your program should produce after it processes input-02.txt using --first-fit,
output-01-best.txt, the output your program should produce after it processes input-01.txt using --best-fit,
output-02-best.txt, the output your program should produce after it processes input-02.txt using --best-fit,
output-01-worst.txt, the output your program should produce after it processes input-01.txt using --worst-fit, and
output-02-worst.txt, the output your program should produce after it processes input-02.txt using --worst-fit.
For example, to test your program using --best-fit, you would issue the following commands:

% rm student.db % assn_2 --best-fit student.db < input-01.txt > my-output-01-best.txt % assn_2 --best-fit student.db < input-02.txt > my-output-02-best.txt
Note: As shown in the example above, you start a "new" student database by removing any existing student file. If your program sees the student file doesn't exist, it can assume that the index and availability files shouldn't exist, either. You can handle this assumption in any way you want. One simple approach would be to open the index and availability files in w+b mode, which enables reading and writing, and automatically discards any existing files with the same names.

You can use diff to compare output from your program to our output files. If your program is running properly and your output is formatted correctly, your program should produce output identical to what is in these files.

Please remember, the files we're providing here are meant to serve as examples only. Apart from holding valid commands, and the previous guarantees of a limit of 10,000 key–offset and 10,000 size–offset pairs, you cannot make any assumptions about the content of the input files we will use to test your program.

###Hand-In Requirements

Use the submit board Assignment 2 here to submit the following files:

assn_2, a Linux executable of your finished assignment, and
all associated source code files (these can be called anything you want).
There are four important requirements that your assignment must satisfy.

Your executable file must be named exactly as shown above. The program will be run and marked electronically using a script file, so using a different name means the executable will not be found, and subsequently will not be marked.
Your program must be compiled to run on remote.eos.ncsu.edu. If we cannot run your program, we will not be able to mark it, and we will be forced to assign you a grade of 0.
Your program must produce output that exactly matches the format described in the Writing Results section of this assignment. If it doesn't, it will not pass our automatic comparison to known, correct output.
You must submit your source code with your executable prior to the submission deadline. If you do not submit your source code, we cannot MOSS it to check for code similarity. Because of this, any assignment that does not include source code will be assigned a grade of 0.


##Assignment 3 - Disk-Based Mergesort

###Introduction

The goals of this assignment are two-fold:

To introduce you to sorting data on disk using mergesort.
To compare the performance of different algorithms for creating and merging runs during mergesort.
Index File

During this assignment you will sort a binary index file of integer key values. The values are stored in the file in a random order. You will use a mergesort to produce a second index file whose key values are sorted in ascending order.

###Program Execution

Your program will be named assn_3 and it will run from the command line. Three command line arguments will be specified: a mergesort method, the name of the input index file, and the name of the sorted output index file.

assn_3 mergesort-method index-file sorted-index-file
Your program must support three different mergesort methods.

1.	--basic	Split the index file into sorted runs stored on disk, then merge the runs to produce a sorted index file.
2.	--multistep	Split the index file into sorted runs. Merge subsets of runs to create super-runs, then merge the super-runs to produce a sorted index file.
3.	--replacement	Split the index file into sorted runs created using replacement selection, then merge the runs to produce a sorted index file.
For example, executing your program as follows

assn_3 --multistep input.bin sort.bin
would apply a multistep mergesort to input.bin to sort it ascending by key value. The result would be stored in sort.bin.

Note. For convenience, we refer to the input index file as input.bin and the output sorted index file as sort.bin throughout the remainder of the assignment.

###Available Memory

Mergesort's run sizes and merge performance depend on the amount of memory available for run creating and merging runs.

Your program will be assigned one input buffer for reading data (e.g., blocks of keys during run creation of parts of runs during merging). The input buffer must be sized to hold a maximum of 1000 integer keys.

Your program will also be assigned one output buffer for writing data (e.g., sorted blocks of keys during run creation or sorted subsets of sort.bin during merging). The output buffer must be sized to hold a maximum of 1000 integer keys.

###Basic Mergesort

If your program sees the merge method --basic, it will implement a standard mergesort of the keys in input.bin. The program should perform the following steps.

Open input.bin and read its contents in 1000-key blocks using the input buffer.
Sort each block and write it to disk as a run file. You can use any in-memory sorting algorithm you want (e.g., C's qsort() function). Name each run file index-file.n, where n is a 3-digit run identifier, starting at 0. For example, if the input index file is input.bin, the run files would be named
input.bin.000 input.bin.001 input.bin.002 ...
Open each run file and buffer part of its contents into the input buffer. The amount of each run you can buffer will depend on how many runs you are merging (e.g., merging 50 runs using the 1000-key input buffer allows you to buffer 20 keys per run).
Merge the runs to produce sorted output. Use the output buffer to write the results in 1000-key chunks as binary data to sort.bin.
Whenever a run's buffer is exhausted, read another block from the run file. Continue until all run files are exhausted.
You must record how much time it takes to complete the basic mergesort. This includes run creation, merging, and writing the results to sort.bin.

Note. You will never be asked to merge more than 1000 runs in Step 3. This guarantees there will always be enough memory to assign a non-empty buffer to each run.

###Multistep Mergesort

If your program sees the merge method --multistep, it will implement a two-step mergesort of the keys in input.bin. The program should perform the following steps.

Create the initial runs for input.bin, exactly like the basic mergesort.
Merge a set of 15 runs to produce a super-run. Open the first 15 run files and buffer them using your input buffer. Merge the 15 runs to produce sorted output, using your output buffer to write the results as binary data to a super-run file.
Continue merging sets of 15 runs until all of the runs have been processed. Name each super-run file index-file.super.n, where n is a 3-digit super-run identifier, starting at 0. For example, if the input file is input.bin, the super-run files would be named
input.bin.super.000 input.bin.super.001 input.bin.super.002 ...
Note. If the number of runs created in Step 1 is not evenly divisible by 15, the final super-run will merge fewer than 15 runs.
Merge all of the super-runs to produce sorted output. Use the input buffer to read part of the contents of each super-run. Use the output buffer to write the results in 1000-key chunks as binary data to sort.bin.
You must record how much time it takes to complete the multistep mergesort. This includes initial run creation, merging to create super-runs, merging super-runs, and writing the results to sort.bin.

Note. You will never be asked to merge more than 1000 super-runs in Step 3. This guarantees there will always be enough memory to assign a non-empty buffer to each super-run.

###Replacement Selection Mergesort

If your program sees the merge method --replacement, it will implement a mergesort that uses replacement selection to create runs from the values in input.bin. The program should perform the following steps.

Divide your input buffer into two parts: 750 entries are reserved for a heap H1 ... H750, and 250 entries are reserved as an input buffer B1 ... B250 to read keys from input.bin.
Read the first 750 keys from input.bin into H, and the next 250 keys into B. Rearrange H so it forms an ascending heap.
Append H1 (the smallest value in the heap) to the current run, managed through the output buffer. Use replacement selection to determine where to place B1.
If H1 ≤ B1, replace H1 with B1.
If H1 > B1, replace H1 with H750, reducing the size of the heap by one. Replace H750 with B1, increasing the size of the secondary heap by one.
Adjust H1 to reform H into a heap.
Continue replacement selection until H is empty, at which point the current run is completed. The secondary heap will be full, so it replaces H, and a new run is started.
Run creation continues until all values in input.bin have been processed. Name the runs exactly as you did for the basic mergesort (i.e., input.bin.000, input.bin.001, and so on).
Merge the runs to produce sorted output, exactly like the merge step in the basic mergesort.
You must record how much time it takes to complete the replacement selection mergesort. This includes replacement selection run creation, merging the replacement selection runs, and writing the results to sort.bin.

Note. You will never be asked to merge more than 1000 runs in Step 6. This guarantees there will always be enough memory to assign a non-empty buffer to each run.

###Programming Environment

All programs must be written in C or C++, and compiled to run on the remote.eos.ncsu.edu Linux server. Any ssh client can be used to access your Unity account and AFS storage space on this machine. Please see here for more information.

###Measuring Time

The simplest way to measure execution time is to use gettimeofday() to query the current time at appropriate locations in your program.

\#include <sys/time.h> struct timeval tm; gettimeofday( &tm, NULL ); printf( "Seconds: %d\n", tm.tv_sec ); printf( "Microseconds: %d\n", tm.tv_usec );
Comparing tv_sec and tv_usec for two timeval structs will allow you to measure the amount of time that's passed between two gettimeofday() calls.

###Writing Results

Sorted keys must be written to sort.bin as binary integers. C's built-in file writing operations allow this to be done very easily.

\#include <stdio.h> FILE *fp; /* Output file stream */ int output_buf[ 1000 ]; /* Output buffer */ fp = fopen( "sort.bin", "wb" ); fwrite( output_buf, sizeof( int ), 1000, fp ); fclose( fp );
Your program must also print the total execution time for the mergesort it performs as a single line on-screen. Assuming the execution time is held in a timeval struct called exec_tm, use the following printf statement to do this.

printf( "Time: %ld.%06ld", exec_tm.tv_sec, exec_tm.tv_usec );
Your assignment will be run automatically, and the output it produces will be compared to known, correct output using diff. Because of this, your output must conform to the above requirements exactly. If it doesn't, diff will report your output as incorrect, and it will be marked accordingly.

###Supplemental Material

In order to help you test your program, we provide example input.bin and sort.bin files.

input.bin, a binary input file file containing 250,000 keys,
sort.bin, a binary output file containing input.bin's 250,000 keys in ascending sorted order.
You can use diff to compare output from your program to our sort.bin file.

Please remember, the files we're providing here are meant to serve as examples only. Apart from holding integers, and a guarantee that the number of runs (or super-runs) will not exceed the input buffer's capacity, you cannot make any assumptions about the size or the content of the input and sort files we will use to test your program.

###Hand-In Requirements

Use the submit board Assignment 3 here to submit the following files:

assn_3, a Linux executable of your finished assignment, and
all associated source code files (these can be called anything you want).
There are four important requirements that your assignment must satisfy.

Your executable file must be named exactly as shown above. The program will be run and marked electronically using a script file, so using a different name means the executable will not be found, and subsequently will not be marked.
Your program must be compiled to run on remote.eos.ncsu.edu. If we cannot run your program, we will not be able to mark it, and we will be forced to assign you a grade of 0.
Your program must produce output that exactly matches the format described in the Writing Results section of this assignment. If it doesn't, it will not pass our automatic comparison to known, correct output.
You must submit your source code with your executable prior to the submission deadline. If you do not submit your source code, we cannot MOSS it to check for code similarity. Because of this, any assignment that does not include source code will be assigned a grade of 0.


##Assignment 4 - B-Trees

###Introduction

The goals of this assignment are two-fold:

To introduce you to searching data on disk using B-trees.
To investigate how changing the order of a B-tree affects its performance.
Index File

During this assignment you will create, search, and manage a binary index file of integer key values. The values stored in the file will be specified by the user. You will structure the file as a B-tree.

###Program Execution

Your program will be named assn_4 and it will run from the command line. Two command line arguments will be specified: the name of the index file, and a B-tree order.

assn_4 index-file order
For example, executing your program as follows

assn_4 index.bin 4
would open an index file called index.bin that holds integer keys stored in an order-4 B-tree. You can assume order will always be ≥ 3. For convenience, we refer to the index file as index.bin throughout the remainder of the assignment.

Note. If you are asked open an existing index file, you can assume the B-tree order specified on the command line matches the order that was used when the index file was first created.

###B-Tree Nodes

Your program is allowed to hold individual B-tree nodes in memory—but not the entire tree—at any given time. Your B-tree node should have a structure and usage similar to the following.

\#include <stdlib.h> int order = 4; /* B-tree order */ typedef struct { /* B-tree node */ int n; /* Number of keys in node */ int *key; /* Node's keys */ long *child; /* Node's child subtree offsets */ } btree_node; btree_node node; /* Single B-tree node */ node.n = 0; node.key = (int *) calloc( order - 1, sizeof( int ) ); node.child = (long *) calloc( order, sizeof( long ) );
Note. Be careful when you're reading and writing data structures with dynamically allocated memory. For example, trying to write node like this

fwrite( &node, sizeof( btree_node ), 1, fp );
will write node's key count, the pointer value for its key array, and the pointer value for its child offset array, but it will not write the contents of the key and child offset arrays. The arrays' contents and not pointers to their contents need to be written explicitly instead.

fwrite( &node.n, sizeof( int ), 1, fp ); fwrite( node.key, sizeof( int ), order - 1, fp ); fwrite( node.child, sizeof( long ), order, fp );
Reading node structures from disk would use a similar strategy.

###Root Node Offset

In order to manage any tree, you need to locate its root node. Initially the root node will be stored near the front of index.bin. If the root node splits, however, a new root will be appended to the end of index.bin. The root node's offset will be maintained persistently by storing it at the front of index.bin when the file is closed, and reading it when the file is opened.

\#include <stdio.h> FILE *fp; /* Input file stream */ long root; /* Offset of B-tree root node */ fp = fopen( "index.bin", "r+b" ); /* If file doesn't exist, set root offset to unknown and create * file, otherwise read the root offset at the front of the file */ if ( fp == NULL ) { root = -1; fp = fopen( "index.bin", "w+b" ); fwrite( &root, sizeof( long ), 1, fp ); } else { fread( &root, sizeof( long ), 1, fp ); }
User Interface

The user will communicate with your program through a set of commands typed at the keyboard. Your program must support four simple commands:

add k
Add a new integer key with value k to index.bin.
find k
Find an entry with a key value of k in index.bin, if it exists.
print
Print the contents and the structure of the B-tree on-screen.
end
Update the root node's offset at the front of the index.bin, and close the index file, and end the program.
Add

Use a standard B-tree algorithm to add a new key k to the index file.

Search the B-tree for the leaf node L responsible for k. If k is stored in L's key list, print
Entry with key=k already exists
on-screen and stop, since duplicate keys are not allowed.
Create a new key list K that contains L's keys, plus k, sorted in ascending order.
If L's key list is not full, replace it with K, update L's child offsets, write L back to disk, and stop.
Otherwise, split K about its median key value km into left and right key lists KL = (k0, ... , km-1) and KR = (km+1, ... , kn-1). Use ceiling to calculate m = ⌈(n-1)/2⌉. For example, if n = 3, m = 1. If n = 4, m = 2.
Save KL and its associated child offsets in L, then write L back to disk.
Save KR and its associated child offsets in a new node R, then append R to the end of the index file.
Promote km , L's offset, and R's offset and insert them in L's parent node. If the parent's key list is full, recursively split its list and promote the median to its parent.
If a promotion is made to a root node with a full key list, split and create a new root node holding km and offsets to L and R.
Find

To find key value k in the index file, search the root node for k. If k is found, the search succeeds. Otherwise, determine the child subtree S that is responsible for k, then recursively search S. If k is found during the recursive search, print

Entry with key=k exists
on-screen. If at any point in the recursion S does not exist, print

Entry with key=k does not exist
on-screen.

Print

This command prints the contents of the B-tree on-screen, level by level. Begin by considering a single B-tree node. To print the contents of the node on-screen, print its key values separated by commas.

int i; /* Loop counter */ btree_node node; /* Node to print */ long off; /* Node's offset */ for( i = 0; i < node.n - 1; i++ ) { printf( "%d,", node.key[ i ] ); } printf( "%d", node.key[ node.n - 1 ] );
To print the entire tree, start by printing the root node. Next, print the root node's children on a new line, separating each child node's output by a space character. Then, print their children on a new line, and so on until all the nodes in the tree are printed. This approach prints the nodes on each level of the B-tree left-to-right on a common line.

For example, inserting the integers 1 through 13 inclusive into an order-4 B-tree would produce the following output.

1: 9 2: 3,6 12 3: 1,2 4,5 7,8 10,11 13
To support trees with more than 9 levels, we leave space for two characters to print the level at the beginning of each line, that is, using printf( "%2d: ", lvl )" or something similar.

Hint. To process nodes left-to-right level-by-level, do not use recursion. Instead, create a queue containing the root node's offset. Remove the offset at the front of the queue (initially the root's offset) and read the corresponding node from disk. Append the node's non-empty subtree offsets to the end of the queue, then print the node's key values. Continue until the queue is empty.

End

This command ends the program by writing the root node's offset to the front of index.bin, then closing the index file.

###Programming Environment

All programs must be written in C or C++, and compiled to run on the remote.eos.ncsu.edu Linux server. Any ssh client can be used to access your Unity account and AFS storage space on this machine. Please see here for more information.

Your assignment will be run automatically, and the output it produces will be compared to known, correct output using diff. Because of this, your output must conform to the print command's description. If it doesn't, diff will report your output as incorrect, and it will be marked accordingly.

###Supplemental Material

In order to help you test your program, we provide example input and output files.

input-01a.txt, an input file of commands applied to an initially empty index file saved as an order-4 B-tree, and
input-02a.txt, an input file of commands applied to the index file produced by input-01a.txt.
The output files show what your program should print after each input file is processed.

output-01a.txt, the output your program should produce after it processes input-01a.txt.
output-02a.txt, the output your program should produce after it processes input-02a.txt.
To test your program, you would issue the following commands:

% rm index.bin % assn_4 index.bin 4 < input-01a.txt > my-output-01.txt % assn_4 index.bin 4 < input-02a.txt > my-output-02.txt
You can use diff to compare output from your program to our output files. If your program is running properly and your output is formatted correctly, your program should produce output identical to what is in these files.

Please remember, the files we're providing here are meant to serve as examples only. Apart from holding valid commands, you cannot make any assumptions about the size or the content of the input files we will use to test your program.

###Hand-In Requirements

Use the submit board Assignment 4 here to submit the following files:

assn_4, a Linux executable of your finished assignment, and
all associated source code files (these can be called anything you want).
There are four important requirements that your assignment must satisfy.

Your executable file must be named exactly as shown above. The program will be run and marked electronically using a script file, so using a different name means the executable will not be found, and subsequently will not be marked.
Your program must be compiled to run on remote.eos.ncsu.edu. If we cannot run your program, we will not be able to mark it, and we will be forced to assign you a grade of 0.
Your program must produce output that exactly matches the format described in the print command section of this assignment. If it doesn't, it will not pass our automatic comparison to known, correct output.
You must submit your source code with your executable prior to the submission deadline. If you do not submit your source code, we cannot MOSS it to check for code similarity. Because of this, any assignment that does not include source code will be assigned a grade of 0.
