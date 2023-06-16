// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Zinx.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AttributeComponent.h"
#include "items/Item.h"
#include "items/Soul.h"
#include "items/Treasure.h"
#include "Animation/AnimInstance.h"
#include "items/Weapons/Weapon.h"
#include "Components/StaticMeshComponent.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
//Enhanced input includes //////////////
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
AZinx::AZinx()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);

}

void AZinx::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterContext, 0);
		
		}
		
	}

	Tags.Add(FName("PlayerCharacter"));


	//Set HUD values
	InitializeSlashOverlay();

}

void AZinx::InitializeSlashOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
			SlashOverlay = SlashHUD->GetSlashOverlay();
			if (SlashOverlay && Attributes)
			{
				SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SlashOverlay->SetStaminaBarPercent(1.f);
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSouls(0);
			}
		}

	}
}



void AZinx::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	//type based on the mapping setup in unreal
 	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

void AZinx::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);

	
}

void AZinx::JumpAct(const FInputActionValue& Value)
{
	//GetCharacterMovement()->DoJump(true);
}

void AZinx::EKey(const FInputActionValue& Value)
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		
		const bool bCanDisarm = (ActionState == EActionState::EAS_Unoccupied) &&
			(CharacterState != ECharacterState::ECS_Unequipped);

		const bool bCanArm = ActionState == EActionState::EAS_Unoccupied &&
			CharacterState == ECharacterState::ECS_Unequipped &&
			EquippedWeapon;


		if (bCanDisarm)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("DISARM"));
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (bCanArm)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString("ARM"));
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	
	}
}

void AZinx::Attack(const FInputActionValue& Value)
{
	
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}
void AZinx::Dodge(const FInputActionValue& Value)
{
	
}
void AZinx::SprintStart(const FInputActionValue& Value)
{
	if (Attributes && Attributes->GetStamina() > Attributes->GetSprintCost())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("START Sprint"));
		//Attributes->UseStamina(true);
		isSprinting = true;
	}
}
void AZinx::SprintEnd(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("END"));
	
	if (Attributes)
	{
		//Attributes->UseStamina(false);
		isSprinting = false;

	}
	
}
void AZinx::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AZinx::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AZinx::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AZinx::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AZinx::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AZinx::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AZinx::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);

	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AZinx::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AZinx::CanAttack()
{
	return  ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void AZinx::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSprintStamina(DeltaTime);
}

void AZinx::UpdateSprintStamina(float DeltaTime)
{
	if (Attributes && Attributes->GetStamina() < Attributes->GetSprintCost())
	{
		isSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (Attributes && isSprinting)
	{
		Attributes->UseStamina(DeltaTime);
	}
	else if (Attributes && !isSprinting)
	{
		Attributes->RegenStamina(DeltaTime);
	}

	if (SlashOverlay)
	{
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}


void AZinx::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Cast player input to EnhancedInputComponent and Bind Action
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AZinx::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZinx::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this,&AZinx::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Started, this, &AZinx::EKey);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AZinx::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AZinx::Dodge);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AZinx::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AZinx::SprintEnd);
	}

}

float AZinx::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCaused)
{
	HandleDamage(DamageAmount);

	SetHUDHealth();

	return DamageAmount;
}

void AZinx::Jump()
{
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		Super::Jump();
	}
}

void AZinx::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());

	}
}

void AZinx::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void AZinx::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AZinx::AddSouls(ASoul* Soul)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("AddSouls"));
	
	if (Attributes && SlashOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		SlashOverlay->SetSouls(Attributes->GetSouls());
	
	}

}

void AZinx::AddGold(ATreasure* Treasure)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		SlashOverlay->SetGold(Attributes->GetGold());

	}
}


