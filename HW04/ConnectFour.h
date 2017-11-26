//
// Created by Burak Kağan Korkmaz on 16.10.2017.
//

#ifndef HW04_CONNECTFOUR_H
#define HW04_CONNECTFOUR_H

#include <vector>
#include <limits>
#include <fstream>


using namespace std;

static int livingCell = 0;
static int finishedCount = 0;

class ConnectFour {
public:
    //Named constants
    static const int STD_SIZE = 5;
    static const int FOUR_IN_ROW = 4;
    static const int WRONG = -1;
    static const int ONE_STEP = 2;
    const char PLAYER = 'P';
    const char COMPUTER = 'C';
    const char MULTI = 'M';
    const char SINGLE = 'S';

    //Constructors

    // no parameter constructor creates a 5x5 game
    ConnectFour() : height(STD_SIZE), width(STD_SIZE){
//        cout << "Connect Four 1\n";
        this->playerOrComp = PLAYER;
        this->gameMode = SINGLE;
        createBoard();
    }

    ConnectFour(char playerComp, char gameMode) : playerOrComp(playerComp), gameMode(gameMode) {
//        cout << "Connect Four 2\n";
        this->height = FOUR_IN_ROW;
        this->width = FOUR_IN_ROW;
        createBoard();
    }

    ConnectFour(int height, int width) : height(height), width(width) {
        this->gameMode = SINGLE;
        playerOrComp = COMPUTER;
        createBoard();
    }

    ConnectFour(int height, int width, char pvpOrComp) {
//        cout << "Connect Four 4\n";
        this->height = height;
        this->width = width;
        this->playerOrComp = pvpOrComp;
        this->gameMode = SINGLE;
        createBoard();
    }

    ConnectFour(int height, int width, char pvpOrComp, char gameMode) {
        this->height = height;
        this->width = width;
        this->playerOrComp = pvpOrComp;
        this->gameMode = gameMode;
        createBoard();
    }

    ConnectFour(string fileName) {
        loadGame(fileName);
    }

    //Getters & Setters
    inline int getHeight() const { return height; }

    inline int getWidth() const { return width; }

    inline char getGameMode() const { return gameMode; }

    inline void setHeight(int boardHeight) { height = boardHeight; }

    inline void setWidth(int boardWidth) { width = boardWidth; }

    inline void setGameMode(char gameMode) { this->gameMode = gameMode; }

    inline void setMultiCtrl(bool ctrl) { multiControl = ctrl; }

    //Other Functions

    bool operator ==(const ConnectFour& other) const;

    bool operator!=(const ConnectFour &other) const;

    void initializeBoard(char c = '.');

    void printBoard();

    static void printLivingCells();

    static int finishedObjects() { return finishedCount; }

    bool playGame();

    bool play();

    bool play(string position);

    bool compareGames(const ConnectFour &other);

    inline int singleLivingCell() const { return (boardSize - moveLeft); }

private:
//PRIVATE INNER CELL CLASS
    class Cell {
    public:

        //Constructors
        Cell() : symbol('.') {
//            cout << "Cell default\n";
        }

        Cell(char position, int row) : position(position), row(row) {}

        Cell(int position, int row) : row(row) {
            this->position = static_cast<char >(position + 'A');
        }


        Cell(int position, int row, char symbol) : row(row), symbol(symbol) {
            this->position = static_cast<char >(position + 'A');
        }

        friend ostream& operator <<(ostream& os, const Cell& cell){
            os << "Cell\nPosition: " << cell.getPosition() << "Row: " << cell.getRow()
               << "\nSymbol: " << cell.getSymbol() << endl;
            return os;
        }
        friend istream& operator >>(istream& is, Cell& cell){
            char pos, symbol;
            int row;
            is >> pos >> row >> symbol;
            cell.setPosition(pos);
            cell.setRow(row);
            cell.setSymbol(symbol);
            return is;
        }

        bool operator ==(const Cell& cell1) const {
            return (cell1.getPosition() == getPosition() ||
            cell1.getRow() == getRow() ||
            cell1.getSymbol() == getSymbol());
        }

        bool operator !=(const Cell& cell1) const{
            return !operator==(cell1);
        }

        Cell operator++(){  //Pre-fix

            switch (getSymbol()){
                case 'X': setSymbol('W');   // user1 -> user2
                    break;
                case 'W': setSymbol('Y');   // user2 -> computer
                    break;
                case 'Y': setSymbol('Z');   // computer -> empty
                    break;
                case 'Z': setSymbol('X');   // empty -> user1
                    break;
                default: setSymbol('X');
            }
            return *this;
        }

        Cell operator++(int ignore){// Post-fix
            Cell c = *this;
            switch (getSymbol()){
                case 'X': setSymbol('W');   // user1 -> user2
                    break;
                case 'W': setSymbol('Y');   // user2 -> computer
                    break;
                case 'Y': setSymbol('Z');   // computer -> empty
                    break;
                case 'Z': setSymbol('X');   // empty -> user1
                    break;
                default: setSymbol('X');
            }
            return c;
        }

        //Getter & Setter
        char getPosition() const { return position; }

        int getRow() const { return row; }

        char getSymbol() const { return this->symbol; }

        void setPosition(char position) { this->position = position; }

        void setPosition(int position) {
            this->position = static_cast<char>(position + 'A');
        }

        void setRow(int row) { this->row = row; }

        void setSymbol(char symbol) { this->symbol = symbol; }

    private:
        char position;
        int row; // Start from zero
        char symbol;

    };// END OF CELL CLASS

    Cell **gameCells;
    int height;
    int width;

    int boardSize = height * width;
    int moveLeft = boardSize;
    bool loaded = false;
    char playerOrComp = 'C';
    char gameMode = 'S';

    bool blockCheck = false;
    char userSymbol = 'X';
    string selectedColumn;
    bool isFinished = false;
    bool isDraw = false;


    bool errorOccurred = false;

    bool multiControl = false;

    int moveCounter = 0; // counts moves for multiObjects ;

    void createBoard();

    inline static void increaseLivingCell() { ++livingCell; }

    int makeMove(char columnPos, char userChar);

    void switchTurn(char &userChar);

    int saveGame(string fileName);

    int loadGame(string fileName);

    void markWinState(int currentRow, int currentCol);

    bool checkWin(int col, int row);

    bool checkWinVertical(int column, int row);

    bool checkWinHorizontal(int column, int row);

    bool checkWinDiagonalL(int column, int row);

    bool checkWinDiagonalR(int column, int row);

    int blockWinState(char target);

    void resizeBoard();

    void removeBoard(bool firstRemove);

    bool getBoard();

    bool playerGame();

    bool computerGame();
};


#endif //HW04_CONNECTFOUR_H
