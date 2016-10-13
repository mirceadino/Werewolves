game.o: game.c
	gcc -c game.c

server.o: server.c
	gcc -c server.c

OBJ=game.o server.o

server: $(OBJ)
	gcc -o server $(OBJ)

clean:
	rm *.o server
