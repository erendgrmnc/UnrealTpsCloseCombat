// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/StopDodgeRollState.h"

#include "TpsCloseCombat/TpsCloseCombatCharacter.h"

void UStopDodgeRollState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference){
	if (MeshComp && MeshComp->GetOwner()){
		TpsCloseCombatCharacter = Cast<ATpsCloseCombatCharacter>(MeshComp->GetOwner());
		if (TpsCloseCombatCharacter){
			TpsCloseCombatCharacter->StopDodgeRoll();
		}
	}
}

void UStopDodgeRollState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference){
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
