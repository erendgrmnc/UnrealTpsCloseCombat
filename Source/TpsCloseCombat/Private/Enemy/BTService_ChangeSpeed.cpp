// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTService_ChangeSpeed.h"

#include "Enemy.h"
#include "Enemy/KhaimeraAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnitConversion.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed(){
	NodeName = TEXT("Change Speed");
	bNotifyBecomeRelevant =true;
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	if (auto const EnemyAIController = Cast<AKhaimeraAIController>(OwnerComp.GetAIOwner())){
		if (auto* const Enemy = Cast<AEnemy>(EnemyAIController->GetPawn())){
			Enemy->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}
