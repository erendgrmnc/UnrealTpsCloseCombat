// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Chase_Player.generated.h"

/**
 * 
 */
UCLASS()
class TPSCLOSECOMBAT_API UBTTask_Chase_Player : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_Chase_Player(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
};
