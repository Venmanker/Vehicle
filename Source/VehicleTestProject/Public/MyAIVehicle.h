// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "Components/SkeletalMeshComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "MyFrontWheel.h"
#include "MyBackWheel.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "DrawDebugHelpers.h"
#include "MyAIController.h"
#include "MyAIVehicle.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLETESTPROJECT_API AMyAIVehicle : public AWheeledVehicle
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = "Overlap", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* collisionBox;

	UFUNCTION(Category = "Exploring the environment", BlueprintCallable)
	bool HaveOverlap();


	UFUNCTION(Category = "Exploring the environment", BlueprintCallable)
	TArray<float> GetNormalizedLineTraceResult();


	UPROPERTY(Category = "Overlap", EditDefaultsOnly, BlueprintReadOnly)
	FVector collisionBoxSize = {230, 110, 74};

	UPROPERTY(Category = "Overlap", EditDefaultsOnly, BlueprintReadOnly)
	FVector collisionBoxShift = {5, 0, 76};

	UPROPERTY(Category = "Exploring the environment", EditDefaultsOnly, BlueprintReadOnly)
	float lineTraceLenght = 2000;

	UPROPERTY(Category = "Exploring the environment", EditDefaultsOnly, BlueprintReadOnly)
	bool showDebug = true;

	UPROPERTY(Category = "Exploring the environment", BlueprintReadOnly)
	int countOverlaps = 0;

	UFUNCTION()
	void onBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void onBoxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	AMyAIVehicle();

	virtual void Tick(float Delta) override;

protected:
	virtual void BeginPlay() override;
	
	
};
