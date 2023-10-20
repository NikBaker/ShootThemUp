// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components\StaticMeshComponent.h"
#include "VertSpinFanActor.generated.h"

UENUM(BlueprintType)
enum class ERotDir : uint8
{
	Clockwise,	// �� ������� �������
	CounterClockwise,	// ������ ������� �������
};

UENUM(BlueprintType)
enum class ERotationAx : uint8
{
	X_Ax,	// �� ��� Ox
	Y_Ax,	// �� ��� Oy
	Z_Ax,	// �� ��� Oz
};

USTRUCT(BlueprintType)
struct FVertSpinGeometryData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Alpha = 1.0f;			// ���� ��������

	UPROPERTY(EditAnywhere, Category = "Movement")
	ERotDir RotationDir = ERotDir::Clockwise;	// ����������� ��������

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool IsFullTurnover = true;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxAngle = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	ERotationAx RotationAx = ERotationAx::X_Ax;	// ��� ��������
};

UCLASS()
class SHOOTTHEMUP_API AVertSpinFanActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVertSpinFanActor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Vert Spin Geometry Data")
	FVertSpinGeometryData VertSpinGeometryData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FRotator InitialRotation;

	void PrintTranform();
	void HandleMovement();
};
