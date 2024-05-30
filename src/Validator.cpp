#include "../headers/Validator.h"
#include <cstdlib> //for the rand()
#include <map>


float Validator::evaluationFunction(vector<int> features, Classifier *classifier, vector<Instance *> dataset) {
    if (features.empty())
        return 0;

    float counter = 0;
    //two ways this goes: we use the featureValues, have some for loop that makes new insatcnes, thne we classify
    //or clasisifer sgeemnets automtically, then we chilll

    vector<Instance *> trainSet(dataset); // Copy dataset to new training set
    for (int i = dataset.size() - 1; i > 0; i--) {
        Instance *testInstance = trainSet[i]; // Save the current testing instance
        trainSet.pop_back(); // Remove the testInstance from the training set

        classifier->train(trainSet); // Train off the training set

        float prediction = classifier->test(features, testInstance);

        if (prediction == testInstance->classLabel) counter++;
    }
    return counter / dataset.size() * 100;
}

Validator::~Validator() = default;

Validator::Validator() = default;


