// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineController.generated.h"


UCLASS()
class SHOOTTHEMUP_API ASplineController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	class USplineComponent* Spline;

	FVector StartSplinePosition;

	UPROPERTY(EditDefaultsOnly, Category = "SplineController")
	TSubclassOf<class AActor> ActorToMoveClass;

	std::vector<class AActor*> ActorsToMove;
	//class AActor* ActorToMove;

	UPROPERTY(EditAnywhere, Category = "SplineController")
	float TotalPathTimeController;

	UPROPERTY(EditAnywhere, Category = "SplineController")
	bool bSplineInLoop;

	bool bCanMoveActor;

	float StartTime;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
