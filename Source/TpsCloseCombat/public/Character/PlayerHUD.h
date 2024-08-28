// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerHUD.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class TPSCLOSECOMBAT_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetInventorySlot(int slotNum);
	void SetHealth(float currentHealth, float maxHealth);
	void SetMana(float currentMana, float maxMana);
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons", meta=(AllowPrivateAccess="true", BindWidget))
	TArray<UImage*> InventoryImages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons", meta=(AllowPrivateAccess="true", BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons", meta=(AllowPrivateAccess="true", BindWidget))
	UProgressBar* ManaBar;
};
