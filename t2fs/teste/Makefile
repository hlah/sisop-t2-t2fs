#
# Makefile para programas de teste
# 

CC=gcc
LIB_DIR=../lib
INC_DIR=../include
CFLAGS=-I$(INC_DIR) -L$(LIB_DIR)/ -lt2fs -Wall -m32

TESTS=$(patsubst %.c, %, $(wildcard *.c))

# build all tests
all: $(TESTS)

# Exemplo:
# testa_yield: teste.c $(LIB_DIR)/.a
#	$(CC) -o teste teste.c -L$(LIB_DIR) $(CFLAGS)

# build a test
%: %.c $(LIB_DIR)/libt2fs.a
	$(CC) $< -o $(patsubst %.c,%,$<) $(CFLAGS)

.PHONY: all clean 
	
clean:
	rm -rf *.o *~ $(TESTS)
	cp ../t2fs_disk.dat ./
