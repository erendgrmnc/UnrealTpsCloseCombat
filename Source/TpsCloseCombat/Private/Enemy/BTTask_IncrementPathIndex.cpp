// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_IncrementPathIndex.h"

#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/KhaimeraAIController.h"
#include "Enemy/PatrolPath.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(const FObjectInitializer& ObjectInitializer){
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	if (auto* const EnemyAIController = Cast<AKhaimeraAIController>(OwnerComp.GetAIOwner())){
		if (auto* Enemy = Cast<AEnemy>(EnemyAIController->GetPawn())){
			if (auto* const BlackboardComponent = OwnerComp.GetBlackboardComponent()){
				const int TotalPathCount = Enemy->GetPatrolPath()->GetPatrolPoinstSize();
				auto Index = BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey());

				//Change Direction if we are at first or last index if we are in biDirectional mode.
				if (bBiDirectional){
					const int MinIndex = 0;
					const int MaxIndex = TotalPathCount -1;
					
					if (Index >= MaxIndex && Direction == EDirectionType::Forward){
						Direction = EDirectionType::Reverse;
					}else if (Index == MinIndex && Direction == EDirectionType::Reverse)
						Direction = EDirectionType::Forward;
				}

				// Write new index value to blackboard
				const int newRoute = Direction == EDirectionType::Forward ? ++Index : --Index;
				const int newValue = (newRoute) % TotalPathCount;
				BlackboardComponent->SetValueAsInt(GetSelectedBlackboardKey(), newValue);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}

		}
	}

	return EBTNodeResult::Failed;
}
