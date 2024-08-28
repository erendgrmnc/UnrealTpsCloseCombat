// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "GameFramework/Actor.h"
#include "WeaponManager.generated.h"

UCLASS(Blueprintable)
class UWeaponData : public UObject{
	GENERATED_BODY()
public:
	int	GetTotalAttackMontagesCount();
	UAnimMontage* GetAttackAnimation(int comboStep);
	UAnimMontage* GetStartGuardingAnimation();
	UAnimMontage* GetIdleGuardingAnimation();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations", meta=(AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AttackAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations", meta=(AllowPrivateAccess = "true"))
	UAnimMontage* StartGuardAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations", meta=(AllowPrivateAccess = "true"))
	UAnimMontage* IdleGuardAnimation;
};

UCLASS()
class TPSCLOSECOMBAT_API AWeaponManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponManager();
	virtual void Tick(float DeltaTime) override;
	UWeaponData* GetWeaponData(EWeaponEnum::Type weaponType);
	TSubclassOf<ABaseWeapon> GetBaseWeaponClass(EWeaponEnum::Type WeaponType);
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABaseWeapon>> Weapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<EWeaponEnum::Type>, TSubclassOf<UWeaponData>> WeaponTypeDataMap;
};
