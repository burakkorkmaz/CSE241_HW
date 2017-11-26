#include <iostream>
#include "ConnectFourAbstract.h"
#include "ConnectFourPlus.h"
#include "ConnectFourPlusUndo.h"
#include "ConnectFourDiag.h"

int main() {

    ConnectFourPlus *cfPlus = new ConnectFourPlus();
    ConnectFourDiag *cfDiag = new ConnectFourDiag();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}