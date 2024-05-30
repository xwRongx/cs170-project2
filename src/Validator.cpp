#include "../headers/Validator.h"
#include <cstdlib> //for the rand()
#include <map>


float Validator::evaluationFunction(vector<int>* features, Classifier* classifier, vector<Instance*> dataset){
      float counter = 0;
      int n = dataset.size();
      //two ways this goes: we use the featureValues, have some for loop that makes new insatcnes, thne we classify
      //or clasisifer sgeemnets automtically, then we chilll

      // TODO: incorporate features into this. only the features passed through should be used for evaluation across all instances in the dataset

      for (int i = 0; i < dataset.size(); i++) {
            vector<Instance*> trainSet(dataset); // Copy dataset to new training set
            Instance* testInstance = trainSet[i]; // Save the current testing instance
            trainSet.pop_back(); // Remove the testInstance from the training set

            classifier->train(trainSet); // Train off the training set

            float prediction = classifier->test(testInstance);

           if(prediction == testInstance->classLabel) counter++;
        }
      return counter/dataset.size();
}

Validator::~Validator() = default;

Validator::Validator() = default;


