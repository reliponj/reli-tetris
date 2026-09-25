#include <iostream>
#include <chrono>
#include <thread>
#include <optional>
#include <array>

#include "components/block.cpp"
#include "utils/random.cpp"
#include "utils/input.cpp"


const int TILE_SIZE = 5;
const int RES_WIDTH = 14;
const int RES_HEIGHT = 18;
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
    std::vector<std::vector<int>> currentCoords;
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
        currentCoords.clear();
        for (Block &block : blocks) {
            block.setCoords(block.x() + offsetX, block.y() + offsetY);
            currentCoords.push_back({block.x(), block.y()});
        }
        offsetX = 0;
        offsetY = 0;
    }

    void gravity() {
        offsetY++;
        processTiles();
    }

    const std::vector<Block> getBlocks() {
        return blocks;
    }

    bool containsCoords(int x, int y) {
        for (std::vector coords : currentCoords) {
            if (coords[0] == x && coords[1] == y) {
                return true;
            }
        }
        return false;
    }

    void moveLeft() {
        offsetX--;
        processTiles();
    }

    void moveRight() {
        offsetX++;
        processTiles();
    }
};


class Field {
private:
    std::array<std::array<std::optional<Block>, RES_WIDTH>, RES_HEIGHT> blocks;
    
public:
    Field() {}

    bool moveBlocks(std::vector<Block> figureBlocks) {
        int maxY = -100;
        std::vector<Block> filteredBlocks;
        for (Block block : figureBlocks) {
            if (block.y() > maxY) {
                maxY = block.y();
                filteredBlocks.clear();
                filteredBlocks.push_back(block);
            } else if (block.y() == maxY) {
                filteredBlocks.push_back(block);
            }
        }

        bool grounded = false;
        for (Block block : filteredBlocks) {
            if (blocks[block.y()+1][block.x()].has_value() || block.y()+1 == RES_HEIGHT) {
                grounded = true;
                break;
            }
        }

        if (grounded) {
            for (Block block : figureBlocks) {
                blocks[block.y()][block.x()].emplace(block);
            }
        }
        return grounded;
    }

    void printField(Figure figure) {
        for (int j=0; j<RES_WIDTH * 5 +1; j++) {
            std::cout << "--";
        }
        std::cout << std::endl;


        for (int i=0; i<RES_HEIGHT; i++) {
            for (int b1=0; b1<TILE_SIZE; b1++) {
                std::cout << "| ";

                for (int j=0; j<RES_WIDTH; j++) {
                    for (int b2=0; b2<TILE_SIZE; b2++) {
                        if (blocks[i][j].has_value() || figure.containsCoords(j, i)) {
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


class Game {
    Figure currentFigure;
    Field field;

public:
    Game() {
        field = Field();
        spawnFigure();
    }

    void spawnFigure() {
        currentFigure = Figure(6, -1);
    }

    void process() {
        using Clock = std::chrono::steady_clock;

        TerminalInput input;

        auto nextInputCheck = Clock::now();
        auto nextGravityUpdate = Clock::now() + std::chrono::seconds(1);

        while (true) {
            auto now = Clock::now();

            if (now >= nextInputCheck) {
                std::system("clear");
                processInput(input);
                nextInputCheck = now + std::chrono::milliseconds(50);
                field.printField(currentFigure);
            }

            if (now >= nextGravityUpdate) {
                bool isGrounded = field.moveBlocks(currentFigure.getBlocks());
                if (isGrounded) {
                    spawnFigure();
                } else {
                    currentFigure.gravity();
                }
                nextGravityUpdate = now + std::chrono::seconds(1);
            }

            std::this_thread::sleep_until(std::min(nextInputCheck, nextGravityUpdate));
        }
    }

    void processInput(TerminalInput& input) {
        switch (input.readKey()) {
            case Key::Left:
                currentFigure.moveLeft();
                break;
            case Key::Right:
                currentFigure.moveRight();
                break;
            case Key::Down:
                currentFigure.gravity();
                break;
            default:
                return;
        }
    }
};


int main() {
    Game game = Game();
    game.process();
    return 0;
}
