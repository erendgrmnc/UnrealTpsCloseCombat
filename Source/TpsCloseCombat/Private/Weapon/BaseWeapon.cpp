// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\public\Weapon\BaseWeapon.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/MeleeHitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "TpsCloseCombat/TpsCloseCombatCharacter.h"

// Sets default values
ABaseWeapon::ABaseWeapon(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

USkinnedMeshComponent* ABaseWeapon::GetWeaponMesh(){
	return WeaponMesh;
}

UShapeComponent* ABaseWeapon::GetWeaponCollision(){
	return WeaponCollision;
}

TEnumAsByte<EWeaponEnum::Type> ABaseWeapon::GetWeaponType() const{
	return WeaponType;
}

void ABaseWeapon::PreInitializeComponents(){
	Super::PreInitializeComponents();
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay(){
	Super::BeginPlay();
	WeaponMesh = FindComponentByClass<USkeletalMeshComponent>();
	WeaponCollision = FindComponentByClass<UShapeComponent>();
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void ABaseWeapon::OnWeaponOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                 const FHitResult& SweepResult){
	if (auto* const OwnerComp = GetOwner()){
		if (const auto* CharacterComp = Cast<ATpsCloseCombatCharacter>(OwnerComp)){
			if (IsValid(SweepResult.GetActor())){
				IMeleeHitInterface* MeleeHitInterface = Cast<IMeleeHitInterface>(SweepResult.GetActor());
				if (MeleeHitInterface){
					MeleeHitInterface->MeleeHit_Implementation(SweepResult);
					UGameplayStatics::ApplyDamage(
						SweepResult.GetActor(),
						Damage,
						CharacterComp->GetController(),
						OwnerComp,
						UDamageType::StaticClass());

					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Apply Damage"));
				}
			}
		}
	}
}
