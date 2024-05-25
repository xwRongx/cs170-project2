#include <vector>
#include <map>
#include "Instance.h"

using namespace std;

class Classifier
{
private:
public:
    Classifier(/* args */);
    ~Classifier();

    // Contains a mapping of [KEY: vector<float>] [VALUE: that set's accuracy]
    // INFO: This map uses Instances (classifier, features) to find a float (accuracy %)
    // Initializes with only the "no features" vector.
    map<Instance, float>* dataset; //keeps all training instances

    //classifier functions
    void train(map<Instance, float>* incomingDataSet); //kind of a setter
    float test(Instance* testInstance); //based on training data, return predicted class of testInstance

    //helper function
    float euclideanDistance(Instance trainingInstance, Instance testInstance);
};