// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "items/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API ASoul : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlapperComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	
};