# Makefile

OBJS	= main.o reader.o SFDD.o

CC	= g++
CFLAGS	= -g -Wall -std=c++0x

sfdd:	$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o start

main.o:  main.cpp
		$(CC) $(CFLAGS) -c main.cpp -o main.o 

SFDD.o:  SFDD.cpp SFDD.h
		$(CC) $(CFLAGS) -c SFDD.cpp -o SFDD.o 

reader.o:  reader.cpp reader.h
		$(CC) $(CFLAGS) -c reader.cpp -o reader.o

.PHONY : clean

clean:
	rm -f *.o sfdd

clrdot:
	rm -f ../dotG/*.dot

clrdotg:
	rm -f ../dotG/*.png
