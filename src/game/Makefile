CC = gcc
CFLAGS = -Wall
SRC_DIR = ..
OBJ_DIR = output
TARGET = othello

SRC_FILES = main.c \
            board.c \
            computer.c \
            digraph.c \
            game.c \
            $(SRC_DIR)/utils.c

OBJ_FILES = $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
