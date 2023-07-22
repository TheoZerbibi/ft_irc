CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -L/mnt/nfs/homes/thzeribi/.config/curl/lib -lcurl
INCLUDE = -I/mnt/nfs/homes/thzeribi/.config/curl/include

all: my_program

my_program: main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(INCLUDE)

clean:
	rm -f my_program
