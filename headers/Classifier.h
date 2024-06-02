#include <vector>
#include <map>
#include <cmath>
#include "Instance.h"
#include <float.h>

using namespace std;

class Classifier
{
private:
    vector<Instance*> dataset; //keeps all training instances

public:
    Classifier(/* args */);
    ~Classifier();

    //classifier functions
    void train(vector<Instance*> incomingDataSet); //kind of a setter
    float test(vector<int> features, Instance* testInstance); //based on training data, return predicted class of testInstance

    //helper function
    float euclideanDistance(vector<int> features, Instance* trainingInstance, Instance* testInstance);
};