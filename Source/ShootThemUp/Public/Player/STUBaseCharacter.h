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


// ����� �������� ���������
UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter();

protected:
	// ������������� ����������� ������, ������� ��� � ������ ���������,
	// ����� �������� ������ �������� ��� ������� � ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent* CameraComponent;		// ��������� �� ����� ������ UCameraComponent

	// ������� ����� ��������� ��� ����������� ��������� ������(�������� ������ ���������, � �� ������ ����������� ���)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent* SpringArmComponent;

	// ��������� ��� ��������� ������������ �������� ��������� ��� ����
	UCharacterMovementComponent* CharacterMovementComponent;

	// ����������, ��� ��������� ������������ �������� (�����������)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (ClampMin = "1.5", ClampMax = "10.0"))
	float RunModifier = 2.0f;

	// ��������� ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USTUHealthComponent* HealthComponent;

	// ��������� ��� ����������� �������� � ��������� ��������� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UTextRenderComponent* HealthTextComponent;

	// ��������� ��� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USTUWeaponComponent* WeaponComponent;

	// ����� ��� ������ ���������
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	// ���������� ��������, ��� ������� ���� �� ������� ����� ����������
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	// ��������� ���� ��� ������� (��� �������� ������� 900 ���� ����� 10, � ��� �������� 1200 - ���� ����� 100)
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ��������� �������, ������� ����� �������� �� ������������� blueprint
	UFUNCTION(BlueprintCallable, category = "Movement")
	bool IsRunning() const;

	// ��������� �������, ������� ����� ���������� �������� ���� ����� ������� �������� � �������� ��������
	UFUNCTION(BlueprintCallable, category = "Movement")
	float GetMovmentDirection() const;

private:
	// ���� �������� ������� �����
	bool WantsToRun = false;
	// ���� ��� �����������, ����� �� ��� �������� ������
	bool IsMovingForward = false;

	// �������, ������� ����� ���������� ��� ������� ������:
	// Amount ��������� �������� 1 - ������ ������� W
	//							 0 - ������� �� ������
	//							-1 - ������ ������� S
	void MoveForward(float Amount);	
	// ����������� ������� ��� �������� ������/�����
	void MoveRight(float Amount);

	// ������� ��� �������� ������
	void LookUp(float Amount);
	void LookAround(float Amount);

	// ������b ��� ����
	void OnStartRunning();
	void OnStopRunning();

	// �������, ���������� �� ������������ ��� ������ ���������
	void OnDeath();

	// �������, ������������ � ��������� ���� ���������� �������� ��� ��� ���������
	void OnHealthChanged(float Health);

	// �������, ������� ����� ����������� ��� ����������� ���������
	// FHitResult - ����. ��������� � UE, � ������� ���������� ���������� � �������� ���� ��������
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

	

};
