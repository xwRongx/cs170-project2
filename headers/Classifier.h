#include <vector>
using namespace std;

class Classifier
{
private:
    vector<vector<float>*>* dataset; //keeps all training instances
public:
    Classifier(/* args */);
    ~Classifier();

    //getter
    vector<vector<float>*>* getDataset();

    //classifier functions
    void train(vector<vector<float>*>* incomingDataSet); //kind of a setter
    float test(vector<float>* testInstance); //based on training data, return predicted class of testInstance

    //helper function
    float euclideanDistance(vector<float>* trainingInstance, vector<float>* testInstance);
};