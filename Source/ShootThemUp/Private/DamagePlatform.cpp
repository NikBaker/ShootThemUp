// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagePlatform.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, String);

DEFINE_LOG_CATEGORY_STATIC(DamagePlatformLog, All, All);


// Sets default values
ADamagePlatform::ADamagePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

	BaseMesh->OnComponentHit.AddDynamic(this, &ADamagePlatform::OnHit);


	//SetActorScale3D({ 1.0, 5.0, 5.0 });
}

// Called when the game starts or when spawned
void ADamagePlatform::BeginPlay()
{
	Super::BeginPlay();

	SetColor(PlatformColor);

	Scale = GetActorScale3D();
	Location = GetActorLocation();
	
}

// Called every frame
void ADamagePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*Scale.X += DeltaTime * 0.2f;
	SetActorScale3D(Scale);*/
	/*Location.X += DeltaTime * 50.0f;
	SetActorLocation(Location);*/

	if (IsMovable)
	{
		FVector CurrentLocation = GetActorLocation();
		float time = GetWorld()->GetTimeSeconds();
		if (IsLeftMove)
		{
			CurrentLocation.X = Location.X + Amplitude * FMath::Sin(Frequency * time);
		}
		else
		{
			CurrentLocation.X = Location.X - Amplitude * FMath::Sin(Frequency * time);
		}

		SetActorLocation(CurrentLocation);
	}

	//UE_LOG(DamagePlatformLog, Warning, TEXT("Actor name: %s Color: %s"), *GetName(), *PlatformColor.ToString());

	//DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);
	if (PlatformColor == FLinearColor::Red)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);
	}
	else
	{
		//UGameplayStatics::ApplyRadialDamage(GetWorld(), 0, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);
	}
}
	

void ADamagePlatform::SetColor(const FLinearColor& Color)
{
	UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("Color", Color);
	}

	PlatformColor = Color;
}

void ADamagePlatform::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//PrintString(FString::Printf(TEXT("Hit: %s"), *OtherActor->GetName()));
	
	if (!IsActivatedPlatform)
	{
		IsActivatedPlatform = true;
		SetColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f));
		//PrintString(FString::Printf(TEXT("FirstTimer START")));
		GetWorldTimerManager().SetTimer(OrangeTimerHandle, this, &ADamagePlatform::OnOrangeTimerFired, OrangeTimerRate, false);

	}
}

void ADamagePlatform::OnOrangeTimerFired()
{
	SetColor(FLinearColor::Red);
	//PrintString(FString::Printf(TEXT("FirstTimer FINISH")));
	GetWorldTimerManager().ClearTimer(OrangeTimerHandle);
	//PrintString(FString::Printf(TEXT("SecondTimer START")));
	GetWorldTimerManager().SetTimer(RedTimerHandle, this, &ADamagePlatform::OnRedTimerFired, RedTimerRate, false);
}

void ADamagePlatform::OnRedTimerFired()
{
	SetColor(FLinearColor::White);
	//PrintString(FString::Printf(TEXT("SecondTimer FINISH")));
	GetWorldTimerManager().ClearTimer(RedTimerHandle);
	//PrintString(FString::Printf(TEXT("ThirdTimer START")));
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ADamagePlatform::OnReloadTimerFired, ReloadTimerRate, false);
}

void ADamagePlatform::OnReloadTimerFired()
{
	//PrintString(FString::Printf(TEXT("ThirdTimer FINISH")));
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	IsActivatedPlatform = false;
}