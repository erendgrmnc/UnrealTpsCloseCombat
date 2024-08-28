// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/KhaimeraAIController.h"

#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TpsCloseCombat/TpsCloseCombatCharacter.h"

AKhaimeraAIController::AKhaimeraAIController(FObjectInitializer const& ObjectInitializer){
	SetupPerceptionSystem();
}

void AKhaimeraAIController::OnPossess(APawn* InPawn){
	Super::OnPossess(InPawn);
	if (AEnemy* const enemy = Cast<AEnemy>(InPawn)){
		if (UBehaviorTree* const behaviorTree = enemy->GetBehaviorTree()){
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(behaviorTree->BlackboardAsset, BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(behaviorTree);
		}
	}
}

void AKhaimeraAIController::SetupPerceptionSystem(){
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig){
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(
			TEXT("Perception Component")));
		SightConfig->SightRadius = 500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AKhaimeraAIController::AKhaimeraAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense((*SightConfig));
	}
}

void AKhaimeraAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus){
	if (auto* const DetectedCharacter = Cast<ATpsCloseCombatCharacter>(Actor)){
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
