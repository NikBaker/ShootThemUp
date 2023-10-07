// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Player/STUPlayerController.h"


DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
}

void ASTUBaseWeapon::Fire()
{
	UE_LOG(BaseWeaponLog, Display, TEXT("Fire!"));

	// ��������� �������
	MakeShot();
}

void ASTUBaseWeapon::MakeShot()
{
	if (!GetWorld()) { return; }

	// �������� ��������� � �������� ����� ��������
	FVector TraceStart;
	FVector TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) { return; }
	
	FHitResult HitResult;

	MakeHit(HitResult, TraceStart, TraceEnd);

	// ���������, ���� �� ����������� � �����-���� �������� ��� ���
	if (HitResult.bBlockingHit)
	{
		// ������� ���� ���������, � �������� ��������
		MakeDamage(HitResult);

		// ������������ ����� �������� �� ����� �� ������, �������� ����� ��������� - ����� ����������� �������� � ��������
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		// �������� ����� � ����� �����������
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

		// ������� ����� �������, � ������� �� ������
		UE_LOG(BaseWeaponLog, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
	}
	else // ���� ������ �� ������
	{
		// ������������ ��� �����
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}
	
} 

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	// �������� ��������� �� ��������� ������
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) { return nullptr; }

	// �������� ��������� �� ����������
	return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	// �������� ��������� �� ���������� ���������
	const auto Controller = GetPlayerController();

	if (!Controller) return false; 

	// �������, ������� ���������� ������� ������ � ���������� �� � ������������
	// ������ �������� - ��������� ������
	// ������ �������� - �������� ������
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;

}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	// ���������� ��������� ������
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) { return false; }

	// �������� ������������� ������ (����� �� ������, �� ������� ���������� �������), ��� ����� ����� ��� Location � Rotation
	// �������� Transform
	// ������ �������� - ��� ������
	// ������ �������� - ������� ���������, � ������� ���� �������� ��� �����
	// ����� ������ ��������
	TraceStart = ViewLocation; //SocketTransform.GetLocation(); ��� ����������� ����������� �������� �� ������ ���� ������ � �������, � �� � �������
	// �����������, ����� �������� ���������� �������� �� ������
	const FVector ShootDirection = ViewRotation.Vector(); //SocketTransform.GetRotation().GetForwardVector();
	// �������� ����� ��������
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld()) { return; }
	// ������ ������ ���� ����� ��������
	// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);

	// ������� ����������� ����� ����� � ��������� �� �����
	// ������� LineTraceSingleByChannel ���������� ���������� � ����������� � ����� ������ ��������, ������� ���������� �� ���� ������ �������
	// ������ �������� - ���������, � ������� ����� ����������� ���������� � ����������� (����� �����������, ����� �������, �����, ������� � ��)
	// ������ �������� - ��������� ����� �������
	// ������ �������� - �������� ����� �������
	// ��������� �������� - CollisionChannel, ��� ������ �������� ����� ����������������� ��� ������� �� �����
	// ����� �������� - ��������� �������� (����� ������ ��������, ����� �� �������� � ����)
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

}

void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult)
{
	// �������� ������, � �������� ������ �� ��������� HitResult
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	// ������� ����
	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}