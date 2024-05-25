#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <time.h>
#include <map>
#include <ctime>
#include <vector>
#include <sstream>
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
        string filename;
        cout << "Welcome to the Feature Selection Algorithm.\n";
        cout << "Type in the name of the file to test : ";
        cin >> filename;
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
        auto* dataset = readDataset(filename); //make sure file name is in same folder
        
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
