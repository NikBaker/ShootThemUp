// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components\StaticMeshComponent.h"
#include "FanBoxActor.generated.h"

UENUM(BlueprintType)
enum class EDirection : uint8
{
	X_dir,
	Y_dir,
	Z_dir
};

USTRUCT(BlueprintType)
struct FGeometryData
{
	GENERATED_USTRUCT_BODY()
	
	// Для перемещения FanBox вперед и назад (по закону синуса)
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Amplitude = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Frequency = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	EDirection Dir = EDirection::X_dir;
};

UCLASS()
class SHOOTTHEMUP_API AFanBoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFanBoxActor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Geometry Data")
	FGeometryData GeometryData;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector InitialLocation;

	UPROPERTY(EditAnywhere, Category = "Geometry Data")
	bool IsMovable = true;

	UPROPERTY(EditAnywhere, Category = "Geometry Data")
	bool IsBeginRight = true;

	void PrintTranform();
	void HandleMovement();
};
