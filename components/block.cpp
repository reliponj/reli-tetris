enum class Color {
    Red,
    Orange,
    Yellow,
    Green,
    Cyan,
    Blue,
    Purple,
};


class Block {
    int _x, _y;
    Color _color;
public:
    Block(int x, int y, Color color) {
        setCoords(x, y);
        _color = color;
    }

    int x() {
        return _x;
    }
    
    int y() {
        return _y;
    }

    void setCoords(int x, int y) {
        _x = x;
        _y = y;
    }
};