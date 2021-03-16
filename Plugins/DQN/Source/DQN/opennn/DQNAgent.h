#include "DQNReplayMemory.h"
#include <fstream>
#include <vector>
#include "opennn.h"

using namespace OpenNN;

class DQNAgent
{
private:
    bool learn;

    int currentTick = 0;
    int batchSize = 128;
    int capasity = 1000;
    int updateNNSize = 1600;


    NeuralNetwork* targetNN;
    NeuralNetwork* mainNN;
    MeanSquaredError* mse;
    AdaptiveMomentEstimation* adam;



    double beta1 = 0.9;
    double beta2 = 0.99;
    double epsilon = 1.e-7;

    string mainfilePath = "MainNN.xml";
    string targetfilePath = "TargetNN.xml";
    int percentError;
    DQNReplayMemory* dqnMemory;
    void CreateModels();
    int GetNNAnswer(vector<double> inputs);
    int EpsilonFunction(vector<double> inputs);
    void TrainStep();

public:
    DQNAgent(bool inLearn);
    ~DQNAgent();
    void AddWorldAnswer(vector<double> inputs, double reward);
    int GetAction(vector<double> inputs);
};

