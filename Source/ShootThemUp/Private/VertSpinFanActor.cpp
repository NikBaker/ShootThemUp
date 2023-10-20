// Fill out your copyright notice in the Description page of Project Settings.


#include "VertSpinFanActor.h"

// Создаем локальную категорию логирования
DEFINE_LOG_CATEGORY_STATIC(VertSpinPlatformLog, All, All);

// Sets default values
AVertSpinFanActor::AVertSpinFanActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void AVertSpinFanActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitialRotation = GetActorRotation();

	//PrintTranform();
}

// Called every frame
void AVertSpinFanActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement();
}

void AVertSpinFanActor::PrintTranform()
{
	// Получаем трансформацию актора
	FTransform Transform = GetActorTransform();
	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();

	//UE_LOG(VertSpinPlatformLog, Warning, TEXT("Actor name %s"), *GetName())
	//UE_LOG(VertSpinPlatformLog, Warning, TEXT("Transform %s"), *Transform.ToString());
	//UE_LOG(VertSpinPlatformLog, Warning, TEXT("Location %s"), *Location.ToString());
	//UE_LOG(VertSpinPlatformLog, Warning, TEXT("Rotation %s"), *Rotation.ToString());
	//UE_LOG(VertSpinPlatformLog, Warning, TEXT("Scale %s"), *Scale.ToString());
	//
	//UE_LOG(VertSpinPlatformLog, Error, TEXT("Human transform %s"), *Transform.ToHumanReadableString());
}

void AVertSpinFanActor::HandleMovement()
{
	FRotator CurrentRotation = GetActorRotation();
	//UE_LOG(VertSpinPlatformLog, Warning, TEXT("Actor name: %s IN CurrentRotation: %s"), *GetName(), *CurrentRotation.ToString());

	//UE_LOG(VertSpinPlatformLog, Warning, TEXT("Actor name: %s (CurrentRotation): %fl"), *GetName(), (AxisRotation));

	
	if (!VertSpinGeometryData.IsFullTurnover)
	{
		//UE_LOG(VertSpinPlatformLog, Warning, TEXT("FIRST IF"));
		//UE_LOG(VertSpinPlatformLog, Warning, TEXT("abs(CurrentRotation.Roll): %fl"), abs(CurrentRotation.Roll));
		if (CurrentRotation.Roll > VertSpinGeometryData.MaxAngle)
		//if (CurrentRotation.Pitch > VertSpinGeometryData.MaxAngle)
		{
			VertSpinGeometryData.RotationDir = ERotDir::CounterClockwise;
		}
		else if (CurrentRotation.Roll < -VertSpinGeometryData.MaxAngle)
		//else if (CurrentRotation.Pitch < -VertSpinGeometryData.MaxAngle)
		{
			VertSpinGeometryData.RotationDir = ERotDir::Clockwise;
		}
	}

	switch (VertSpinGeometryData.RotationDir)
	{
		case ERotDir::Clockwise:
		{
			CurrentRotation.Roll = CurrentRotation.Roll + VertSpinGeometryData.Alpha;
			//CurrentRotation.Pitch = CurrentRotation.Pitch + VertSpinGeometryData.Alpha;
			break;
		}
		case ERotDir::CounterClockwise:
		{
			CurrentRotation.Roll = CurrentRotation.Roll - VertSpinGeometryData.Alpha;
			//CurrentRotation.Pitch = CurrentRotation.Pitch - VertSpinGeometryData.Alpha;
			break;
		}
		default:
		{
			CurrentRotation.Roll = CurrentRotation.Roll + VertSpinGeometryData.Alpha;
			//CurrentRotation.Pitch = CurrentRotation.Pitch + VertSpinGeometryData.Alpha;
			break;
		}
	}

	SetActorRotation(CurrentRotation);
}



