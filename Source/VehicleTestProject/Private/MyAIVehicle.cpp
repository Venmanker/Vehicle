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



	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("collision box"));
	collisionBox->SetCollisionProfileName(TEXT("BoxCollision"));
	collisionBox->InitBoxExtent(collisionBoxSize);
 	collisionBox->SetupAttachment(RootComponent);
	collisionBox->AddLocalOffset(collisionBoxShift);
	collisionBox->SetGenerateOverlapEvents(true);
	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyAIVehicle::onBoxOverlapBegin);
	collisionBox->OnComponentEndOverlap.AddDynamic(this, &AMyAIVehicle::onBoxOverlapEnd);

	AIControllerClass = AMyAIController::StaticClass();
}


void AMyAIVehicle::Tick(float Delta)
{
	Super::Tick(Delta);
	

	for(auto line : GetNormalizedLineTraceResult())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Up vector: %s"),  *FString::SanitizeFloat(line)));
	}
	if (showDebug && GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, FString::Printf(TEXT("Up vector: %s"),  *GetActorUpVector().ToString()));
		if(HaveOverlap())
		{
			GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Red, FString::Printf(TEXT("Overlap")));
		}
	}
	
	
	//int k = 0;
	//GetVehicleMovementComponent()->SetSteeringInput(k % 3 - 1);
	//k /= 3;
	//GetVehicleMovementComponent()->SetThrottleInput(k % 3 - 1);
	//GetVehicleMovementComponent()->SetHandbrakeInput(k / 3);
}

void AMyAIVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void AMyAIVehicle::onBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && (OtherActor != this) && OtherComp)
	{
		countOverlaps++;
	}
}

void AMyAIVehicle::onBoxOverlapEnd( class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && (OtherActor != this) && OtherComp)
	{
		countOverlaps--;
	}
}

bool AMyAIVehicle::HaveOverlap()
{
	//GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Red, FString::FromInt(countOverlaps));
	return countOverlaps;
}


TArray<float> AMyAIVehicle::GetNormalizedLineTraceResult()
{
	TArray<float> result;
	FCollisionQueryParams collisionParams;
	FHitResult outHit;
	TMap<FVector, TArray<FVector>> needLineTraceParams;

	FVector frontLine = GetActorForwardVector() * lineTraceLenght;
	FVector backLine = -GetActorForwardVector() * lineTraceLenght;
	FVector rightLine = GetActorRightVector() * lineTraceLenght;
	FVector leftLine = -GetActorRightVector() * lineTraceLenght;

	FVector frontRightLine = (GetActorForwardVector() + GetActorRightVector()) / sqrt(2) * lineTraceLenght;
	FVector backRightLine = (-GetActorForwardVector() + GetActorRightVector()) / sqrt(2) * lineTraceLenght;
	FVector frontLeftLine = (GetActorForwardVector() + -GetActorRightVector()) / sqrt(2) * lineTraceLenght;
	FVector backLeftLine = (-GetActorForwardVector() + -GetActorRightVector()) / sqrt(2) * lineTraceLenght;

	FVector frontPoint = GetActorLocation() + GetActorRotation().RotateVector(collisionBoxShift + collisionBoxSize * FVector(1, 0, 0));
	FVector frontRightPoint = GetActorLocation() + GetActorRotation().RotateVector(collisionBoxShift + collisionBoxSize * FVector(1, 1, 0));
	FVector rightPoint = GetActorLocation() + GetActorRotation().RotateVector(collisionBoxShift + collisionBoxSize * FVector(0, 1, 0));
	FVector backRightPoint = GetActorLocation() + GetActorRotation().RotateVector(collisionBoxShift + collisionBoxSize * FVector(-1, 1, 0));
	FVector backPoint = GetActorLocation() + GetActorRotation().RotateVector(collisionBoxShift + collisionBoxSize * FVector(-1, 0, 0));
	FVector backLeftPoint = GetActorLocation() + GetActorRotation().RotateVector(collisionBoxShift + collisionBoxSize * FVector(-1, -1, 0));
	FVector leftPoint = GetActorLocation() + GetActorRotation().RotateVector(collisionBoxShift + collisionBoxSize * FVector(0, -1, 0));
	FVector frontLeftPoint = GetActorLocation() + GetActorRotation().RotateVector(collisionBoxShift + collisionBoxSize * FVector(1, -1, 0));

	needLineTraceParams.Add(frontPoint, TArray<FVector>({frontPoint + frontLine}));
	needLineTraceParams.Add(frontRightPoint, TArray<FVector>({frontRightPoint + frontLine, frontRightPoint + frontRightLine, frontRightPoint + rightLine}));
	needLineTraceParams.Add(rightPoint, TArray<FVector>({rightPoint + rightLine}));
	needLineTraceParams.Add(backRightPoint, TArray<FVector>({backRightPoint + rightLine, backRightPoint + backRightLine, backRightPoint + backLine}));
	needLineTraceParams.Add(backPoint, TArray<FVector>({backPoint + backLine}));
	needLineTraceParams.Add(backLeftPoint, TArray<FVector>({backLeftPoint + backLine, backLeftPoint + backLeftLine, backLeftPoint + leftLine}));
	needLineTraceParams.Add(leftPoint, TArray<FVector>({leftPoint + leftLine}));
	needLineTraceParams.Add(frontLeftPoint, TArray<FVector>({frontLeftPoint + leftLine, frontLeftPoint + frontLeftLine, frontLeftPoint + frontLine}));

	for(auto it = needLineTraceParams.CreateConstIterator(); it; ++it)
	{
		for(auto line : it.Value())
		{
			if (GetWorld()->LineTraceSingleByChannel(outHit, it.Key(), line, ECC_Visibility, collisionParams))
			{
				result.Push(outHit.Distance/lineTraceLenght);
			}
			else
			{
				result.Push(1);
			}
		}
	}


	if(showDebug)
	{
		for(auto it = needLineTraceParams.CreateConstIterator(); it; ++it)
		{
			for(auto line : it.Value())
			{
				
				DrawDebugLine(GetWorld(), it.Key(), line, FColor::Red, false, 0, 0, 5);
			}
		}
	}
	return result;
}