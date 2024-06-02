#include "../headers/Validator.h"


float Validator::evaluationFunction(vector<int> features, Classifier *classifier, vector<Instance *> dataset) {
    if (features.empty())
        return 0;

    float counter = 0;


    for (int i = 0; i < dataset.size(); i++) {
        vector<Instance *> trainSet(dataset); // Copy dataset to new training set
        Instance *testInstance = trainSet[i]; // Save the current testing instance
        trainSet.erase(trainSet.begin() + i); // Remove the testInstance from the training set

        classifier->train(trainSet); // Train off the training set

        float prediction = classifier->test(features, testInstance);

        if (prediction == testInstance->classLabel) counter++;
    }
    return 100 * (counter / dataset.size());
}

Validator::~Validator() = default;

Validator::Validator() = default;


