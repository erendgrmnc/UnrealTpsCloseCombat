// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_Chase_Player.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Enemy/KhaimeraAIController.h"

UBTTask_Chase_Player::UBTTask_Chase_Player(FObjectInitializer const& ObjectInitializer){
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_Chase_Player::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	if (auto* const EnemyAIController = Cast<AKhaimeraAIController>(OwnerComp.GetAIOwner())){
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		//Move to player location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(EnemyAIController, PlayerLocation);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
	
}
