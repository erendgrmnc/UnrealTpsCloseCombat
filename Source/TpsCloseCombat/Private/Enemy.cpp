// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::EnemyMainAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,TEXT("Called From Enemy Class."));
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::MeleeHit_Implementation(FHitResult HitResult)
{
	//Do impact sound
	
	// Play niagra effects

	//play hit montage
	const UAnimMontage* MontageToPlay = Health == 0 ? OnDeathMontage : OnTakeHitMontage;;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(MontageToPlay))
	{
		AnimInstance->Montage_Play(OnTakeHitMontage);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f) {
		Health = 0;
		Destroy();
	}
	else
	{
		Health -= DamageAmount;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,TEXT("Enemy Took Damage"));
	}

	return DamageAmount;
}

const float AEnemy::GetBaseDamage()
{
	return BaseDamage;
}

UBehaviorTree* AEnemy::GetBehaviorTree() const{
	return BehaviourTree;
}

APatrolPath* AEnemy::GetPatrolPath() const{
	return PatrolPath;
}

