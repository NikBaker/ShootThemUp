// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "WindTriggerBox.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWindBlockDir: uint8
{
	X_dir,
	Y_dir,
	Z_dir
};

USTRUCT(BlueprintType)
struct FWindBlockGeometryData
{
	GENERATED_USTRUCT_BODY()

	// ƒл€ перемещени€ FanBox вперед и назад (по закону синуса)
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Amplitude = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Frequency = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	EWindBlockDir Dir = EWindBlockDir::X_dir;
};

UCLASS()
class SHOOTTHEMUP_API AWindTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Geometry Data")
	FWindBlockGeometryData GeometryData;

public:
	virtual void Tick(float DeltaTime) override;
	
	AWindTriggerBox();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

private:
	AActor* OverlapActor;

	bool IsActorInWind = false;

	FVector InitWindLocation;

	UPROPERTY(EditAnywhere, Category = "WindMove")
	bool IsOnlyForwardWind = false;

	UPROPERTY(EditAnywhere, Category = "WindMove")
	bool IsMovableWind = false;

	UPROPERTY(EditAnywhere, Category = "WindMove")
	float Radius = 200.0f;

	UPROPERTY(EditAnywhere, Category = "WindMove")	
	float TimerRate = 2.0f;

	FTimerHandle TimerHandle;

	void OnTimerFired();

	UPROPERTY(EditAnywhere, Category = "WindMove")
	float WindForce = 5.0f;		// —ила ветра

	int XOffset = -5.0f;	// —мещение персонажа по оси X когда дует ветер
	int YOffset = 0.0f;		// —мещение персонажа по оси Y когда дует ветер

	int TempX;				//  оордината X точки на окружности, котора€ будет мен€тьс€ каждые 2 сек
	int TempY;				//  оордината Y точки на окружности, котора€ будет мен€тьс€ каждые 2 сек

	UPROPERTY(EditAnywhere, Category = "WindMove")
	bool IsVertWind = false;
	int ZOffset = 5.0f;

	FVector InitialWindBlockLocation;

	void WindBlockHandleMovement();
};
