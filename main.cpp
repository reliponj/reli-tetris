#include <iostream>
#include <chrono>
#include <thread>


const int TILE_SIZE = 5;
const int RES_WIDTH = 14;
const int RES_HEIGHT = 28;


class Field {
private:
    int** coords[RES_HEIGHT][RES_WIDTH];

public:
    Field() {
        for (int i=0; i<RES_HEIGHT; i++) {
            for (int j=0; j<RES_WIDTH; j++) {
                coords[i][j] = 0;
            }
        }
    }

    void printField() {
        for (int j=0; j<RES_WIDTH * 5 +1; j++) {
            std::cout << "--";
        }
        std::cout << std::endl;


        for (int i=0; i<RES_HEIGHT; i++) {
            for (int b1=0; b1<TILE_SIZE; b1++) {
                std::cout << "| ";

                for (int j=0; j<RES_WIDTH; j++) {
                    for (int b2=0; b2<TILE_SIZE; b2++) {
                        if (coords[i][j]) {
                            std::cout << "# ";
                        } else {
                            std::cout << "  ";
                        }
                    }
                }

                std::cout << "|";
                std::cout << std::endl;
            }
        }

        for (int j=0; j<RES_WIDTH * 5 + 1; j++) {
            std::cout << "--";
        }
        std::cout << std::endl;
    }
};


class Block {
    int** block[2][2];
    int _x, _y;

public:
    Block() {}

    Block(int x, int y) {
        for (int i=0; i<2; i++) {
            for (int j=0; j<2; j++) {
                block[i][j] = 0;
            }
        }
        _x = x;
        _y = y;
    }
};


class Game {
    Block currentBlock;
    Field field;

public:
    Game() {
        field = Field();
        currentBlock = Block(13, 28);
    }

    void process() {
        while (true) {
            std::system("clear");
            field.printField();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};


int main() {
    Game game = Game();
    game.process();
    return 0;
}
