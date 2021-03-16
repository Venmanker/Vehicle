#include "DQNMemory.h"

AgentMemory::AgentMemory(vector<double> inState, int inAction)
{
    state = inState;
    action = inAction;
}

AgentMemory* AgentMemory::AddNextStateAndReward(vector<double> inNextState, double inReward)
{
    nextState = inNextState;
    reward = inReward;
    return this;
}

AgentMemory::~AgentMemory()
{
    state.clear();
    nextState.clear();
}