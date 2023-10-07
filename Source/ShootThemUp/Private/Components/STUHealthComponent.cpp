// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Dev/STUFireDamageType.h"
#include "Dev/STUIceDamageType.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ��� ������ ������ ���� ��� ��� ������ ������ ���������, �������� ��� ������ ����� ������������
	SetHealth(MaxHealth);

	//������������� �� ������� OnTakeAnyDamage (callback �������)
	AActor* ComponentOwner = GetOwner(); 
	if (ComponentOwner) 
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandle);
	}
	
}

void USTUHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) { return; }

	SetHealth(Health - Damage);

	// �������� ������� ��� ��������� ������ ���������
	OnHealthChanged.Broadcast(Health);

	// ������������� ������ ��� �������������� ��������, ����� ��� ������� ����
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead()) 
	{
		OnDeath.Broadcast(); // ��������� ����, ��������, ������� ��������� �� ������ �������, ��� ��� �������� �����
	}
	else if (AutoHeal && GetWorld())	// ���������, �������� �� ������� ��������, ����� �� ���� ����� �����
	{
		// ������������� ������
		// ������ �������� - ���������� ������� - �����, ������� �� �������
		// ������ �������� - ������, �� ������� ����� ���������� callback ������� 
		// ������ �������� - ������ �� ������, ������� ����� ����������
		// ���������� �������� - ������� ���������� �������
		// ����� �������� - ���� ���������� ������ �������
		// ������ �������� - �������� ������� ����� ����� ������ �������
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}

	// �������� ���������� ����
	// Damage - ���� ��������� ������
	//UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

	//if (DamageType)
	//{
	//	// ��������� ��� ����� ��� ���������
	//	if (DamageType->IsA<USTUFIreDamageType>())
	//	{
	//		UE_LOG(LogHealthComponent, Display, TEXT("So hoooooooot !!!"));
	//	}
	//	else if (DamageType->IsA<USTUIceDamageType>())
	//	{
	//		UE_LOG(LogHealthComponent, Display, TEXT("So cooooooold !!!"));
	//	}
	//}


}

void USTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	// ������������� ������, ����� �������� �������� �������� MaxHealth
	// IsNearlyEqual ��������� ������������ ��������� ���� ����� � ��������� ������ � ��������� ��������� �������(������� ����� ������ ������� ���������)
	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	// Clamp ��������� ������������� ��
	// ������ �������� - ����������� ��������, ������ � ������ - �������, � ������� ����� ���� ��������
	// ���� �������� ������ �� �������, �� ����� ���������� ��������� �������
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	// ����� ��������� �������� ��������� ���� ��������, ��� �������� ����������
	OnHealthChanged.Broadcast(Health);
}