//
// Created by Burak Kağan Korkmaz on 23.11.2017.
//

#ifndef HW05_CELL_H
#define HW05_CELL_H


class Cell {

public:

    int getRow(){ return row;}
    char getPos(){ return pos;}
    char getSymbol(){ return symbol;}

    void setRow(int rowValue){row = rowValue;}
    void setPosition(char posValue){pos = posValue;}
    void setPosition(int positionInt) {pos = static_cast<char>(positionInt + 'A'); }
    void setSymbol(char symbolValue){symbol = symbolValue;}

private:
    int row;
    char pos;
    char symbol = '.';
};


#endif //HW05_CELL_H
