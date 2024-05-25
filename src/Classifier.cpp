#include "../headers/Classifier.h"

Classifier::Classifier(){
    dataset = nullptr;
}

//classifier functions
void Classifier::train(map<Instance, float>* incomingDataSet){ //kind of a setter
    //class label for each instance is provided along w feature vector
    dataset = incomingDataSet;
}

//NN classifier
float Classifier::test(Instance* testInstance){
    //compute distance of testInstance to all training points (in dataset)
    Instance closestInstance = new Instance();
    float closestInstanceSimilarity; //stores similarity value (calculated with euclidean) of the closest training instance

    for(auto instance : *dataset){
        float euclideanDist = euclideanDistance(instance.first /*instance's Instance*/, *testInstance);
        if(closestInstanceSimilarity < euclideanDist){
            closestInstanceSimilarity = euclideanDist;
            closestInstance = instance.first;
        }
    }
    return closestInstance.classLabel; //return class label of the nearest training point
}

//helper functions
//take in training and test instance, return float similarity value
float Classifier::euclideanDistance(Instance trainingInstance, Instance testInstance){
    //euclidean distance for n features, where n = size of inner vector
    return 0;
}

Classifier::~Classifier() = default;
