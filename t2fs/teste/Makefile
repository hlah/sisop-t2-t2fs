#
# Makefile para programas de teste
# 

CC=gcc
LIB_DIR=../lib
CFLAGS=-L$(LIB_DIR)/ -lt2fs -Wall -m32

TESTS=$(patsubst %.c, %, $(wildcard *.c))

# build all tests
all: $TESTS

# Exemplo:
# testa_yield: teste.c $(LIB_DIR)/.a
#	$(CC) -o teste teste.c -L$(LIB_DIR) $(CFLAGS)

# build a test
%: %.cpp $(LIB_DIR)/libt2fs.a
	$(CC) $(CFLAGS) $< -o $(patsubst %.cpp,%,$<)

.PHONY: all clean 
	
clean:
	rm -rf *.o *~ $(TESTS)
