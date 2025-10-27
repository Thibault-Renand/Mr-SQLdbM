# Variables
CC = gcc
CFLAGS = -Wall -g
OBJ = game/main.o game/board.o game/game.o utils.o bdd/database_manager.o bdd/player_table.o bdd/game_table.o bdd/move_table.o bdd/testdb.o

# Nom de l'exécutable
TARGET = othello

# Règle par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Règles pour compiler les fichiers objets
game/%.o: game/%.c
	$(CC) $(CFLAGS) -c $< -o $@

bdd/%.o: bdd/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJ) $(TARGET)

# Règle pour exécuter le programme
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run