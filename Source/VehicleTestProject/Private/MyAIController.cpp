// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAIController.h"
#include <vector>
void AMyAIController::BeginPlay()
{
    Super::BeginPlay();
    RunBehaviorTree(behaviorTree);
    behaviorTreeComponent->StartTree(*behaviorTree);
    //agent = new DQNAgent(learn);
}

void AMyAIController::SetVehicleMovement(float ThrottleInput = 1.f, float SteeringInput = 0.f, bool HandbrakeInput = false)
{
	Cast<AMyAIVehicle>(GetPawn())->GetVehicleMovementComponent()->SetThrottleInput(ThrottleInput);
	Cast<AMyAIVehicle>(GetPawn())->GetVehicleMovementComponent()->SetSteeringInput(SteeringInput);
	Cast<AMyAIVehicle>(GetPawn())->GetVehicleMovementComponent()->SetHandbrakeInput(HandbrakeInput);
}

AMyAIController::AMyAIController(FObjectInitializer const& objectInitializer)
{
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/MyBehaviorTree.MyBehaviorTree'"));
    if (obj.Succeeded())
    {
        behaviorTree = obj.Object;
    }
    behaviorTreeComponent = objectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTree"));
    myBlackboard = objectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("Blackboard"));
}

void AMyAIController::OnPossess(APawn* const pawn)
{
    Super::OnPossess(pawn);
    if(Blackboard)
    {
        myBlackboard->InitializeBlackboard(*behaviorTree->BlackboardAsset);
    }
}

void AMyAIController::MoveVehicleUsingNN(TArray<float> inputs, float currentRevard)
{
    int result = GetNNResult(inputs, currentRevard);
    SetVehicleMovement(result % 3 - 1, (result / 3) % 3 - 1, result / 9);
}


int AMyAIController::GetNNResult(TArray<float> inputs, float currentRevard = 0)
{
    std::vector<double> vecInputs;
    for(int i = 0; i < inputs.Num(); i ++)
    {
        vecInputs.push_back(inputs[i]);
    }
    if(learn)
    {
        //agent->AddWorldAnswer(vecInputs, currentRevard);
    } 
    return 0;//agent->GetAction(vecInputs);
}

void AMyAIController::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    //delete agent;
}