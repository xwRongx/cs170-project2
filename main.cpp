#include <iostream>
#include <algorithm>
#include <ctime>
#include <map>
#include "headers/Validator.h"
#include "headers/Instance.h"

using namespace std;

auto validator = new Validator();
auto classifier = new Classifier();

// Prints a single features
void printFeatures(const vector<float> features) {
    if(features.empty()) {
        cout << "{}";
    } else {
        cout << "{ ";
        for (float feature: features) {
            cout << feature << " ";
        }
        cout << "}";
    }
}

vector<float> forwardSelectionAlgorithm(vector<float> s) {
    auto* rootNode = new Instance();
    // add root node to dataset
    classifier->dataset->insert({
        rootNode,
        validator->evaluationFunction(rootNode->features, classifier, classifier->dataset)
    });

    cout << "Root node: {}, Accuracy: "
         << classifier->dataset->at(rootNode) << "%\n";

    auto* setGlobalHighest = rootNode;
    Instance* setLocalHighest = rootNode;

    // LOOP 1 : Iterate for N number of s
    for (auto i = s.begin(); i < s.end(); i++) {

        // LOOP 2 : Iterate for N number of s
        for (auto feature: s) { // INFO: auto keyword - smartly fills in type based on the variable given (in this case s is int)
            /*
             *  Current instance in the iteration.
             *      -> contains the highest accuracy subset so far +1 feature
             */
            auto* instance = new Instance(*setGlobalHighest);
            instance->features.push_back(feature);

            if (classifier->dataset->find(instance) == classifier->dataset->end()) { // The instance "instance" does not exist in the mapping "dataset". We haven't mapped it yet.
                float a = validator->evaluationFunction(instance->features, classifier, classifier->dataset);
                classifier->dataset->insert({instance, a}); // Create a new entry in the map

                // PRINT: a single instance with its a -----------
                cout << "\tUsing features ";
                printFeatures(instance->features);
                cout << " accuracy is " << a << "%\n";
                // ------------------------------------------

                /*
                 *  if accuracy of instance is greater than the current best instance's accuracy, make that instance the new highest
                 */
                if (a > classifier->dataset->at(setLocalHighest)) {
                    setLocalHighest = instance;
                }
            }
        }

        // PRINT: selection of best set (each "step") in the algorithm -----------
        cout << "Feature set ";
        printFeatures(setLocalHighest->features);
        cout << " was best, accuracy is " << classifier->dataset->at(setLocalHighest) << "%\n";
        // -----------------------------------------------------------------------

        // Can't climb any further. setLocalHighest has a smaller accuracy AKA All the sets from the operation results have a smaller accuracy.
        if (classifier->dataset->at(setLocalHighest) < classifier->dataset->at(setGlobalHighest)) {
            break;
        }

        setGlobalHighest = setLocalHighest; // setLocalHighest has a larger accuracy. update setGlobalHighest.
    }
    return setGlobalHighest->features;
};

vector<float> backwardsSelectionAlgorithm(vector<float> s) {
    auto* rootNode = new Instance();
    rootNode->features = s;
    // add root node to dataset
    classifier->dataset->insert({
        rootNode,
        validator->evaluationFunction(rootNode->features, classifier, classifier->dataset)
    });

    cout << "Root node: ";
    printFeatures(rootNode->features);
    cout << ", Accuracy: " << classifier->dataset->at(rootNode) << "%\n";

    auto* setGlobalHighest = rootNode;
    Instance* setLocalHighest = rootNode;

    // LOOP 1 : Iterate for N number of s
    for (auto i = s.begin(); i < s.end(); i++) {

        // LOOP 2 : Iterate for N number of s
        for (auto j = s.begin(); j < s.end(); j++) { // INFO: auto keyword - smartly fills in type based on the variable given (in this case s is int)
            /*
             *  Current instance in the iteration.
             *      -> contains the highest accuracy subset so far -1 feature
             */
            auto* instance = new Instance(*setGlobalHighest);
            instance->features.erase(j);

            if (classifier->dataset->find(instance) == classifier->dataset->end()) { // The instance "instance" does not exist in the mapping "dataset". We haven't mapped it yet.
                float a = validator->evaluationFunction(instance->features, classifier, classifier->dataset);
                classifier->dataset->insert({instance, a}); // Create a new entry in the map

                // PRINT: a single instance with its a -----------
                cout << "\tUsing features ";
                printFeatures(instance->features);
                cout << " accuracy is " << a << "%\n";
                // ------------------------------------------

                /*
                 *  First iteration - assigns setLocalHighest to first instance found
                 *  All other iterations - if accuracy is greater than the current best accuracy, make that accuracy the new highest
                 */
                if (a > classifier->dataset->at(setLocalHighest)) {
                    setLocalHighest = instance;
                }
            }
        }

        // PRINT: selection of best set (each "step") in the algorithm -----------
        cout << "Feature set ";
        printFeatures(setLocalHighest->features);
        cout << " was best, accuracy is " << classifier->dataset->at(setLocalHighest) << "%\n";
        // -----------------------------------------------------------------------

        // Can't climb any further. setLocalHighest has a smaller accuracy AKA All the sets from the operation results have a smaller accuracy.
        if (classifier->dataset->at(setLocalHighest) < classifier->dataset->at(setGlobalHighest)) {
            break;
        }

        setGlobalHighest = setLocalHighest; // setLocalHighest has a larger accuracy. update setGlobalHighest.
    }
    return setGlobalHighest->features;
};

int main() {
    srand(time(nullptr));
    int choice = -1;

    while (choice != 0) {
        classifier->dataset = new map<Instance*, float>();  // initialize map with root node

        /*
         *  USER INPUT : Number of features
         */
        cout << "Welcome to the Feature Selection Algorithm.\n"
            << "Please enter the total number of features: ";

        cin >> choice;
        vector<float> features;

        for (int i = 1; i <= choice; i++) {
            features.push_back(i);
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
                answer = forwardSelectionAlgorithm(features);

                cout << "The overall best feature selection is: ";
                printFeatures(answer);
                cout << "\n";
                break;

            case 2: // Backward Elimination
                answer = backwardsSelectionAlgorithm(features);

                cout << "The overall best feature selection is: ";
                printFeatures(answer);
                cout << "\n";
                break;

            default:
                cout << "Invalid choice.\n";
                break;
        }
    }
}