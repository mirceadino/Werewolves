CC=g++
CFLAGS=-std=c++11

SRC=src
BIN=bin
OBJ=obj

all: server client

_server_OBJ=game.o utils.o
server_OBJ=$(patsubst %,$(OBJ)/%,$(_server_OBJ))

server: $(server_OBJ)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $(SRC)/$@.cpp $(server_OBJ)

_client_OBJ=game.o utils.o
client_OBJ=$(patsubst %,$(OBJ)/%,$(_client_OBJ))

client: $(client_OBJ)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $(SRC)/$@.cpp $(client_OBJ)

$(OBJ)/%.o: $(SRC)/%.cpp $(SRC)/%.h
	$(CC) $(CFLAGS) -c -o $@ $(CFLAGS) $<

clean:
	rm $(OBJ)/* $(BIN)/*
