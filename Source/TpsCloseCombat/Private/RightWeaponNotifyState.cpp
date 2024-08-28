// Fill out your copyright notice in the Description page of Project Settings.


#include "RightWeaponNotifyState.h"

#include "TpsCloseCombat/TpsCloseCombatCharacter.h"

void URightWeaponNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{ 
	if (MeshComp && MeshComp->GetOwner())
	{
		TpsCloseCombatCharacter = Cast<ATpsCloseCombatCharacter>(MeshComp->GetOwner());
		if (TpsCloseCombatCharacter){
			TpsCloseCombatCharacter->ActivateRightWeapon();
		}
	}
}

void URightWeaponNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (TpsCloseCombatCharacter){
			TpsCloseCombatCharacter->DeactivateRightWeapon(); 
		}
	}
}