# Makefile

OBJS	=  test0.o test_vtree_reader.o test2.o test3.o test4.o main.0 reader.o SFDD.o manager.o
T_OBJS = $(patsubst %.o, objs/%.o, $(OBJS))
T0_OBJS = $(patsubst %.o, objs/%.o, test0.o SFDD.o)
T1_OBJS = $(patsubst %.o, objs/%.o, test_vtree_reader.o SFDD.o)
T2_OBJS = $(patsubst %.o, objs/%.o, test2.o SFDD.o)
T3_OBJS = $(patsubst %.o, objs/%.o, test3.o SFDD.o)
T4_OBJS = $(patsubst %.o, objs/%.o, test4.o SFDD.o)
M_OBJS = $(patsubst %.o, objs/%.o, main.o sfdd_node.o sfdd_vtree.o manager.o)

CC	= g++
CFLAGS	= -Isrc -g -Wall -std=c++0x -pg
VPATH = src:tester:objs


# all: test0 test2 test_vtree_reader sfdd
all: sfdd

# all: test0 test_vtree_reader test2 test3 test4  sfdd

# test4:	test4.o SFDD.o
# 		$(CC) $(CFLAGS) $(T4_OBJS) -o test4

# test3:	test3.o SFDD.o
# 		$(CC) $(CFLAGS) $(T3_OBJS) -o test3

# test2:	test2.o SFDD.o
# 		$(CC) $(CFLAGS) $(T2_OBJS) -o test2

# test_vtree_reader:	test_vtree_reader.o SFDD.o
# 		$(CC) $(CFLAGS) $(T1_OBJS) -o test_vtree_reader

# test0:	test0.o SFDD.o
# 		$(CC) $(CFLAGS) $(T0_OBJS) -o test0 -O0

sfdd:	main.o sfdd.o manager.o
		$(CC) $(CFLAGS) $(M_OBJS) -o sfdd -O0

%.o:%.cpp
		$(CC) -c $(CFLAGS) $< -o ./objs/$@ -O0

.PHONY : clean

clean:
	rm -f src/*.o objs/*.o sfdd test*

# clrdot:
# 	rm -f dotG/test*/*.dot

clrdotg:
	rm -f dotG/test*/*.dot
	rm -f dotG/test*/*.png
