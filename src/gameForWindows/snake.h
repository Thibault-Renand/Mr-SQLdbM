// Nom du fichier : snake.h

#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define MAX_SNAKE_LENGTH 100
#define TILE_SIZE 20
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[MAX_SNAKE_LENGTH];
    int length;
    int dx, dy;
} Snake;

typedef struct {
    int x, y;
} Food;

void initSnake(Snake *snake);
void moveSnake(Snake *snake);
void growSnake(Snake *snake);
bool checkCollision(Snake *snake);
bool checkFoodCollision(Snake *snake, Food *food);
void renderSnake(Snake *snake);
void spawnFood(Food *food);
void renderFood(Food *food);

#endif
