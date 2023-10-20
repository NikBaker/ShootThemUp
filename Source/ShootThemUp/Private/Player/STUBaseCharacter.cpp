// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"		// Заголовочный файл для использования камеры
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


// Создаем локальную категорию логирования для логирования дамага
DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);


// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Создаем компонент SpringArmComponent в конструкторе:
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	// Приаттачиваем наш компонент к корневому компоненту персонажа (Character)
	SpringArmComponent->SetupAttachment(GetRootComponent()); // GetRootComponent - получение корневого компонента для character
	// Разрешаем вращать камеру по оси Y во время игры
	SpringArmComponent->bUsePawnControlRotation = true;
	// Устанавливаем положение камеры
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	// Создаем компонент камеры в конструкторе:
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	// Приаттачиваем наш компонент камеры к корневому компоненту персонажа (Character)
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Получаем указатель на MovementComponent нашего персонажа и присваиваем и
	// присваиваем в CharacterMovementComponent,
	// чтобы потом через него можно было менять макс скорость персонажа
	CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetCharacterMovement());

	// Создаем компонент для здоровья
	// Данный компонент чисто логический, у него нет представления, нет сцены, поэтому
	// для него не нужно вызывать SetupAttachment
	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

	// Создаем компонент для отображения здоровья персонажа в текстовом поле
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	// Устанавливаем свойство, чтобы пользователь не видел свои хп сверху
	//HealthTextComponent->SetOwnerNoSee(true);


	// Создаем компонент для оружия
	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");

}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Проверяем на нулевые казатели
	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());
	check(WeaponComponent);

	// Так как вызов BeginPlay у актора происходит после вызова BeginPlay его компонентов
	// Поэтому для начала явно вызовем нашу функцию:
	OnHealthChanged(HealthComponent->GetHealth());

	// Подписываемся на делегат OnDeath
	// Первый параметр - указатель на объект, второй - указатель на функцию, которая будет выполняться
	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	// Можем убрать логику обновления текстового компонента со здоровьем с функции тик, чтобы отрисовывать только при изменении жизней
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

	// Подписываемся на делегат LandedDelegate, отвечающий за приземление персонажа
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

	// Для ветра!!!
	/*
	auto Location = GetActorLocation();
	Location.X -= 5.0f;
	SetActorLocation(Location);
	*/
	

	// Каждый фрейм передаем компоненту HealthTextComponent значение переменной Health
	//const auto Health = HealthComponent->GetHealth();
	//HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));

	// Проверка нанесения урона персонажу каждый фрейм
	// Первый параметр - количество урона
	// Второй параметр - эвент, который вызывает урон (взрыв, пуля и тд.)
	// Третий параметр - контроллер
	// Четвертый параметр - от кого приходит урон
	// TakeDamage(0.1f, FDamageEvent{}, Controller, this);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Связываем наши функции для управления MoveForward и MoveRight с мэппингом, который задали в Эдиторе
	// Первый аргумент - имя нашего мэппинга
	// Второй - указатель на объект, функция, которого будет вызываться
	// Тертий - ссылка на функцию, которая будет вызываться каждый фрейм с текущим состоянием нашей кнопки
	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
	// Аналогично для MoveRight
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
	// Для поворотов камеры мышкой:
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookAround", this, &ASTUBaseCharacter::LookAround);

	// Биндим прыжок:
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
	// Биндим ускорение:
	//PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
	//PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);

	// Биндим стрельбу
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Fire);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	if (Amount == 0.0f) return;
	// Для перемещения используем функцию AddMovementInput
	// Первый аргумент - направление, вдоль которого хотим перемещать нашего персонажа
	// Второй парамерт - scale, число, на которое будет умножаться покоординатно наш вектор направления
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f) return;
	// Аналогично MoveForward
	AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::LookUp(float Amount)
{
	// Для поворота камеры используется функция AddControllerPitchInput
	// Принимает один параметр - значение угла, который будет добавлен к вращению
	AddControllerPitchInput(Amount);
}

void ASTUBaseCharacter::LookAround(float Amount)
{
	// Аналогично LookUp
	AddControllerYawInput(Amount);
}

void ASTUBaseCharacter::OnStartRunning()
{
	// Меняем максимальную скорость нашего персонажа
	//CharacterMovementComponent->MaxWalkSpeed = 1000.0f;
	if (IsMovingForward) {
		CharacterMovementComponent->MaxWalkSpeed *= RunModifier;
	}
	WantsToRun = true;
}

void ASTUBaseCharacter::OnStopRunning()
{
	// Возвращаем прежнюю максимальную скорость нашего персонажа
	//CharacterMovementComponent->MaxWalkSpeed = 600.0f;
	if (CharacterMovementComponent->MaxWalkSpeed > 600.0f) {
		CharacterMovementComponent->MaxWalkSpeed /= RunModifier;
	}
	WantsToRun = false;
}

bool ASTUBaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();  // !GetVelocity().IsZero() нужна для проверки, что персонаж не бежит на месте
}

float ASTUBaseCharacter::GetMovmentDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();	// высчитываем нормаль вектора скорости
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));	// Высчитываем скалярное произведение между форвард вектором и вектором нормали (затем берем от этого арккосинус, чтобы получить угол)
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);	// Высчитываем ортогональный вектор (чтобы учитывать знак)
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);		// Переводим AngleBetWeen в градусы и домножаем на знак
}

void ASTUBaseCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

	// Проигрываем анимацию смерти персонажа
	// Первый параметр - указатель на анимацию, которую хотим проиграть
	PlayAnimMontage(DeathAnimMontage);

	// После смерти останавливаем всё движение персонажа
	GetCharacterMovement()->DisableMovement();

	// Добавляем уничтожение нашего чарактера через пять секунд после смерти
	SetLifeSpan(5.0f);

	// Заменяем дефолтный класс на Spectator Class - класс наблюдателя, чтобы после смерти можно было управлять свободной камерой
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	// При смерти отключаем колизии с капсулой персонажа
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
	// Логируем z-компоненту скорости персонажа
	// Будем наносить персонажу урон при падении, который будет пропорционален скорости падения
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
	UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);


	if (FallVelocityZ < 3000 && FallVelocityZ > 2000)
	{
		TakeDamage(100.0f, FDamageEvent{}, nullptr, nullptr);
		return;
	}

	if (FallVelocityZ > 3000) { return; }

	// Проверяем, что скорость не меньше 900
	if (FallVelocityZ < LandedDamageVelocity.X) { return; }

	// Высчитываем урон от падения
	// Первый параметр - входной отрезок (в нашем случае вектор из мин и макс допустимой скорости)
	// Второй параметр - выходной отрезок (в нашем случае вектор из мин и макс допустимого урона)
	// Третий параметр - значение (в нашем случае - скорость, с которой мы упали)
	// На выходе получаем число с отрезка LandedDamage, которое в процентном соотношении совпадает со скоростью на отрезке скорости
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	// Залогируем наносимый урон
	UE_LOG(BaseCharacterLog, Display, TEXT("Final damage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);

}

