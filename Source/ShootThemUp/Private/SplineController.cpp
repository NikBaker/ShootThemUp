// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineController.h"
#include "Engine/Engine.h"
#include "Components/SplineComponent.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, String);


// Sets default values
ASplineController::ASplineController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("SPLINE"));
	Spline->SetupAttachment(Root);
	Spline->Duration = TotalPathTimeController;
	Spline->bDrawDebug = true;

	StartSplinePosition = Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);

}

// Called when the game starts or when spawned
void ASplineController::BeginPlay()
{
	Super::BeginPlay();

	if (ActorToMoveClass != nullptr)
	{
		float Distance = 50.0f;
		for (int i = 0; i < 33; ++i)
		{
			auto TempRotation = Spline->GetComponentTransform().GetRotation();
			auto TempScale = Spline->GetComponentTransform().GetScale3D();
			auto TempLocation= Spline->GetLocationAtDistanceAlongSpline(Distance * i, ESplineCoordinateSpace::World);

			auto TempTransform = FTransform(TempRotation, TempLocation, TempScale);

			//auto ActorToMove = GetWorld()->SpawnActor<AActor>(ActorToMoveClass, Spline->GetComponentTransform());
			auto ActorToMove = GetWorld()->SpawnActor<AActor>(ActorToMoveClass, TempTransform);
			if (ActorToMove != nullptr)
			{
				ActorsToMove.push_back(ActorToMove);
			}
		}
		
		StartTime = GetWorld()->GetTimeSeconds();
		bCanMoveActor = true;
	}
	
}

// Called every frame
void ASplineController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	for (int i = 32; i >= 0; --i)
	{
		//if ((ActorsToMove[i] != nullptr) && bCanMoveActor)
		//{
			//float CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTimeController;
			float CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTimeController;

			float Distance = (int)(Spline->GetSplineLength() * CurrentSplineTime + 50.0f * (29 - i)) % (int)(Spline->GetSplineLength());

			FVector Position = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			ActorsToMove[i]->SetActorLocation(Position);

			FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();
			if (Rotator.Yaw == 180)
			{
				Rotator.Yaw = 0;
			}
			ActorsToMove[i]->SetActorRotation(Rotator);

			//if (i == 4)
			//{
				//PrintString(FString::Printf(TEXT("Position[4] : %s"), *Position.ToString()));
				//PrintString(FString::Printf(TEXT("Rotation[4] : %s"), *Rotator.ToString()));
			//}

			/*if (i == 4)
			{
				PrintString(FString::Printf(TEXT("CurrentSplineTime[4] : %fl"), CurrentSplineTime));
				if (CurrentSplineTime >= 1.0f)
				{
					if (bSplineInLoop)
					{
						bCanMoveActor = true;

						StartTime = GetWorld()->GetTimeSeconds();

						CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTimeController;
					}
				}
			}*/
			
		//}
	}
	/*if ((ActorToMove != nullptr) && bCanMoveActor)
	{
		float CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTimeController;

		float Distance = Spline->GetSplineLength() * CurrentSplineTime;

		FVector Position = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		ActorToMove->SetActorLocation(Position);

		FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();
		ActorToMove->SetActorRotation(Rotator);

		if (CurrentSplineTime >= 1.0f)
		{
			if (bSplineInLoop)
			{
				bCanMoveActor = true;

				StartTime = GetWorld()->GetTimeSeconds();

				CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTimeController;
			}
		}
	}*/

}

