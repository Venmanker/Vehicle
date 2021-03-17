#include <random>
#include <ctime>
#include <vector>
#include "DQNMemory.h"

using namespace std;

class DQNReplayMemory
{
private:
    AgentMemory* currentMemory;
    vector<AgentMemory*> replayMemory;
public:
    int Lenght();
    DQNReplayMemory();
    DQNReplayMemory* AddAgentAction(vector<double> inState, int inAction);
    DQNReplayMemory* AddWorldAnswer(vector<double> inNextState, double inReward);
    vector<AgentMemory*> SampleMemory(int size);
    ~DQNReplayMemory();
};


