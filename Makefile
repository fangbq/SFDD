# Makefile

OBJS	= test1.o reader.o SFDD.o
T_OBJS = $(patsubst %.o, objs/%.o, test1.o reader.o SFDD.o)
CC	= g++
CFLAGS	= -Isrc -g -Wall -std=c++0x
VPATH = src:tester:objs


sfdd:	$(OBJS)
		$(CC) $(CFLAGS) $(T_OBJS) -o sfdd

%.o:%.cpp
		$(CC) -c $(CFLAGS) $< -o ./objs/$@

.PHONY : clean

clean:
	rm -f src/*.o objs/*.o sfdd

clrdot:
	rm -f dotG/test*/*.dot

clrdotg:
	rm -f dotG/test*/*.png
