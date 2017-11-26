//
// Created by Burak Kağan Korkmaz on 23.11.2017.
//

#include "ConnectFourPlus.h"

bool ConnectFourPlus::CheckWin(int col, int row) {
    return (checkWinHorizontal(col,row) || checkWinVertical(col,row));
}
