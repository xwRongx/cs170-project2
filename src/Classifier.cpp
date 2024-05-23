#include "../headers/Classifier.h"

Classifier::Classifier(){
    dataset = nullptr;
}

//getter
vector<vector<float>*>* Classifier::getDataset(){
    if(dataset == nullptr){
        return nullptr;
    }else{
        return dataset;
    }
}

//classifier functions
void Classifier::train(vector<vector<float>*>* incomingDataSet){ //kind of a setter
    //class label for each instance is provided along w feature vector
    dataset = incomingDataSet;
}

//NN classifier
float Classifier::test(vector<float>* testInstance){
    //compute distance of testInstance to all training points (in dataset)
    vector<float>* closestInstance = nullptr;
    float closestInstanceSimilarity; //stores similarity value (calculated with euclidean) of closest training instance

    for(vector<float>* trainingInstance:*dataset){
        int euclideanDist = euclideanDistance(trainingInstance, testInstance);
        if(closestInstanceSimilarity < euclideanDist){
            closestInstanceSimilarity = euclideanDist;
            closestInstance = trainingInstance;
        }
    }
    return closestInstance->at(0); //return class label of nearest training point
}

//helper functions
//take in training and test instance, return float similarity value
float Classifier::euclideanDistance(vector<float>* trainingInstance, vector<float>* testInstance){
    //euclidean distance for n features, where n = size of inner vector

    return 0;
}
