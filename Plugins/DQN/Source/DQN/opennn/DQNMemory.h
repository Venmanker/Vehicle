#include <vector>

using namespace std;

class AgentMemory
{
public:
    vector<double> state, nextState;
    double reward;
    int action;

    AgentMemory(vector<double> inState, int inAction);
    AgentMemory* AddNextStateAndReward(vector<double> inNextState, double inReward);
    ~AgentMemory();
};