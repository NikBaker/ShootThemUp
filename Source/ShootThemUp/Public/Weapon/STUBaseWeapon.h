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
	// Внешний вид оружия
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;
	
	// Имя сокета(точки на оружии, из которой происходит выстрел)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName MuzzleSocketName = "MuzzleSocket";

	// Максимальная дальность выстрела
	float TraceMaxDistance = 1500.0f;

	// Наносимый урон при попадании выстрела
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DamageAmount = 10.0f;

	// Функция для логики выстрела
	void MakeShot();

	// Функция для получения указателя на контроллер персонажа
	APlayerController* GetPlayerController() const;

	// Функция для определения позиции камеры и ориентации ее в пространстве 
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	// Функция для получения позиции сокета
	FVector GetMuzzleWorldLocation() const;

	// Функция для получения точки начала и точки конца выстрела
	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	// Функция для получения информации о выстреле
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	// Функция для нанесения урона персонажу, в которого попали
	void MakeDamage(const FHitResult& HitResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
