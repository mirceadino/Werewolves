CC=g++
CFLAGS=-std=c++14

SRC=src
BIN=bin
OBJ=obj

# all
all: server client

# client
SRC_CLIENT=$(SRC)/client

DEPS_TMP_client=client_main.o client_controller.o client_textview.o
DEPS_client=$(patsubst %,$(OBJ)/%,$(DEPS_TMP_client))
client: $(DEPS_TMP_client)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $(DEPS_client) -l boost_system

CPP_TMP_client_main=main.cpp
H_TMP_client_main=
DEPS_TMP_client_main=$(CPP_TMP_client_main) $(H_TMP_client_main)
CPP_client_main=$(patsubst %,$(SRC_CLIENT)/%,$(CPP_TMP_client_main))
H_client_main=$(patsubst %,$(SRC_CLIENT)/%,$(H_TMP_client_main))
DEPS_client_main=$(patsubst %,$(SRC_CLIENT)/%,$(DEPS_TMP_client_main))
client_main.o: $(DEPS_client_main)
	$(CC) $(CFLAGS) -c -o $(OBJ)/$@ $(CPP_client_main)

CPP_TMP_client_controller=controller.cpp
H_TMP_client_controller=controller.h
DEPS_TMP_client_controller=$(CPP_TMP_client_controller) $(H_TMP_client_controller)
CPP_client_controller=$(patsubst %,$(SRC_CLIENT)/%,$(CPP_TMP_client_controller))
H_client_controller=$(patsubst %,$(SRC_CLIENT)/%,$(H_TMP_client_controller))
DEPS_client_controller=$(patsubst %,$(SRC_CLIENT)/%,$(DEPS_TMP_client_controller))
client_controller.o: $(DEPS_client_controller)
	$(CC) $(CFLAGS) -c -o $(OBJ)/$@ $(CPP_client_controller)

CPP_TMP_client_textview=textview.cpp
H_TMP_client_textview=textview.h
DEPS_TMP_client_textview=$(CPP_TMP_client_textview) $(H_TMP_client_textview)
CPP_client_textview=$(patsubst %,$(SRC_CLIENT)/%,$(CPP_TMP_client_textview))
H_client_textview=$(patsubst %,$(SRC_CLIENT)/%,$(H_TMP_client_textview))
DEPS_client_textview=$(patsubst %,$(SRC_CLIENT)/%,$(DEPS_TMP_client_textview))
client_textview.o: $(DEPS_client_textview)
	$(CC) $(CFLAGS) -c -o $(OBJ)/$@ $(CPP_client_textview)

# server
SRC_SERVER=$(SRC)/server

DEPS_TMP_server=server_main.o server_controller.o server_client_handler.o
DEPS_server=$(patsubst %,$(OBJ)/%,$(DEPS_TMP_server))
server: $(DEPS_TMP_server)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $(DEPS_server) -lboost_system -lboost_thread-mt

CPP_TMP_server_main=main.cpp
H_TMP_server_main=
DEPS_TMP_server_main=$(CPP_TMP_server_main) $(H_TMP_server_main)
CPP_server_main=$(patsubst %,$(SRC_SERVER)/%,$(CPP_TMP_server_main))
H_server_main=$(patsubst %,$(SRC_SERVER)/%,$(H_TMP_server_main))
DEPS_server_main=$(patsubst %,$(SRC_SERVER)/%,$(DEPS_TMP_server_main))
server_main.o: $(DEPS_server_main)
	$(CC) $(CFLAGS) -c -o $(OBJ)/$@ $(CPP_server_main)

CPP_TMP_server_controller=controller.cpp
H_TMP_server_controller=controller.h
DEPS_TMP_server_controller=$(CPP_TMP_server_controller) $(H_TMP_server_controller)
CPP_server_controller=$(patsubst %,$(SRC_SERVER)/%,$(CPP_TMP_server_controller))
H_server_controller=$(patsubst %,$(SRC_SERVER)/%,$(H_TMP_server_controller))
DEPS_server_controller=$(patsubst %,$(SRC_SERVER)/%,$(DEPS_TMP_server_controller))
server_controller.o: $(DEPS_server_controller)
	$(CC) $(CFLAGS) -c -o $(OBJ)/$@ $(CPP_server_controller)

CPP_TMP_server_client_handler=client_handler.cpp
H_TMP_server_client_handler=client_handler.h
DEPS_TMP_server_client_handler=$(CPP_TMP_server_client_handler) $(H_TMP_server_client_handler)
CPP_server_client_handler=$(patsubst %,$(SRC_SERVER)/%,$(CPP_TMP_server_client_handler))
H_server_client_handler=$(patsubst %,$(SRC_SERVER)/%,$(H_TMP_server_client_handler))
DEPS_server_client_handler=$(patsubst %,$(SRC_SERVER)/%,$(DEPS_TMP_server_client_handler))
server_client_handler.o: $(DEPS_server_client_handler)
	$(CC) $(CFLAGS) -c -o $(OBJ)/$@ $(CPP_server_client_handler)

# clean
clean:
	rm $(OBJ)/* $(BIN)/*
