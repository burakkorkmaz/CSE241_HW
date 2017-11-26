//
// Created by Burak Kağan Korkmaz on 23.11.2017.
//

#ifndef HW05_CONNECTFOURDIAG_H
#define HW05_CONNECTFOURDIAG_H


#include "ConnectFourAbstract.h"

class  ConnectFourDiag : public ConnectFourAbstract{

    virtual bool checkWin(int col, int row);
};


#endif //HW05_CONNECTFOURDİAG_H
