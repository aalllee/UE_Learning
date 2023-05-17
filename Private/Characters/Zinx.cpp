// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Zinx.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
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
}

void AZinx::Move(const FInputActionValue& Value)
{
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

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::SanitizeFloat(LookAxisVector.Y));
}

void AZinx::Jump(const FInputActionValue& Value)
{
	//GetCharacterMovement()->DoJump(true);
}

void AZinx::EKey(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString("E key pressed"));
}

void AZinx::Attack(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString("Attack"));
}

void AZinx::Dodge(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString("Dodge"));
}

void AZinx::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZinx::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Cast player input to EnhancedInputComponent and Bind Action
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AZinx::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZinx::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this,&ACharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &AZinx::EKey);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AZinx::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AZinx::Dodge);
	}

}

