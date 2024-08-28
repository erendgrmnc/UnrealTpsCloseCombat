// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Character/WukongCharacterInterface.h"
#include "Components/BoxComponent.h"
#include "Weapon/BaseWeapon.h"
#include "TpsCloseCombatCharacter.generated.h"


class UWeaponData;
class AWeaponManager;
class UPlayerHUD;
class AWukongPolearm;
class ABaseWeapon;

UENUM()
namespace ETpsCharacterEnum{
	
	enum CurrentEquipType{
		BothHands UMETA(DisplayName = "BothHands"),
		RightTwoHanded UMETA(DisplayName = "RightTwoHanded"),
		LeftTwoHanded UMETA(DisplayName = "LeftTwoHanded")
	};

}

UCLASS(config=Game)

class ATpsCloseCombatCharacter : public ACharacter, public IWukongCharacterInterface{
	GENERATED_BODY()

public:
	ATpsCloseCombatCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void ActivateRightWeapon();
	virtual void DeactivateRightWeapon();

	void StopDodgeRoll();

	void Tick(float DeltaSeconds) override;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Weapon, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* WeaponMesh;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DodgeRollAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RecallAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightHandAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeftHandAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TargetLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess = "true"))
	UShapeComponent* RightWeaponCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess = "true"))
	UShapeComponent* LeftWeaponCollision;
	//TODO(erendgrmnc): move to weapon manager.


	UFUNCTION(Server, Reliable)
	void ToggleRightWeaponCollisionOnServer(ECollisionEnabled::Type collisonType);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const{ return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const{ return FollowCamera; }

protected:
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	bool IsDodgeRolling = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"))
	bool IsAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UAnimMontage* DodgeRollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Target Lock", meta=(AllowPrivateAccess="true"))
	float LockOnRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Target Lock", meta=(AllowPrivateAccess="true"))
	float LockOnRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Target Lock", meta=(AllowPrivateAccess="true"))
	float LockOnRotationSpeed = 10.f;
	AActor* CurrentTarget; 
	bool bIsLockedOn;
	
	// Montage references
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	UAnimMontage* ComboMontage1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	UAnimMontage* ComboMontage2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	UAnimMontage* ComboMontage3;

	UPROPERTY(EditAnywhere, Category="Stats", meta=(AllowPrivateAccess="true"))
	float Health = 100.f;

	UPROPERTY(ReplicatedUsing = OnRepHealth)
	float CurrentHealth;

	UPROPERTY(BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess="true"))
	float Mana = 100.f;
	UPROPERTY(BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess="true"))
	float CurrentMana;
	UPROPERTY(BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess="true"))
	TEnumAsByte<ETpsCharacterEnum::CurrentEquipType> CurrentEquipType = ETpsCharacterEnum::BothHands;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	ABaseWeapon* RightWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	UWeaponData* RightWeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	int RightWeaponSelectedIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	ABaseWeapon* LeftWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	UWeaponData* LeftWeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	int LeftWeaponSelectedIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Weapon", meta=(AllowPrivateAccess="true"))
	TArray<TEnumAsByte<EWeaponEnum::Type>> RightWeaponInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Weapon", meta=(AllowPrivateAccess="true"))
	TArray<TEnumAsByte<EWeaponEnum::Type>> LeftWeaponInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Weapon", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AWeaponManager> WeaponManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Skills")
	UAnimMontage* RecallMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD")
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD")
	UPlayerHUD* PlayerHUD;

	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	UPROPERTY()
	bool IsRecalling = false;

	// Current step in the combo
	int32 ComboStep;
	UWeaponData* LastUsedWeapon;
	TEnumAsByte<ETpsCharacterEnum::CurrentEquipType> LastUsedEquipType;
	
	
	float ComboResetTime;

	FTimerHandle ComboResetTimerHandle;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void TriggerAttack(const FInputActionValue& Value, UWeaponData* Weapon);

	UFUNCTION(Server,Reliable)
	void TriggerRightHandAction(const FInputActionValue& Value);
	
	UFUNCTION(Server,Reliable)
	void TriggerLeftHandAction(const FInputActionValue& Value);
	
	UFUNCTION(Reliable, Server)
	void Attack(const FInputActionValue& Value, UWeaponData* Weapon);

	UFUNCTION(Reliable, Server)
	void Guard(const FInputActionValue& Value, UWeaponData* Weapon);

	
	UFUNCTION(Reliable, NetMulticast)
	void PlayAttackAnimation(UWeaponData* WeaponData, int ComboNum);

	UFUNCTION(Reliable, NetMulticast)
	void PlayGuardAnimation(UWeaponData* WeaponData);

	UFUNCTION(Server, Reliable)
	void Recall();

	UFUNCTION(Reliable, NetMulticast)
	void PlayRecallAnimation();

	UFUNCTION()
	bool FindLockOnTarget();
	UFUNCTION()
	void ToggleLockOn();

	void ResetComboStep();

	void OnCombatResetTimerExpired();

	void ChangeAnimMontage(UAnimMontage* AnimMontageToPlay = nullptr,
	                       std::function<void(UAnimInstance*)> callback = nullptr) const;

	void SetupStimulusSource();

	void SetupWeapons();
	void SetupRightWeapon();
	void SetupLeftWeapon();

	void CreatePlayerHUD();

	void SetupStats();

	void OnStartGuardingFinished(UAnimMontage* Montage, bool bInterrupted);

	void DodgeRoll(const FInputActionValue& Value);
	virtual void Jump() override;

	UFUNCTION()
	void OnRepHealth();

	bool bInputConsumed;
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();
};
