// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components\StaticMeshComponent.h"
#include "SpinPlatformActor.generated.h"

UENUM(BlueprintType)
enum class ERotationDirection : uint8
{
	Clockwise,	// По часовой стрелке
	CounterClockwise,	// Против часовой стрелки
};

USTRUCT(BlueprintType)
struct FSpinGeometryData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Alpha = 30.0f;			// Угол вращения

	UPROPERTY(EditAnywhere, Category = "Movement")
	ERotationDirection RotationDir = ERotationDirection::Clockwise;
};

UCLASS()
class SHOOTTHEMUP_API ASpinPlatformActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpinPlatformActor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spin Geometry Data")
	FSpinGeometryData SpinGeometryData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FRotator InitialRotation;

	void PrintTranform();
	void HandleMovement();
};
