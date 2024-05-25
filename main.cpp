#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <map>
#include "headers/Validator.h"
#include "headers/Instance.h"
#include <ctime>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;


vector<vector<float>*>* readDataset(const string& filename) { 
    ifstream infile(filename); //opens file
    if (!infile.is_open()) {//checks if files exists
        cerr << "Error opening file: " << filename << endl;//will print error message if any issues occur
        exit(1);
    }
    string line;//will hold value
    auto* data = new vector<vector<float>*>; //creating a vector<float>* pointer
    //auto* keyword is used to declare a pointer type implicitly, so vector<vector<float>*>* data <==> auto* data :)
    int numFeatures = 0; // number of features excluding the first column
    int numInstances = 0; //number of instances (total rows)

    while (getline(infile, line)) { //will loop through file until empty
        stringstream ss(line); //getting single values
        auto* row = new vector<float>; //creating vector of type float to hold values
        float value; //float value will be stored here and appended to row vector
        numInstances++; //incrementing instance count
        while (ss >> value) { //looping though single line of data each iteration
            row->push_back(value); //pushing single float value until line is empty
            //cout << fixed << setprecision(8) << value << " ";
            //cout << scientific << value << " ";
        }
        //cout << numInstances << endl;
        //cout << endl;
        numFeatures = row->size(); //setting feature count
        data->push_back(row);//pushing row vector into data == 1 complete instance being pushed into data vector
    }
    numFeatures = numFeatures -1; //excluding first column

    infile.close(); //closing the file

    //Normalizing the data (X – mean(X))/std(x)
    for (auto* row : *data) { //looping through data rows, auto* row is a single row instance
        float mean = 0.0; 
        int n = row->size(); //total size of a row i.e 11 for small-test-dataset.txt
        for(int i = 1; i < n;++i ){ //adding up all values in for single row (excluding the row[0] since its the class label)
            mean += row->at(i);
        }
        mean = mean /(n-1); //calculating the mean for row
        
        float stddev = 0.0;
        for(int i = 1; i < n;++i){ //adding up values for stddev (row-val - mean)^2
            stddev += pow(row->at(i) - mean, 2);
        }
        stddev = sqrt(stddev / (n-1)); //stddev for row
        
        for(int i = 1; i < n;++i){ //X = (X – mean(X))/std(x) putting it all together
            row->at(i) = (row->at(i) - mean) / stddev;
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
    cout << "This dataset has " << numFeatures << " features (not including the class attribute), with " << numInstances << " instances." << endl;
    return data;
}

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
    for (auto i = s.begin(); i != s.end(); i++) {

        // LOOP 2 : Iterate for N number of s
        for (int j = 0; j < s.size(); j++) { // INFO: auto keyword - smartly fills in type based on the variable given (in this case s is int)
            /*
             *  Current instance in the iteration.
             *      -> contains the highest accuracy subset so far -1 feature
             */
            auto* instance = new Instance(*setGlobalHighest);
            instance->features.erase(remove(instance->features.begin(), instance->features.end(), j), instance->features.end());

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
        cout << "Welcome to the Feature Selection Algorithm.\n";
        cout << "Type in the name of the file to test : ";
        cin >> filename;
        cout << "Please enter the total number of features: ";
        int numFeatures;
        cin >> numFeatures;

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
        auto* dataset = readDataset(filename); //make sure file name is in same folder

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