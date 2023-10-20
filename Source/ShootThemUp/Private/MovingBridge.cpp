// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingBridge.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, String);

DEFINE_LOG_CATEGORY_STATIC(BridgeLog, All, All);

// Sets default values
AMovingBridge::AMovingBridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

}

// Called when the game starts or when spawned
void AMovingBridge::BeginPlay()
{
	Super::BeginPlay();

	RememberedStartLocation = GetActorLocation();
	StartLocation = GetActorLocation();
	
	if (IsStayBegin)
	{
		//PrintString(FString::Printf(TEXT("CHECK")));
		GetWorldTimerManager().SetTimer(StartPauseTimerHandle, this, &AMovingBridge::OnStartPauseTimerFired, LeftPauseTimerRate, false);
	}
}

// Called every frame
void AMovingBridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMovable)
	{
		//UE_LOG(BridgeLog, Warning, TEXT("Actor name: %s IsStayBegin : %d"), *GetName(), IsStayBegin);
		if (!IsStayBegin)
		{
			FVector CurrentLocation = GetActorLocation();
			if (IsForvardMove)
			{
				//CurrentLocation.X += DeltaTime * 100.0f;
				//CurrentLocation.X += (MoveDistance * DeltaTime) / 1.0f;
				
				if (CurrentLocation.X - StartLocation.X >= MoveDistance)
				{
					IsMovable = false;
					StartLocation = GetActorLocation();
					//PrintString(FString::Printf(TEXT("RightTimer START : %fl"), GetWorld()->GetTimeSeconds()));
					//UE_LOG(BridgeLog, Warning, TEXT("Actor name: %s RightTimer START : %fl"), *GetName(), GetWorld()->GetTimeSeconds());
					GetWorldTimerManager().SetTimer(PauseTimerHandle, this, &AMovingBridge::OnPauseTimerFired, RightPauseTimerRate, false);
				}
				else
				{
					CurrentLocation.X += (MoveDistance * DeltaTime) / 1.0f;
				}
			}
			else
			{
				//CurrentLocation.X -= DeltaTime * 100.0f;
				//CurrentLocation.X -= (MoveDistance * DeltaTime) / 1.0f;
				
				if (FMath::Abs(CurrentLocation.X - StartLocation.X) >= MoveDistance)
				{
					IsMovable = false;
					StartLocation = GetActorLocation();
					if (StartLocation.X > RememberedStartLocation.X)
					{
						StartLocation = RememberedStartLocation;
					}
					//PrintString(FString::Printf(TEXT("LeftTimer START : %fl"), GetWorld()->GetTimeSeconds()));
					//UE_LOG(BridgeLog, Warning, TEXT("Actor name: %s LeftTimer START : %fl"), *GetName(), GetWorld()->GetTimeSeconds());
					GetWorldTimerManager().SetTimer(PauseTimerHandle, this, &AMovingBridge::OnPauseTimerFired, LeftPauseTimerRate, false);

					// 
					//PrintString(FString::Printf(TEXT("SETCOLLISIONS = TRUE")));
					this->SetActorEnableCollision(true);
					//
				}
				else
				{	
					CurrentLocation.X -= (MoveDistance * DeltaTime) / 1.0f;

					//
					//PrintString(FString::Printf(TEXT("SETCOLLISIONS = FALSE")));
					this->SetActorEnableCollision(false);
					//
				}
			}
			SetActorLocation(CurrentLocation);
		}
		
	}
}

void AMovingBridge::OnPauseTimerFired()
{
	//PrintString(FString::Printf(TEXT("Timer FINISH : %fl"), GetWorld()->GetTimeSeconds()));
	//UE_LOG(BridgeLog, Warning, TEXT("Actor name: %s Timer FINISH : %fl"), *GetName(), GetWorld()->GetTimeSeconds());
	IsMovable = true;
	IsForvardMove = !IsForvardMove;

	GetWorldTimerManager().ClearTimer(PauseTimerHandle);
}

void AMovingBridge::OnStartPauseTimerFired()
{
	//PrintString(FString::Printf(TEXT("CHECK")));
	IsStayBegin = false;

	GetWorldTimerManager().ClearTimer(StartPauseTimerHandle);
}

