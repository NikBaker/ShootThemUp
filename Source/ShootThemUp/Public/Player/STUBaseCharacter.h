// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUWeaponComponent;


// Класс главного персонажа
UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter();

protected:
	// Воспользуемся компонентом камера, добавим его к нашему персонажу,
	// чтобы получить полный контроль над камерой в игре
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent* CameraComponent;		// Указатель на класс камеры UCameraComponent

	// Создаем новый компонент для правильного поведения камеры(вращения вокруг персонажа, а не вокруг собственной оси)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent* SpringArmComponent;

	// Компонент для изменения максимальной скорости персонажа при беге
	UCharacterMovementComponent* CharacterMovementComponent;

	// Переменная, для изменения максимальной скорости (модификатор)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (ClampMin = "1.5", ClampMax = "10.0"))
	float RunModifier = 2.0f;

	// Компонент здоровья
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USTUHealthComponent* HealthComponent;

	// Компонент для отображения здоровья в некотором текстовом поле поле
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UTextRenderComponent* HealthTextComponent;

	// Компонент для оружия
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USTUWeaponComponent* WeaponComponent;

	// Ассет для смерти персонажа
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	// Предельная скорость, при которой урон от падения будет наноситься
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	// Наносимый урон при падении (при скорости падения 900 урон будет 10, а при скорости 1200 - урон будет 100)
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Добавляем функцию, которую будем вызывать из анимационного blueprint
	UFUNCTION(BlueprintCallable, category = "Movement")
	bool IsRunning() const;

	// Добавляем функцию, которая будет возвращать значение угла между форвард вектором и вектором скорости
	UFUNCTION(BlueprintCallable, category = "Movement")
	float GetMovmentDirection() const;

private:
	// флаг проверки зажатия шифта
	bool WantsToRun = false;
	// Флаг для определения, бежит ли наш персонаж вперед
	bool IsMovingForward = false;

	// Функции, которые будут вызываться при нажатии клавиш:
	// Amount принимает значения 1 - нажата клавиша W
	//							 0 - клавиша не нажата
	//							-1 - нажата клавиша S
	void MoveForward(float Amount);	
	// Аналогичная функция для движения вправо/влево
	void MoveRight(float Amount);

	// Функции для поворота камеры
	void LookUp(float Amount);
	void LookAround(float Amount);

	// Функциb для бега
	void OnStartRunning();
	void OnStopRunning();

	// Функция, отвечающая за происходящее при смерти персонажа
	void OnDeath();

	// Функция, отображающая в текстовом поле количество здоровья при его изменении
	void OnHealthChanged(float Health);

	// Функция, которая будет срабатывать при приземлении персонажа
	// FHitResult - спец. структура в UE, в которой содержится информация о контакте двух объектов
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

	

};
