// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponComponent();

	// Функция для стрельбы
	void Fire();

protected:
	// Класс оружия, которое хотим заспавнить
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ASTUBaseWeapon> WeaponClass;

	//
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponAttachPointName = "WeaponSocket";

	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;

	// функция для спавна оружия и присоединения его к мешу персонажа
	void SpawnWeapon();

};
