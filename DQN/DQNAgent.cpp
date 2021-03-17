#include "DQNAgent.h"
#include <iostream>


void DQNAgent::CreateModels()
{
    ifstream mfs(mainfilePath);
    ifstream tfs(targetfilePath);
    if (mfs.good() && tfs.good())
    {
        targetNN = new NeuralNetwork();
        mainNN = new NeuralNetwork();

        targetNN = new NeuralNetwork();
        mainNN = new NeuralNetwork();
        auto unl = new UnscalingLayer(20);
        unl->set_unscaling_method(UnscalingLayer::NoUnscaling);
        mainNN->add_layer(unl);
        mainNN->add_layer(new PerceptronLayer(20, 19, PerceptronLayer::ActivationFunction::RectifiedLinear));
        mainNN->add_layer(new PerceptronLayer(19, 19, PerceptronLayer::ActivationFunction::RectifiedLinear));
        mainNN->add_layer(new PerceptronLayer(19, 18, PerceptronLayer::ActivationFunction::Linear));


        unl = new UnscalingLayer(20);
        unl->set_unscaling_method(UnscalingLayer::NoUnscaling);
        targetNN->add_layer(unl);
        targetNN->add_layer(new PerceptronLayer(20, 19, PerceptronLayer::ActivationFunction::RectifiedLinear));
        targetNN->add_layer(new PerceptronLayer(19, 19, PerceptronLayer::ActivationFunction::RectifiedLinear));
        targetNN->add_layer(new PerceptronLayer(19, 18, PerceptronLayer::ActivationFunction::Linear));

        mainNN->load_parameters(mainfilePath);
        targetNN->load_parameters(targetfilePath);
    }
    else
    {
        targetNN = new NeuralNetwork();
        mainNN = new NeuralNetwork();
        auto unl = new UnscalingLayer(20);
        unl->set_unscaling_method(UnscalingLayer::NoUnscaling);
        mainNN->add_layer(unl);
        mainNN->add_layer(new PerceptronLayer(20, 19, PerceptronLayer::ActivationFunction::RectifiedLinear));
        mainNN->add_layer(new PerceptronLayer(19, 19, PerceptronLayer::ActivationFunction::RectifiedLinear));
        mainNN->add_layer(new PerceptronLayer(19, 18, PerceptronLayer::ActivationFunction::Linear));

        unl = new UnscalingLayer(20);
        unl->set_unscaling_method(UnscalingLayer::NoUnscaling);
        targetNN->add_layer(unl);
        targetNN->add_layer(new PerceptronLayer(20, 19, PerceptronLayer::ActivationFunction::RectifiedLinear));
        targetNN->add_layer(new PerceptronLayer(19, 19, PerceptronLayer::ActivationFunction::RectifiedLinear));
        targetNN->add_layer(new PerceptronLayer(19, 18, PerceptronLayer::ActivationFunction::Linear));
    

        mainNN->randomize_parameters_normal();
        targetNN->randomize_parameters_normal();

        mainNN->save_parameters(mainfilePath);
        targetNN->save_parameters(targetfilePath);
    }
    


    mse = new MeanSquaredError(mainNN);
}


int DQNAgent::GetNNAnswer(vector<double> input)
{
    Tensor<double> inputs(input.size());
    for(int i = 0; i < input.size(); i ++)
    {
        inputs[i] = input[i];
    }
    return maximal_index(mainNN->calculate_outputs(inputs));
}

DQNAgent::DQNAgent(bool inLearn)
{
    learn = inLearn;
    if(learn) 
    {
        dqnMemory = new DQNReplayMemory();
    }
    CreateModels();
}

DQNAgent::~DQNAgent()
{
    if (learn)
    {
        delete dqnMemory;
    }

    targetNN->save_parameters(targetfilePath);
    mainNN->save_parameters(mainfilePath);
    delete targetNN;
    delete mainNN;
    delete mse;
}

void DQNAgent::AddWorldAnswer(vector<double> inputs, double reward)
{
    currentTick++;
    int currentSize = dqnMemory->AddWorldAnswer(inputs, reward)->Lenght();
    if (currentSize == capasity)
    {
        TrainStep();
    }
    if (currentTick == updateNNSize)
    {
        currentTick = 0;
        
        targetNN->set_parameters(mainNN->get_parameters());
        targetNN->save_parameters(targetfilePath);
    }
}

void DQNAgent::TrainStep()
{
    vector<AgentMemory*> sample = dqnMemory->SampleMemory(batchSize);

    auto param_num = mainNN->get_parameters_number();
    auto last_gradient_exponential_decay = Vector<double>(param_num, 0.0);
    auto last_square_gradient_exponential_decay = Vector<double>(param_num, 0.0);

    for(int i = 0; i < batchSize; i++)
    {
        Tensor<double> nextStateTensor(20);
        Tensor<double> stateTensor(20);

        for (int j = 0; j < sample[i]->state.size(); j++)
        {
            nextStateTensor[j] = sample[i]->nextState[j];
            stateTensor[j] = sample[i]->state[j];
        }
        auto targetMini = targetNN->calculate_outputs(nextStateTensor) * 0.99 + sample[i]->reward;
        auto forward = mainNN->calculate_trainable_forward_propagation(stateTensor);
        auto out = Tensor<double>(18);
        out[sample[i]->action] = targetNN->calculate_outputs(stateTensor)[sample[i]->action];
        Tensor<double> qwe = (out - targetMini) * 2.0 / (double)batchSize;

        auto a = mse->calculate_layers_delta(forward, qwe);
        auto batch_error_gradient = mse->calculate_error_gradient(stateTensor, forward, a);
        auto learning_rate = 0.001 *sqrt(1.0 - pow(beta2, i + 1))/(1.0 - pow(beta1, i + 1));
        auto params = mainNN->get_parameters();


        auto gradient_exponential_decay = last_gradient_exponential_decay* beta1 + batch_error_gradient * (1 - beta1);
        last_gradient_exponential_decay = gradient_exponential_decay;
        auto square_gradient_exponential_decay = last_square_gradient_exponential_decay * beta2 + batch_error_gradient * batch_error_gradient * (1 - beta2);
        last_square_gradient_exponential_decay = square_gradient_exponential_decay;
        mainNN->set_parameters(params - gradient_exponential_decay*learning_rate/(square_root(square_gradient_exponential_decay)+ epsilon));

    }
    mainNN->save_parameters(mainfilePath);
    
}

int DQNAgent::GetAction(vector<double> inputs)
{
    int nNAnswer;
    if(learn)
    {
        nNAnswer = EpsilonFunction(inputs);
        dqnMemory->AddAgentAction(inputs, nNAnswer);
    }
    else
    {
        nNAnswer = GetNNAnswer(inputs);
    }
    return nNAnswer;
}


int DQNAgent::EpsilonFunction(vector<double> inputs)
{
    srand(time(nullptr));
    if (rand() % 100 < percentError)
    {
        return rand() % 18;
    }
    else
    {
        return GetNNAnswer(inputs);
    }
}
