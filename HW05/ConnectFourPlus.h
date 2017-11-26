//
// Created by Burak Kağan Korkmaz on 23.11.2017.
//

#ifndef HW05_CONNECTFOURPLUS_H
#define HW05_CONNECTFOURPLUS_H


#include "ConnectFourAbstract.h"

class ConnectFourPlus : public ConnectFourAbstract{

    ConnectFourPlus(){
        ConnectFourAbstract();
    }

    ConnectFourPlus(const string& fileName){
        ConnectFourAbstract();
    }

    bool CheckWin(int col, int row);
};


#endif //HW05_CONNECTFOURPLUS_H
