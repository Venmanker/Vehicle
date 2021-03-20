// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIVehicle.h"
#include "Engine/TargetPoint.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyAIController.generated.h"


/**
 * 
 */
UCLASS()
class VEHICLETESTPROJECT_API AMyAIController : public AAIController
{
	GENERATED_BODY()

private:
	bool TurnRight = false;
	bool TurnLeft = false;

	FVector NeedLocation= FVector::ZeroVector;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;

	UFUNCTION(BlueprintCallable, Category="AI Manager")
	void SetVehicleMovement(float ThrottleInput, float SteeringInput, bool HandbrakeInput);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Exploring the environment")
	ATargetPoint* ACurrentTargetPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Exploring the environment")
	float AcceptableRadius = 100.f;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Exploring the environment")
	float VehicleSpeed = 0.75;


	AMyAIController();
};
