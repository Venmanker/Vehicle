// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "Components/SkeletalMeshComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "MyFrontWheel.h"
#include "MyBackWheel.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "DrawDebugHelpers.h"
#include "MyAIController.h"
#include "Containers/Array.h"
#include "Engine/TargetPoint.h"
#include "MyAIVehicle.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLETESTPROJECT_API AMyAIVehicle : public AWheeledVehicle
{
	GENERATED_BODY()

private:

	AMyAIVehicle();

public:

	UFUNCTION(BlueprintCallable, Category = "Exploring the environment")
	void GetLineTraceForward(bool &bHit, FVector &FNeedLocation);

	UFUNCTION(BlueprintCallable, Category="Exploring the environment")
	ATargetPoint* GetNewTargetPoint();



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Overlap")
	FVector FCollisionBoxSize = {230, 110, 74};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exploring the environment")
	float LineTraceLengthByForward = 2200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool bDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Exploring the environment")
	TArray<ATargetPoint*> ATargetPointList;

protected:

	virtual void BeginPlay() override;
	
	
};
