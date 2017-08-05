# Makefile

OBJS	=  test0.o test1.o test2.o test3.o test4.o reader.o SFDD.o
T_OBJS = $(patsubst %.o, objs/%.o, test0.o test1.o test2.o test3.o test4.o reader.o SFDD.o)
T0_OBJS = $(patsubst %.o, objs/%.o, test0.o SFDD.o)
T1_OBJS = $(patsubst %.o, objs/%.o, test1.o SFDD.o)
T2_OBJS = $(patsubst %.o, objs/%.o, test2.o SFDD.o)
T3_OBJS = $(patsubst %.o, objs/%.o, test3.o SFDD.o)
T4_OBJS = $(patsubst %.o, objs/%.o, test4.o SFDD.o)

CC	= g++
CFLAGS	= -Isrc -g -Wall -std=c++0x
VPATH = src:tester:objs


# sfdd:	$(OBJS)
# 		$(CC) $(CFLAGS) $(T_OBJS) -o sfdd

all: test0 test1 test2 test3 test4

test4:	test4.o SFDD.o
		$(CC) $(CFLAGS) $(T4_OBJS) -o test4

test3:	test3.o SFDD.o
		$(CC) $(CFLAGS) $(T3_OBJS) -o test3

test2:	test2.o SFDD.o
		$(CC) $(CFLAGS) $(T2_OBJS) -o test2

test1:	test1.o SFDD.o
		$(CC) $(CFLAGS) $(T1_OBJS) -o test1

test0:	test0.o SFDD.o
		$(CC) $(CFLAGS) $(T0_OBJS) -o test0

%.o:%.cpp
		$(CC) -c $(CFLAGS) $< -o ./objs/$@

.PHONY : clean

clean:
	rm -f src/*.o objs/*.o sfdd test*

clrdot:
	rm -f dotG/test*/*.dot

clrdotg:
	rm -f dotG/test*/*.png
