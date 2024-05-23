#include <iostream>
#include <algorithm>
#include <ctime>
#include <map>
#include "headers/Validator.h"
#include "headers/Instance.h"

using namespace std;

// Contains a mapping of [KEY: vector<float>] [VALUE: that set's accuracy]
// INFO: This map uses a vector<float>* (subset of features) to find a float (accuracy %)
// Initializes with only the "no features" vector.
map<Instance, float> dataset;

auto validator = new Validator();
auto classifier = new Classifier();

// Prints a single instance
void printInstance(const Instance& instance) {
    if(instance.features.empty()) {
        cout << "{}";
    } else {
        cout << "{ ";
        for (float feature: instance.features) {
            cout << feature << " ";
        }
        cout << "}";
    }

}

vector<float> forwardSelectionAlgorithm(const vector<float>& s) {
    if (s.empty()) {return {};}

    // INFO: we use set because sets cannot have duplicates and is auto sorted
    vector<float> setGlobalHighest = {};
    vector<float> setLocalHighest;

    // LOOP 1 : Iterate for N number of s
    for (int i = 1; i <= s.size(); i++) {
        setLocalHighest.clear();

        // LOOP 2 : Iterate for N number of s
        for (auto feature: s) { // INFO: auto keyword - smartly fills in type based on the variable given (in this case s is int)
            /*
             *  Current set in the iteration.
             *      -> contains the highest accuracy subset so far +1 feature
             */
            vector<float> set = setGlobalHighest;
            set.push_back(feature);

            if (dataset.find(set) == dataset.end()) { // The set "set" does not exist in the mapping "dataset". We haven't mapped it yet.
                float a = validator->evaluationFunction(set, classifier, dataset);
                dataset[set] = a; // Create a new entry in the map

                // PRINT: a single set with its a -----------
                cout << "\tUsing s ";
                printInstance(set);
                cout << " accuracy is " << a << "%\n";
                // ------------------------------------------

                /*
                 *  First iteration - assigns setLocalHighest to first set found
                 *  All other iterations - if accuracy is greater than the current best accuracy, make that accuracy the new highest
                 */
                if (setLocalHighest.empty() || a > dataset[setLocalHighest]) {
                    setLocalHighest = set;
                }
            }
        }

        // PRINT: selection of best set (each "step") in the algorithm -----------
        cout << "Feature set ";
        printInstance(setLocalHighest);
        cout << " was best, accuracy is " << dataset[setLocalHighest] << "%\n";
        // -----------------------------------------------------------------------

        // Can't climb any further. setLocalHighest has a smaller accuracy AKA All the sets from the operation results have a smaller accuracy.
        if (dataset[setLocalHighest] < dataset[setGlobalHighest]) {
            break;
        }

        setGlobalHighest = setLocalHighest; // setLocalHighest has a larger accuracy. update setGlobalHighest.
    }
    return setGlobalHighest;
};

vector<float> backwardsSelectionAlgorithm(const vector<float>& s) {
    if (s.empty()) {return {};}

    vector<float> setGlobalHighest = s;
    vector<float> setLocalHighest;

    // LOOP 1
    for (int i = 1; i <= s.size(); i++) {
        setLocalHighest.clear();

        // LOOP 2
        for (int j = 1; j <= s.size(); j++) {
            /*
             *  Current set in the iteration.
             *      -> contains the highest accuracy subset so far +1 feature
             */
            vector<float> set = setGlobalHighest;
            set.erase(j);

            if (dataset.find(set) == dataset.end()) { // The set "set" does not exist in the mapping "dataset". We haven't mapped it yet.
                float a = evaluationFunction(set);
                dataset[set] = a; // Create a new entry in the map

                // PRINT: a single set with its a -----------
                cout << "\tUsing features ";
                printInstance(set);
                cout << " accuracy is " << a << "%\n";
                // ------------------------------------------

                if (setLocalHighest.empty() || a > dataset[setLocalHighest]) {
                    setLocalHighest = set;
                }
            }
        }

        // PRINT: selection of best set (each "step") in the algorithm -----------
        cout << "Feature set ";
        printInstance(setLocalHighest);
        cout << " was best, accuracy is " << dataset[setLocalHighest] << "%\n";
        // -----------------------------------------------------------------------

        // Can't climb any further. setLocalHighest has a smaller accuracy AKA All the sets from the operation results have a smaller accuracy.
        if (dataset[setLocalHighest] < dataset[setGlobalHighest]) {
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
        vector<float> setFeatures;

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
        vector<float> answer;

        switch (choice) {
            case 1: // Forward Selection
                dataset = {{{}, evaluationFunction({})}}; // initialize map with root node

                cout << "Root node: {}, Accuracy: "
                     << dataset[{}] << "%\n";

                answer = forwardSelectionAlgorithm(setFeatures);

                cout << "The overall best feature selection is: ";
                printInstance(answer);
                cout << "\n";
                break;

            case 2: // Backward Elimination
                dataset = {{setFeatures, evaluationFunction(setFeatures)}}; // initialize map with root node

                cout << "Root node: ";
                printInstance(setFeatures);
                cout << ", Accuracy: " << dataset[setFeatures] << "%\n";

                answer = backwardsSelectionAlgorithm(setFeatures);

                cout << "The overall best feature selection is: ";
                printInstance(answer);
                cout << "\n";
                break;

            default:
                cout << "Invalid choice.\n";
                break;
        }
    }
}