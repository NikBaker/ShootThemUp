// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamageActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUDevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUDevDamageActor();

	// Добавляем сцен компонент, чтобы у нашего актора была трансформация, и мы могли свободно его перемещать по сцене
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	// Радиус сферы
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 300.0f;

	// Цвет сферы
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor SphereColor = FColor::Red;

	// Дамаг
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10.0f;

	// Флаг нанесения полного урона во всей сфере
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DoFullDamage = false;

	// Класс урона, который мы хотим нанести нашему актору
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
