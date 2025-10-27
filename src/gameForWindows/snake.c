// Nom du fichier : snake.c

#include "snake.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

extern SDL_Renderer *renderer;

void initSnake(Snake *snake) {
    snake->length = 1;
    snake->body[0].x = 10;
    snake->body[0].y = 10;
    snake->dx = 1;
    snake->dy = 0;
}

void moveSnake(Snake *snake) {
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0].x += snake->dx;
    snake->body[0].y += snake->dy;
}

void growSnake(Snake *snake) {
    if (snake->length < MAX_SNAKE_LENGTH) {
        snake->length++;
    }
}

bool checkCollision(Snake *snake) {
    Point head = snake->body[0];

    if (head.x < 0 || head.x >= SCREEN_WIDTH / TILE_SIZE ||
        head.y < 0 || head.y >= SCREEN_HEIGHT / TILE_SIZE) {
        return true;
    }

    for (int i = 1; i < snake->length; i++) {
        if (snake->body[i].x == head.x && snake->body[i].y == head.y) {
            return true;
        }
    }

    return false;
}

bool checkFoodCollision(Snake *snake, Food *food) {
    return snake->body[0].x == food->x && snake->body[0].y == food->y;
}

void renderSnake(Snake *snake) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < snake->length; i++) {
        SDL_Rect rect = { snake->body[i].x * TILE_SIZE, snake->body[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_RenderFillRect(renderer, &rect);
    }
}

void spawnFood(Food *food) {
    food->x = rand() % (SCREEN_WIDTH / TILE_SIZE);
    food->y = rand() % (SCREEN_HEIGHT / TILE_SIZE);
}

void renderFood(Food *food) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = { food->x * TILE_SIZE, food->y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_RenderFillRect(renderer, &rect);
}
