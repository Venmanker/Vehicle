// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAIController.h"

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();
	auto AVehiclePawn = Cast<AMyAIVehicle>(GetPawn());
	ACurrentTargetPoint = AVehiclePawn->GetNewTargetPoint();//В самом начале берем точку
}

void AMyAIController::SetVehicleMovement(float ThrottleInput = 1.f, float SteeringInput = 0.f, bool HandbrakeInput = false)
{
	auto AVehiclePawn = Cast<AMyAIVehicle>(GetPawn());
	AVehiclePawn->GetVehicleMovementComponent()->SetThrottleInput(ThrottleInput);
	AVehiclePawn->GetVehicleMovementComponent()->SetSteeringInput(SteeringInput);
	AVehiclePawn->GetVehicleMovementComponent()->SetHandbrakeInput(HandbrakeInput);
}

AMyAIController::AMyAIController()
{

}

void AMyAIController::Tick(float Delta)
{
	Super::Tick(Delta);
	auto AVehiclePawn = Cast<AMyAIVehicle>(GetPawn());
	if(ACurrentTargetPoint && FVector::Distance(ACurrentTargetPoint->GetActorLocation(), AVehiclePawn->GetActorLocation()) < AcceptableRadius)
	{
		ACurrentTargetPoint = AVehiclePawn->GetNewTargetPoint();
	}
	if (ACurrentTargetPoint)
	{
		FVector FNewNeedLocation;
		bool Result = false;
		DrawDebugSphere(GetWorld(), ACurrentTargetPoint->GetActorLocation(), 100.f, 16, FColor::Red, false, -1, 0, 30.0);
		if (TurnRight)
		{
			AVehiclePawn->GetLineTraceRight(Result, FNewNeedLocation);
			if(Result)
			{
				if(GEngine)
				{
					GEngine->AddOnScreenDebugMessage(0, 0, FColor::Black, TEXT("Право"));
				}
				auto FTemp = (UKismetMathLibrary::FindLookAtRotation(AVehiclePawn->GetActorLocation(), FNewNeedLocation) - AVehiclePawn->GetActorRotation()).Yaw; 
				if(FTemp > 0)
				{
					NeedLocation = FNewNeedLocation;
				}
			}
			if(FVector::Distance(AVehiclePawn->GetActorLocation(), NeedLocation) < AcceptableRadius)
			{
				TurnRight = false;
			}
			else
			{
				auto FTempRotation = (UKismetMathLibrary::FindLookAtRotation(AVehiclePawn->GetActorLocation(), NeedLocation) - AVehiclePawn->GetActorRotation()).Yaw/180; 
				SetVehicleMovement(1 - (UKismetMathLibrary::Abs(FTempRotation)) , UKismetMathLibrary::SignOfFloat(FTempRotation) * UKismetMathLibrary::Sqrt(UKismetMathLibrary::Abs(FTempRotation)), FTempRotation > 0.7f);
				DrawDebugSphere(GetWorld(), NeedLocation, 100.f, 16, FColor::Green, false, -1, 0, 100.0);
			}
		}
		if(TurnLeft)
		{
			AVehiclePawn->GetLineTraceLeft(Result, FNewNeedLocation);
			if(Result)
			{
				if(GEngine)
				{
					GEngine->AddOnScreenDebugMessage(0, 0, FColor::Black, TEXT("Лево"));
				}
				auto FTemp = (UKismetMathLibrary::FindLookAtRotation(AVehiclePawn->GetActorLocation(), FNewNeedLocation) - AVehiclePawn->GetActorRotation()).Yaw; 
				if(FTemp < 0)
				{
					NeedLocation = FNewNeedLocation;
				}
			}
			if(FVector::Distance(AVehiclePawn->GetActorLocation(), NeedLocation) < AcceptableRadius)
			{
				TurnLeft = false;
			}
			else
			{
				auto FTempRotation = (UKismetMathLibrary::FindLookAtRotation(AVehiclePawn->GetActorLocation(), NeedLocation) - AVehiclePawn->GetActorRotation()).Yaw/180; 
				SetVehicleMovement(1 - (UKismetMathLibrary::Abs(FTempRotation)), UKismetMathLibrary::SignOfFloat(FTempRotation) * UKismetMathLibrary::Sqrt(UKismetMathLibrary::Abs(FTempRotation)), FTempRotation < -0.7f);
				DrawDebugSphere(GetWorld(), NeedLocation, 100.f, 16, FColor::Green, false, -1, 0, 100.0);
			}
		}
		if(!TurnLeft && !TurnRight)
		{
			AVehiclePawn->GetLineTraceForward(Result, NeedLocation);
			if(Result)
			{
				auto FTemp = (UKismetMathLibrary::FindLookAtRotation(AVehiclePawn->GetActorLocation(), NeedLocation) - AVehiclePawn->GetActorRotation()).Yaw; 
				if(FTemp > 0)
				{
					TurnRight = true;
				}
				else
				{
					TurnLeft = true;
				}
			}
			else
			{
				auto FTempRotation = (UKismetMathLibrary::FindLookAtRotation(AVehiclePawn->GetActorLocation(), ACurrentTargetPoint->GetActorLocation()) - AVehiclePawn->GetActorRotation()).GetNormalized().Yaw / 180; 
				SetVehicleMovement(1 - UKismetMathLibrary::Abs(FTempRotation), UKismetMathLibrary::Sqrt(UKismetMathLibrary::Abs(FTempRotation)) * UKismetMathLibrary::SignOfFloat(FTempRotation));
			}
		}
	}
	else
	{
		SetVehicleMovement();
	}
}
