// Fill out your copyright notice in the Description page of Project Settings.


#include "SpinPlatformActor.h"

// Создаем локальную категорию логирования
DEFINE_LOG_CATEGORY_STATIC(SpinPlatformLog, All, All);

// Sets default values
ASpinPlatformActor::ASpinPlatformActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ASpinPlatformActor::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorRotation();

	//PrintTranform();
	
}

// Called every frame
void ASpinPlatformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement();

}

void ASpinPlatformActor::PrintTranform()
{
	// Получаем трансформацию актора
	FTransform Transform = GetActorTransform();
	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();

	//UE_LOG(SpinPlatformLog, Warning, TEXT("Actor name %s"), *GetName())
	//UE_LOG(SpinPlatformLog, Warning, TEXT("Transform %s"), *Transform.ToString());
	//UE_LOG(SpinPlatformLog, Warning, TEXT("Location %s"), *Location.ToString());
	//UE_LOG(SpinPlatformLog, Warning, TEXT("Rotation %s"), *Rotation.ToString());
	//UE_LOG(SpinPlatformLog, Warning, TEXT("Scale %s"), *Scale.ToString());
	//
	//UE_LOG(SpinPlatformLog, Error, TEXT("Human transform %s"), *Transform.ToHumanReadableString());
}

void ASpinPlatformActor::HandleMovement()
{
	FRotator CurrentRotation = GetActorRotation();
	float Time = GetWorld()->GetTimeSeconds();	// время, прошедшее сначала запуска
	
	switch (SpinGeometryData.RotationDir)
	{
	case ERotationDirection::Clockwise :
		CurrentRotation.Yaw = InitialRotation.Yaw + SpinGeometryData.Alpha * Time;
		break;
	case ERotationDirection::CounterClockwise :
		CurrentRotation.Yaw = InitialRotation.Yaw - SpinGeometryData.Alpha * Time;
		break;
	default:
		CurrentRotation.Yaw = InitialRotation.Yaw + SpinGeometryData.Alpha * Time;
		break;
	}

	SetActorRotation(CurrentRotation);
}

