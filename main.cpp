#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <map>
#include "headers/Validator.h"
#include "headers/Instance.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;

// Contains a mapping of [KEY: vector<float>] [VALUE: that set's accuracy]
// INFO: This map uses Instances (classifier, featureValues) to find a float (accuracy %)
// Initializes with only the "no featureValues" vector.
map<vector<int>*, float>* memoizedFeatures; //keeps all training instances


vector<Instance*> readDataset(const string& filename) {
    ifstream infile(filename); //opens file
    if (!infile.is_open()) {//checks if files exists
        cerr << "Error opening file: " << filename << endl;//will print error message if any issues occur
        exit(1);
    }
    string line;//will hold value
    vector<Instance*> data; //creating a vector<float>* pointer
    //auto* keyword is used to declare a pointer type implicitly, so vector<vector<float>*>* data <==> auto* data :)
    int numFeatures = 0; // number of featureValues excluding the first column
    int numInstances = 0; //number of instances (total rows)

    while (getline(infile, line)) { //will loop through file until empty
        stringstream ss(line); //getting single values
        auto* row = new Instance(); //creating vector of type float to hold values
        float value; //float value will be stored here and appended to row vector
        numInstances++; //incrementing instance count

        // Class label
        ss >> value;
        row->classLabel = value;

        while (ss >> value) { //looping though single line of data each iteration
            row->featureValues.push_back(value); //pushing single float value until line is empty
            //cout << fixed << setprecision(8) << value << " ";
            //cout << scientific << value << " ";
        }
        //cout << numInstances << endl;
        //cout << endl;
        numFeatures = row->featureValues.size(); //setting feature count
        data.push_back(row);//pushing row vector into data == 1 complete instance being pushed into data vector
    }

    infile.close(); //closing the file

    //Normalizing the data (X – mean(X))/std(x)
    int numberOfInstances = data.size();
    // j = # of columns | i = # of rows

    for (int j = 0; j < data.front()->featureValues.size(); j++) {// column iterator
        /*
         * CALCULATE MEAN
         */
        float mean = 0.0;
        for(int i = 0; i < numberOfInstances; i++){ //adding up all values in for single column
            mean += data[i]->featureValues.at(j);
        }
        mean = mean / numberOfInstances; //calculating the mean for the column

        /*
         * CALCULATE STANDARD DEVIATION
         */
        float stddev = 0.0;
        for(int i = 0; i < numberOfInstances; i++){ //adding up values for stddev (row-val - mean)^2
            stddev += pow(data[i]->featureValues.at(j) - mean, 2);
        }
        stddev = sqrt(stddev / numberOfInstances); //stddev for row

        /*
         * UPDATING ALL THE DATA WITH THEIR NEW VALUES
         */
        for (auto* row : data) { //looping through data rows, auto* row is a single row instance
            row->featureValues.at(j) = (row->featureValues.at(j) - mean) / stddev; //X = (X – mean(X))/std(x) putting it all together
            //cout << fixed << setprecision(8) << row->at(i) << " ";
        }
    }

    /*
    //checking if values were properly updated after normalizing
    for (auto* row : *data){
        for(int i = 0; i < row->size();++i){ //X = (X – mean(X))/std(x)
        cout << fixed << setprecision(8) << row->at(i) << " ";
        }
        cout << endl;
    }*/


    cout << "This dataset has " << numFeatures << " featureValues (not including the class attribute), with " << numInstances << " instances." << endl;
    return data;
}

auto validator = new Validator();
auto classifier = new Classifier();
vector<Instance*> dataset;

// Prints a single featureValues
void printFeatures(const vector<int> features) {
    // TODO: sort features low to high before printing

    if(features.empty()) {
        cout << "{}";
    } else {
        cout << "{ ";
        for (int feature: features) {
            cout << feature+1 << " ";
        }
        cout << "}";
    }
}

vector<int> forwardSelectionAlgorithm(int featureCount) {
    auto rootNode = new vector<int>;
    // add root node to memoizedFeatures
    memoizedFeatures->insert({
        rootNode,
        validator->evaluationFunction(*rootNode, classifier, dataset)
    });

    cout << "Root node: {}, Accuracy: "
         << memoizedFeatures->at(rootNode) << "%\n";

    auto setGlobalHighest = rootNode;
    auto setLocalHighest = rootNode;

    // LOOP 1 : Iterate for N number of s
    for (int i = 0; i < featureCount; i++) {

        // LOOP 2 : Iterate for N number of s
        for (int j = 0; j < featureCount; j++) { // INFO: auto keyword - smartly fills in type based on the variable given (in this case s is int)
            /*
             *  Current featureList in the iteration.
             *      -> contains the highest accuracy subset so far +1 feature
             */
            auto featureList = new vector<int>(*setGlobalHighest);

            if(find(featureList->begin(), featureList->end(), j) != featureList->end()) // featureList contains feature j already, skip it, no duplicate features allowed
                continue;

            featureList->push_back(j);

            if (memoizedFeatures->find(featureList) == memoizedFeatures->end()) { // The featureList "featureList" does not exist in the mapping "dataset". We haven't mapped it yet.
                float a = validator->evaluationFunction(*featureList, classifier, dataset);
                memoizedFeatures->insert({featureList, a}); // Create a new entry in the map

                // PRINT: a single featureList with its a -----------
                cout << "\tUsing feature(s) ";
                printFeatures(*featureList);
                cout << " accuracy is " << a << "%\n";
                // ------------------------------------------

                /*
                 *  if accuracy of featureList is greater than the current best featureList's accuracy, make that featureList the new highest
                 */
                if (a > memoizedFeatures->at(setLocalHighest)) {
                    setLocalHighest = featureList;
                }
            }
        }

        // PRINT: selection of best set (each "step") in the algorithm -----------
        cout << "Feature set ";
        printFeatures(*setLocalHighest);
        cout << " was best, accuracy is " << memoizedFeatures->at(setLocalHighest) << "%\n";
        // -----------------------------------------------------------------------

        // Can't climb any further. setLocalHighest has a smaller accuracy AKA All the sets from the operation results have a smaller accuracy.
        if (memoizedFeatures->at(setLocalHighest) <= memoizedFeatures->at(setGlobalHighest)) {
            break;
        }

        setGlobalHighest = setLocalHighest; // setLocalHighest has a larger accuracy. update setGlobalHighest.
    }
    return *setGlobalHighest;
};

vector<int> backwardsSelectionAlgorithm(int featureCount) {
    // create root node
    auto rootNode = new vector<int>;
    for(int k = 0; k < featureCount; k++) {
        rootNode->push_back(k);
    }

    // add root node to memoizedFeatures
    memoizedFeatures->insert({
        rootNode,
        validator->evaluationFunction(*rootNode, classifier, dataset)
    });

    cout << "Root node: ";
    printFeatures(*rootNode);
    cout <<   ", Accuracy: " << memoizedFeatures->at(rootNode) << "%\n";

    auto setGlobalHighest = rootNode;
    auto setLocalHighest = rootNode;

    // LOOP 1 : Iterate for N number of s
    for (int i = 0; i < featureCount; i++) {

        auto* featureList = new vector<int>(*setGlobalHighest);
        featureList->erase(remove(featureList->begin(), featureList->end(), i), featureList->end()); // remove one feature
        // LOOP 2 : Iterate for N number of s
        for (int j = 0; j < featureList->size(); j++) { // INFO: auto keyword - smartly fills in type based on the variable given (in this case s is int)

            if (memoizedFeatures->find(featureList) == memoizedFeatures->end()) { // The featureList "featureList" does not exist in the mapping "dataset". We haven't mapped it yet.
                float a = validator->evaluationFunction(*featureList, classifier, dataset);
                memoizedFeatures->insert({featureList, a}); // Create a new entry in the map

                // PRINT: a single featureList with its a -----------
                cout << "\tUsing feature(s) ";
                printFeatures(*featureList);
                cout << " accuracy is " << a << "%\n";
                // ------------------------------------------

                /*
                 *  if accuracy of featureList is greater than the current best featureList's accuracy, make that featureList the new highest
                 */
                if (a > memoizedFeatures->at(setLocalHighest)) {
                    setLocalHighest = featureList;
                }
            }
        }

        // PRINT: selection of best set (each "step") in the algorithm -----------
        cout << "Feature set ";
        printFeatures(*setLocalHighest);
        cout << " was best, accuracy is " << memoizedFeatures->at(setLocalHighest) << "%\n";
        // -----------------------------------------------------------------------

        // Can't climb any further. setLocalHighest has a smaller accuracy AKA All the sets from the operation results have a smaller accuracy.
        if (memoizedFeatures->at(setLocalHighest) <= memoizedFeatures->at(setGlobalHighest)) {
            break;
        }

        setGlobalHighest = setLocalHighest; // setLocalHighest has a larger accuracy. update setGlobalHighest.
    }
    return *setGlobalHighest;
}

int main() {
    srand(time(nullptr));
    int choice = -1;

    while (choice != 0) {
        string filename;
        memoizedFeatures = new map<vector<int>*, float>();  // initialize map with root node

        /*
         *  USER INPUT : File name
         */
        cout << "Welcome to the Feature Selection Algorithm.\n";
        cout << "Type in the name of the file to test: ";
        cin >> filename;

        if(filename == "d")
            filename = "../small-test-dataset.txt";

        dataset = readDataset(filename); //make sure file name is in same folder

        /*
         *  USER INPUT : Algorithm selection
         */
        cout << "Type the number of the algorithm you want to run.\n"
            << "1. Forward Selection\n"
            << "2. Backward Elimination\n"
            << "0. to Exit\n";

        cin >> choice;
        vector<int> answer;

        switch (choice) {
            case 1: // Forward Selection
                answer = forwardSelectionAlgorithm(dataset[0]->featureValues.size());

                cout << "The overall best feature selection is: ";

                printFeatures(answer);
                cout << "\n";
                break;

            case 2: // Backward Elimination
                answer = backwardsSelectionAlgorithm(dataset[0]->featureValues.size());

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
