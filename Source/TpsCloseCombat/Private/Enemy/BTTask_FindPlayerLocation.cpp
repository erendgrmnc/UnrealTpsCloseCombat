// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer){
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	if (auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)){
		//Get Player location to use as an origin
		auto const PlayerLocation = Player->GetActorLocation();
		if (SearchRandom){
			FNavLocation NavLoc;
			// get the navigation system and generate a random location near the player.
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld())){

				//Try to get a random location near player
				if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, NavLoc)){
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLoc.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
