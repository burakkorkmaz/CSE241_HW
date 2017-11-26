//
// Created by Burak Kağan Korkmaz on 16.10.2017.
//

#ifndef HW03_CONNECTFOUR_H
#define HW03_CONNECTFOUR_H

#include <vector>
#include <limits>
#include <fstream>



using namespace std;

static int livingCell = 0;
static int finishedCount = 0;

class ConnectFour {
public:
    //Named constants
    static int const FOUR_IN_ROW = 4;
    static int const WRONG = -1;
    static int const ONE_STEP = 2;
    char const PLAYER = 'P';
    char const COMPUTER = 'C';
    char const MULTI = 'M';
    char const SINGLE = 'S';

    //Constructors
    ConnectFour(){
//        cout << "Connect Four 1\n";
        this->height = FOUR_IN_ROW;
        this->width = FOUR_IN_ROW;
        this->playerOrComp = PLAYER;
        this->gameMode = SINGLE;
    }
    ConnectFour(char playerComp, char gameMode){
//        cout << "Connect Four 2\n";
        this->height = FOUR_IN_ROW;
        this->width = FOUR_IN_ROW;
        this->playerOrComp = playerComp;
        this->gameMode = gameMode;

    }
    ConnectFour(int height, int width) : height(height), width(width) {
        this->gameMode = SINGLE;
        playerOrComp = COMPUTER;
    }

    ConnectFour(int height, int width, char pvpOrComp){
//        cout << "Connect Four 4\n";
        this->height = height;
        this->width = width;
        this->playerOrComp = pvpOrComp;
        this->gameMode = SINGLE;
    }
    ConnectFour(int height, int width, char pvpOrComp, char gameMode){
        this->height = height;
        this->width = width;
        this->playerOrComp = pvpOrComp;
        this->gameMode = gameMode;
    }
    ConnectFour(string fileName){
        loadGame(fileName);
    }

    //Getters & Setters
    inline int getHeight() const{ return height;}
    inline int getWidth() const{ return width;}
    inline char getGameMode() const{ return gameMode;}
    inline void setHeight(int boardHeight) { height = boardHeight;}
    inline void setWidth(int boardWidth) { width = boardWidth;}
    inline void setGameMode(char gameMode){this->gameMode = gameMode;}
    inline void setMultiCtrl(bool ctrl){multiControl = ctrl;}

    void resizeBoard();
    void initializeBoard();
    void printBoard();
    static void printLivingCells();
    static int finishedObjects(){ return  finishedCount;}

    bool playGame();
    bool play();
    bool play(string position);
    bool compareGames(const ConnectFour & other);

    inline int singleLivingCell()const{ return (boardSize - moveLeft);}

private:
//PRIVATE INNER CELL CLASS
    class Cell {
    public:

        //Constructors
        Cell() : symbol('.'){
//            cout << "Cell default\n";
        }

        Cell(char position, int row) : position(position), row(row) {}

        Cell(int position,int row):row(row){
            this->position = static_cast<char >(position + 'A');}


        Cell(int position, int row, char symbol) : row(row), symbol(symbol){
            this->position = static_cast<char >(position + 'A'); }

        //Getter & Setter
        char getPosition() const{ return position;}

        int getRow() const{ return row;}

        char getSymbol() const { return this->symbol;}

        void setPosition(char position){ this->position = position;}

        void setPosition(int position){
            this->position = static_cast<char>(position + 'A');}

        void setRow(int row){ this->row = row; }


        void setSymbol(char symbol){ this->symbol = symbol;}
    private:
        char position;
        int row; // Start from zero
        char symbol;

    };// END OF CELL CLASS
    vector<vector< Cell > > gameCells;
    int height;
    int width;

    int boardSize;
    int moveLeft;
    bool loaded;
    char gameMode;
    bool blockCheck;

    char userSymbol;
    string selectedColumn;
    bool isFinished = false;
    bool isDraw = false;
    bool errorOccurred;


    bool multiControl = false;

    int moveCounter; // counts moves for multiObjects ;

    char playerOrComp;

    inline static void increaseLivingCell(){ ++livingCell; }

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
};


#endif //HW03_CONNECTFOUR_H
