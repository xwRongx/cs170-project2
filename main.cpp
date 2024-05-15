#include <iostream>
#include "headers/Node.h"
#include "../headers/AlgGeneric.h"
#include "../headers/AlgForward.h"
#include "../headers/AlgBackward.h"

int main(){
    int featureNumber;
    std::cout << "feature: ";
    std::cin >> featureNumber;

    int userChoice = -1;
    std::cout << "user choices: ";
    std::cin >> userChoice;
    switch(userChoice){
        case 1: //forward selection

            break;
        case 2: //backward elimination

            break;
        default:
            break;
    }
}