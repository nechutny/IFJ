#
# IFJ 2014/2015 Makefie
#

CC=gcc
CFLAGS=-O2 -std=c99 -lm -Wall -pedantic
OBJFILES=$(patsubst src/%.c, build/%.o, $(shell ls src/*.c))

# build
all: $(OBJFILES)
	$(CC) $(CFLAGS) build/*.o -o ifj

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $(subst src/, build/, $<.o)

# Clean compiled files
clean:
	rm -rf build/* ifj IFJ.zip

# Commit changes
commit: clean
	git commit -a
	git pull
	git push

# Create zip file for submitting
zip: clean
	zip IFJ.zip src build doc Makefile
