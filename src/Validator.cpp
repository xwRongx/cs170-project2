#include "../headers/Validator.h"
#include <cstdlib> //for the rand()
#include <map>


float Validator::evaluationFunction(vector<float> features, Classifier* classifier, map<Instance*, float>* dataset){
      float counter = 0;
      int n = dataset->size();
      //two ways this goes: we use the features, have some for loop that makes new insatcnes, thne we classify
      //or clasisifer sgeemnets automtically, then we chilll

      for (const auto& instancePair : *dataset) {
            Instance* testInstance = instancePair.first;
            map<Instance*, float> trainSet(*dataset);
            trainSet.erase(testInstance);

            classifier->train(&trainSet);

            float prediction = classifier->test(testInstance);

           if(prediction == testInstance->classLabel) counter++;
        }
      return counter/dataset->size();
}

Validator::~Validator() = default;

Validator::Validator() = default;


