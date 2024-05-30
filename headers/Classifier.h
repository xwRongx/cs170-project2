#include <vector>
#include <map>
#include <cmath>
#include "Instance.h"
#include <float.h>

using namespace std;

class Classifier
{
private:

public:
    Classifier(/* args */);
    ~Classifier();

    vector<Instance*> dataset; //keeps all training instances

    //classifier functions
    void train(vector<Instance*> incomingDataSet); //kind of a setter
    float test(Instance* testInstance); //based on training data, return predicted class of testInstance

    //helper function
    float euclideanDistance(Instance* trainingInstance, Instance* testInstance);
};