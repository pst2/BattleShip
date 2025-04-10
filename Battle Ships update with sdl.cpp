#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <chrono>

using namespace std;

const int BOARD_SIZE = 10;
const int CELL_SIZE = 50;
const int SCREEN_WIDTH = BOARD_SIZE * CELL_SIZE;
const int SCREEN_HEIGHT = BOARD_SIZE * CELL_SIZE;
const int NUM_SHIPS = 3; 
const int MAX_ATTEMPTS = 30; 
const int TIME_LIMIT = 60;

struct Ship {
    int x, y;
    int length;
    bool horizontal;
};

vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE, '~')); 
vector<Ship> ships; 
int attempts = 0; 
int hits = 0; 

//render
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer* ren) {
    SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());
    if (!texture) {
        cout << "Can't render image: " << file << " - Error: " << IMG_GetError() << endl;
    }
    return texture;
}

void renderBoard(SDL_Renderer* renderer) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            if (board[i][j] == 'X') {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
            } else if (board[i][j] == 'O') {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); 
            } else {
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); 
            }
            SDL_RenderFillRect(renderer, &cell);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
}

//đặt thuyền
bool isValidPlacement(const Ship& ship) {
    if (ship.horizontal) {
        if (ship.y + ship.length > BOARD_SIZE) return false;
        for (int i = 0; i < ship.length; ++i) {
            if (board[ship.x][ship.y + i] != '~') return false;
        }
    } else {
        if (ship.x + ship.length > BOARD_SIZE) return false;
        for (int i = 0; i < ship.length; ++i) {
            if (board[ship.x + i][ship.y] != '~') return false;
        }
    }
    return true;
}

void placeShip(const Ship& ship) {
    if (ship.horizontal) {
        for (int i = 0; i < ship.length; ++i) {
            board[ship.x][ship.y + i] = 'S';
        }
    } else {
        for (int i = 0; i < ship.length; ++i) {
            board[ship.x + i][ship.y] = 'S';
        }
    }
    ships.push_back(ship);
}

void placeShips() {
    srand(time(0));
    for (int i = 0; i < NUM_SHIPS; ++i) {
        Ship ship;
        ship.length = rand() % 2 + 2; 
        ship.horizontal = rand() % 2 == 0;

        do {
            ship.x = rand() % BOARD_SIZE;
            ship.y = rand() % BOARD_SIZE;
        } while (!isValidPlacement(ship));

        placeShip(ship);
    }
}
//trúng
bool isHit(int x, int y) {
    for (const auto& ship : ships) {
        if (ship.horizontal) {
            if (ship.y <= y && y < ship.y + ship.length && ship.x == x) return true;
        } else {
            if (ship.x <= x && x < ship.x + ship.length && ship.y == y) return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL không thể khởi tạo: " << SDL_GetError() << endl;
        return -1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "SDL_image không thể khởi tạo: " << IMG_GetError() << endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Battleship", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

   //render đồ họa
    SDL_Texture* background = loadTexture("board.png", renderer);
    if (!background) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

     SDL_Texture* winImage = loadTexture("Congratulation! You win!.png", renderer);
     if (!winImage) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
     }
     SDL_Texture* lostImage = loadTexture("You lost.png", renderer);
     if (!lostImage) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
     }
     SDL_Texture* TimesoutImage = loadTexture("Times out!.png", renderer);
     if (!TimesoutImage) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
     }


    placeShips();

    bool running = true;
    SDL_Event event;

//đếm thời gian
    auto start_time = std::chrono::high_resolution_clock::now();

    while (running) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();

        if (elapsed_time >= TIME_LIMIT) {
            cout << "Times out! You lost." << endl;
            SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, TimesoutImage, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
            running = false;
        }

        while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) running = false;
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.y / CELL_SIZE;
        int y = event.button.x / CELL_SIZE;

        if (board[x][y] == '~') { 
            board[x][y] = 'O'; 
            cout << "Missed (" << x << ", " << y << ")!" << endl;
            attempts++;
        } else if (board[x][y] == 'S') { 
            board[x][y] = 'X'; 
            cout << "Correct (" << x << ", " << y << ")!" << endl;
            hits++;
            attempts++;
        }
    }
}
        if (attempts >= MAX_ATTEMPTS) {
            cout << "Times play out! You lost." << endl;
            SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, lostImage, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
            running = false;
        }

        //thắng
        if (hits >= NUM_SHIPS * 2) { 
            cout << "Congratulation! You win" << endl;
            SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, winImage, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
            running = false;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL); 
        renderBoard(renderer); 
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}

