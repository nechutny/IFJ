#
# IFJ 2014/2015 Makefie
#

CC=gcc
CFLAGS=-O2 -std=c99 -lm -Wall -pedantic -g -rdynamic
OBJFILES=$(patsubst src/%.c, build/%.c.o, $(shell ls src/*.c))
TESTFILES=$(patsubst tests/%.pas, tests/%, $(shell ls tests/*.pas));

# build
all: ifj

ifj: $(OBJFILES)
	$(CC) $(CFLAGS) build/*.o -o ifj

build/%.c.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $(subst src/, build/, $<.o)

# Clean compiled files
clean:
	rm -rf build/* ifj IFJ.zip tests/*.out.run doc/dokumentace.pdf doc/dokumentace.aux doc/dokumentace.log xlogin.zip

# Commit changes
commit: clean
	git commit -a
	git pull
	git push

# Create zip file for submitting
zip:
	rm -rf tmp
	mkdir tmp
	cp src/*.c tmp/
	cp src/*.h tmp/
	cp rozsireni tmp/
	cp rozdeleni tmp/
	mv tmp/uStack.h tmp/ustack.h
	for i in tmp/*; do sed -i 's/uStack.h/ustack.h/g' $$i; done
	echo -e 'all: ifj\n\nifj: *.c\n\tgcc -O2 -std=c99 -lm -Wall -pedantic *.c -o ifj' > tmp/Makefile
	zip -j xlogin00.zip tmp/*
	rm -rf tmp

# Test
tests: ifj $(TESTFILES)
	# Done

tests/%: tests/%.pas
	-$(shell ./ifj $< > $@.stdout.real 2> $@.stderr.real < $@.stdin;echo "$$?" > $@.exit.real;diff $@.stdout.real $@.stdout.correct > /dev/null;if [ "$$?" = "0" ]; then diff $@.stderr.real $@.stderr.correct > /dev/null;if [ "$$?" = "0" ]; then diff $@.exit.real $@.exit.correct > /dev/null; if [ "$$?" = "0" ]; then echo "# OK ... $@"; rm $@.stdout.real $@.stderr.real $@.exit.real; else echo "# ERROR ... $@"; fi; else echo "# ERROR ... $@"; fi; else echo "# ERROR ... $@"; fi)

# Documentation
documentation: doc/dokumentace.tex
	pdflatex  -output-directory=./doc/ doc/dokumentace.tex

sandwich:
	-$(shell if [[ $$(whoami) == "root" ]] ; then echo "# Yes, my lord."; else echo "# You are not root!"; fi)

