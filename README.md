IFJ
===

IFJ Project 2014/2015

## How to start
1. Install git, gcc, GNU make

    Ubuntu/Debian:

         sudo apt-get install git gcc make
    Fedora:

         sudo yum install git gcc make

2. Clone repository

         git clone git@github.com:nechutny/IFJ.git

3. Compile (run in clonned directory)

         make

## Makefile
* make - create object files in build/ directory and link ifj executable file in root directory
* make clean - clean build/ directory, delete executable file and zip file
* make commit - utilize git. Delete temporary files (object files etc.), call commit -a, pull before push (for auto-merge) and then push
* make zip - create zip file named IFJ.zip containing sources, empty build directory, documentation and Makefile

## Code-style
* Tabs
* { and } on separate lines
* functions, variables and comments in english
* function names and variables with _ as word separator
* Maximal line length 80 characters
* Space after comma separating arguments
* Files in utf-8
* Each commit should leave project in working state
* Code block separated by 1 empty line, functions separated by 2 lines
* Commit message should explain what it does, not just "fix", or "update"
* Use multi include header file protection

         #ifndef _FILENAME_H_
         #define _FILENAME_H_
         ... content ...
         #endif
