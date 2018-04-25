CC = gcc
CFLAG := -std=c99 -O2 -finline-functions -Imy_include
LIB_FLAG := -L. -lsdd -lm
SRC := $(wildcard my_src/*.c)
TEST_SRC := $(wildcard *.c)

all: test1 test3 test4

OBJS := $(patsubst %.c, %.o, $(SRC))
TEST_OBJS := $(patsubst %.c, %.o, $(TEST_SRC))

test1: test1.o $(OBJS)
	$(CC) $^ $(LIB_FLAG) -o $@

test3: test3.o $(OBJS)
	$(CC) $^ $(LIB_FLAG) -o $@

test4: test4.o $(OBJS)
	$(CC) $^ $(LIB_FLAG) -o $@

$(TEST_OBJS): %.o: %.c
	$(CC) -c $(CFLAG) $^ -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAG) $^ -o $@

.PHONY: clean

clean : 
	rm test1 test4 test3 $(TEST_OBJS) $(OBJS)
