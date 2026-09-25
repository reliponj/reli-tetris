#include <termios.h>
#include <unistd.h>

enum class Key {
    None,
    Left,
    Right,
    Down,
    Quit
};

class TerminalInput {
    termios original{};

public:
    TerminalInput() {
        tcgetattr(STDIN_FILENO, &original);

        termios raw = original;
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;

        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }

    ~TerminalInput() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original);
    }

    Key readKey() {
        char input[3]{};
        const ssize_t count = read(STDIN_FILENO, input, sizeof(input));

        if (count == 1 && input[0] == 'q') {
            return Key::Quit;
        }

        if (count == 3 && input[0] == '\033' && input[1] == '[') {
            if (input[2] == 'B') return Key::Down;
            if (input[2] == 'D') return Key::Left;
            if (input[2] == 'C') return Key::Right;
        }

        return Key::None;
    }
};