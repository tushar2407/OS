CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = q1.o q2a.o q2b.o q2c.o q3a.o q3b.o q3c.o q4.o q5.o q6.o  

%.o: %.c $(DEPS)
	$(CC) -o $@ $< $(CFLAGS)

all: $(OBJ)

clean:
	rm -f $(OBJ)

run1: 
	gcc q1.o && ./a.out

run2: 
	gcc q2a.o && ./a.out
	gcc q2b.o && ./a.out
	gcc q2c.o && ./a.out
run3: 
	gcc q3c.o -lpthread -o server && ./server
	gcc q3a.o -lpthread -o client1 && ./client1
	gcc q3b.o -lpthread -o client2 && ./client2

run4: 
	gcc q4.o -lpthread && ./a.out
run5:
	gcc q5.c -pthread && ./a.out
run6:
	gcc q6.c -pthread && ./a.out