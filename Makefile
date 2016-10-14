CC=gcc
CFLAGS=

DEPS=game.h

ODIR=obj
_OBJ=game.o server.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

server: $(OBJ)
	$(CC) -o $@ $(OBJ)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm $(ODIR)/*.o server
