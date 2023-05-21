// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ZinxAnimInstance.h"
#include "Characters/Zinx.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UZinxAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ZinxCharacter = Cast<AZinx>(TryGetPawnOwner());

	if (ZinxCharacter)
	{
		ZinxCharacterMovement =	ZinxCharacter->GetCharacterMovement();
	}

}

void UZinxAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (ZinxCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(ZinxCharacterMovement->Velocity);
		IsFalling =	ZinxCharacterMovement->IsFalling();
		CharacterState = ZinxCharacter->GetCharacterState();
	}
}
