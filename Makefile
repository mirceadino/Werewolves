CC=gcc
CFLAGS=

SRC=src
BIN=bin
OBJ=obj

all: server client

_server_OBJ=game.o utils.o
server_OBJ=$(patsubst %,$(OBJ)/%,$(_server_OBJ))

server: $(server_OBJ)
	$(CC) -o $(BIN)/$@ $(SRC)/$@.c $(server_OBJ)

_client_OBJ=game.o utils.o
client_OBJ=$(patsubst %,$(OBJ)/%,$(_client_OBJ))

client: $(client_OBJ)
	$(CC) -o $(BIN)/$@ $(SRC)/$@.c $(client_OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm $(OBJ)/* $(BIN)/*
