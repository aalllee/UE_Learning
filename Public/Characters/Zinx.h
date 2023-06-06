// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Zinx.generated.h"

class UInputMappingContext;
class UInputAction;

class USpringArmComponent;
class UCameraComponent;

class AItem;
class UAnimMontage;



UCLASS()
class SLASH_API AZinx : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AZinx();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	/*
	Callbacks For Input
	*/

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void EKey(const FInputActionValue& Value);
	virtual void Attack(const FInputActionValue& Value) override;
	void Dodge(const FInputActionValue& Value);

	/*
	Play Montage Functions
	*/

	
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	void PlayEquipMontage(FName SectionName);

	UFUNCTION(BlueprintCallable)
	void Disarm();


	UFUNCTION(BlueprintCallable)
	void Arm();


	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* CharacterContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EKeyAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;




private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;



	

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;}
};
