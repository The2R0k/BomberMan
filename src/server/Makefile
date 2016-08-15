target = server

objects = logic.o server.o

CFLAGS = -Wall -pedantic -ansi -g
 
VPATH = VPATH:../logic:../logic/include

all : $(target)

$(target) : $(objects)
	$(CC) -o server $(objects)

logic.o : logic.h bomb_info.h fire_field.h player_info.h player_state.h
logic.o : logic.c
	$(CC) -c $(CFLAGS) $< -o $@ 

server.o : header.h logic.h
server.o : server.c
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	-rm $(target) *.o

.PHONY : clean
