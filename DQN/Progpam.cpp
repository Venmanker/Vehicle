#include "DQNAgent.h"

int main(int argc, char const *argv[])
{
    DQNAgent* a = new DQNAgent(true);

    Tensor<double> in(20);

    for (size_t i = 0; i < 160000; i++)
    {
        cout << i << endl;
        in.randomize_normal();
        a->AddWorldAnswer(in, random_normal());
        a->GetAction(in);
    }
    
    delete a;
    return 0;
}
