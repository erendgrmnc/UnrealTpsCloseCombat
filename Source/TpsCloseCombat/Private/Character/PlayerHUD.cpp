// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerHUD.h"

void UPlayerHUD::SetInventorySlot(int slotNum){
}

void UPlayerHUD::SetHealth(float currentHealth, float maxHealth){
	if (maxHealth != 0 && HealthBar != nullptr){
		HealthBar->SetPercent(currentHealth / maxHealth);
	}
}

void UPlayerHUD::SetMana(float currentMana, float maxMana){
	if (maxMana != 0.f && ManaBar){
		ManaBar->SetPercent(currentMana / maxMana);
	}
}
