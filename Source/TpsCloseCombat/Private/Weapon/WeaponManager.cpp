// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponManager.h"

int UWeaponData::GetTotalAttackMontagesCount(){
	return AttackAnimations.Num();
}

UAnimMontage* UWeaponData::GetAttackAnimation(int comboStep){
	return AttackAnimations[comboStep];
}

UAnimMontage* UWeaponData::GetStartGuardingAnimation(){
	return StartGuardAnimation;
}

UAnimMontage* UWeaponData::GetIdleGuardingAnimation(){
	return IdleGuardAnimation;
}

// Sets default values
AWeaponManager::AWeaponManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

TSubclassOf<ABaseWeapon> AWeaponManager::GetBaseWeaponClass(EWeaponEnum::Type WeaponType){
	for (const auto& weapon : Weapons){
		if (weapon.GetDefaultObject()->GetWeaponType() == WeaponType){
			return weapon;
		}
	}

	return Weapons[0];
}

// Called when the game starts or when spawned
void AWeaponManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UWeaponData* AWeaponManager::GetWeaponData(EWeaponEnum::Type weaponType){
	return WeaponTypeDataMap[weaponType].GetDefaultObject();
}

