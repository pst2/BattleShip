#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <chrono>
#include <fstream>

using namespace std;

const int BOARD_SIZE = 10;
const int CELL_SIZE = 60;
const int SCREEN_HEIGHT = BOARD_SIZE * CELL_SIZE + 100;//+100 để hiện thị thêm bộ đếm trên màn hình
const int SCREEN_WIDTH = BOARD_SIZE * CELL_SIZE;
const int NUM_SHIPS = 3;
const int MAX_ATTEMPTS = 390;
const int TIME_LIMIT = 90;

Mix_Chunk* hitSound = nullptr;
Mix_Chunk* missSound = nullptr;
Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* winSound = nullptr;
Mix_Chunk* loseSound = nullptr;

struct Ship {
    int x, y;
    int length;
    bool horizontal;
};

vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE, '~'));
vector<Ship> ships;
int attempts = 0;
int hits = 0;
int totalShipCells = 0;

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer* ren) {
    SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());
    if (!texture) {
        cout << "Can't render image: " << file << " - Error: " << IMG_GetError() << endl;
    }
    return texture;
}
bool isShipSunk(const Ship& ship) {
    for (int i = 0; i < ship.length; ++i) {
        int x = ship.x + (ship.horizontal ? 0 : i);
        int y = ship.y + (ship.horizontal ? i : 0);
        if (board[x][y] != 'X') return false;
    }
    return true;
}
//Tạo bảng
void renderBoard(SDL_Renderer* renderer, SDL_Texture* ship2HImage, SDL_Texture* ship3HImage, SDL_Texture* ship2VImage, SDL_Texture* ship3VImage) {
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
        SDL_Texture* shipTex = nullptr;
        if (ship.length == 2){
            shipTex = ship.horizontal ? ship2HImage : ship2VImage;
           }
        else{
            shipTex = ship.horizontal ? ship3HImage : ship3VImage;
            }
        SDL_Rect dst;
        if (ship.horizontal) {
            dst = {ship.y * CELL_SIZE, ship.x * CELL_SIZE, ship.length * CELL_SIZE, CELL_SIZE};
        } else {
            dst = {ship.y * CELL_SIZE, ship.x * CELL_SIZE, CELL_SIZE, ship.length * CELL_SIZE};
        }
        SDL_RenderCopy(renderer, shipTex, NULL, &dst);
    }
}

}
//Render vị trí tàu
void revealShips(SDL_Renderer* renderer, SDL_Texture* ship2HImage, SDL_Texture* ship3HImage, SDL_Texture* ship2VImage, SDL_Texture* ship3VImage) {
    for (const auto& ship : ships) {
        SDL_Texture* shipTex = nullptr;
        if (ship.length == 2){
            shipTex = ship.horizontal ? ship2HImage : ship2VImage;
           }
        else{
            shipTex = ship.horizontal ? ship3HImage : ship3VImage;
            }
        SDL_Rect dst = {
            ship.y * CELL_SIZE,
            ship.x * CELL_SIZE,
            (ship.horizontal ? ship.length : 1) * CELL_SIZE,
            (ship.horizontal ? 1 : ship.length) * CELL_SIZE
        };
        SDL_RenderCopy(renderer, shipTex, NULL, &dst);
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
    totalShipCells = 0;
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
        totalShipCells += ship.length;
        if (ship.length == 2) numLength2Ships++;
        placeShip(ship);
    }
}

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
//Vẽ nút với trạng thái hover
void renderButton(SDL_Renderer* renderer, TTF_Font* font, const string& text, SDL_Rect rect, bool hovered, bool centerText = true) {
    SDL_Color color = hovered ? SDL_Color{255, 215, 0, 255} : SDL_Color{255, 255, 0, 255};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), {0, 0, 0, 255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect;

        textRect = {
            rect.x + (rect.w - surface->w) / 2,
            rect.y + (rect.h - surface->h) / 2,
            surface->w,
            surface->h
        };
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
}

//Bắt đầu trò chơi
string waitForKeyToStart(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* background) {
    string playerName = "";
    bool waiting = true;
    bool isHovered = false;
    bool isClicked = false;

    SDL_Event event;
    SDL_StartTextInput();

    SDL_Rect playButton = {SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 + 80, 150, 50};
//Vẽ nút Play
    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            else if (event.type == SDL_TEXTINPUT) {
                playerName += event.text.text;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !playerName.empty()) {
                    playerName.pop_back();
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                int mx = event.motion.x;
                int my = event.motion.y;
                isHovered = (mx >= playButton.x && mx <= playButton.x + playButton.w &&
                             my >= playButton.y && my <= playButton.y + playButton.h);
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;
                if (mx >= playButton.x && mx <= playButton.x + playButton.w &&
                    my >= playButton.y && my <= playButton.y + playButton.h) {
                    isClicked = true;
                }
            } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;
                if (mx >= playButton.x && mx <= playButton.x + playButton.w &&
                    my >= playButton.y && my <= playButton.y + playButton.h &&
                    !playerName.empty()) {
                    waiting = false;
                }
                isClicked = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 131, 131, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);

        renderCenterText(renderer, font, "Enter your name: " + playerName);
        renderButton(renderer, font, "Play", playButton, isHovered, isClicked);

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
    return playerName;
}
//Nút quit và reset
void renderexitButton(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Rect rect, bool hovered, bool clicked) {
    SDL_Color color;
    if (clicked) {
        color = {255, 165, 0, 255};
    } else if (hovered) {
        color = {255, 255, 0, 255};
    } else {
        color = {200, 200, 0, 255};
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);


    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), {0, 0, 0, 255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect;
    textRect.w = surface->w;
    textRect.h = surface->h;
    textRect.x = rect.x + (rect.w - textRect.w) / 2;
    textRect.y = rect.y + (rect.h - textRect.h) / 2;

    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}



int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    //Khởi tạo âm thanh trúng, trượt
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    hitSound = Mix_LoadWAV("hit.wav");
    missSound = Mix_LoadWAV("miss.wav");
    if (!hitSound || !missSound) {
    cout << "Failed to load sound: " << Mix_GetError() << endl;
    return -1;
    }
    //Nhạc nền
    backgroundMusic = Mix_LoadMUS("background_music.mp3");
    if (!backgroundMusic) {
    cout << "Failed to load background music: " << Mix_GetError() << endl;
    }
    else{
        Mix_PlayMusic(backgroundMusic, -1);//Lặp
    }
    //Âm thanh thắng
    winSound = Mix_LoadWAV("win_sound.wav");
    if (!winSound) {
    SDL_Log("Failed to load win music SDL_mixer Error: %s", Mix_GetError());
    }
    //Âm thanh thua
    loseSound = Mix_LoadWAV("lose_sound.wav");
    if (!loseSound) {
    SDL_Log("Failed to load lose music SDL_mixer Error: %s", Mix_GetError());
    }


//Tạo cửa sổ, đồ họa, font chữ
    SDL_Window* window = SDL_CreateWindow("Battleship", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* background = loadTexture("board.png", renderer);
    SDL_Texture* winImage = loadTexture("Congratulation! You win!.png", renderer);
    SDL_Texture* lostImage = loadTexture("You lost.png", renderer);
    SDL_Texture* TimesoutImage = loadTexture("Times out!.png", renderer);
    SDL_Texture* ship2VImage = loadTexture("tàu 2 ngang.png", renderer);
    SDL_Texture* ship3VImage = loadTexture("tàu 2 ngang.png", renderer);
    SDL_Texture* ship2HImage = loadTexture("tàu 2.png", renderer);
    SDL_Texture* ship3HImage = loadTexture("tàu 2.png", renderer);


    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        cout << "Can't reload font: " << TTF_GetError() << endl;
        return -1;
    }
    std::string playerName;

    while (true) {
        board = vector<vector<char>>(BOARD_SIZE, vector<char>(BOARD_SIZE, '~'));
        ships.clear(); attempts = 0; hits = 0;

        playerName = waitForKeyToStart(renderer, font, background);
        placeShips();
//Bộ đếm thời gian
        auto start_time = chrono::high_resolution_clock::now();
        bool running = true;
        SDL_Event event;

        while (running) {
            auto current_time = chrono::high_resolution_clock::now();
            int elapsed_time = chrono::duration_cast<chrono::seconds>(current_time - start_time).count();

            if (elapsed_time >= TIME_LIMIT) {
                Mix_PlayChannel(-1, loseSound, 0);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, TimesoutImage, NULL, NULL);
                SDL_RenderPresent(renderer);
                SDL_Delay(3500);

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);
                renderBoard(renderer, ship2HImage,ship3HImage, ship2VImage,ship3VImage);
                revealShips(renderer, ship2HImage,ship3HImage, ship2VImage,ship3VImage);
                SDL_RenderPresent(renderer);
                SDL_Delay(3000);

                break;
            }

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) return 0;
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int x = event.button.y / CELL_SIZE;
                    int y = event.button.x / CELL_SIZE;
                    if (board[x][y] == '~') { board[x][y] = 'O'; attempts++; Mix_PlayChannel(-1, missSound, 0); }
                    else if (board[x][y] == 'S') { board[x][y] = 'X'; attempts++; hits++; Mix_PlayChannel(-1, hitSound, 0); }
                }
            }

            if (attempts >= MAX_ATTEMPTS) {
                Mix_PlayChannel(-1, loseSound, 0);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, lostImage, NULL, NULL);
                SDL_RenderPresent(renderer);
                SDL_Delay(3500);

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);
                renderBoard(renderer, ship2VImage, ship3VImage, ship2HImage,ship3HImage);
                revealShips(renderer, ship2VImage, ship3VImage, ship2HImage,ship3HImage);
                SDL_RenderPresent(renderer);
                SDL_Delay(3000);

                break;
            }

            if (hits == totalShipCells) {
                Mix_PlayChannel(-1, winSound, 0);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, winImage, NULL, NULL);
                SDL_RenderPresent(renderer);
                SDL_Delay(3500);

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);
                renderBoard(renderer, ship2VImage, ship3VImage, ship2HImage,ship3HImage);
                revealShips(renderer, ship2VImage, ship3VImage, ship2HImage,ship3HImage);
                SDL_RenderPresent(renderer);
                SDL_Delay(3000);

                break;
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background, NULL, NULL);
            renderBoard(renderer, ship2VImage, ship3VImage, ship2HImage,ship3HImage);
//Hiển thị bố đếm thời gian và lượt chơi trên màn hình
            string timeText = "Time left: " + to_string(TIME_LIMIT - elapsed_time) + "s";
            string attemptText = "Attempts left: " + to_string(MAX_ATTEMPTS - attempts);
            renderText(renderer, font, timeText, 10, SCREEN_HEIGHT - 90);
            renderText(renderer, font, attemptText, 10, SCREEN_HEIGHT - 50);
            renderText(renderer, font, "Player: " + playerName, SCREEN_WIDTH - 200, SCREEN_HEIGHT - 90);


            SDL_RenderPresent(renderer);
        }
//nút reset, quit
       bool wait = true;
       bool isPlayHovered = false, isQuitHovered = false;
       bool isPlayClicked = false, isQuitClicked = false;
       SDL_Rect playAgainButton = {SCREEN_WIDTH / 2 - 160, SCREEN_HEIGHT / 2 + 40, 140, 50};
       SDL_Rect quitButton = {SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2 + 40, 120, 50};
       while (wait) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return 0;
        else if (event.type == SDL_MOUSEMOTION) {
            int mx = event.motion.x;
            int my = event.motion.y;
            isPlayHovered = (mx >= playAgainButton.x && mx <= playAgainButton.x + playAgainButton.w &&
                             my >= playAgainButton.y && my <= playAgainButton.y + playAgainButton.h);
            isQuitHovered = (mx >= quitButton.x && mx <= quitButton.x + quitButton.w &&
                             my >= quitButton.y && my <= quitButton.y + quitButton.h);
        } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int mx = event.button.x;
            int my = event.button.y;
            if (mx >= playAgainButton.x && mx <= playAgainButton.x + playAgainButton.w &&
                my >= playAgainButton.y && my <= playAgainButton.y + playAgainButton.h)
                isPlayClicked = true;

            if (mx >= quitButton.x && mx <= quitButton.x + quitButton.w &&
                my >= quitButton.y && my <= quitButton.y + quitButton.h)
                isQuitClicked = true;
        } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
            int mx = event.button.x;
            int my = event.button.y;

            if (mx >= playAgainButton.x && mx <= playAgainButton.x + playAgainButton.w &&
                my >= playAgainButton.y && my <= playAgainButton.y + playAgainButton.h)
                wait = false;

            if (mx >= quitButton.x && mx <= quitButton.x + quitButton.w &&
                my >= quitButton.y && my <= quitButton.y + quitButton.h)
                return 0;

            isPlayClicked = false;
            isQuitClicked = false;
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 131, 131, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    renderCenterText(renderer, font, "Do you want to play again?");

    renderButton(renderer, font, "Play Again", playAgainButton, isPlayHovered, isPlayClicked);
    renderButton(renderer, font, "Quit", quitButton, isQuitHovered, isQuitClicked);

    SDL_RenderPresent(renderer);
}
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    SDL_DestroyTexture(ship2VImage);
    SDL_DestroyTexture(ship3VImage);
    SDL_DestroyTexture(ship2HImage);
    SDL_DestroyTexture(ship3HImage);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(missSound);
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(winSound);
    Mix_FreeChunk(loseSound);
    Mix_CloseAudio();

    return 0;
}
