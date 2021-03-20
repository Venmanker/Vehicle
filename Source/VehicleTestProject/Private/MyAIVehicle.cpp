// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIVehicle.h"


AMyAIVehicle::AMyAIVehicle()
{
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	AIControllerClass = AMyAIController::StaticClass();
	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UMyFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);
	
	Vehicle4W->WheelSetups[1].WheelClass = UMyFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);
	
	Vehicle4W->WheelSetups[2].WheelClass = UMyBackWheel::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);
	
	Vehicle4W->WheelSetups[3].WheelClass = UMyBackWheel::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	AIControllerClass = AMyAIController::StaticClass();
}



void AMyAIVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void AMyAIVehicle::GetLineTraceForward(bool &bHit, FVector &FNeedLocation)
{
	FHitResult FirstLineHit;
	FHitResult SecondLineHit;
	FCollisionQueryParams FCollisionParams;

	FVector FStartFrontPoint = GetActorLocation() + GetActorRotation().RotateVector(FCollisionBoxSize * FVector(1, 0, 0.5f));
	FVector FEndFrontLine = FStartFrontPoint + GetActorForwardVector() * LineTraceLengthByForward;
	FEndFrontLine.Z = GetActorLocation().Z + FCollisionBoxSize.Z * 0.5;


	bHit = GetWorld()->LineTraceSingleByChannel(FirstLineHit, FStartFrontPoint, FEndFrontLine, ECC_Visibility, FCollisionParams);
	if(bDebug)
	{
		DrawDebugLine(GetWorld(), FStartFrontPoint, FEndFrontLine, FColor::Red,  false, 0.f, 0, 50.f);
	}
	if(bHit)
	{
		auto Mirror = UKismetMathLibrary::MirrorVectorByNormal((FirstLineHit.Location - FStartFrontPoint), FirstLineHit.Normal);
		Mirror  = Mirror / FVector::Distance(FStartFrontPoint, FirstLineHit.Location) * LineTraceLengthByForward / 2;
		FNeedLocation = FirstLineHit.Location + Mirror;
		
		FNeedLocation.Z = GetActorLocation().Z;
	}
}

void AMyAIVehicle::GetLineTraceRight(bool &bHit, FVector &FNeedLocation)
{
	FHitResult FirstLineHit;
	FHitResult SecondLineHit;
	FCollisionQueryParams FCollisionParams;

	FVector FStartFrontPoint = GetActorLocation() + GetActorRotation().RotateVector(FCollisionBoxSize * FVector(1, -1, 0.5f));
	FVector FEndFrontLine = FStartFrontPoint + GetActorForwardVector() * LineTraceLengthByForward + LineTraceDifferenceBySide * GetActorRightVector();
	FEndFrontLine.Z = GetActorLocation().Z + FCollisionBoxSize.Z * 0.5;


	bHit = GetWorld()->LineTraceSingleByChannel(FirstLineHit, FStartFrontPoint, FEndFrontLine, ECC_Visibility, FCollisionParams);
	if(bDebug)
	{
		DrawDebugLine(GetWorld(), FStartFrontPoint, FEndFrontLine, FColor::Red,  false, 0.f, 0, 50.f);
	}
	if(bHit)
	{
		auto Mirror = UKismetMathLibrary::MirrorVectorByNormal((FirstLineHit.Location - FStartFrontPoint), FirstLineHit.Normal);
		Mirror  = Mirror / FVector::Distance(FStartFrontPoint, FirstLineHit.Location) * LineTraceLengthByForward / 4;
		FNeedLocation = FirstLineHit.Location + Mirror;
		
		FNeedLocation.Z = GetActorLocation().Z;
	}
}

void AMyAIVehicle::GetLineTraceLeft(bool &bHit, FVector &FNeedLocation)
{
	FHitResult FirstLineHit;
	FHitResult SecondLineHit;
	FCollisionQueryParams FCollisionParams;

	FVector FStartFrontPoint = GetActorLocation() + GetActorRotation().RotateVector(FCollisionBoxSize * FVector(1, 1, 0.5f));
	FVector FEndFrontLine = FStartFrontPoint + GetActorForwardVector() * LineTraceLengthByForward - LineTraceDifferenceBySide * GetActorRightVector();
	FEndFrontLine.Z = GetActorLocation().Z + FCollisionBoxSize.Z * 0.5;


	bHit = GetWorld()->LineTraceSingleByChannel(FirstLineHit, FStartFrontPoint, FEndFrontLine, ECC_Visibility, FCollisionParams);
	if(bDebug)
	{
		DrawDebugLine(GetWorld(), FStartFrontPoint, FEndFrontLine, FColor::Red,  false, 0.f, 0, 50.f);
	}
	if(bHit)
	{
		auto Mirror = UKismetMathLibrary::MirrorVectorByNormal((FirstLineHit.Location - FStartFrontPoint), FirstLineHit.Normal);
		Mirror  = Mirror / FVector::Distance(FStartFrontPoint, FirstLineHit.Location) * LineTraceLengthByForward / 4;
		FNeedLocation = FirstLineHit.Location + Mirror;
		FNeedLocation.Z = GetActorLocation().Z;
	}
}

ATargetPoint* AMyAIVehicle::GetNewTargetPoint()
{
	if(ATargetPointList.Num())
	{
		auto TempATargetPoint = ATargetPointList[0];
		ATargetPointList.RemoveAt(0);
		return TempATargetPoint;
	}
	else
	{
		return 0;
	}
}
