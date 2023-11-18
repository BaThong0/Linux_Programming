.PHONY: all clean 

LIB_NAME = shared_test
CUR_DIR := .
BIN_DIR := $(CUR_DIR)/bin 
INC_DIR := $(CUR_DIR)/inc 
SRC_DIR := $(CUR_DIR)/src 
OBJ_DIR := $(CUR_DIR)/objs

LIB_DIR := $(CUR_DIR)/libs 
LIB_STATIC := $(LIB_DIR)/static 
LIB_SHARED := $(LIB_DIR)/shared 

INC_FLAGS := -I $(INC_DIR)
CC := gcc 

mk_objs: 
	$(CC) -c $(CUR_DIR)/main.c -o $(OBJ_DIR)/main.o $(INC_FLAGS)
	$(CC) -c -fPIC $(CUR_DIR)/src/math.c -o $(OBJ_DIR)/math.o $(INC_FLAGS)
	$(CC) -c -fPIC $(CUR_DIR)/src/hello.c -o $(OBJ_DIR)/hello.o $(INC_FLAGS)

mk_static: 
	ar rcs $(CUR_DIR)/libs/static/libstatic_test.a $(OBJ_DIR)/hello.o $(OBJ_DIR)/math.o

mk_shared:
	$(CC) -shared $(OBJ_DIR)/hello.o $(OBJ_DIR)/math.o -o $(CUR_DIR)/libs/shared/lib$(LIB_NAME).so

all: mk_objs mk_static mk_shared 
	$(CC) $(OBJ_DIR)/main.o -L$(CUR_DIR)/libs/shared -l$(LIB_NAME) -o  $(CUR_DIR)/bin/use-shared-library

clean: 
	rm -rf $(OBJ_DIR)/*
	rm -rf $(CUR_DIR)/libs/shared/*
	rm -rf $(CUR_DIR)/libs/static/*
	rm -rf $(CUR_DIR)/bin/*
