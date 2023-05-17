// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Item.h"

#include "Slash/DebugMacros.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Avg<int32>(1, 3);

	UE_LOG(LogTemp, Warning, TEXT("Begin Play called"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Emerald, FString("Item OnScreen message"));
	
	}

	UWorld* World = GetWorld();
	
	SetActorLocation(FVector(0.f, 0.f, 50.f));

	DRAW_SPHERE(GetActorLocation())
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

