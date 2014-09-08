#
# IFJ 2014/2015 Makefie
#

CC=gcc
CFLAGS=-O2 -std=c99 -lm -Wall -pedantic
OBJFILES=$(patsubst src/%.c, build/%.c.o, $(shell ls src/*.c))
TESTFILES=$(patsubst tests/%.pas, tests/%.out, $(shell ls tests/*.pas));

# build
all: ifj

ifj: $(OBJFILES)
	$(CC) $(CFLAGS) build/*.o -o ifj

build/%.c.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $(subst src/, build/, $<.o)

# Clean compiled files
clean:
	rm -rf build/* ifj IFJ.zip tests/*.out

# Commit changes
commit: clean
	git commit -a
	git pull
	git push

# Create zip file for submitting
zip: clean
	zip IFJ.zip src build doc Makefile

# Test
tests: ifj $(TESTFILES)
	# Done

tests/%.out: tests/%.pas
	-$(shell ./ifj $< > $@.run; diff $@.run $@.correct > /dev/null; if [ "$$?" = "0" ]; then echo "# OK ... $<"; rm $@.run; else echo "# ERROR ... $<"; fi)
	
