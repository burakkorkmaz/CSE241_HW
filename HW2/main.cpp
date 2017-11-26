/*
 *          CSE 241
 * Object Oriented Programming
 *      Homework # 2
 *
 *  BURAK KAĞAN KORKMAZ
 *      141044041
 *
 *          08.10.2017
 */

#include <iostream>
#include <fstream>

using namespace std;


#define MAX_ROW 20
#define MAX_COL 20
#define MIN_ROW 4
#define WRONG -1

#define WIN_SCORE 4
#define COMPUTER 'C'
#define PLAYER 'P'


void printBoard(char board[][MAX_COL], int SizeOfBoard);

void initializeBoard(char Board[MAX_ROW][MAX_COL]);

void switchTurn(char &userChar);

// changes letters with smaller one at win state
void markWinState(char &unmarkedChar);

// assign user char to array and return row or -1
int makeMove(char [MAX_ROW][MAX_COL], char column, int sizeOfBoard, char userChar);

// all win possibilities checks by calling other check func.
bool checkWin(char Board[][MAX_COL], int sizeOfBoard, int column, int row);

bool checkWinVertical(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, int column, int row);

bool checkWinHorizontal(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, int column, int row);

bool checkWinDiagonalL(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, int column, int row);

bool checkWinDiagonalR(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, int column, int row);

// try whether next move of user wins
int blockWinState(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, char userChar);


int saveGame(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, int moveLeft,
         char gameMode, char userChar, const string & fileName );

int loadGame(char Board[MAX_ROW][MAX_COL], int & sizeOfBoard, int & moveLeft,
char & gameMode, char & userChar, const string & fileName) ;

/* * * * * * * * * * * * * *
 *                          *
 *      FILE  FORMAT        *
 *                          *
 *                          *
 *  GAME MODE(PVP - COMP)   *
 *      BOARD SIZE          *
 *     CURRENT USER         *
 *      MOVE LEFT           *
 *  /----------------\      *
 *  |     BOARD      |      *
 *  |                |      *
 *  |      ...       |      *
 *  |                |      *
 *  |                |      *
 *  \----------------/      *
 *                          *
 * * * * * * * * * * * * * */


int main() {
    cout << "Welcome To ConnectFour v1.0" << endl;
    cout << "Please enter board size (4, 6, ... ,20): " << endl;
    int boardSize;
    cin >> boardSize;

    //board size control
    if ((boardSize < MIN_ROW || boardSize > MAX_ROW) || boardSize % 2 == 1) {
        cerr << "Wrong Board Size entered!\n";
        return EXIT_FAILURE;
    }

    char board[MAX_ROW][MAX_COL];
    char user = 'X';

    initializeBoard(board);

    char players;
    do {
        cout << "Do you want to play one or two players (C, P): " << endl;
        cin >> players;
    } while (players != 'C' && players != 'P');

    int move = boardSize * boardSize;
    int currentRow = 0;
    int currentCol = 0;

    bool loaded = false;
    do {
        // pvp game
        if (players == PLAYER) {
            loaded = false;
            while (move > 0) {

                printBoard(board, boardSize);
                if (user == 'X') {
                    cout << "Please make a move USER1 -> ";
                } else {
                    cout << "Please make a move USER2 -> ";
                }
                string selectedColumn;
                getline(cin, selectedColumn);

                if (selectedColumn.size() == 1) {

                    char column = selectedColumn.c_str()[0];

                    currentCol = column - 'A';
                    currentRow = makeMove(board, column, boardSize, user);

                    // if move is valid
                    if (currentRow != WRONG) {
                        --move;
                        if (checkWin(board, boardSize, currentCol, currentRow)) {
                            printBoard(board, boardSize);
                            if (user == 'X')
                                cout << "USER1 win!";
                            else
                                cout << "USER2 win!";

                            return 1;
                        }

                        //if board is full
                        if (move == 0) {
                            printBoard(board, boardSize);
                            cout << "Game Over! It's draw." << endl;
                        }
                        switchTurn(user);
                    }
                } else if (selectedColumn.size() > 1) { // SAVE & LOAD COMMAND

                    auto size = selectedColumn.size();
                    auto position = selectedColumn.find(' ');
                    string command = selectedColumn.substr(0, position);
                    string filename = selectedColumn.substr(position, size);
//                    cout << "COMMAND: " << command << " FILE: " << filename << endl;

                    if (command == "SAVE") {
                        if(saveGame(board, boardSize, move, players, user, filename) != WRONG){

                            cout << "SAVE SUCCESS!" << endl;
                        } else{
                            cerr << "SAVE FAILED!" << endl;

                        }


                    } else if (command == "LOAD") {
                        if(loadGame(board, boardSize, move, players, user, filename) != WRONG){
                            loaded = true;
                            cout << "LOAD SUCCESS!" << endl;
                        } else{
                            cerr << "LOAD FAILED!" << endl;
                        }
                        break;
                    }


                }


            }

            // player vs. computer game
        } else if (players == COMPUTER) {
//        char comp = 'O';
            loaded = false;
            while (move > 0) {

                string selectedColumn = "";

                if (user == 'X') {  // User 1 - PLAYER
                    printBoard(board, boardSize);
                    cout << "Please make a move USER1 -> ";

                    getline(cin, selectedColumn);
                    if (selectedColumn.size() == 1) {

                        decltype(selectedColumn.c_str()[0]) column
                                = selectedColumn.c_str()[0];

                        currentCol = column - 'A';
//                    currentRow = makeMove(board, column, boardSize, user);

                    } else if (selectedColumn.size() > 1) { // SAVE & LOAD COMMAND

                        auto size = selectedColumn.size();
                        auto position = selectedColumn.find(' ');
                        string command = selectedColumn.substr(0, position);
                        string filename = selectedColumn.substr(position, size);
//                        cout << "COMMAND: " << command << " FILE: " << filename << endl;

                        if (command == "SAVE") {
                            if(saveGame(board, boardSize, move, players, user, filename) != WRONG){

                                cout << "SAVE SUCCESS!" << endl;
                            } else{
                                cerr << "SAVE FAILED!" << endl;

                            }


                        } else if (command == "LOAD") {

                            loadGame(board, boardSize, move, players, user, filename);
                            if(loadGame(board, boardSize, move, players, user, filename) != WRONG){
                                loaded = true;
                                cout << "LOAD SUCCESS!" << endl;
                            } else{
                                cerr << "LOAD FAILED!" << endl;
                            }
                            break;

                        } else {
                            cerr << "Invalid Command!\n";
                        }
                        continue;

                    }

                } else { // User 2 - COMPUTER

                    printBoard(board, boardSize);
                    cout << "USER2(Computer) making a move..." << endl;
                    // blocks next move of user if user will win
                    int row = blockWinState(board, boardSize, 'X');
//                    cout << row << endl;
                    if (row != WRONG) {

                        selectedColumn[0] = (char) row + 'A';
                    } else {

                        // if next move of comp. will win, move this cell
                        row = blockWinState(board, boardSize, 'O');
                        if (row != WRONG) {

                            selectedColumn[0] = (char) row + 'A';
                        } else {

                            int col = 0;

                            while (board[0][col] != '.') {
                                ++col;
                            }

                            selectedColumn[0] = (char) col + 'A';
                        }

                    }


                    cout << selectedColumn << endl;

//                currentRow = makeMove(board, selectedColumn, boardSize, user);


                    currentCol = selectedColumn[0] - 'A';
                }

                currentRow = makeMove(board, selectedColumn[0], boardSize, user);


//                cout << "cR: " << currentRow << " cC: " << currentCol << endl;

                if (currentRow != WRONG) {
                    --move;

                    if (checkWin(board, boardSize, currentCol, currentRow)) {

                        printBoard(board, boardSize);
                        if (user == 'O')
                            cout << "USER2(Computer) win!";
                        else
                            cout << "USER1 win!";

                        return 1;
                    }

                    if (move == 0) {
                        printBoard(board, boardSize);
                        cout << "Game Over! It's draw." << endl;
                    }
                    switchTurn(user);
                }

            }

        }

    }while(loaded);

    return 0;
}

void initializeBoard(char Board[][MAX_COL]) {

    for (int i = 0; i < MAX_ROW; ++i) {
        for (int j = 0; j < MAX_COL; ++j) {
            Board[i][j] = '.';
        }
    }
}

void printBoard(char board[][MAX_COL], int SizeOfBoard) {

    cout << endl;
    for (int k = 0; k < SizeOfBoard; ++k) {
        cout << static_cast<char> ('A' + k) << " ";
    }
    cout << endl;

    for (int i = 0; i < SizeOfBoard; ++i) {
        for (int j = 0; j < SizeOfBoard; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

}

void switchTurn(char &userChar) {

    if (userChar == 'X')
        userChar = 'O';
    else if (userChar == 'O')
        userChar = 'X';
}

void markWinState(char &unmarkedChar) {

    if (unmarkedChar == 'X')
        unmarkedChar = 'x';
    else if (unmarkedChar == 'O')
        unmarkedChar = 'o';

}

void unmarkCells(char Board[MAX_ROW][MAX_COL], int sizeOfBoard) {

    for (int i = 0; i < sizeOfBoard; ++i) {
        for (int j = 0; j < sizeOfBoard; ++j) {
            if (Board[i][j] == 'x')
                Board[i][j] = 'X';
            else if (Board[i][j] == 'o')
                Board[i][j] = 'O';
        }
    }
}

int makeMove(char Board[][MAX_COL], char column, int sizeOfBoard, char userChar) {
    int row = 0;
    bool move = false;
    int index = column - 'A';

    int i = sizeOfBoard - 1;
    if (index > i || index < 0) {
//        cerr << "Wrong Column!" << endl;
        return WRONG;
    }
    while (i >= 0 && !move) {
        if (Board[i][index] == '.') {
            Board[i][index] = userChar;
            row = i;
            move = true;
        } else {
            --i;
        }
    }
    if (i < 0) {
//        cerr << "Invalid Move!\n";
        row = WRONG;
    }

    return row;
}

int blockWinState(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, char userChar) {

    int win = -1;
    for (int i = 0; win < 0 && i < sizeOfBoard; ++i) {
        // try next move win or not
        int row = makeMove(Board, (char) i + 'A', sizeOfBoard, userChar);
        if (row == WRONG)
            continue;

        if (checkWin(Board, sizeOfBoard, i, row)) {
            win = i;
        }

        // undo move
        Board[row][i] = '.';
        unmarkCells(Board, sizeOfBoard);
    }
    return win;
}

bool checkWin(char Board[][MAX_COL], int sizeOfBoard, int column, int row) {

    return (checkWinVertical(Board, sizeOfBoard, column, row) ||
            checkWinHorizontal(Board, sizeOfBoard, column, row) ||
            checkWinDiagonalL(Board, sizeOfBoard, column, row) ||
            checkWinDiagonalR(Board, sizeOfBoard, column, row));
}

bool checkWinVertical(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, int column, int row) {
    bool win = false;
    if (row + 3 >= sizeOfBoard || column >= sizeOfBoard) {
        return win;
    }
    char user = Board[row][column];

    if (user == Board[row + 1][column] &&
        user == Board[row + 2][column] &&
        user == Board[row + 3][column]) {
        win = true;

        for (int i = row, j = column, k = WIN_SCORE; k > 0; ++i, --k) {
            markWinState(Board[i][j]);
        }
    }


    return win;
}

bool checkWinHorizontal(char Board[][MAX_COL], int sizeOfBoard, int column, int row) {
    bool win = false;
    if (column < 0 || column > sizeOfBoard - 1) {
        return false;
    }

    char user = Board[row][column];
    int i = row;
    int j = column;

    while (column != 0 && j != 0) {
        --j;
        if (Board[i][j] == user) {
            --column;
        } else {
            break;
        }

    }
    j = column;
    int counter = 1;
    while (!win && user == Board[row][column]) {

        if (column > sizeOfBoard - 1) {
            break;
        }

        ++column;

        if (Board[row][column] == user) {
            ++counter;
        }

        if (counter == WIN_SCORE) {
            win = true;
            for (int lcv1 = i, lcv2 = j, k = WIN_SCORE; k > 0; ++lcv2, --k) {
                markWinState(Board[lcv1][lcv2]);
            }
        }
    }
    return win;
}

bool checkWinDiagonalL(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, int column, int row) {
    bool win = false;
    if (column < 0 || row < 0 || column > sizeOfBoard - 1 || row > sizeOfBoard - 1) {
        return false;
    }

    char user = Board[row][column];
    int i = row;
    int j = column;

    while (i != 0 || j != 0) {
        --i;
        --j;
        if (Board[i][j] == user) {
            --column;
            --row;
        } else {
            break;
        }
    }
    i = row;
    j = column;
    int counter = 1;
    while (!win && user == Board[row][column]) {

        if (row > sizeOfBoard - 1 || column > sizeOfBoard - 1) {
            break;
        }

        ++column;
        ++row;

        if (Board[row][column] == user) {
            ++counter;
        }

        if (counter == WIN_SCORE) {
            win = true;
            for (int lcv1 = i, lcv2 = j, k = WIN_SCORE;
                 k > 0;
                 ++lcv1, ++lcv2, --k) {

                markWinState(Board[lcv1][lcv2]);
            }
        }
    }
    return win;


}

bool checkWinDiagonalR(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, int column, int row) {
    bool win = false;
    if (column < 0 || row < 0 || column > sizeOfBoard - 1 || row > sizeOfBoard - 1) {
        return false;
    }

    char user = Board[row][column];
    int i = row;
    int j = column;

    while (i != sizeOfBoard - 1 || j != 0) {
        ++i;
        --j;
        if (Board[i][j] == user) {
            ++row;
            --column;
        } else {
            break;
        }
    }
    i = row;
    j = column;
    int counter = 1;
    while (!win && user == Board[row][column]) {

        if (row < 0 || column > sizeOfBoard - 1) {
            break;
        }

        --row;
        ++column;

        if (Board[row][column] == user) {
            ++counter;
        }

        if (counter == WIN_SCORE) {
            win = true;
            for (int lcv1 = i, lcv2 = j, k = WIN_SCORE;
                 k > 0;
                 --lcv1, ++lcv2, --k) {

                markWinState(Board[lcv1][lcv2]);
            }
        }
    }
    return win;

}

/* * * * * * * * * * * * * *
 *                          *
 *      FILE  FORMAT        *
 *                          *
 *                          *
 *  GAME MODE(PVP - COMP)   *
 *      BOARD SIZE          *
 *     CURRENT USER         *
 *      MOVE LEFT           *
 *  /----------------\      *
 *  |     BOARD      |      *
 *  |                |      *
 *  |      ...       |      *
 *  |                |      *
 *  |                |      *
 *  \----------------/      *
 *                          *
 * * * * * * * * * * * * * */


int saveGame(char Board[MAX_ROW][MAX_COL], int sizeOfBoard, int moveLeft,
             char gameMode, char userChar, const string & fileName = "FILE.TXT") {
    cout << fileName << endl;
    ofstream fileStream(fileName);
    if (fileStream.is_open()) {
        fileStream << gameMode << endl
                   << sizeOfBoard << endl
                   << userChar << endl
                   << moveLeft << endl;
        for (int i = 0; i < sizeOfBoard; ++i) {
            for (int j = 0; j < sizeOfBoard; ++j) {
                fileStream << Board[i][j];
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

int loadGame(char Board[MAX_ROW][MAX_COL], int & sizeOfBoard, int & moveLeft,
             char & gameMode, char & userChar, const string & fileName = "FILE.TXT") {

    ifstream fileStream(fileName);
    if (fileStream.is_open()) {
        fileStream >> gameMode >> sizeOfBoard >> userChar >> moveLeft;
        cout << "Game is Loading...\nMode: " << gameMode << " Size: " << sizeOfBoard << " User: " << userChar << " Move Left: " << moveLeft
             << endl;
        initializeBoard(Board);
        for (int i = 0; i < sizeOfBoard; ++i) {
            for (int j = 0; j < sizeOfBoard; ++j) {
                fileStream >> Board[i][j];
            }
        }

    }
    else{
        cerr << "File Error!\n";
        return WRONG;
    }

    fileStream.close();

    return 0;
}