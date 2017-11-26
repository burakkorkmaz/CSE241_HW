#include <iostream>
#include "ConnectFour.h"
#define OBJECTS 5
int main() {


    string gameMode;
    cout << "Please Select Game Mode [S]ingle / [M]ultiple:";
    do{
        cin >> gameMode;

    }while(gameMode != "S" && gameMode != "M");


    if(gameMode == "S"){
        ConnectFour cF;
        cF.playGame();

    }else{

        int counter = 1;

        ConnectFour cF[OBJECTS];

        for (int i = 0; i < OBJECTS; ++i) {

            cF[i].setGameMode(gameMode[0]);
            cF[i].setMultiCtrl(false);
            cout << "Object " << counter + i << ":\n";
            cF[i].playGame();
        }

        while(finishedCount != OBJECTS){
            do {
                cout << "Select an Object [1 - " << OBJECTS << "]:\n";
                while (!(cin >> counter)) {
                    cout << "Incorrect input, Please try again.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }while (counter < 1 || counter > OBJECTS);

            cout << "Object " << counter  << ":\n";

            cF[counter - 1].setMultiCtrl(true);
            cF[counter - 1].playGame();
//            cout << "finCon:"<< cF[counter - 1].finishedObjects()<< endl;

        }


    }



    return 0;
}