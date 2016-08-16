objects = main.o gui.o network.o

CFLAGS = -Wall -pedantic -ansi -g

all : client

client : $(objects)
	$(CC) -o client $^

main.o : gui.h
main.o : network.h
gui.o : network.h
gui.o : gui.h
network.o : network.h

$(objects) : %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	-rm $(objects) client

.PHONY : clean
