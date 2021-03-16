#include "DQNReplayMemory.h"

DQNReplayMemory::DQNReplayMemory()
{
}


DQNReplayMemory* DQNReplayMemory::AddAgentAction(vector<double> inState, int inAction)
{
    currentMemory = new AgentMemory(inState, inAction);
    return this;
}

DQNReplayMemory* DQNReplayMemory::AddWorldAnswer(vector<double> inNextState, double inReward)
{
    if (currentMemory)
    {
        currentMemory->AddNextStateAndReward(inNextState, inReward);
        replayMemory.push_back(currentMemory);
    }
    return this;
}


vector<AgentMemory*> DQNReplayMemory::SampleMemory(int size)
{
    vector<AgentMemory*> result;
    srand(time(nullptr)); 
    for (int i = 0; i < size; i ++)
    {
        int needMemoryId = rand() % replayMemory.size();
        result.push_back(replayMemory[needMemoryId]);
    }
    replayMemory.clear();
    return result;
}

DQNReplayMemory::~DQNReplayMemory()
{
    replayMemory.clear();
}


int DQNReplayMemory::Lenght()
{
    return replayMemory.size();
}