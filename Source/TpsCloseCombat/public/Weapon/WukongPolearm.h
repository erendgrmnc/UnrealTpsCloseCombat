// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "UWeaponInterface.h"
#include "GameFramework/Actor.h"
#include "WukongPolearm.generated.h"

UCLASS()
class TPSCLOSECOMBAT_API AWukongPolearm : public ABaseWeapon, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWukongPolearm();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PrimaryAction() override;
	void SecondaryAction() override;

	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
