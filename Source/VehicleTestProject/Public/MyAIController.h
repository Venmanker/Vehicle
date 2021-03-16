// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "MyAIVehicle.h"
//#include "DQN/lib/DQNAgent.a"
#include "MyAIController.generated.h"


/**
 * 
 */
UCLASS()
class VEHICLETESTPROJECT_API AMyAIController : public AAIController
{
	GENERATED_BODY()
protected:
	//DQNAgent* agent;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="AI Manager")
	void SetVehicleMovement(float ThrottleInput, float SteeringInput, bool HandbrakeInput);


public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI Manager")
	class UBehaviorTreeComponent* behaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI Manager")
	class UBehaviorTree* behaviorTree;

	UPROPERTY(BlueprintReadOnly, Category = "AI Manager")
	bool learn = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI Manager")
	class UBlackboardComponent* myBlackboard;

	UFUNCTION(BlueprintCallable, Category="AI Manager")
	int GetNNResult(TArray<float> inputs, float currentRevard);

	UFUNCTION(BlueprintCallable, Category="AI Manager")
	void MoveVehicleUsingNN(TArray<float> inputs, float currentRevard);

	AMyAIController(FObjectInitializer const& objectInitializer = FObjectInitializer::Get());

	void OnPossess(APawn* const pawn) override;
	void EndPlay(const EEndPlayReason::Type endPlayReason) override;
};
