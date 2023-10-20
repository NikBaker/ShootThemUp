// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"		// ������������ ���� ��� ������������� ������
#include "Components/InputComponent.h"	
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/MovementComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


// ������� ��������� ��������� ����������� ��� ����������� ������
DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);


// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ������� ��������� SpringArmComponent � ������������:
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	// ������������� ��� ��������� � ��������� ���������� ��������� (Character)
	SpringArmComponent->SetupAttachment(GetRootComponent()); // GetRootComponent - ��������� ��������� ���������� ��� character
	// ��������� ������� ������ �� ��� Y �� ����� ����
	SpringArmComponent->bUsePawnControlRotation = true;
	// ������������� ��������� ������
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	// ������� ��������� ������ � ������������:
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	// ������������� ��� ��������� ������ � ��������� ���������� ��������� (Character)
	CameraComponent->SetupAttachment(SpringArmComponent);

	// �������� ��������� �� MovementComponent ������ ��������� � ����������� �
	// ����������� � CharacterMovementComponent,
	// ����� ����� ����� ���� ����� ���� ������ ���� �������� ���������
	CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetCharacterMovement());

	// ������� ��������� ��� ��������
	// ������ ��������� ����� ����������, � ���� ��� �������������, ��� �����, �������
	// ��� ���� �� ����� �������� SetupAttachment
	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

	// ������� ��������� ��� ����������� �������� ��������� � ��������� ����
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	// ������������� ��������, ����� ������������ �� ����� ���� �� ������
	//HealthTextComponent->SetOwnerNoSee(true);


	// ������� ��������� ��� ������
	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");

}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ��������� �� ������� ��������
	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());
	check(WeaponComponent);

	// ��� ��� ����� BeginPlay � ������ ���������� ����� ������ BeginPlay ��� �����������
	// ������� ��� ������ ���� ������� ���� �������:
	OnHealthChanged(HealthComponent->GetHealth());

	// ������������� �� ������� OnDeath
	// ������ �������� - ��������� �� ������, ������ - ��������� �� �������, ������� ����� �����������
	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	// ����� ������ ������ ���������� ���������� ���������� �� ��������� � ������� ���, ����� ������������ ������ ��� ��������� ������
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

	// ������������� �� ������� LandedDelegate, ���������� �� ����������� ���������
	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);


}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	auto CurrentLocation = GetActorLocation();
	if (CurrentLocation.X > 20066 - 1000 && CurrentLocation.X < 20066 + 1000 && CurrentLocation.Y > -2500 && CurrentLocation.Y < 2500)
	{
		// Finish
		ULevel* CurrentLevel = GetWorld()->GetCurrentLevel();
		UGameplayStatics::OpenLevel(GetWorld(), FName(*(CurrentLevel->GetOuter()->GetName())), true);
	}
	

	//auto Velocity = GetVelocity();
	//UE_LOG(BaseCharacterLog, Warning, TEXT("Velocity: %fl"), Velocity.Size());

	//GetCharacterMovement()->MaxWalkSpeed = 300.f;

	// ��� �����!!!
	/*
	auto Location = GetActorLocation();
	Location.X -= 5.0f;
	SetActorLocation(Location);
	*/
	

	// ������ ����� �������� ���������� HealthTextComponent �������� ���������� Health
	//const auto Health = HealthComponent->GetHealth();
	//HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));

	// �������� ��������� ����� ��������� ������ �����
	// ������ �������� - ���������� �����
	// ������ �������� - �����, ������� �������� ���� (�����, ���� � ��.)
	// ������ �������� - ����������
	// ��������� �������� - �� ���� �������� ����
	// TakeDamage(0.1f, FDamageEvent{}, Controller, this);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ��������� ���� ������� ��� ���������� MoveForward � MoveRight � ���������, ������� ������ � �������
	// ������ �������� - ��� ������ ��������
	// ������ - ��������� �� ������, �������, �������� ����� ����������
	// ������ - ������ �� �������, ������� ����� ���������� ������ ����� � ������� ���������� ����� ������
	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
	// ���������� ��� MoveRight
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
	// ��� ��������� ������ ������:
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookAround", this, &ASTUBaseCharacter::LookAround);

	// ������ ������:
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
	// ������ ���������:
	//PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
	//PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);

	// ������ ��������
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Fire);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	if (Amount == 0.0f) return;
	// ��� ����������� ���������� ������� AddMovementInput
	// ������ �������� - �����������, ����� �������� ����� ���������� ������ ���������
	// ������ �������� - scale, �����, �� ������� ����� ���������� ������������� ��� ������ �����������
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f) return;
	// ���������� MoveForward
	AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::LookUp(float Amount)
{
	// ��� �������� ������ ������������ ������� AddControllerPitchInput
	// ��������� ���� �������� - �������� ����, ������� ����� �������� � ��������
	AddControllerPitchInput(Amount);
}

void ASTUBaseCharacter::LookAround(float Amount)
{
	// ���������� LookUp
	AddControllerYawInput(Amount);
}

void ASTUBaseCharacter::OnStartRunning()
{
	// ������ ������������ �������� ������ ���������
	//CharacterMovementComponent->MaxWalkSpeed = 1000.0f;
	if (IsMovingForward) {
		CharacterMovementComponent->MaxWalkSpeed *= RunModifier;
	}
	WantsToRun = true;
}

void ASTUBaseCharacter::OnStopRunning()
{
	// ���������� ������� ������������ �������� ������ ���������
	//CharacterMovementComponent->MaxWalkSpeed = 600.0f;
	if (CharacterMovementComponent->MaxWalkSpeed > 600.0f) {
		CharacterMovementComponent->MaxWalkSpeed /= RunModifier;
	}
	WantsToRun = false;
}

bool ASTUBaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();  // !GetVelocity().IsZero() ����� ��� ��������, ��� �������� �� ����� �� �����
}

float ASTUBaseCharacter::GetMovmentDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();	// ����������� ������� ������� ��������
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));	// ����������� ��������� ������������ ����� ������� �������� � �������� ������� (����� ����� �� ����� ����������, ����� �������� ����)
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);	// ����������� ������������� ������ (����� ��������� ����)
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);		// ��������� AngleBetWeen � ������� � ��������� �� ����
}

void ASTUBaseCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

	// ����������� �������� ������ ���������
	// ������ �������� - ��������� �� ��������, ������� ����� ���������
	PlayAnimMontage(DeathAnimMontage);

	// ����� ������ ������������� �� �������� ���������
	GetCharacterMovement()->DisableMovement();

	// ��������� ����������� ������ ��������� ����� ���� ������ ����� ������
	SetLifeSpan(5.0f);

	// �������� ��������� ����� �� Spectator Class - ����� �����������, ����� ����� ������ ����� ���� ��������� ��������� �������
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	// ��� ������ ��������� ������� � �������� ���������
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);


	// Restart the current level
	ULevel* CurrentLevel = GetWorld()->GetCurrentLevel();
	UGameplayStatics::OpenLevel(GetWorld(), FName(*(CurrentLevel->GetOuter()->GetName())), true);
	//
	
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	// �������� z-���������� �������� ���������
	// ����� �������� ��������� ���� ��� �������, ������� ����� �������������� �������� �������
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
	UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);


	if (FallVelocityZ < 3000 && FallVelocityZ > 2000)
	{
		TakeDamage(100.0f, FDamageEvent{}, nullptr, nullptr);
		return;
	}

	if (FallVelocityZ > 3000) { return; }

	// ���������, ��� �������� �� ������ 900
	if (FallVelocityZ < LandedDamageVelocity.X) { return; }

	// ����������� ���� �� �������
	// ������ �������� - ������� ������� (� ����� ������ ������ �� ��� � ���� ���������� ��������)
	// ������ �������� - �������� ������� (� ����� ������ ������ �� ��� � ���� ����������� �����)
	// ������ �������� - �������� (� ����� ������ - ��������, � ������� �� �����)
	// �� ������ �������� ����� � ������� LandedDamage, ������� � ���������� ����������� ��������� �� ��������� �� ������� ��������
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	// ���������� ��������� ����
	UE_LOG(BaseCharacterLog, Display, TEXT("Final damage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);

}

