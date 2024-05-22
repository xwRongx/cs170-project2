#include <iostream>
#include <set>
#include <algorithm>
#include <time.h>
#include <map>
#include <ctime>
using namespace std;


float evaluationFunction(set<int> s){ //i'm assuming this will eventually take in a vector of set??
    //stub function
    return  static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(100-0))); //return random number betwwen 0 and 100
};

// float evaluationFunction(set<int> s){ //i'm assuming this will eventually take in a vector of set??
//     int sum = 0;
//     for (int val : s)
//     {
//         sum += val;
//     }
//     return static_cast<float>(sum);
// };

set<int> forwardSelectionAlgorithm(set<int> s) {
    set<int> globalHighest; //we use set bc no duplicates and auto sorted
    set<int> localHighest;
    map<set<int>, float> evaluatedSets; //this map uses a set<int> (subset of features) to find a float (accuracy %)

    int n = s.size();

    float globalAccuracy = 0;
    float localAccuracy = 0;
    for(int i = 1; i <= n; i++) {
        localHighest.clear();
        localAccuracy = 0;
        for(auto feature : s) { //auto keyword- smartly fills in type based on the variable given (in this case s is int)
            set<int> evaluationSet = globalHighest; //best subset so far
            evaluationSet.insert(feature); //best + 1 new feature

            if (evaluatedSets.find(evaluationSet) == evaluatedSets.end()) { //?
                float accuracy = evaluationFunction(evaluationSet);
                evaluatedSets[evaluationSet] = accuracy; //putting accuracy corresponding to set

                //prints:
                cout << "\tUsing features { ";
                for (int feat : evaluationSet) {
                    cout << feat << " ";
                }
                cout << "} accuracy is " << accuracy << "%" << endl;

                if (localHighest.empty() || accuracy > localAccuracy) { //update localAccuracy
                    localHighest = evaluationSet;
                    localAccuracy = accuracy;
                }
            }
        }

        if (localHighest.empty()) break;

        //prints:
        cout << "Feature set { ";
        for (int feature : localHighest) {
            cout << feature << " ";
        }
        cout << "} was best, accuracy is " << localAccuracy << "%" << endl;

        if(globalHighest.empty()){ //set to local values
            globalHighest = localHighest;
            globalAccuracy = localAccuracy;
        }else{
            globalAccuracy = evaluationFunction(globalHighest);
            if (localAccuracy < globalAccuracy) { //accuracy decreased
                cout << "Warning: Accuracy decreased" << endl;
                break;
            } else if(localAccuracy > globalAccuracy) { //accuracy increased
                globalHighest = localHighest;
                globalAccuracy = localAccuracy;
            } //nothing if accuracy stayed same
        }

    }
    return globalHighest;
};

set<int> backwardsSelectionAlgorithm(set<int> s) {
    set<int> globalHighest = s;
    set<int> localHighest;
    map<set<int>, float> evaluatedSets;

    float globalAccuracy = evaluationFunction(globalHighest);
    float localAccuracy;

    int n = s.size();

    for(int i = 1; i <= n; i++) {
        localHighest = globalHighest;
        localAccuracy = 0;

        for(int j = 1; j <=n; j++) {
            set<int> evaluationSet = globalHighest;
            evaluationSet.erase(j);

            if (evaluatedSets.find(evaluationSet) == evaluatedSets.end()) {
                float accuracy = evaluationFunction(evaluationSet);
                evaluatedSets[evaluationSet] = accuracy;

                cout << "\tUsing features { ";
                for (int feature : evaluationSet) {
                    cout << feature << " ";
                }
                cout << "} accuracy is " << accuracy << "%" << endl;

                if (evaluationSet.size() > 0 && (localAccuracy == 0 || accuracy > localAccuracy)) {
                    localHighest = evaluationSet;
                    localAccuracy = accuracy;
                }
            }
        }

        if (localHighest.empty()) break;

        cout << "Feature set { ";
        for (int feature : localHighest) {
            cout << feature << " ";
        }
        cout << "} was best, accuracy is " << localAccuracy << "%" << endl;

        if (localAccuracy < globalAccuracy) {
            cout << "Warning: Accuracy decreased" << endl;
            break;
        } else {
            globalHighest = localHighest;
            globalAccuracy = localAccuracy;
        }
    }
    return globalHighest;
};

int main(){
    srand(time(0));
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
                cout << "The overall best feature selection is: " << endl;
                for (int feature : bestFeatures) {
                    cout << feature << " ";
                }
                cout << endl;
                break;
            case 2:
                cout << "Using no features and \"random\" evaluation, I get an accuracy of " << evaluationFunction(bestFeatures) << endl;
                bestFeatures = backwardsSelectionAlgorithm(features);
                cout << "The overall best feature selection is: " << endl;
                for (int feature : bestFeatures) {
                    cout << feature << " ";
                }
                cout << endl;
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }

}
