// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Treasure.h"
#include "Characters/Zinx.h"
#include "Kismet/GameplayStatics.h"


void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlapperComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZinx* PlayerCharacter = Cast<AZinx>(OtherActor);
	if (PlayerCharacter)
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}
		Destroy();
	}


}
