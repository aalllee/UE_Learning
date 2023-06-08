// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterTypes.h"
#include "Characters/BaseCharacter.h"
#include "Enemy.generated.h"



class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()


public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCaused) override;
	virtual void Destroyed() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	virtual void Attack(const FInputActionValue& Value) override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
	virtual void AttackEnd() override;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	
private:
	//AI Behaviour
	void InitializeEnemy();

	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	void StartAttackTimerFinished();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	void SpawnDefaultWeapon();
	UPROPERTY(VisibleAnywhere)
		UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> WeaponClass;



	UPROPERTY(EditAnywhere)
		double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
		double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	UPROPERTY()
		class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float WaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float WaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat)

		float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float PatrollingSpeed = 125.f;
	UPROPERTY(EditAnywhere, Category = Combat)
		float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathLifeSpan = 3.f;


};
