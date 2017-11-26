//
// Created by Burak Kağan Korkmaz on 25.11.2017.
//

#include "ConnectFourAbstract.h"

bool ConnectFourAbstract::playGame() {

    playerOrComp;

    userSymbol = 'X';
    resizeBoard();

    initializeBoard();
    moveLeft = width * height;
    do {
        cout << "Do you want to play with [C]omputer or [P]layer : " << endl;
        cin.clear();
        cin >> playerOrComp;
    } while (playerOrComp != COMPUTER && playerOrComp != PLAYER);
    loaded = false;


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


    return false;
}

bool ConnectFourAbstract::play() {

    int currentRow = 0;
    int currentCol = 0;

    printBoard();
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
        if (checkWin(currentCol, currentRow)) {

            printBoard();
            if (userSymbol == 'O')
                cout << "USER2(Computer) win!\n";
            isFinished = true;
            return isFinished;
        }
        if (moveLeft == 0) {
            printBoard();
            cout << "Game Over! It's draw." << endl;
            isFinished = true;
            isDraw = true;
        }
        if (!isFinished){
            switchTurn(userSymbol);
        }
    }
}

// Does required controls for computer game
bool ConnectFourAbstract::computerGame() {
    bool quit = false;
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
            cout << "Please make a move USER1 -> ";
            do {
                getline(cin, selectedColumn);
            } while (selectedColumn.empty());

            play(selectedColumn);

        } else {    // User 2 - COMPUTER

            play();
        }
    }
    return quit;
}

bool ConnectFourAbstract::play(const string& selectedColumn) {
    errorOccurred = false;
    if (selectedColumn.size() == 1) {

        decltype(selectedColumn.c_str()[0]) column
                = selectedColumn.c_str()[0];

        int currentCol = column - 'A';
        int currentRow = makeMove(column, userSymbol);

        // if move is valid
        if (currentRow != WRONG) {
            --moveLeft;
            if (checkWin(currentCol, currentRow)) {
                printBoard();
                if (userSymbol == 'X')
                    cout << "USER1 win!\n";
                else
                    cout << "USER2 win!\n";
                isFinished = true;
                return isFinished;
            }

            //if board is full
            if (moveLeft == 0) {
                printBoard();
                cout << "Game Over! It's draw." << endl;
                isFinished = true;
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

// Does required controls for PVP game
bool ConnectFourAbstract::playerGame() {

    bool quit = false;
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


void ConnectFourAbstract::createBoard() {

    gameCells = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        gameCells[i] = new Cell[width];

    }
}

void ConnectFourAbstract::resizeBoard() {
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

/*
 * Resizes the vector and fills cells. DEFAULT: '.'
 */
void ConnectFourAbstract::initializeBoard(char c) {
    if(gameCells == nullptr){
        createBoard();
    }
    else if(getHeight() != STD_SIZE || getWidth() != STD_SIZE) {
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
void ConnectFourAbstract::printBoard() {
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

void ConnectFourAbstract::removeBoard(bool firstRemove) {

    int size = STD_SIZE;
    if(!firstRemove){
        size = width;
    }

    for (int i = 0; i < size; ++i) {
        delete [] gameCells[i];
        gameCells[i] = nullptr;
    }
    delete gameCells;
    gameCells = nullptr;

}

/*
 * Makes move user character to the given position.
 * Returns row position of current move, otherwise -1
 * */
int ConnectFourAbstract::makeMove(char columnPos, char userChar) {
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

void ConnectFourAbstract::switchTurn(char &userChar) {

    if (userChar == 'X')
        userChar = 'O';
    else if (userChar == 'O')
        userChar = 'X';
}

int ConnectFourAbstract::saveGame(const string & fileName) const{

    cout << fileName << endl;
    ofstream fileStream(fileName);
    if (fileStream.is_open()) {
        fileStream
                << playerOrComp << endl
                << width << endl
                << height << endl
                << userSymbol << endl
                << moveLeft << endl;
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

int ConnectFourAbstract::loadGame(const string &fileName) {
    ifstream fileStream(fileName);
    if (fileStream.is_open()) {
        fileStream
                >> playerOrComp
                >> width
                >> height
                >> userSymbol
                >> moveLeft;
        initializeBoard('.');
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

bool ConnectFourAbstract::checkWinVertical(int column, int row) {
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

bool ConnectFourAbstract::checkWinHorizontal(int column, int row) {
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

bool ConnectFourAbstract::checkWinDiagonalL(int column, int row) {
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

bool ConnectFourAbstract::checkWinDiagonalR(int column, int row) {
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


void ConnectFourAbstract::markWinState(int currentRow, int currentCol) {

    if (gameCells[currentRow][currentCol].getSymbol() == 'X')
        gameCells[currentRow][currentCol].setSymbol('x');
    else if (gameCells[currentRow][currentCol].getSymbol() == 'O')
        gameCells[currentRow][currentCol].setSymbol('o');

}

int ConnectFourAbstract::blockWinState(char target) {
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
