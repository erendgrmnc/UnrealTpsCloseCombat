// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class TPSCLOSECOMBAT_API AEnemyMelee : public AEnemy
{
	GENERATED_BODY()

public:
	AEnemyMelee();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(AllowPrivateAccess="true"))
	TArray<UAnimMontage*> AttackMontages;
private:
	
};
