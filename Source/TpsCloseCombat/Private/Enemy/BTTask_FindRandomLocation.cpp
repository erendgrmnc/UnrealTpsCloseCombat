// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/KhaimeraAIController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer){
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	if (AKhaimeraAIController* const KhaimeraAIController = Cast<AKhaimeraAIController>(OwnerComp.GetAIOwner())){
		if (auto* const npc = KhaimeraAIController->GetPawn()){
			auto const Origin = npc->GetActorLocation();

			//Get navigation system and generate random location
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld())){
				FNavLocation NavLocation;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, NavLocation)){
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
				}

				//Finish w Success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}


