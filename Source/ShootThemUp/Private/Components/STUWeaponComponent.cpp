// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// Спавним оружие персонажа
	SpawnWeapon();
}

void USTUWeaponComponent::SpawnWeapon()
{
	if (!GetWorld()) { return; }

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) { return; }

	// Спавним оружие
	CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
	if (!CurrentWeapon) { return; }
	// Присоединяем оружие к мешу нашего персонажа
	// Первый параметр - указатель на компонент, к которому хотим присоединить данного актора
	// Второй параметр - правило присоединения
	// Третий параметр - название соккета
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
	// Устанавливаем владельца для оружия
	CurrentWeapon->SetOwner(Character);

}

void USTUWeaponComponent::Fire()
{
	if (!CurrentWeapon) { return; }
	CurrentWeapon->Fire();
}