//
// Created by Burak Kağan Korkmaz on 23.11.2017.
//

#include "ConnectFourDiag.h"

bool ConnectFourDiag::checkWin(int col, int row) {
    return (checkWinDiagonalL(col,row) || checkWinDiagonalR(col,row));
}
