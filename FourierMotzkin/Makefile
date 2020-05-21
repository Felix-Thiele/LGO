CC = g++
CFLAGS = -std=c++17 -Wall -Werror -Wpedantic -Wextra

all: prog

clean:
	del *.o
	del prog.exe

prog:  m.o f.o i.o
	$(CC) $(CFLAGS) m.o f.o i.o -o prog.exe

m.o: f.o i.o
	$(CC) $(CFLAGS) -c main.cpp -o m.o

f.o:
	$(CC) $(CFLAGS) -c FourierMotzkin.cpp -o f.o

i.o:
	$(CC) $(CFLAGS) -c input.cpp -o i.o

