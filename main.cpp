#include <iostream>
#include <set>
#include <algorithm>
#include <ctime>
#include <map>

using namespace std;

// [[STUB]]
float evaluationFunction(const set<int>& s) { // TODO: i'm assuming this will eventually take in a vector of set??
    return static_cast <float> (rand()) /
           (static_cast <float> (RAND_MAX / (100 - 0))); // return random number between 0 and 100
};

// Contains a mapping of [KEY: set] [VALUE: that set's accuracy]
// INFO: This map uses a set<int> (subset of features) to find a float (accuracy %)
// Initializes with only the "no features" set.
map<set<int>, float> mapEvaluatedSets;

/*
float evaluationFunction(set<int> s){ //i'm assuming this will eventually take in a vector of set??
    int sum = 0;
    for (int val : s)
    {
        sum += val;
    }
    return static_cast<float>(sum);
};
 */

// Prints a set
void printSet(const set<int>& s) {
    if(s.empty()) {
        cout << "{}";
    } else {
        cout << "{ ";
        for (int feature: s) {
            cout << feature << " ";
        }
        cout << "}";
    }

}

set<int> forwardSelectionAlgorithm(const set<int>& s) {
    if (s.empty()) {return {};}

    // INFO: we use set because sets cannot have duplicates and is auto sorted
    set<int> setGlobalHighest = {};
    set<int> setLocalHighest;

    // LOOP 1 : Iterate for N number of features
    for (int i = 1; i <= s.size(); i++) {
        setLocalHighest.clear();

        // LOOP 2 : Iterate for N number of features
        for (auto feature: s) { // INFO: auto keyword - smartly fills in type based on the variable given (in this case s is int)
            /*
             *  Current set in the iteration.
             *      -> contains the highest accuracy subset so far +1 feature
             */
            set<int> set = setGlobalHighest;
            set.insert(feature);

            if (mapEvaluatedSets.find(set) == mapEvaluatedSets.end()) { // The set "set" does not exist in the mapping "mapEvaluatedSets". We haven't mapped it yet.
                float a = evaluationFunction(set);
                mapEvaluatedSets[set] = a; // Create a new entry in the map

                // PRINT: a single set with its a -----------
                cout << "\tUsing features ";
                printSet(set);
                cout << " accuracy is " << a << "%\n";
                // ------------------------------------------

                /*
                 *  First iteration - assigns setLocalHighest to first set found
                 *  All other iterations - if accuracy is greater than the current best accuracy, make that accuracy the new highest
                 */
                if (setLocalHighest.empty() || a > mapEvaluatedSets[setLocalHighest]) {
                    setLocalHighest = set;
                }
            }
        }

        // PRINT: selection of best set (each "step") in the algorithm -----------
        cout << "Feature set ";
        printSet(setLocalHighest);
        cout << " was best, accuracy is " << mapEvaluatedSets[setLocalHighest] << "%\n";
        // -----------------------------------------------------------------------

        // Can't climb any further. setLocalHighest has a smaller accuracy AKA All the sets from the operation results have a smaller accuracy.
        if (mapEvaluatedSets[setLocalHighest] < mapEvaluatedSets[setGlobalHighest]) {
            break;
        }

        setGlobalHighest = setLocalHighest; // setLocalHighest has a larger accuracy. update setGlobalHighest.
    }
    return setGlobalHighest;
};

set<int> backwardsSelectionAlgorithm(const set<int>& s) {
    if (s.empty()) {return {};}

    set<int> setGlobalHighest = s;
    set<int> setLocalHighest;

    // LOOP 1
    for (int i = 1; i <= s.size(); i++) {
        setLocalHighest.clear();

        // LOOP 2
        for (int j = 1; j <= s.size(); j++) {
            /*
             *  Current set in the iteration.
             *      -> contains the highest accuracy subset so far +1 feature
             */
            set<int> set = setGlobalHighest;
            set.erase(j);

            if (mapEvaluatedSets.find(set) == mapEvaluatedSets.end()) { // The set "set" does not exist in the mapping "mapEvaluatedSets". We haven't mapped it yet.
                float a = evaluationFunction(set);
                mapEvaluatedSets[set] = a; // Create a new entry in the map

                // PRINT: a single set with its a -----------
                cout << "\tUsing features ";
                printSet(set);
                cout << " accuracy is " << a << "%\n";
                // ------------------------------------------

                if (setLocalHighest.empty() || a > mapEvaluatedSets[setLocalHighest]) {
                    setLocalHighest = set;
                }
            }
        }

        // PRINT: selection of best set (each "step") in the algorithm -----------
        cout << "Feature set ";
        printSet(setLocalHighest);
        cout << " was best, accuracy is " << mapEvaluatedSets[setLocalHighest] << "%\n";
        // -----------------------------------------------------------------------

        // Can't climb any further. setLocalHighest has a smaller accuracy AKA All the sets from the operation results have a smaller accuracy.
        if (mapEvaluatedSets[setLocalHighest] < mapEvaluatedSets[setGlobalHighest]) {
            break;
        }

        setGlobalHighest = setLocalHighest; // setLocalHighest has a larger accuracy. update setGlobalHighest.
    }
    return setGlobalHighest;
};

int main() {
    srand(time(nullptr));
    int choice = -1;

    while (choice != 0) {
        /*
         *  USER INPUT : Number of features
         */
        cout << "Welcome to the Feature Selection Algorithm.\n"
            << "Please enter the total number of features: ";

        cin >> choice;
        set<int> setFeatures;

        for (int i = 1; i <= choice; i++) {
            setFeatures.insert(i);
        }

        /*
         *  USER INPUT : Algorithm selection
         */
        cout << "Type the number of the algorithm you want to run.\n"
            << "1. Forward Selection\n"
            << "2. Backward Elimination\n"
            << "0. to Exit\n";

        cin >> choice;
        set<int> answer;

        switch (choice) {
            case 1: // Forward Selection
                mapEvaluatedSets = {{{}, evaluationFunction({})}}; // initialize map with root node

                cout << "Root node: {}, Accuracy: "
                     << mapEvaluatedSets[{}] << "%\n";

                answer = forwardSelectionAlgorithm(setFeatures);

                cout << "The overall best feature selection is: ";
                printSet(answer);
                cout << "\n";
                break;

            case 2: // Backward Elimination
                mapEvaluatedSets = {{setFeatures, evaluationFunction(setFeatures)}}; // initialize map with root node

                cout << "Root node: ";
                printSet(setFeatures);
                cout << ", Accuracy: " << mapEvaluatedSets[setFeatures] << "%\n";

                answer = backwardsSelectionAlgorithm(setFeatures);

                cout << "The overall best feature selection is: ";
                printSet(answer);
                cout << "\n";
                break;

            default:
                cout << "Invalid choice.\n";
                break;
        }
    }
}