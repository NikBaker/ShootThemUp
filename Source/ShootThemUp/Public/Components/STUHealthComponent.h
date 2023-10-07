// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

// Объявляем делегат, который будет оповещать нашего персонажа, умерли мы или нет
DECLARE_MULTICAST_DELEGATE(FOnDeath)
// Дополнительный делегат для вызова при изменении жизней персонажа
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

// Класс для здоровья главного персонажа
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUHealthComponent();

	// Функция, определяющая умер наш персонаж или нет
	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	// делегат, который будет оповещать нашего персонажа, умерли мы или нет
	FOnDeath OnDeath;
	// делегат для вызова при изменении жизней персонажа
	FOnHealthChanged OnHealthChanged;

	float GetHealth() const { return Health; }

protected:
	// Максимально возможное здоровье
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxHealth = 100.0f;

	// Переменные для автоматического восстановления здоровья
	// Первая переменная определяет, включена или нет логика на восстановление жизни
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool AutoHeal = true;
	// Вторая переменная - частота обновлений (meta = (EditCondition = "AutoHeal") делает эту переменную активной тогда и только тогда, когда активно проперти AutoHeal)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealUpdateTime = 1.0f;
	// Третья переменная - задержка перед автоматическим возобновлением здоровья
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealDelay = 3.0f;
	// Четвертая переменная - количество жизней, которое добавляем при одном обновлении нашего таймера
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealModifier = 5.0f;

	// Called when the game starts
	virtual void BeginPlay() override;
private:
	// Здоровье персонажа
	float Health = 0.0f;

	// Дескриптор таймера для восстановления здоровья
	FTimerHandle HealTimerHandle;

	// Функция, которая будет вызываться каждый раз, когда будет срабатывать наш таймер для восстановления здоровья
	void HealUpdate();

	// Функция устанавливающее здоровье
	void SetHealth(float NewHealth);
	
	// Функция для нанесения урона персонажу
	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
		

};
