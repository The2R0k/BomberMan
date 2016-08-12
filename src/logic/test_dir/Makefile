target = test

objects = logic.o test.o

CFLAGS = -Wall -pedantic -ansi -g

VPATH = ..

all : $(target)

$(target) : $(objects)
	$(CC) $^ -o $(target)

logic.o : logic.h
logic.o : logic.c
	$(CC) -c $(CFLAGS) $< -o $@
	
test.o : test.c
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	-rm *.o $(target)

.PHONY : clean
