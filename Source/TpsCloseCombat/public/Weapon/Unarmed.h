// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UWeaponInterface.h"
#include "Weapon/BaseWeapon.h"
#include "Unarmed.generated.h"

/**
 * 
 */
UCLASS()
class TPSCLOSECOMBAT_API AUnarmed : public ABaseWeapon, public IWeaponInterface
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void PrimaryAction() override;
	virtual void SecondaryAction() override;

	virtual void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
