// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBaseWeapon();

	virtual void Fire();

protected:
	// ������� ��� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;
	
	// ��� ������(����� �� ������, �� ������� ���������� �������)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName MuzzleSocketName = "MuzzleSocket";

	// ������������ ��������� ��������
	float TraceMaxDistance = 1500.0f;

	// ��������� ���� ��� ��������� ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DamageAmount = 10.0f;

	// ������� ��� ������ ��������
	void MakeShot();

	// ������� ��� ��������� ��������� �� ���������� ���������
	APlayerController* GetPlayerController() const;

	// ������� ��� ����������� ������� ������ � ���������� �� � ������������ 
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	// ������� ��� ��������� ������� ������
	FVector GetMuzzleWorldLocation() const;

	// ������� ��� ��������� ����� ������ � ����� ����� ��������
	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	// ������� ��� ��������� ���������� � ��������
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	// ������� ��� ��������� ����� ���������, � �������� ������
	void MakeDamage(const FHitResult& HitResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};