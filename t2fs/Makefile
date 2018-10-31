#
# Makefile de EXEMPLO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# É NECESSARIO ADAPTAR ESSE ARQUIVO de makefile para suas necessidades.
#  1. Cuidado com a regra "clean" para não apagar o "support.o"
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "cthread"
# 

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
ifdef DEBUG
C_FLAGS=-Wall -m32 -DDEBUG
else
C_FLAGS=-Wall -m32
endif

OBJS=$(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))

all: build_lib
	echo $(OBJS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< $(C_FLAGS) -I$(INC_DIR) -o $@ 

build_lib: $(OBJS)
	mkdir -p $(LIB_DIR)
	ar crs $(LIB_DIR)/libt2fs.a $(BIN_DIR)/*.o $(LIB_DIR)/apidisk.o

clean:
	rm -rf $(BIN_DIR)/* $(LIB_DIR)/*.a $(SRC_DIR)/*~ $(INC_DIR)/*~ *~ .*~ *.sw?
	make -C teste/ clean

.PHONY: clean build-lib all
