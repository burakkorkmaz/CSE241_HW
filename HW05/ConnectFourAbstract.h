//
// Created by Burak Kağan Korkmaz on 23.11.2017.
//

#ifndef HW05_CONNECTFOURABSTRACT_H
#define HW05_CONNECTFOURABSTRACT_H

#include <iostream>
#include <string>
#include <limits>   // for numeric_limits, to use invalid checking
#include <fstream>
#include "Cell.h"

using namespace std;

class ConnectFourAbstract {
public:
    //Named constants
    static const int STD_SIZE = 5;
    static const int FOUR_IN_ROW = 4;
    static const int WRONG = -1;
    const char PLAYER = 'P';
    const char COMPUTER = 'C';
/*
    ConnectFourAbstract() : height(STD_SIZE), width(STD_SIZE){
        createBoard();
    }
    ConnectFourAbstract(const string & fileName) {
        createBoard();
        loadGame(fileName);
    }*/

    virtual bool playGame();

    virtual bool play();

    virtual bool play(const string& position);

    inline int getWidth() const{ return width;}

    inline  int getHeight() const{ return height;}

protected:
    void createBoard();

    void printBoard();

    void initializeBoard(char c = '.');

    virtual int saveGame(const string & fileName) const;

    virtual int loadGame(const string & fileName);

    virtual bool checkWin(int col, int row) = 0;

    bool checkWinVertical(int column, int row);

    bool checkWinHorizontal(int column, int row);

    bool checkWinDiagonalL(int column, int row);
    bool checkWinDiagonalR(int column, int row);

    Cell **gameCells = nullptr;
    int width;
    int height;
    int boardSize = height * width;
private:
    int moveLeft = boardSize;
    bool loaded = false;
    char playerOrComp = COMPUTER;
    bool blockCheck = false;
    char userSymbol = 'X';
    string selectedColumn;

    bool isFinished = false;

    bool isDraw = false;

//Helper Functions
    bool errorOccurred = false;

    bool playerGame();

    bool computerGame();

    void resizeBoard();

    void removeBoard(bool firstRemove);

    int makeMove(char columnPos, char userChar);

    void switchTurn(char &userChar);

    void markWinState(int currentRow, int currentCol);

    int blockWinState(char target);

    //bool getBoard();
};


#endif //HW05_CONNECTFOURABSTRACT_H
