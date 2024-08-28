// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "KhaimeraAIController.generated.h"

/**
 * 
 */
UCLASS()
class TPSCLOSECOMBAT_API AKhaimeraAIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit AKhaimeraAIController(FObjectInitializer const& ObjectInitializer);
protected:
	virtual void OnPossess(APawn* InPawn) override;
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
