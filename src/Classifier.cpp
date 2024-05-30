#include "../headers/Classifier.h"


//classifier functions
void Classifier::train(vector<Instance*> incomingDataSet){ //kind of a setter
    //class label for each instance is provided along w feature vector
    dataset = incomingDataSet;
}

//NN classifier
float Classifier::test(vector<int> features, Instance* testInstance){
    auto* closestInstance = new Instance();
    float closestInstanceSimilarity = FLT_MAX; //stores similarity value (calculated with euclidean) of the closest training instance

    //go through dataset and find trainingInstance with the highest similarity value
    for(auto instance : dataset){
        float similarity = euclideanDistance(features, instance /*instance's Instance*/, testInstance);
        if(closestInstanceSimilarity > similarity){ //if new highest similarity, update closestInstance
            closestInstanceSimilarity = similarity;
            closestInstance = instance;
        }
    }
    return closestInstance->classLabel; //return class label of the nearest training point
}

//helper functions
//take in training and test instance, return float similarity value
float Classifier::euclideanDistance(vector<int> features, Instance* trainingInstance, Instance* testInstance){
    // TODO: figure out if this value is being calculated correctly.

    //euclidean distance for n featureValues, where n = size of inner vector
    float similarity = 0.0;

    //euclidean dist equation: similarity = sqrt((a1-a2)^2 + (b1-b2)^2 + (c1-c2)^2 + (d1-d2)^2...))
    for(int i = 0; i < features.size(); i++){
        similarity += pow(trainingInstance->featureValues.at(features[i]) - testInstance->featureValues.at(features[i]), 2);
    }
    similarity = sqrt(similarity);

    return similarity; //higher the similarity, more similiar the two instances are
}

Classifier::Classifier() = default;

Classifier::~Classifier() = default;
