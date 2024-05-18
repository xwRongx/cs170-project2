#include <iostream>
#include "headers/Node.h"
#include "../headers/AlgGeneric.h"
#include "../headers/AlgForward.h"
#include "../headers/AlgBackward.h"
using namespace std;

int main(){
    int choice = -1;
    while(choice !=0){
        cout << "Welcome to the Feature Selection Algorithm.\n";
        cout << "Please enter the total number of features: ";
        int numFeatures;
        cin >> numFeatures;


    
        cout << "Type the number of the algorithm you want to run.\n";
        cout << "1. Forward Selection\n";
        cout << "2. Backward Elimination\n";
        cout << "0. to Exit\n";
        
        cin >> choice;

        switch (choice) {
            case 1:
                //forwardSelection(numFeatures); //takes in the number of features
                break;
            case 2:
                cout << "Backward Elimination not implemented yet.\n";
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }

}
