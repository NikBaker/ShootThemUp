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

	// ������ �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 125.0f;

	// ���� �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor SphereColor = FColor::Red;

	// �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 1.0f;

	// ���� ��������� ������� ����� �� ���� �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DoFullDamage = true;

	// ����� �����, ������� �� ����� ������� ������ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor PlatformColor = FLinearColor::White;
	//FLinearColor PlatformColor = FLinearColor::White;

	// ������ ������
	UPROPERTY(EditAnywhere, Category = "Design")
	float OrangeTimerRate = 1.0f;	// ����� ������������ ������� �������

	UPROPERTY(VisibleAnywhere, Category = "Design")
	int32 MaxOrangeTimerCount = 1;	// ������������ ����� ������������ ������� �������

	UPROPERTY(EditAnywhere, Category = "Design")
	int32 OrangeTimerCount = 1;		// ������� ����� ������������ ������� �������
	
	bool IsActivatedPlatform = false;	// ���� ��������� ���������

	// ������ ������
	UPROPERTY(EditAnywhere, Category = "Design")
	float RedTimerRate = 0.16f;	// ����� ������������ ������� �������

	UPROPERTY(VisibleAnywhere, Category = "Design")
	int32 MaxRedTimerCount = 1;	// ������������ ����� ������������ ������� �������

	UPROPERTY(EditAnywhere, Category = "Design")
	int32 RedTimerCount = 1;		// ������� ����� ������������ ������� �������


	// ������ ������
	UPROPERTY(EditAnywhere, Category = "Design")
	float ReloadTimerRate = 5.0f;	// ����� ������������ �������� �������

	UPROPERTY(VisibleAnywhere, Category = "Design")
	int32 MaxReloadTimerCount = 1;	// ������������ ����� ������������ �������� �������

	UPROPERTY(EditAnywhere, Category = "Design")
	int32 ReloadTimerCount = 1;		// ������� ����� ������������ �������� �������

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
	
	FTimerHandle OrangeTimerHandle;		// Handle ��� ������� ������� (��������� ����� ��������� �� ��������� �� 1 ���)

	FTimerHandle RedTimerHandle;		// Handle ��� ������� ������� (��������� ����� ��������� �� ������� �� 0.3 ���)
	
	FTimerHandle ReloadTimerHandle;		// Handle ��� �������� ������� (����������� ��������� 5.0 ���)

	void SetColor(const FLinearColor& Color);
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void OnOrangeTimerFired();

	void OnRedTimerFired();

	void OnReloadTimerFired();
};
