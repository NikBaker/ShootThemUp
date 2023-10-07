// Fill out your copyright notice in the Description page of Project Settings.


#include "Dev/STUDevDamageActor.h"
#include "DrawDebugHelpers.h"	// Для отрисовки сферы
#include "Kismet/GameplayStatics.h"

// Sets default values
ASTUDevDamageActor::ASTUDevDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

}

// Called when the game starts or when spawned
void ASTUDevDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTUDevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Рисуем сферу каждый фрейм
	// Первый параметр - указатель на world
	// Второй параметр - центр сферы (в нашем случае GetActorLocation - место, где находится актор
	// Третий - радиус сферы
	// Четвертый - количество сегментов сферы
	// Пятый - цвет сферы
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);

	// Каждый фрейм будем наносить урон
	// ApplyRadialDamage - наносит урок всем акторам, которые находятся в сфере
	// Первый параметр - указатель на world
	// Второй параметр - количество урона
	// Третий параметр - центр сферы урона
	// Четвертый параметр - радиус сферы
	// Пятый параметр -тип урона
	// Шестой параметр - массив акторов, которым не будет наноситься урок
	// Седьмой параметр - указатель на актора, который наносит дамаг
	// Восьмой указатель - контроллер
	// Девятый параметр - булевый, если true, то во всей сфере получаем полный урон, если false, то полный урон получаем только в центре сферы и чем дальше от центра, тем меньше урона
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);

}

