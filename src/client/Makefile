objects = main.o gui.o network.o graphics.o

LIBS = -lSDL2

CFLAGS = -Wall -pedantic -ansi -g

VPATH = VPATH:../graphics

all : client

client : $(objects) graphics.o
	$(CC) -o client $^ $(LIBS)
	touch tempfile

main.o : gui.h
main.o : network.h
gui.o : network.h
gui.o : gui.h
network.o : network.h
graphics.o : ../graphics/graphics.c

$(objects) : %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	-rm $(objects) client tempfile

.PHONY : clean
.PHONY : client

