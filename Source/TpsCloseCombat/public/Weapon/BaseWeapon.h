// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UENUM(BlueprintType)
namespace EWeaponEnum{
	enum Type{
		Sword UMETA(DisplayName = "Sword"),
		Shield UMETA(DisplayName = "Shield"),
		MageStaff UMETA(DisplayName = "MageStaff"),
		Bow UMETA(DisplayName = "Bow"),
		Polearm UMETA(DisplayName = "Polearm"),
		Unarmed UMETA(DisplayName = "Unarmed")
		
	};
}

UCLASS()
class TPSCLOSECOMBAT_API ABaseWeapon : public AActor{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWeapon();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(Server, Reliable)
	virtual void OnWeaponOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);

	UFUNCTION()
	virtual USkinnedMeshComponent* GetWeaponMesh();
	
	UFUNCTION()
	virtual UShapeComponent* GetWeaponCollision();

	UFUNCTION()
	TEnumAsByte<EWeaponEnum::Type> GetWeaponType() const;

	void PreInitializeComponents() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	USkinnedMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"));
	TEnumAsByte<EWeaponEnum::Type> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	float AttackDelay = 1.f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	UShapeComponent* WeaponCollision;
};
