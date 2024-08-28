// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_FindPathPoint.h"

#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/KhaimeraAIController.h"
#include "Enemy/PatrolPath.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(const FObjectInitializer& ObjectInitializer){
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	if (auto* const EnemyAIController = Cast<AKhaimeraAIController>(OwnerComp.GetAIOwner())){
		if (auto* const BlackboardComponent = OwnerComp.GetBlackboardComponent()){
			// get the current patrol path index from the blackboard
			auto const Index = BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey());

			if (auto* Enemy = Cast<AEnemy>(EnemyAIController->GetPawn())){
				//get the current patrol path vector from the enemy - this is local to the patrol path actor.
				auto const Point = Enemy->GetPatrolPath()->GetPatrolPoint(Index);

				// Convert the local vector to a global point.
				auto const GlobalPoint = Enemy->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
				BlackboardComponent->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
