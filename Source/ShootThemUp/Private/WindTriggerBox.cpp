// Fill out your copyright notice in the Description page of Project Settings.

#include "WindTriggerBox.h"
#include "Player/STUBaseCharacter.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, String);

DEFINE_LOG_CATEGORY_STATIC(WindLog, All, All);

static FORCEINLINE int32 RandRange(int32 Min, int32 Max)
{
	const int32 Range = (Max - Min) + 1;
	return Min + FMath::RandHelper(Range);
}

static FORCEINLINE bool RandBool()
{
	return (RandRange(0, 1) == 1) ? true : false;
}

AWindTriggerBox::AWindTriggerBox()
{
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &AWindTriggerBox::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AWindTriggerBox::OnOverlapEnd);
}

void AWindTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	OverlapActor = nullptr;
	//DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);

	if (!IsOnlyForwardWind)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AWindTriggerBox::OnTimerFired, TimerRate, true);
	}
	
	auto CurLoc = GetActorLocation();
	if (IsVertWind)
	{
		//PrintString(FString::Printf(TEXT("CHECK IsVertWindStart")));
		CurLoc.Z += Radius;
	}
	else
	{
		CurLoc.X -= Radius;
		TempX = -Radius;
		TempY = 0;
	}
	
	InitWindLocation = CurLoc;

	InitialWindBlockLocation = GetActorLocation();
}

void AWindTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMovableWind)
	{
		WindBlockHandleMovement();
		auto CurLoc = GetActorLocation();
		CurLoc.X -= Radius;
		TempX = -Radius;
		TempY = 0;
		InitWindLocation = CurLoc;
	}

	//DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, DeltaTime, 1, 5);
	DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), InitWindLocation, 100.0f, FColor::Purple, true, DeltaTime, 1, 5);

	if (IsActorInWind)
	{
		if (OverlapActor)
		{
			auto CurrentLocation = OverlapActor->GetActorLocation();
			if (IsVertWind)
			{
				//PrintString(FString::Printf(TEXT("CHECK")));
				CurrentLocation.Z += WindForce;
			}
			else 
			{
				// Пропорции для получения нужных смещений по осям
				XOffset = (WindForce * TempX) / Radius;
				YOffset = (WindForce * TempY) / Radius;
				CurrentLocation.X += XOffset;
				CurrentLocation.Y += YOffset;
			}
			
			OverlapActor->SetActorLocation(CurrentLocation);
		}
	}
}

void AWindTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{

	if (OtherActor && (OtherActor != this))
	{
		auto ClassName = OtherActor->GetClass();
		//PrintString(FString::Printf(TEXT("OtherActor NAME : %s"), *ClassName->GetName()));

		if (ClassName->GetName() == "BP_STUBaseCharacter_C")
		{
			OverlapActor = OtherActor;
			IsActorInWind = true;
			PrintString(FString::Printf(TEXT("BEGIN Overlap, OtherActorName : %s"), *OtherActor->GetName()));
		}
	}
	

	//if (ClassName ==)
	//if (OtherActor && (OtherActor != this))
	//{
		//OverlapActor = OtherActor;
		//IsActorInWind = true;
		
	//}
}

void AWindTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		auto ClassName = OtherActor->GetClass();
		//PrintString(FString::Printf(TEXT("OtherActor NAME : %s"), *ClassName->GetName()));

		if (ClassName->GetName() == "BP_STUBaseCharacter_C")
		{
			//ASTUBaseCharacter* HitCharacter = Cast<ASTUBaseCharacter>(OverlapActor);
			//HitCharacter->Jump();

			OverlapActor = nullptr;
			IsActorInWind = false;
			PrintString(FString::Printf(TEXT("END Overlap, OtherActorName : %s"), *OtherActor->GetName()));
		}
	}
	

	//if (OtherActor && (OtherActor != this))
	//{
		//OverlapActor = nullptr;
		//IsActorInWind = false;
		
	//}
}

void AWindTriggerBox::OnTimerFired()
{
	//UE_LOG(WindLog, Warning, TEXT("WindDirection: %s"), *InitWindLocation.ToString());
	TempX = RandRange(-Radius, Radius);
	TempY = 0;
	//UE_LOG(WindLog, Warning, TEXT("Random X: %d"), TempX);
	int First_Y = FMath::Sqrt(Radius * Radius - TempX * TempX);
	int Second_Y = -FMath::Sqrt(Radius * Radius - TempX * TempX);

	bool YPositiveFlag = RandBool();
	if (YPositiveFlag)
	{
		TempY = First_Y;
	}
	else
	{
		TempY = Second_Y;
	}

	auto CurrentLoc = GetActorLocation();
	CurrentLoc.X += TempX;
	CurrentLoc.Y += TempY;
	InitWindLocation = CurrentLoc;
}

void AWindTriggerBox::WindBlockHandleMovement()
{
	FVector CurLoc= GetActorLocation();
	float Time = GetWorld()->GetTimeSeconds();	// время, прошедшее сначала запуска
	switch (GeometryData.Dir)
	{
	case EWindBlockDir::X_dir:
		CurLoc.X = InitialWindBlockLocation.X + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
		break;
	case EWindBlockDir::Y_dir:
		CurLoc.Y = InitialWindBlockLocation.Y + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
		break;
	case EWindBlockDir::Z_dir:
		CurLoc.Z = InitialWindBlockLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
		break;
	default:
		CurLoc.X = InitialWindBlockLocation.X + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
		break;
	}

	SetActorLocation(CurLoc);
}