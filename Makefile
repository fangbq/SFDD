# Makefile

OBJS	= test1.o test2.o reader.o SFDD.o
T_OBJS = $(patsubst %.o, objs/%.o, test1.o test2.o reader.o SFDD.o)
T1_OBJS = $(patsubst %.o, objs/%.o, test1.o SFDD.o)
T2_OBJS = $(patsubst %.o, objs/%.o, test2.o SFDD.o)

CC	= g++
CFLAGS	= -Isrc -g -Wall -std=c++0x
VPATH = src:tester:objs


# sfdd:	$(OBJS)
# 		$(CC) $(CFLAGS) $(T_OBJS) -o sfdd

all: test1 test2

test2:	test2.o SFDD.o
		$(CC) $(CFLAGS) $(T2_OBJS) -o test2

test1:	test1.o SFDD.o
		$(CC) $(CFLAGS) $(T1_OBJS) -o test1

%.o:%.cpp
		$(CC) -c $(CFLAGS) $< -o ./objs/$@

.PHONY : clean

clean:
	rm -f src/*.o objs/*.o sfdd test*

clrdot:
	rm -f dotG/test*/*.dot

clrdotg:
	rm -f dotG/test*/*.png
