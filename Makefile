CC = gcc
FLAGS = -Iinclude -std=c99
DEBUGFLAGS = -Wall -Wextra  -Wshadow -Wpedantic -g

TARGET = sakk.exe
SRC_DIR = source
OBJ_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) $(DEBUGFLAGS) -c $< -o $@


$(OBJ_DIR):
	mkdir $(OBJ_DIR)


run: $(TARGET)
	./$(TARGET)


clean:
	-rmdir /S /Q $(OBJ_DIR)
	-del /Q $(TARGET)



