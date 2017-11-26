//
// Created by Burak Kağan Korkmaz on 16.10.2017.

#include <iostream>
#include "ConnectFour.h"
using namespace std;

bool ConnectFour::playGame() {

    playerOrComp;

    if (gameMode == SINGLE || !multiControl) {
        moveCounter = 0;
        userSymbol = 'X';
//        resizeBoard();

        // if not read from file , creates 5*5 board
        if(!getBoard()){
            initializeBoard();
           moveLeft = width * height;
        }
        do {
            cout << "Do you want to play with [C]omputer or [P]layer : " << endl;
            cin.clear();
            cin >> playerOrComp;
        } while (playerOrComp != COMPUTER && playerOrComp != PLAYER);


        loaded = false;
    }
    if (gameMode == SINGLE || multiControl) {
        bool quit = false;
        do {

            if (playerOrComp == PLAYER ) {

                quit = playerGame();

            }
                // player vs. computer game
            else if (playerOrComp == COMPUTER ) {

                quit = computerGame();

            }
        } while (!isFinished && !quit);
    }


    return false;
}

// Does required controls for PVP game
bool ConnectFour::playerGame() {

    bool quit = false;
    moveCounter = 0;
    loaded = false;
    if (isFinished) {
//                    cout << "pfinCon:"<< finishedCount<< endl;
        cout << "The game is already finished!" << endl;
        if (!isDraw) {

            if (userSymbol == 'X') {
                cout << "USER1 won!" << endl;
            } else {
                cout << "USER2 won!" << endl;
            }
        } else{
            cout << "Draw!\n";
        }

    }
    // PVP GAME LOOP
    while (!isFinished && moveLeft > 0 && !loaded) {

        printBoard();
        printLivingCells();
        if(gameMode == MULTI &&  moveCounter == ONE_STEP){
            quit = true;
            break;
        }

        if (userSymbol == 'X') {
            cout << "Please make a move USER1:" << endl;
        } else {
            cout << "Please make a move USER2:" << endl;
        }
        do {
            getline(cin, selectedColumn);
            if(cin.eof()){
                cerr << "END OF FILE!\n";
                exit(0);
            }
        } while (selectedColumn.empty());

        play(selectedColumn);
    }

    return quit;
}

// Does required controls for computer game
bool ConnectFour::computerGame() {
    bool quit = false;
    moveCounter = 0;
    loaded = false;
    if (isFinished) {
//                    cout << "cfinCon:"<< finishedCount<< endl;
        cout << "The game is already finished!" << endl;
        if (!isDraw) {
            if (userSymbol == 'X') {
                cout << "USER1 won!" << endl;
            } else {
                cout << "COMPUTER won!" << endl;
            }
        } else{
            cout << "Draw!\n";
        }

    }

    //PVC LOOP
    while (moveLeft > 0 && !isFinished &&
           !loaded ) {

        selectedColumn = "";

        if (userSymbol == 'X') {  // User 1 - PLAYER
            printBoard();
            printLivingCells();

            cout << "Please make a move USER1 -> ";

            do {
                getline(cin, selectedColumn);
            } while (selectedColumn.empty());

            play(selectedColumn);

        } else {    // User 2 - COMPUTER

            play();
        }
        if(gameMode == MULTI &&  moveCounter == ONE_STEP){
            printBoard();
            printLivingCells();
            quit = true;
            break;
        }
    }
    return quit;
}

bool ConnectFour::play() {

    int currentRow = 0;
    int currentCol = 0;

    printBoard();
    printLivingCells();
    cout << "USER2(Computer) making a move..." << endl;
    // blocks next move of user if user will win
    int row = blockWinState('X');

    if (row != WRONG) {
        selectedColumn[0] = (char) row + 'A';
    } else {
        // if next move of comp. will win, move this cell
        row = blockWinState('O');

        if (row != WRONG) {
            selectedColumn[0] = (char) row + 'A';

        } else {
            // computer searchs valid column to move
            int col = 0;
            int rowTemp = height - 1;
            char c;
            while ((c = gameCells[rowTemp][col].getSymbol()) != '.') {
                if(c == ' '){
                    rowTemp = height - 1;
                    ++col;
                }
                else if(--rowTemp < 0){
                    ++col;
                    rowTemp = height - 1;
                }
            }
            selectedColumn[0] = (char) col + 'A';
        }
    }
    //column is selected, makes move
    cout << selectedColumn[0] << endl;
    currentCol = selectedColumn[0] - 'A';
    currentRow = makeMove(selectedColumn[0], userSymbol);

    if (currentRow != WRONG) {
        --moveLeft;
        increaseLivingCell();
        if (checkWin(currentCol, currentRow)) {

            printBoard();
            if (userSymbol == 'O')
                cout << "USER2(Computer) win!\n";
            isFinished = true;
            if(gameMode == 'M')
                ++finishedCount;
            return isFinished;
        }
        if (moveLeft == 0) {
            printBoard();
            cout << "Game Over! It's draw." << endl;
            isFinished = true;
            isDraw = true;
            if(gameMode == 'M')
                ++finishedCount;
        }
        if (!isFinished){
            switchTurn(userSymbol);
        }
    }
}

bool ConnectFour::play(string selectedColumn) {
    errorOccurred = false;
    if (selectedColumn.size() == 1) {

        decltype(selectedColumn.c_str()[0]) column
                = selectedColumn.c_str()[0];

        int currentCol = column - 'A';
        int currentRow = makeMove(column, userSymbol);

        // if move is valid
        if (currentRow != WRONG) {
            --moveLeft;
            increaseLivingCell();
            if (checkWin(currentCol, currentRow)) {
                printBoard();
                if (userSymbol == 'X')
                    cout << "USER1 win!\n";
                else
                    cout << "USER2 win!\n";
                isFinished = true;
                if(gameMode == 'M')
                    ++finishedCount;
                return isFinished;
            }

            //if board is full
            if (moveLeft == 0) {
                printBoard();
                cout << "Game Over! It's draw." << endl;
                isFinished = true;
                if(gameMode == 'M')
                    ++finishedCount;
            }

        } else {
            errorOccurred = true;
        }
    } else if (selectedColumn.size() > 1) { // SAVE & LOAD COMMAND

        auto size = selectedColumn.size();
        auto position = selectedColumn.find(' ');
        if (position == string::npos) {
            cerr << "Invalid Command!\n";
            errorOccurred = true;
            return false;
        }
        string command = selectedColumn.substr(0, position);
        string filename = selectedColumn.substr(position, size);

        if (command == "SAVE") {
            if (saveGame(filename) != WRONG) {

                cout << "SAVE SUCCESS!" << endl;
            } else {
                cerr << "SAVE FAILED!" << endl;
                errorOccurred = true;
            }
        } else if (command == "LOAD") {
            if (loadGame(filename) != WRONG) {
                loaded = true;
                cout << "LOAD SUCCESS!" << endl;
            } else {
                cerr << "LOAD FAILED!" << endl;
                errorOccurred = true;
            }
//            continue;
        } else {
            cerr << "Invalid Command!!\n";
            errorOccurred = true;
        }
        return false;

    }
    if (!isFinished && !errorOccurred){
        switchTurn(userSymbol);

    }

}

/*
 * Resizes the vector and fills cells. DEFAULT: '.'
 */
void ConnectFour::initializeBoard(char c) {

//    cout << "w: " << width << "h: " << height << endl;

    if(getHeight() != STD_SIZE || getWidth() != STD_SIZE) {
        removeBoard(true);
        createBoard();
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            gameCells[i][j].setRow(i);
            gameCells[i][j].setPosition(j);
            gameCells[i][j].setSymbol(c);
        }
    }
}

/*
 * Prints game board from vector
 */
void ConnectFour::printBoard() {
    boardSize = width * height;

    for (char col = 'A'; col - 'A' < width; ++col) {
        cout << col << " ";
    }
    cout << endl;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cout << gameCells[i][j].getSymbol() << " ";
        }
        cout << endl;
    }
}

/*
 * Makes move user character to the given position.
 * Returns row position of current move, otherwise -1
 * */
int ConnectFour::makeMove(char columnPos, char userChar) {
    int row = 0;
    bool move = false;
    int index = columnPos - 'A';

    int i = height - 1;
//    cout << "Row: " << i << "Col: " << index << endl;
    if (index >= width || index < 0) {
        if (!blockCheck)
            cerr << "Wrong Column!" << endl;
        errorOccurred = true;
        return WRONG;
    }
    while (i >= 0 && !move) {
        if (gameCells[i][index].getSymbol() == '.') {
            gameCells[i][index].setSymbol(userChar);
            row = i;
            move = true;
        } else {
            --i;
        }
    }
    if (i < 0) {
        if (!blockCheck)
            cerr << "Invalid Move!\n";
        errorOccurred = true;
        row = WRONG;
    }

    return row;
}

void ConnectFour::switchTurn(char &userChar) {

    if (userChar == 'X')
        userChar = 'O';
    else if (userChar == 'O')
        userChar = 'X';
    if(gameMode == 'M')
        ++moveCounter;
}


/* * * * * * * * * * * * * *
 *                          *
 *      FILE  FORMAT        *
 *                          *
 *                          *
 *                          *
 *     (PVP - COMP)         *
 *      BOARD WIDTH         *
 *      BOARD HEIGHT        *
 *     CURRENT USER         *
 *      MOVE LEFT           *
 *   /----------------\     *
 *   |     BOARD      |     *
 *   |                |     *
 *   |      ...       |     *
 *   |                |     *
 *   |                |     *
 *   \----------------/     *
 *                          *
 * * * * * * * * * * * * * */

int ConnectFour::saveGame(string fileName) {

    cout << fileName << endl;
    ofstream fileStream(fileName);
    if (fileStream.is_open()) {
        fileStream  /*<< gameMode << endl*/
                    << playerOrComp << endl
                    << width << endl
                    << height << endl
                    << userSymbol << endl
                    << moveLeft << endl
                    << livingCell << endl;
        for (int i = 0; i < getHeight(); ++i) {
            for (int j = 0; j < getWidth(); ++j) {
                fileStream << gameCells[i][j].getSymbol();
            }
            fileStream << endl;
        }
    }
    else{
        cerr << "File Error!\n";
        return WRONG;
    }
    fileStream.close();

    return 0;
}

int ConnectFour::loadGame(string fileName) {

    int temp = livingCell;
    int thisLiving = singleLivingCell();
    int tempLiving;
    ifstream fileStream(fileName);
    if (fileStream.is_open()) {
        fileStream /*>> gameMode*/
                   >> playerOrComp
                   >> width
                   >> height
                   >> userSymbol
                   >> moveLeft
                   >> livingCell;
        if(gameMode == 'M'){
            livingCell += (temp - thisLiving);

        }

//        initializeBoard(' ');
        string temp;
        for (int i = 0; i < getHeight(); ++i) {
            getline(fileStream,temp);
            cout << temp << "|\n";
            if(temp.empty())
                getline(fileStream,temp);

            for (int j = 0; j < getWidth(); ++j) {
                gameCells[i][j].setSymbol(temp[j]);
            }
//            cout << endl;
        }
    }
    else{
        cerr << "File Error!\n";
        return WRONG;
    }
    fileStream.close();

    return 0;
}

bool ConnectFour::checkWin(int column, int row) {

    return (checkWinVertical(column, row) ||
            checkWinHorizontal(column, row) ||
            checkWinDiagonalL(column, row) ||
            checkWinDiagonalR(column, row));
}

bool ConnectFour::checkWinVertical(int column, int row) {
    bool win = false;
    if (row + 3 >= height || column >= width) {
        return win;
    }
    char user = gameCells[row][column].getSymbol();

    if (user == gameCells[row + 1][column].getSymbol() &&
        user == gameCells[row + 2][column].getSymbol() &&
        user == gameCells[row + 3][column].getSymbol()) {
        win = true;

        if (!blockCheck) {
            for (int i = row, j = column, k = FOUR_IN_ROW; k > 0; ++i, --k) {
                markWinState(i, j);
            }
        }
    }
    return win;
}

bool ConnectFour::checkWinHorizontal(int column, int row) {
    bool win = false;
    if (column < 0 || column > width - 1) {
        return false;
    }
    char user = gameCells[row][column].getSymbol();
    int i = row;
    int j = column;

    while (column != 0 && j != 0) {
        --j;
        if (gameCells[i][j].getSymbol() == user) {
            --column;
        } else {
            break;
        }
    }
    j = column;
    int counter = 1;
    while (!win && user == gameCells[row][column].getSymbol()) {
        if (column > width - 1) {
            break;
        }
        ++column;
        if (gameCells[row][column].getSymbol() == user) {
            ++counter;
        }
        if (counter == FOUR_IN_ROW) {
            win = true;
            if (!blockCheck) {
                for (int lcv1 = i, lcv2 = j, k = FOUR_IN_ROW; k > 0; ++lcv2, --k) {
                    markWinState(lcv1, lcv2);
                }
            }
        }
    }
    return win;
}

bool ConnectFour::checkWinDiagonalL(int column, int row) {
    bool win = false;
    if (column < 0 || row < 0 || column > width - 1 || row > height - 1) {
        return false;
    }
    char user = gameCells[row][column].getSymbol();
    int i = row;
    int j = column;
    while (i != 0 && j != 0) {
        --i;
        --j;
        if (gameCells[i][j].getSymbol() == user) {
            --column;
            --row;
        } else {
            break;
        }
    }
    i = row;
    j = column;
    int counter = 1;
    while (!win && user == gameCells[row][column].getSymbol()) {
        if (row >= height - 1 || column >= width - 1) {
            break;
        }
        ++column;
        ++row;
        if (gameCells[row][column].getSymbol() == user) {
            ++counter;
        }
        if (counter == FOUR_IN_ROW) {
            win = true;
            if (!blockCheck) {
                for (int lcv1 = i, lcv2 = j, k = FOUR_IN_ROW;
                     k > 0;
                     ++lcv1, ++lcv2, --k) {

                    markWinState(lcv1, lcv2);
                }
            }
        }
    }
    return win;
}

bool ConnectFour::checkWinDiagonalR(int column, int row) {
    bool win = false;
    if (column < 0 || row < 0 || column > width - 1 || row > height - 1) {
        return false;
    }
    char user = gameCells[row][column].getSymbol();
    int i = row;
    int j = column;

    while (i != height - 1 && j != 0) {
        ++i;
        --j;
//        cout << "i:" << i << " j:" << j <<endl;
        if (gameCells[i][j].getSymbol() == user) {
            ++row;
            --column;
        } else {
            break;
        }
    }
    i = row;
    j = column;
    int counter = 1;
    while (!win && user == gameCells[row][column].getSymbol()) {

        if (row <= 0 || column >= height - 1) {
            break;
        }
        --row;
        ++column;
        if (gameCells[row][column].getSymbol() == user) {
            ++counter;
        }
        if (counter == FOUR_IN_ROW) {

            win = true;
            if (!blockCheck) {
                for (int lcv1 = i, lcv2 = j, k = FOUR_IN_ROW;
                     k > 0;
                     --lcv1, ++lcv2, --k) {

                    markWinState(lcv1, lcv2);
                }
            }
        }
    }
    return win;

}

bool ConnectFour::compareGames(const ConnectFour &other) {
    return other.singleLivingCell() <= singleLivingCell();
}

void ConnectFour::markWinState(int currentRow, int currentCol) {

    if (gameCells[currentRow][currentCol].getSymbol() == 'X')
        gameCells[currentRow][currentCol].setSymbol('x');
    else if (gameCells[currentRow][currentCol].getSymbol() == 'O')
        gameCells[currentRow][currentCol].setSymbol('o');

}

int ConnectFour::blockWinState(char target) {
    blockCheck = true;
    int win = WRONG;
    // try next move win or not
    bool aa;
    for (int i = 0; win < 0 && i < width; ++i) {
        int row = makeMove((char) i + 'A', target);
        if (row == WRONG)
            continue;

        if ((aa = checkWin(i, row))) {
            win = i;
        }
        // undo move
        gameCells[row][i].setSymbol('.');
    }
    blockCheck = false;
    return win;
}

void ConnectFour::printLivingCells() {
    cout << "Living Cell: " << livingCell << endl;
}

void ConnectFour::resizeBoard() {
    cout << width << " " << height << endl;
    do {
        cout << "Please enter board width : " << endl;
        while (!(cin >> width)) {
            cout << "Incorrect input, Please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Please enter board height : " << endl;
        while (!(cin >> height)) {
            cout << "Incorrect input, Please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        //board size control
        if ((width < FOUR_IN_ROW || height < FOUR_IN_ROW)) {
            cerr << "Board Sizes must be larger than three!\n";
        }
    } while ((width < FOUR_IN_ROW || height < FOUR_IN_ROW));

}

void ConnectFour::createBoard() {

    gameCells = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        gameCells[i] = new Cell[width];
    }

}

void ConnectFour::removeBoard(bool first) {


    int size = STD_SIZE;
    if(!first){
        size = width;
    }

    for (int i = 0; i < size; ++i) {
        delete [] gameCells[i];
        gameCells[i] = nullptr;
    }
    delete gameCells;
    gameCells = nullptr;
}

bool ConnectFour::getBoard() {
    int countWidth = 0;
    int countHeight = 0;
    string fileName;
    cout << "Please Enter File Name including board:\n";
    cin >> fileName;
    ifstream fileStream(fileName);
    // does file exist?
    if (fileStream.is_open()) {
        int i = 0;
        // store data at file in vector , at the same time compute sizes of board
        vector <string> boardFromFile;
        string line;
        while(!fileStream.eof()){
            getline(fileStream,line);
            if(line != ""){
                ++countHeight;
                boardFromFile.push_back(line);
            }
        }
        countWidth = static_cast<int>(boardFromFile[0].length());
        if(countHeight < FOUR_IN_ROW || countWidth < FOUR_IN_ROW){
            cerr << "Board sizes are not appropriate!" << endl;
            return false;
        }
        width = countWidth;
        height = countHeight;
        removeBoard(true);
        createBoard();
        //
        moveLeft = 0;
        char c;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if((c = boardFromFile[i][j]) == '*') {
                    ++moveLeft;     // only valid cells count
                    gameCells[i][j].setSymbol('.');
                }
                else{
//                    cout << "board char:" << (int)c << endl;
                    gameCells[i][j].setSymbol(c);
                }
            }
        }
        return true;
    } else{
        cerr << "File NOT FOUND! The game will start by 5x5.\n";
        return false;
    }
}

bool ConnectFour::operator==(const ConnectFour &other) const{

    if(this->getWidth() != other.getWidth()) { return false;}
    if(this->getHeight() != other.getHeight()) { return false;}
    if(this->getGameMode() != other.getGameMode()) { return false;}
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(gameCells[i][j].getSymbol() != other.gameCells[i][j].getSymbol()){
                return false;
            }
        }
    }
    return true;
}

bool ConnectFour::operator!=(const ConnectFour &other) const{
    return (!operator==(other));
}

