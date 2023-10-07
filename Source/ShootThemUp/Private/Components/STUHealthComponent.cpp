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
	
	// При каждом старте игры или при каждом спавне персонажа, значение его жизней будет максимальным
	SetHealth(MaxHealth);

	//Подписываемся на делегат OnTakeAnyDamage (callback функция)
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

	// Вызываем делегат при изменении жизней персонажа
	OnHealthChanged.Broadcast(Health);

	// Останавливаем таймер для восстановления здоровья, когда нам нанесли урон
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead()) 
	{
		OnDeath.Broadcast(); // Оповещаем всех, клиентов, которые подписаны на данный делегат, что наш персонаж погиб
	}
	else if (AutoHeal && GetWorld())	// Проверяем, включена ли функция автохила, когда мы живы после урона
	{
		// Устанавливаем таймер
		// Первый параметр - дескриптор таймера - хэндл, который мы создали
		// Второй параметр - объект, на котором будет вызываться callback функция 
		// Третий параметр - ссылка на функию, которая будет вызываться
		// Четрвертый параметр - частота обновления таймера
		// Пятый параметр - флаг повторного вызова таймера
		// Шестой параметр - задержка таймера перед самым первым вызовом
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}

	// Логируем получаемый урон
	// Damage - урон наносимый актору
	//UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

	//if (DamageType)
	//{
	//	// Проверяем тип урона при получении
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

	// Останавливаем таймер, когда значение здоровья достигло MaxHealth
	// IsNearlyEqual проверяет приближенное равенство двух чисел с плавающей точкой с некоторой точностью Епсилон(который можно задать третьим парметром)
	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	// Clamp проверяет отрицательное хп
	// Первый аргумент - вычисляемое значение, второй и третий - границы, в которых может быть значение
	// Если значение выйдет за границы, то будет возвращена ближайшая граница
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	// После изменения здоровья оповещаем всех клиентов, что здоровье изменилось
	OnHealthChanged.Broadcast(Health);
}