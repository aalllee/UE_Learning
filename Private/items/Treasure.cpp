// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Treasure.h"
#include "Characters/Zinx.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/PickupInterface.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlapperComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->AddGold(this);

		SpawnPickupSound();
		Destroy();
	}
	

}
