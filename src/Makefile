all: createrunfile

createrunfile: graph_test.o graphics.o
	gcc graph_test.o graphics.o -o ./bin/runfile `sdl2-config --cflags --libs`

graph_test.o: graph_test.c 
	gcc -c graph_test.c `sdl2-config --cflags --libs`

graphics.o: graphics.c
	gcc -c graphics.c `sdl2-config --cflags --libs`

.PHONY: clean

clean:
	rm -f ./bin/runfile graph_test.o graphics.o