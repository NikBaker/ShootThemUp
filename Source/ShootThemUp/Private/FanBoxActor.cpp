// Fill out your copyright notice in the Description page of Project Settings.


#include "FanBoxActor.h"

// Создаем локальную категорию логирования
DEFINE_LOG_CATEGORY_STATIC(FanBoxLog, All, All);

// Sets default values
AFanBoxActor::AFanBoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

}

// Called when the game starts or when spawned
void AFanBoxActor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	//PrintTranform();
	
}

// Called every frame
void AFanBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMovable)
	{
		HandleMovement();
	}

	//
	/*TSubclassOf<UDamageType> DamageType;
	DrawDebugSphere(GetWorld(), GetActorLocation(), 100, 24, FColor::Red);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 10, GetActorLocation(), 100, DamageType, {}, this, nullptr, false);*/
	//
}

void AFanBoxActor::PrintTranform()
{
	// Получаем трансформацию актора
	FTransform Transform = GetActorTransform();
	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();

	//UE_LOG(FanBoxLog, Warning, TEXT("Actor name %s"), *GetName())
	//UE_LOG(FanBoxLog, Warning, TEXT("Transform %s"), *Transform.ToString());
	//UE_LOG(FanBoxLog, Warning, TEXT("Location %s"), *Location.ToString());
	//UE_LOG(FanBoxLog, Warning, TEXT("Rotation %s"), *Rotation.ToString());
	//UE_LOG(FanBoxLog, Warning, TEXT("Scale %s"), *Scale.ToString());

	//UE_LOG(FanBoxLog, Error, TEXT("Human transform %s"), *Transform.ToHumanReadableString());
}

void AFanBoxActor::HandleMovement()
{
	FVector CurrentLocation = GetActorLocation();
	float Time = GetWorld()->GetTimeSeconds();	// время, прошедшее сначала запуска
	if (IsBeginRight)
	{
		switch (GeometryData.Dir)
		{
		case EDirection::X_dir:
			CurrentLocation.X = InitialLocation.X + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			break;
		case EDirection::Y_dir:
			CurrentLocation.Y = InitialLocation.Y + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			break;
		case EDirection::Z_dir:
			CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			break;
		default:
			CurrentLocation.X = InitialLocation.X + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			break;
		}
	}
	else
	{
		switch (GeometryData.Dir)
		{
		case EDirection::X_dir:
			CurrentLocation.X = InitialLocation.X - GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			break;
		case EDirection::Y_dir:
			CurrentLocation.Y = InitialLocation.Y - GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			break;
		case EDirection::Z_dir:
			CurrentLocation.Z = InitialLocation.Z - GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			break;
		default:
			CurrentLocation.X = InitialLocation.X - GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			break;
		}
	}
	

	SetActorLocation(CurrentLocation);
}

