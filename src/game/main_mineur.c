#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../bdd/testdb.h"
#define GRID_SIZE 10
#define CELL_SIZE 50
#define NUM_MINES 15
#define WINDOW_WIDTH (GRID_SIZE * CELL_SIZE)
#define WINDOW_HEIGHT (GRID_SIZE * CELL_SIZE)

TTF_Font *font;

typedef struct {
    int is_mine;
    int is_revealed;
    int is_flagged;
    int adjacent_mines;
} Cell;

Cell grid[GRID_SIZE][GRID_SIZE];
int game_over = 0;
int flags_remaining = NUM_MINES;
int won = 0;

void initialize_grid() {
    srand(time(NULL));
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].is_mine = 0;
            grid[i][j].is_revealed = 0;
            grid[i][j].is_flagged = 0;
            grid[i][j].adjacent_mines = 0;
        }
    }

    // Place mines
    for (int n = 0; n < NUM_MINES; n++) {
        int x, y;
        do {
            x = rand() % GRID_SIZE;
            y = rand() % GRID_SIZE;
        } while (grid[x][y].is_mine);
        grid[x][y].is_mine = 1;

        // Update adjacent cells
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) {
                    grid[nx][ny].adjacent_mines++;
                }
            }
        }
    }
    game_over = 0;
    won = 0;
    flags_remaining = NUM_MINES;
}

void readBinaryFile() {

    FILE *file = fopen("../bdd/output/gamerecords.dat", "rb"); // Ouvre le fichier en mode binaire

    if (file == NULL) {

        perror("Erreur d'ouverture du fichier");

        return;

    }

 

    GameRecord record; // Variable pour stocker une instance de la structure

    while (fread(&record, sizeof(GameRecord), 1, file) == 1) {

        // Affiche les données lues

        printf("Game ID: %d\n", record.game_id);

        printf("Player 1 ID: %d\n", record.player1_id);

        printf("Player 2 ID: %d\n", record.player2_id);

        printf("Score Player 1: %d\n", record.score_player1);

        printf("Score Player 2: %d\n", record.score_player2);

        printf("Date: %s\n", record.date);

        printf("Winner ID: %d\n", record.gagnant_id);

        printf("Game: %s\n", record.jeux);

        printf("----------------------\n");

    }

 

    fclose(file); // Ferme le fichier

}




void reveal_cell(int x, int y) {
    if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE || grid[x][y].is_revealed || grid[x][y].is_flagged) {
        return;
    }

    grid[x][y].is_revealed = 1;

    if (grid[x][y].is_mine) {
        game_over = 1;
    } else if (grid[x][y].adjacent_mines == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                reveal_cell(x + dx, y + dy);
            }
        }
    }
}

void handle_click(int x, int y, int button) {
    if (game_over || won) return;

    int cell_x = x / CELL_SIZE;
    int cell_y = y / CELL_SIZE;

    if (button == SDL_BUTTON_LEFT) {
        reveal_cell(cell_y, cell_x);
    } else if (button == SDL_BUTTON_RIGHT) {
        if (!grid[cell_y][cell_x].is_revealed) {
            grid[cell_y][cell_x].is_flagged = !grid[cell_y][cell_x].is_flagged;
            flags_remaining += grid[cell_y][cell_x].is_flagged ? -1 : 1;
        }
    }
}

void draw_message(SDL_Renderer *renderer, const char *message) {
    SDL_Color textColor = {255, 0, 0, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, message, textColor);
    if (textSurface) {
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {WINDOW_WIDTH / 4, WINDOW_HEIGHT / 3, WINDOW_WIDTH / 2, 50};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void draw_grid(SDL_Renderer *renderer) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};

            // Determine cell color
            if (grid[i][j].is_revealed) {
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            }
            SDL_RenderFillRect(renderer, &cell);

            // Draw grid lines
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &cell);

            // Draw flags or mine indicators
            if (grid[i][j].is_flagged) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawLine(renderer, cell.x, cell.y, cell.x + CELL_SIZE, cell.y + CELL_SIZE);
                SDL_RenderDrawLine(renderer, cell.x + CELL_SIZE, cell.y, cell.x, cell.y + CELL_SIZE);
            } else if (grid[i][j].is_revealed && grid[i][j].is_mine) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &cell);
            } else if (grid[i][j].is_revealed && grid[i][j].adjacent_mines > 0) {
                char num_text[2];
                sprintf(num_text, "%d", grid[i][j].adjacent_mines);
                SDL_Color textColor = {0, 0, 255, 255};
                SDL_Surface *textSurface = TTF_RenderText_Solid(font, num_text, textColor);
                if (textSurface) {
                    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    SDL_Rect textRect = {cell.x + CELL_SIZE / 4, cell.y + CELL_SIZE / 4, CELL_SIZE / 2, CELL_SIZE / 2};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_FreeSurface(textSurface);
                    SDL_DestroyTexture(textTexture);
                }
            }
        }
    }
}

void check_win() {
    int revealed_count = 0;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j].is_revealed && !grid[i][j].is_mine) {
                revealed_count++;
            }
        }
    }
    if (revealed_count == GRID_SIZE * GRID_SIZE - NUM_MINES) {
        won = 1;
    }
}

int main(int argc, char *argv[]) {
        
        readBinaryFile();

	    users player1;
        //saisieuser("./src/bdd/output/logsinscriptions.dat",bin);
        // Connexion du premier joueur
        printf("Veuillez vous connecter :\n");
        saisieuserlogins(&player1);
        
        if (!connectionuser(&player1, "../bdd/output/logsinscriptions.dat")) {
            printf("Échec de la connexion pour Joueur 1.\n");
            return 1;
        }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("CairoPlay-Black.ttf", 24);
    if (!font) {
        printf("Echec du chargement de la police: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Démineur", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("La fenêtre n'a pas pu s'ouvrir: SDL_erreur %s\n", SDL_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("le moteur de rendu n'a pas pu se creer! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    initialize_grid();

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN && (game_over || won)) {
                if (event.key.keysym.sym == SDLK_r) {
                    initialize_grid();
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                handle_click(event.button.x, event.button.y, event.button.button);
                check_win();
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        draw_grid(renderer);
        
         GameRecord record;
         record.game_id=uidfinder("../bdd/output/gamerecords.dat")+1;
    	 record.player1_id = player1.uid;
   	     record.player2_id = 0;
   	     record.score_player1 = 0;
    	 record.score_player2 = 0;
        strcpy(record.jeux, "demineur");
        strcpy(record.date, "date"); 

        if (game_over) {
            record.gagnant_id = 0;
            draw_message(renderer, "Game Over! Appuyez sur R pour rejouer");
        } else if (won) {
            record.gagnant_id = player1.uid;
            draw_message(renderer, "Victoire! Appuyez sur R pour rejouer");
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

