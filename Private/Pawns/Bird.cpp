// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// Sets default values
ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BIRDMESH"));
	BirdMesh->SetupAttachment(RootComponent);

	SpringArm1 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm1->SetupAttachment(GetRootComponent());
	SpringArm1->TargetArmLength = 300.f;

	Camera1 = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	Camera1->SetupAttachment(SpringArm1);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}


void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
}

void ABird::MoveForward(float Value)
{
	if ((Controller != nullptr)&&(Value != 0.f)) {
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Value);
	
	}
}

void ABird::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	if ((Controller != nullptr) && (DirectionValue != 0.f)) {
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, DirectionValue);
	}
}

void ABird::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}


void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	
	}
//	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABird::MoveForward);
}

