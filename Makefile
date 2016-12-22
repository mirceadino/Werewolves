CC=g++
CFLAGS=-std=c++14

SRC=src
BIN=bin
OBJ=obj

# all
all: server client

# client
SRC_CLIENT=$(SRC)/client

DEPS_TMP_client=client_main.o client_controller.o
DEPS_client=$(patsubst %,$(OBJ)/%,$(DEPS_TMP_client))
client: $(DEPS_TMP_client)
	$(CC) $(CFLAGS) -o $(BIN)/client $(DEPS_client) -l boost_system

CPP_TMP_client_controller=controller.cpp
H_TMP_client_controller=controller.h
DEPS_TMP_client_controller=$(CPP_TMP_client_controller) $(H_TMP_client_controller)
CPP_client_controller=$(patsubst %,$(SRC_CLIENT)/%,$(CPP_TMP_client_controller))
H_client_controller=$(patsubst %,$(SRC_CLIENT)/%,$(H_TMP_client_controller))
DEPS_client_controller=$(patsubst %,$(SRC_CLIENT)/%,$(DEPS_TMP_client_controller))
client_controller.o: $(DEPS_client_controller)
	$(CC) $(CFLAGS) -c -o $(OBJ)/$@ $(CPP_client_controller)

CPP_TMP_client_main=main.cpp
H_TMP_client_main=
DEPS_TMP_client_main=$(CPP_TMP_client_main) $(H_TMP_client_main)
CPP_client_main=$(patsubst %,$(SRC_CLIENT)/%,$(CPP_TMP_client_main))
H_client_main=$(patsubst %,$(SRC_CLIENT)/%,$(H_TMP_client_main))
DEPS_client_main=$(patsubst %,$(SRC_CLIENT)/%,$(DEPS_TMP_client_main))
client_main.o: $(DEPS_client_main)
	$(CC) $(CFLAGS) -c -o $(OBJ)/$@ $(CPP_client_main)

# server
SRC_SERVER=$(SRC)/server

DEPS_TMP_server=server_main.o
DEPS_server=$(patsubst %,$(OBJ)/%,$(DEPS_TMP_server))
server: $(DEPS_TMP_server)
	$(CC) $(CFLAGS) -o $(BIN)/server $(DEPS_server) -l boost_system

CPP_TMP_server_main=main.cpp
H_TMP_server_main=
DEPS_TMP_server_main=$(CPP_TMP_server_main) $(H_TMP_server_main)
CPP_server_main=$(patsubst %,$(SRC_SERVER)/%,$(CPP_TMP_server_main))
H_server_main=$(patsubst %,$(SRC_SERVER)/%,$(H_TMP_server_main))
DEPS_server_main=$(patsubst %,$(SRC_SERVER)/%,$(DEPS_TMP_server_main))
server_main.o: $(DEPS_server_main)
	$(CC) $(CFLAGS) -c -o $(OBJ)/$@ $(CPP_server_main)

# clean
clean:
	rm $(OBJ)/* $(BIN)/*
