// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

	template<typename T>
	T Avg(T First, T Second);

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;
};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First+Second)/2;
}
