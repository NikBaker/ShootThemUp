// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingBridge.generated.h"

UCLASS()
class SHOOTTHEMUP_API AMovingBridge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingBridge();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	FVector StartLocation;

	FVector RememberedStartLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool IsMovable = true;

	UPROPERTY(EditAnywhere, Category = "Move")
	bool IsStayBegin = false;

	UPROPERTY(EditAnywhere, Category = "Move")
	float MoveDistance = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Move")
	bool IsForvardMove = true;

	UPROPERTY(EditAnywhere, Category = "Move")
	//float RightPauseTimerRate = 3.0f;	
	float RightPauseTimerRate = 3.0f;	

	UPROPERTY(EditAnywhere, Category = "Move")
	//float LeftPauseTimerRate = 1.0f;	
	float LeftPauseTimerRate = 1.0f;	

	FTimerHandle PauseTimerHandle;

	void OnPauseTimerFired();

	FTimerHandle StartPauseTimerHandle;

	void OnStartPauseTimerFired();

};
