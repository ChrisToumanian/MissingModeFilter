Version: 7.3.2019

Contributors:
Stephen Pinkerton     spinkert@usc.edu
Chris Toumanian       cct_580@usc.edu

---------
File list
---------

findmodes
main.c
makefile         compile with gcc, using the command $ make
readme.txt

----------------------
Command Line Arguments
----------------------

$ ./findmodes <fitted modes file> <missing modes file>

The program will write to the console if no output file is specified.

Example:
$ ./findmodes modeFittedTable.dat modeMissingTable.dat >> mode3AdjacentTable.dat

Produces a .dat text file listing possible modes to fit using a list of fitted modes and a list of missing modes.