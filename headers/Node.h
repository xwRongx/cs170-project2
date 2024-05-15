#include <vector>
#include <iostream>


class Node
{
private:
    std::vector<float> features;
    float accuracy;
public:
    Node(/* args */);
    ~Node();

    void displayNode();
    std::vector<Node> expand();
};

