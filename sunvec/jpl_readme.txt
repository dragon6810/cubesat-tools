*****************************************************************************
*****      C version software for the JPL planetary ephemerides.        *****
*****************************************************************************

version 1.6.1, June 15, 2024

Piotr A. Dybczynski, Astronomical Observatory Institute, A. Mickiewicz Universty,
Sloneczna 36, 60-286 Poznan, POLAND.
e-mail: dybol@amu.edu.pl
www: https://pad2.astro.amu.edu.pl/PAD/index.php?n=Dybol.JPLEph

*****************************************************************************
File list:
   Size   date     time   file
  11302 2024-06-15 13:10 asc2eph.c
   3318 2024-06-15 13:10 jplbin.h
   4630 2024-06-15 13:24 readme.txt
  33627 2024-06-15 13:10 testeph.c

*****************************************************************************

CHANGES FROM VERSION 1.6

It has been pointed to me by Tess Harding that there is some inconsistency
between jplbin.h and testeph.c.

While JPL included additional parameters (RPT and TPT arrays) in their heder
files these parameters are not included in binary ephemerides.
I decided to comment out all lines handling these arrays both in jplbin.h
and asc2eph.c
The code of testeph.c remains unchanged, except for some additional comments.

****************************************************************************

CHANGES FROM VERSION 1.5:

There are two changes in this software:

1. In the jplbin.h file the definitions for DE440 and DE441 were added.

2. I have revived the old asc2eph.c program for two reasons:
   A: there were no binary files at the JPL server when I started to use 
      the DE440 ephemeris. Happily now the Linux binary files are available.
   B: binary ephemeris files are now very large so a user might wish to
      create smaller ephemeris, covering some sub-interval.

   I have added several new definitions to the end of the jplbih.h file
   to allow asc2eph.c program to work with different ephemeris versions.

According to my tests this software works correctly both on 64 bit
and 32 bit Linux systems. The tests were performed under Debian 10
with the default gcc version (8.3.0).

****************************************************************************

CHANGES FROM VERSION 1.4:

The only important change is in the jplbin.h file. Explicit rules for packing
structures were added because of some changes in the default gcc behavior.

As a result this software works correctly both on 64 bit and 32 bit Linux
systems.

In the testeph.c file the usage of 'long int' is abandoned since it is
identical with 'int' with current versions of a gcc compiler.

****************************************************************************

CHANGES FROM VERSION 1.3: added support for DE430 and DE431.
                          The main change is to allow for more than 400 ephemeris
                          constants, as required in case of DE430 and DE431.
                          Some minor improvements are also done.

******************************************************************************
 The original JPL datafiles, FORTRAN code and documentation can be found at:
              http://ssd.jpl.nasa.gov/?planet_eph_export
******************************************************************************

Version (1.4) works with:

DE200, DE403, DE404, DE405, DE406, DE421, DE422, DE430 and DE431

This file describes public domain software for using and manipulating
JPL export planetary ephemerides, written in C language.

In original JPL export packages you can find several FORTRAN source files
for reading and testing ephemerides.

Here you can find C version of TESTEPH program.

TESTEPH.C is a manual translation of the original JPL 
FORTRAN code, slightly changed and adapted for C language.

*****************************************************************************
IT IS NECESSARY TO ADJUST MANUALLY SOURCE FILES before you compile and run
them. First, look at the file: JPLBIN.H, used by TESTEPH.C
It contains the definition of DENUM, JPL ephemeris number. Some variables
obtain their values depending on DENUM (eg. RECSIZE - record size ).
Second, look at lines with fopen() calls in TESTEPH.C 
Adjust them for your environment, adding a path before filenames where necessary.
******************************************************************************

All source files are rich of comments and ( I hope ) therefore are
self explanatory.

All suggestions, bug reports or questions may be directed to the author:

Piotr A. Dybczynski, e-mail: dybol@amu.edu.pl

Please send me also a word if you use my software with success.

April 18, 2021.

