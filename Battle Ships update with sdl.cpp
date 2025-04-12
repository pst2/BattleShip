#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>

using namespace std;

const int BOARD_SIZE = 10;
const int CELL_SIZE = 50;
const int SCREEN_HEIGHT = BOARD_SIZE * CELL_SIZE + 100;//+100 để hiện thị thêm bộ đếm trên màn hình
const int SCREEN_WIDTH = BOARD_SIZE * CELL_SIZE;
const int NUM_SHIPS = 3;// 3 tàu
const int MAX_ATTEMPTS = 36;//36 lượt
const int TIME_LIMIT = 90;//90 giây 

struct Ship {
    int x, y;
    int length;
    bool horizontal;
};

vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE, '~'));
vector<Ship> ships;
int attempts = 0;
int hits = 0;

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer* ren) {
    SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());
    if (!texture) {
        cout << "Can't render image: " << file << " - Error: " << IMG_GetError() << endl;
    }
    return texture;
}
}
//Tạo bảng
void renderBoard(SDL_Renderer* renderer, SDL_Texture* shipImage) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            if (board[i][j] == 'X') {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &cell);

                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(renderer, &cell);
            } else if (board[i][j] == 'O') {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer, &cell);
            } else {
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                SDL_RenderFillRect(renderer, &cell);
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }

    for (const auto& ship : ships) {
        if (isShipSunk(ship)) {
            for (int i = 0; i < ship.length; ++i) {
                int x = ship.x + (ship.horizontal ? 0 : i);
                int y = ship.y + (ship.horizontal ? i : 0);
                SDL_Rect dst = {y * CELL_SIZE, x * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_RenderCopy(renderer, shipImage, NULL, &dst);
            }
        }
    }
}
//Đặt ví trí cho tàu
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
    int numLength2Ships = 0;//số tàu 2 đã đặt

    for (int i = 0; i < NUM_SHIPS; ++i) {
        Ship ship;
        do {
            int len = rand() % 2 + 2;//2 hoặc 3
            if (len == 2 && numLength2Ships >= 2) len = 3;//đủ 2 tàu 2 thì ép length = 3
            ship.length = len;//chiều dài
            ship.horizontal = rand() % 2 == 0;//hướng
            ship.x = rand() % BOARD_SIZE;
            ship.y = rand() % BOARD_SIZE;
        } while (!isValidPlacement(ship));

        if (ship.length == 2) numLength2Ships++;
        placeShip(ship);
    }
}
bool isShipSunk(const Ship& ship) {
    for (int i = 0; i < ship.length; ++i) {
        int x = ship.x + (ship.horizontal ? 0 : i);
        int y = ship.y + (ship.horizontal ? i : 0);
        if (board[x][y] != 'X') return false;
    }
    return true;
//Render chữ trên màn hình
void renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}
void renderCenterText(SDL_Renderer* renderer, TTF_Font* font, const string& text) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {SCREEN_WIDTH/ 2 - surface->w / 2, SCREEN_HEIGHT/ 2 - surface->h / 2, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}
//Bắt đầu trò chơi
void waitForKeyToStart(SDL_Renderer* renderer, TTF_Font* font) {
    bool waiting = true;
    SDL_Event event;
   //Màn hình chờ nhấn P
    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                waiting = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 131, 131, 255);
        SDL_RenderClear(renderer);
        renderCenterText(renderer, font, "Press P to start.");
        SDL_RenderPresent(renderer);
    }
}
//Vị trí kết quả tàu
void revealShips(SDL_Renderer* renderer, SDL_Texture* shipImage) {
    for (const auto& ship : ships) {
        for (int i = 0; i < ship.length; ++i) {
            int x = ship.x + (ship.horizontal ? 0 : i);
            int y = ship.y + (ship.horizontal ? i : 0);
            SDL_Rect dst = {y * CELL_SIZE, x * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(renderer, shipImage, NULL, &dst);
        }
    }
}
int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
//Tạo cửa sổ, đồ họa, font chữ
    SDL_Window* window = SDL_CreateWindow("Battleship", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* background = loadTexture("board.png", renderer);
    SDL_Texture* winImage = loadTexture("Congratulation! You win!.png", renderer);
    SDL_Texture* lostImage = loadTexture("You lost.png", renderer);
    SDL_Texture* TimesoutImage = loadTexture("Times out!.png", renderer);
    SDL_Texture* shipImage = loadTexture("tàu 2.jpg", renderer);

    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);//font chữ và size
    if (!font) {
        cout << "Can't reload font: " << TTF_GetError() << endl;
        return -1;
    }

    while (true) {
        board = vector<vector<char>>(BOARD_SIZE, vector<char>(BOARD_SIZE, '~'));
        ships.clear(); attempts = 0; hits = 0;

        waitForKeyToStart(renderer, font);
        placeShips();
//Bộ đếm thời gian
        auto start_time = chrono::high_resolution_clock::now();
        bool running = true;
        SDL_Event event;

        while (running) {
            auto current_time = chrono::high_resolution_clock::now();
            int elapsed_time = chrono::duration_cast<chrono::seconds>(current_time - start_time).count();

            if (elapsed_time >= TIME_LIMIT) {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, TimesoutImage, NULL, NULL);
                SDL_RenderPresent(renderer);
                SDL_Delay(3000);

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);
                renderBoard(renderer, shipImage);
                revealShips(renderer, shipImage);
                SDL_RenderPresent(renderer);
                SDL_Delay(5000);

                break;
            }
//Tương tác chuột
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) return 0;
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int x = event.button.y / CELL_SIZE;
                    int y = event.button.x / CELL_SIZE;
                    if (board[x][y] == '~') { board[x][y] = 'O'; attempts++; }
                    else if (board[x][y] == 'S') { board[x][y] = 'X'; attempts++; hits++; }
                }
            }
//Đếm lượt
            if (attempts >= MAX_ATTEMPTS) {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, lostImage, NULL, NULL);
                SDL_RenderPresent(renderer);
                SDL_Delay(3000);

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);
                renderBoard(renderer, shipImage);
                revealShips(renderer, shipImage);
                SDL_RenderPresent(renderer);
                SDL_Delay(5000);

                break;
            }

            if (hits > NUM_SHIPS*2 && hits < NUM_SHIPS*3) {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, winImage, NULL, NULL);
                SDL_RenderPresent(renderer);
                SDL_Delay(3000);

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);
                renderBoard(renderer, shipImage);
                revealShips(renderer, shipImage);
                SDL_RenderPresent(renderer);
                SDL_Delay(5000);

                break;
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background, NULL, NULL);
            renderBoard(renderer, shipImage);
//Hiển thị bố đếm thời gian và lượt chơi trên màn hình
            string timeText = "Time left: " + to_string(TIME_LIMIT - elapsed_time) + "s";
            string attemptText = "Attempts left: " + to_string(MAX_ATTEMPTS - attempts);
            renderText(renderer, font, timeText, 10, SCREEN_HEIGHT - 90);
            renderText(renderer, font, attemptText, 10, SCREEN_HEIGHT - 50);

            SDL_RenderPresent(renderer);
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        renderCenterText(renderer, font, "Press R to reset or Q to exit.");
        SDL_RenderPresent(renderer);

        bool wait = true;
        while (wait) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) wait = false;
                else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_q) return 0;
                    else if (event.key.keysym.sym == SDLK_r) wait = false;
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
