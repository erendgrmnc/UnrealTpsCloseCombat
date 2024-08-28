// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class TPSCLOSECOMBAT_API APatrolPath : public AActor
{
	GENERATED_BODY()
public:
	APatrolPath();

	FVector GetPatrolPoint(const int index ) const;
	int GetPatrolPoinstSize() const;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(MakeEditWidget="true", AllowPrivateAccess="true"))
	TArray<FVector> PatrolPoints;

};
