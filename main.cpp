#include <iostream>
#include <chrono>
#include <thread>
#include "components/block.cpp"
#include "utils/random.cpp"


const int TILE_SIZE = 5;
const int RES_WIDTH = 14;
const int RES_HEIGHT = 28;
const std::vector<Color> COLORS = {
    Color::Red,
    Color::Orange,
    Color::Yellow,
    Color::Green,
    Color::Cyan,
    Color::Blue,
    Color::Purple,
};



class Figure {
    std::vector<Block> blocks;
    int offsetX, offsetY;
public:
    Figure() {}

    Figure(int x, int y) {
        int randomColor = randomInt(0, 6);
        Color color = COLORS[randomColor];
        blocks = {
            Block(x, y, color),
            Block(x, y-1, color),
            Block(x+1, y-1, color),
            Block(x+1, y, color),
        };
        offsetX = 0;
        offsetY = 0;
        processTiles();
    }
    
    void processTiles() {
        for (Block block : blocks) {
            block.setCoords(block.x() + offsetX, block.y() + offsetY);
            offsetX = 0;
            offsetY = 0;
        }
    }

    void gravity() {
        offsetY++;
        processTiles();
    }
};


class Field {
private:
    int coords[RES_HEIGHT][RES_WIDTH];

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

    void processField(Block block) {
        const auto& blockCoords = block.getBlockCoords();
        for (int i = 0; i<2; i++) {
            for (int j = 0; j<2; j++) {
                int x = blockCoords[i][j][0];
                int y = blockCoords[i][j][1];
                coords[y][x] = 1;
            }
        }
    }
};


class Game {
    Block currentBlock;
    Field field;

public:
    Game() {
        field = Field();
        currentBlock = Block(13, -1);
    }

    void process() {
        while (true) {
            std::system("clear");
            field.printField();
            std::this_thread::sleep_for(std::chrono::seconds(1));

            currentBlock.gravity();
            field.processField(currentBlock);
        }
    }
};


int main() {
    Game game = Game();
    game.process();
    return 0;
}
