#include <iostream>
#include "headers/Node.h"
#include <set>
#include <algorithm>
#include <time.h>
using namespace std;


float evaluationFunction(set<int> s){ //i'm assuming this will eventually take in a vector of set??
      //stub function 
      return  static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(100-0))); //return random number betwwen 0 and 100
};

set<int> forwardSelectionAlgorithm(set<int> s)
{
    set<int> globalHighest;
    set<int> localHighest;

    int n = s.size();

    float globalAccuracy = 0;
    float localAcurracy = 0;
    for(int i = 1; i <= n; i++)
    {
        localHighest.clear();
        for(int j = 1; j <= n; j++)
        {
            set<int> evaluationSet = globalHighest;
            evaluationSet.insert(j);

            float accuracy = evaluationFunction(evaluationSet);
            cout << "\tUsing features { ";
            for (int feature : evaluationSet) 
            {
                cout << feature << " ";
            }
            cout << "} accuracy is " << accuracy << "%" << endl;

            if (localHighest.empty() || accuracy > localAcurracy) 
            {
                localHighest = evaluationSet;
                localAcurracy = accuracy;
            }        
        }
        cout << "Feature set { ";
        for (int feature : localHighest) 
        {
            cout << feature << " ";
        }


        cout << "} was best, accuracy is " << localAcurracy << endl;
        
        globalAccuracy = evaluationFunction(globalHighest);
        if (!globalHighest.empty() && localAcurracy < globalAccuracy) 
        {
            cout << "Warning: Accuracy decreased" << endl;
        }
        else if(localAcurracy > globalAccuracy)
        {
            globalHighest = localHighest;
            globalAccuracy = localAcurracy;
        }
    }
    return globalHighest;
};

set<int> backwardsSelectionAlgorithm(set<int> s)
{
    set<int> globalHighest;
    // set<int> localHighest;

    // int n = s.size();

    // float globalAccuracy = 0;
    // float localAcurracy = 0;
    // globalHighest = s;
    // globalAccuracy = evaluationFunction(globalHighest);
    // for(int i = 1; i <= n; i++)
    // {
    //     localHighest.clear();
    //     for(int j = 1; j <= n; j++)
    //     {
    //         set<int> evaluationSet = globalHighest;
    //         evaluationSet.insert(j);

    //         float accuracy = evaluationFunction(evaluationSet);
    //         cout << "\tUsing features { ";
    //         for (int feature : evaluationSet) 
    //         {
    //             cout << feature << " ";
    //         }
    //         cout << "} accuracy is " << accuracy << "%" << endl;

    //         if (localHighest.empty() || accuracy > localAcurracy) 
    //         {
    //             localHighest = evaluationSet;
    //             localAcurracy = accuracy;
    //         }        
    //     }
    //     cout << "Feature set { ";
    //     for (int feature : localHighest) 
    //     {
    //         cout << feature << " ";
    //     }


    //     cout << "} was best, accuracy is " << localAcurracy << endl;
        
    //     globalAccuracy = evaluationFunction(globalHighest);
    //     if (!globalHighest.empty() && localAcurracy < globalAccuracy) 
    //     {
    //         cout << "Warning: Accuracy decreased" << endl;
    //     }
    //     else if(localAcurracy > globalAccuracy)
    //     {
    //         globalHighest = localHighest;
    //         globalAccuracy = localAcurracy;
    //     }
    // }
    return globalHighest;
};

int main(){
    srand(time(NULL));
    int choice = -1;
    while(choice !=0){
        cout << "Welcome to the Feature Selection Algorithm.\n";
        cout << "Please enter the total number of features: ";
        int numFeatures;
        cin >> numFeatures;

        set<int> features;

        for(int i = 1; i <= numFeatures; i++)
        {
            features.insert(i);
        }
        cout << "Type the number of the algorithm you want to run.\n";
        cout << "1. Forward Selection\n";
        cout << "2. Backward Elimination\n";
        cout << "0. to Exit\n";
        
        cin >> choice;
        set<int> bestFeatures;

        switch (choice) {
            case 1:
                cout << "Using no features and \"random\" evaluation, I get an accuracy of " << evaluationFunction(bestFeatures) << endl;
                bestFeatures = forwardSelectionAlgorithm(features);
                break;
            case 2:
                cout << "Using no features and \"random\" evaluation, I get an accuracy of " << evaluationFunction(bestFeatures) << endl;
                bestFeatures = backwardsSelectionAlgorithm(features);
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }

}
