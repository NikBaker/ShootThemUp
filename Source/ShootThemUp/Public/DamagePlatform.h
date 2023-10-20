// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components\StaticMeshComponent.h"
#include "DamagePlatform.generated.h"

UCLASS()
class SHOOTTHEMUP_API ADamagePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamagePlatform();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	// Радиус сферы
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 125.0f;

	// Цвет сферы
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor SphereColor = FColor::Red;

	// Дамаг
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 1.0f;

	// Флаг нанесения полного урона во всей сфере
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DoFullDamage = true;

	// Класс урона, который мы хотим нанести нашему актору
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor PlatformColor = FLinearColor::White;
	//FLinearColor PlatformColor = FLinearColor::White;

	// ПЕРВЫЙ ТАЙМЕР
	UPROPERTY(EditAnywhere, Category = "Design")
	float OrangeTimerRate = 1.0f;	// Время срабатывания первого таймера

	UPROPERTY(VisibleAnywhere, Category = "Design")
	int32 MaxOrangeTimerCount = 1;	// максимальное число срабатываний первого таймера

	UPROPERTY(EditAnywhere, Category = "Design")
	int32 OrangeTimerCount = 1;		// текущее число срабатываний первого таймера
	
	bool IsActivatedPlatform = false;	// флаг активации платформы

	// ВТОРОЙ ТАЙМЕР
	UPROPERTY(EditAnywhere, Category = "Design")
	float RedTimerRate = 0.16f;	// Время срабатывания второго таймера

	UPROPERTY(VisibleAnywhere, Category = "Design")
	int32 MaxRedTimerCount = 1;	// максимальное число срабатываний второго таймера

	UPROPERTY(EditAnywhere, Category = "Design")
	int32 RedTimerCount = 1;		// текущее число срабатываний второго таймера


	// ТРЕТИЙ ТАЙМЕР
	UPROPERTY(EditAnywhere, Category = "Design")
	float ReloadTimerRate = 5.0f;	// Время срабатывания третьего таймера

	UPROPERTY(VisibleAnywhere, Category = "Design")
	int32 MaxReloadTimerCount = 1;	// максимальное число срабатываний третьего таймера

	UPROPERTY(EditAnywhere, Category = "Design")
	int32 ReloadTimerCount = 1;		// текущее число срабатываний третьего таймера

	UPROPERTY(EditAnywhere, Category = "Move")
	bool IsMovable = false;		

	UPROPERTY(EditAnywhere, Category = "Move")
	float Amplitude = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Move")
	float Frequency = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Move")
	bool IsLeftMove = true;

	FVector Scale;
	FVector Location;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	FTimerHandle OrangeTimerHandle;		// Handle для первого таймера (изменение цвета платформы на оранжевый на 1 сек)

	FTimerHandle RedTimerHandle;		// Handle для второго таймера (изменение цвета платформы на красный на 0.3 сек)
	
	FTimerHandle ReloadTimerHandle;		// Handle для третьего таймера (перезарядка платформы 5.0 сек)

	void SetColor(const FLinearColor& Color);
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void OnOrangeTimerFired();

	void OnRedTimerFired();

	void OnReloadTimerFired();
};
