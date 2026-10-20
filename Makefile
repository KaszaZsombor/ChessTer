CC = gcc
FLAGS = -Wall -Wextra  -Wshadow -Wpedantic -Werror -g -Iinclude 

TARGET = sakk.exe
SRC_DIR = src
OBJ_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

run: $(TARGET)
	./$(TARGET)

clean:
	-rmdir /S /Q $(OBJ_DIR)
	-del /Q $(TARGET)



