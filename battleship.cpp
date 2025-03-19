#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;

const int BOARD_SIZE = 10;
const int NUM_SHIPS = 3;
const int TIME_LIMIT = 60*5;
const int PLAY_LIMIT = 20;

struct Ship {
    int x, y;
    int length;
    bool horizontal;
};

void TaoBang(vector<vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        vector<char> row(BOARD_SIZE, '~');
        board.push_back(row);
    }
}
bool TaoBang(vector<vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        vector<char> row(BOARD_SIZE, '~');
        board.push_back(row);
    }
}
bool ViTriHopLe(const vector<vector<char>>& board, const Ship& ship) {
    if (ship.horizontal) {
        if (ship.y + ship.length > BOARD_SIZE) {
            return false;
        }
        for (int i = 0; i < ship.length; ++i) {
            if (board[ship.x][ship.y + i] != '~') {
                return false;
            }
        }
    } else {
        if (ship.x + ship.length > BOARD_SIZE) {
            return false;
        }
        for (int i = 0; i < ship.length; ++i) {
            if (board[ship.x + i][ship.y] != '~') {
                return false;
            }
        }
    }
    return true;
}
void Dat_vi_tri_thuyen(vector<vector<char>>& board, const Ship& ship) {
    if (ship.horizontal) {
        for (int i = 0; i < ship.length; ++i) {
            board[ship.x][ship.y + i] = '~';
        }
    } else {
        for (int i = 0; i < ship.length; ++i) {
            board[ship.x + i][ship.y] = '~';
        }
    }
}

void placeShips(vector<Ship>& ships, vector<vector<char>>& board) {
    srand(time(0));
    for (int i = 0; i < NUM_SHIPS; ++i) {
        Ship ship;
        ship.length = rand() % 2 + 2; 
        ship.horizontal = rand() % 2 == 0;

        
        do {
            ship.x = rand() % BOARD_SIZE;
            ship.y = rand() % BOARD_SIZE;
        } while (!ViTriHopLe(board, ship));

        Dat_vi_tri_thuyen(board, ship);
        ships.push_back(ship);
    }
}

bool isHit(const vector<Ship>& ships, int x, int y) {
    for (const auto& ship : ships) {
        if (ship.horizontal) {
            if (ship.y == y && x >= ship.x && x < ship.x + ship.length) {
                return true;
            }
        } else {
            if (ship.x == x && y >= ship.y && y < ship.y + ship.length) {
                return true;
            }
        }
    }
    return false;
}

void inBang(const vector<vector<char>>& board, bool showShips = false) {
    for (const auto& row : board) {
        for (const auto& cell : row) {
           if (cell == 'S' && !showShips) {
                cout << "~ ";
            } else {
                cout << cell << " ";
            }
        }
        cout << endl;
    }
}

bool DemThoiGian(high_resolution_clock::time_point start_time){
    auto current_time = higgh_resolution_clock::now();
    auto duration = duration_cast<seconds>(current_time - start_time).count()
    return duration >= TIME_LIMIT;

bool GioiHanLuotChoi(int so_lan_thu) {
    return so_lan_thu >= PLAY_LIMIT;
}

int main() {
    vector<vector<char>> board;
    vector<Ship> ships;

    TaoBang(board);
    placeShips(ships, board);

    int attempts = 0;
    int hits = 0;

    auto start_time = high_resolution_clock::now();

    while (hits < NUM_SHIPS) {
        
        int x, y;
        cout << "Chọn tọa độ (x y): ";
        cin >> x >> y;

        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
            cout << "Không tồn tại. Hãy thử lại!" << endl;
            continue;
        }

        if (board[x][y] == 'X' || board[x][y] == 'O') {
            cout << "Tọa độ đã được đánh dấu. Hãy thử lại!" << endl;
            continue;
        }

        attempts++;

        if (isHit(ships, x, y)) {
            cout << "Đã trúng!" << endl;
            board[x][y] = 'X';
            hits++;
        } else {
            cout << "Trượt." << endl;
            board[x][y] = 'O';
        }

         if (DemThoiGian(start_time)){
            cout << "Hết thời gian! Bạn đã thua." << endl;
             intbang(board, true);
            return 0;
         }

        if (GioiHanLuotChoi(attempts)) {
            cout << "Hết lượt chơi! Bạn đã thua." << endl;
            inbang(board, true); 
            return 0;
        }
        inBang(board);
    }

    cout << "Chúc mừng bạn đã chiến thắng!" << endl;
    intbang(board, true);

    return 0;
}
