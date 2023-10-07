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

	// Совершаем выстрел
	MakeShot();
}

void ASTUBaseWeapon::MakeShot()
{
	if (!GetWorld()) { return; }

	// Получаем начальную и конечную точку выстрела
	FVector TraceStart;
	FVector TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) { return; }
	
	FHitResult HitResult;

	MakeHit(HitResult, TraceStart, TraceEnd);

	// Проверяем, было ли пересечение с каким-либо объектом или нет
	if (HitResult.bBlockingHit)
	{
		// Наносим урон персонажу, в которого попадаем
		MakeDamage(HitResult);

		// Отрисовываем линию выстрела всё также из сокета, конечная точка отрисовки - точка пересечения выстрела с объектом
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		// Нарисуем сферу в точке пересечения
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

		// Выведем кость скелета, в которую мы попали
		UE_LOG(BaseWeaponLog, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
	}
	else // Если никуда не попали
	{
		// Отрисовываем всю линию
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}
	
} 

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	// Получаем указатель на владельца оружия
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) { return nullptr; }

	// Получаем указатель на контроллер
	return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	// Получаем указатель на контроллер персонажа
	const auto Controller = GetPlayerController();

	if (!Controller) return false; 

	// Функция, которая возвращает позицию камеры и ориентацию ее в пространстве
	// Первый параметр - положение камеры
	// Второй параметр - вращение камеры
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;

}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	// Возвращаем положение сокета
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) { return false; }

	// Получаем трансформацию сокета (точки на оружии, из которой происходит выстрел), нам нужно будет его Location и Rotation
	// Получаем Transform
	// Первый параметр - имя сокета
	// Второй параметр - система координат, в которой хоти получить наш сокет
	// Точка начала выстрела
	TraceStart = ViewLocation; //SocketTransform.GetLocation(); Для правильного отображения выстрела он должен быть связан с камерой, а не с сокетом
	// Направление, вдоль которого происходит стрельба из оружия
	const FVector ShootDirection = ViewRotation.Vector(); //SocketTransform.GetRotation().GetForwardVector();
	// Конечная точка выстрела
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld()) { return; }
	// Теперь рисуем саму линию выстрела
	// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);

	// Находим пересечение нашей линии с объектами на сцене
	// Функция LineTraceSingleByChannel возвращает информацию о пересечении с самым первым объектом, который встретится на пути нашего отрезка
	// Первый параметр - структура, в которой будет содержаться информация о пересечении (время пересечения, точка пересеч, актор, нормаль и тд)
	// Второй параметр - начальная точка отрезка
	// Третий параметр - конечная точка отрезка
	// Четвертый параметр - CollisionChannel, при помощи которого будут отфильтровываться все объекты на сцене
	// Пятый параметр - параметры коллизии (можно задать значение, чтобы не попадать в себя)
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

}

void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult)
{
	// Получаем Актора, в которого попали из структуры HitResult
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	// Наносим урон
	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}