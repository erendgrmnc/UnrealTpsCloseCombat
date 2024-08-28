// Copyright Epic Games, Inc. All Rights Reserved.

#include "TpsCloseCombatCharacter.h"

#include "Enemy.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/PlayerHUD.h"
#include "Enemy/MeleeHitInterface.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "UObject/UnrealTypePrivate.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/WeaponManager.h"
#include "Weapon/WukongPolearm.h"


//////////////////////////////////////////////////////////////////////////
// ATpsCloseCombatCharacter

ATpsCloseCombatCharacter::ATpsCloseCombatCharacter(){
	bInputConsumed = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	PlayerHUD = nullptr;
	PlayerHUDClass = nullptr;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 2000.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	SetupStimulusSource();

	ComboStep = 0;
	ComboResetTime = 2.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATpsCloseCombatCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ATpsCloseCombatCharacter, IsDodgeRolling, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(ATpsCloseCombatCharacter, CurrentHealth, COND_OwnerOnly);
}

void ATpsCloseCombatCharacter::DeactivateRightWeapon(){
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Deactivate Weapon"));
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//ToggleRightWeaponCollisionOnServer_Implementation(ECollisionEnabled::NoCollision);
}

void ATpsCloseCombatCharacter::ToggleRightWeaponCollisionOnServer_Implementation(ECollisionEnabled::Type collisonType){
	RightWeaponCollision->SetCollisionEnabled(collisonType);
}

void ATpsCloseCombatCharacter::ActivateRightWeapon(){
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Activate Weapon"));
	if (RightWeaponCollision != nullptr){
		RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ATpsCloseCombatCharacter::BeginPlay(){
	// Call the base class  
	Super::BeginPlay();

	SetupStats();
	SetupWeapons();
	CreatePlayerHUD();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)){
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())){
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (RightWeaponCollision){
		RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(RightWeapon, &ABaseWeapon::OnWeaponOverlap);

		RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		RightWeaponCollision->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
		RightWeaponCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATpsCloseCombatCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent){
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)){
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATpsCloseCombatCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Dodging
		EnhancedInputComponent->BindAction(DodgeRollAction, ETriggerEvent::Triggered, this,
		                                   &ATpsCloseCombatCharacter::DodgeRoll);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATpsCloseCombatCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATpsCloseCombatCharacter::Look);

		EnhancedInputComponent->BindAction(RightHandAction, ETriggerEvent::Triggered, this,
		                                   &ATpsCloseCombatCharacter::TriggerRightHandAction);

		EnhancedInputComponent->BindAction(LeftHandAction, ETriggerEvent::Triggered, this,
		                                   &ATpsCloseCombatCharacter::TriggerLeftHandAction);

		EnhancedInputComponent->BindAction(RecallAction, ETriggerEvent::Triggered, this,
		                                   &ATpsCloseCombatCharacter::Recall);
		EnhancedInputComponent->BindAction(TargetLockAction, ETriggerEvent::Triggered, this,
		                                   &ATpsCloseCombatCharacter::ToggleLockOn);
	}
}

void ATpsCloseCombatCharacter::Move(const FInputActionValue& Value){
	if (IsDodgeRolling){
		return;
	}
	if (IsRecalling){
		IsRecalling = false;
	}

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr){
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATpsCloseCombatCharacter::Look(const FInputActionValue& Value){
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr){
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATpsCloseCombatCharacter::TriggerLeftHandAction_Implementation(const FInputActionValue& Value){
	if (!bInputConsumed){
		switch (CurrentEquipType){
		case ETpsCharacterEnum::BothHands: {
			//TODO(erendgrmnc): If it's a shield, defend
			if (LeftWeapon->GetWeaponType() == EWeaponEnum::Shield){
				Guard(Value, LeftWeaponData);
			}
			else{
				Attack(Value, LeftWeaponData);
			}

		}
		default: ;
		}
	}
}

void ATpsCloseCombatCharacter::TriggerRightHandAction_Implementation(const FInputActionValue& Value){
	if (!bInputConsumed){
		switch (CurrentEquipType){
		case ETpsCharacterEnum::BothHands: {
			Attack(Value, RightWeaponData);
		}
		default: ;
		}
	}
}

void ATpsCloseCombatCharacter::TriggerAttack_Implementation(const FInputActionValue& Value, UWeaponData* Weapon){
}

void ATpsCloseCombatCharacter::Attack_Implementation(const FInputActionValue& Value, UWeaponData* Weapon){
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack triggered"));

	if (LastUsedEquipType != CurrentEquipType && LastUsedWeapon != Weapon){
		ComboStep = 0;
	}

	PlayAttackAnimation(Weapon, ComboStep);

	const int TotalMontages = Weapon->GetTotalAttackMontagesCount();
	ComboStep = (ComboStep + 1) % TotalMontages;
	LastUsedWeapon = Weapon;
	LastUsedEquipType = CurrentEquipType;
}

void ATpsCloseCombatCharacter::PlayAttackAnimation_Implementation(UWeaponData* WeaponData, int ComboNum){
	if (auto* AttackMontage = WeaponData->GetAttackAnimation(ComboNum)){
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage)){
			AnimInstance->Montage_Play(AttackMontage);
			ResetComboStep();
			bInputConsumed = false;
		}
	}
}

void ATpsCloseCombatCharacter::PlayRecallAnimation_Implementation(){
	if (RecallMontage){
		ChangeAnimMontage(RecallMontage, [](UAnimInstance* AnimInstance){
			AnimInstance->Montage_JumpToSection("Recall");
		});
	}
}

void ATpsCloseCombatCharacter::Recall_Implementation(){
	IsRecalling = true;
	PlayRecallAnimation();
}

void ATpsCloseCombatCharacter::Guard_Implementation(const FInputActionValue& Value, UWeaponData* Weapon){
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Guard triggered"));

	PlayGuardAnimation(Weapon);

	LastUsedWeapon = Weapon;
	LastUsedEquipType = CurrentEquipType;
}

void ATpsCloseCombatCharacter::PlayGuardAnimation_Implementation(UWeaponData* WeaponData){
	//UAnimMontage* StartGuardingAnim = WeaponData->GetStartGuardingAnimation();
	UAnimMontage* IdleGuardAnimation = WeaponData->GetIdleGuardingAnimation();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(IdleGuardAnimation, 1.0f);
	bInputConsumed = false;
}

bool ATpsCloseCombatCharacter::FindLockOnTarget(){
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerCharacter || !PlayerController){
		return false;
	}

	FVector StartLocation = PlayerCharacter->GetActorLocation();
	FRotator CameraRotation = PlayerController->GetControlRotation();
	FVector EndLocation = StartLocation + CameraRotation.Vector() * LockOnRange;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // Look for Pawns (enemies)

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this); // Ignore yourself

	FHitResult OutHit;
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(), StartLocation, EndLocation, LockOnRadius, ObjectTypes, true, IgnoredActors,
		EDrawDebugTrace::ForDuration, OutHit, true);

	if (bHit && OutHit.GetActor()){
		AActor* HitActor = OutHit.GetActor();
		// Check if the hit actor is a valid target (e.g., has a specific tag)
		if (HitActor->ActorHasTag(FName("Enemy"))) // Replace with your target tag
		{
			CurrentTarget = HitActor;
			return true;
		}
	}
	return false;
}

void ATpsCloseCombatCharacter::ToggleLockOn(){
	if (!bIsLockedOn){
		const bool isTargetFound = FindLockOnTarget();
		bIsLockedOn = isTargetFound;
	}
	else{
		bIsLockedOn = false;
		CurrentTarget = nullptr;
	}
}

void ATpsCloseCombatCharacter::ResetComboStep(){
	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this,
	                                       &ATpsCloseCombatCharacter::OnCombatResetTimerExpired, ComboResetTime);
}

void ATpsCloseCombatCharacter::OnCombatResetTimerExpired(){
	ComboStep = 0;
}

void ATpsCloseCombatCharacter::ChangeAnimMontage(UAnimMontage* AnimMontageToPlay,
                                                 std::function<void(UAnimInstance*)> callback) const{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(AnimMontageToPlay)){
		AnimInstance->Montage_Play(AnimMontageToPlay);
		if (callback){
			callback(AnimInstance);
		}
	}
}

void ATpsCloseCombatCharacter::SetupStimulusSource(){
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource){
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void ATpsCloseCombatCharacter::SetupWeapons(){
	if (WeaponManager){
		SetupRightWeapon();
		SetupLeftWeapon();
	}
}

void ATpsCloseCombatCharacter::SetupRightWeapon(){
	if (const TSubclassOf<ABaseWeapon> WeaponToSpawn = WeaponManager.GetDefaultObject()->GetBaseWeaponClass(
		RightWeaponInventory[RightWeaponSelectedIndex])){
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this; // Set character as owner
		SpawnParams.Instigator = GetInstigator();
		if (auto* SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponToSpawn, FVector::ZeroVector,
		                                                              FRotator::ZeroRotator, SpawnParams)){
			SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
			                                 FName("weapon_r_socket"));
			RightWeapon = SpawnedWeapon;
			RightWeaponData = WeaponManager.GetDefaultObject()->GetWeaponData(RightWeapon->GetWeaponType());
			RightWeaponCollision = SpawnedWeapon->GetWeaponCollision();
		}
	}
}

void ATpsCloseCombatCharacter::SetupLeftWeapon(){
	if (const TSubclassOf<ABaseWeapon> WeaponToSpawn = WeaponManager.GetDefaultObject()->GetBaseWeaponClass(
		LeftWeaponInventory[LeftWeaponSelectedIndex])){
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this; // Set character as owner
		SpawnParams.Instigator = GetInstigator();
		if (auto* SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponToSpawn, FVector::ZeroVector,
		                                                              FRotator::ZeroRotator, SpawnParams)){
			SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
			                                 FName("Weapon_L"));
			LeftWeapon = SpawnedWeapon;
			LeftWeaponData = WeaponManager.GetDefaultObject()->GetWeaponData(LeftWeapon->GetWeaponType());
			LeftWeaponCollision = SpawnedWeapon->GetWeaponCollision();
		}
	}
}

void ATpsCloseCombatCharacter::CreatePlayerHUD(){
	if (IsLocallyControlled() && PlayerHUDClass){
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller)){
			PlayerHUD = CreateWidget<UPlayerHUD>(PlayerController, PlayerHUDClass);
			check(PlayerHUD);

			//TODO(eren.dgrmnc): attach these functions to event that changes health and mana values/
			PlayerHUD->SetHealth(CurrentHealth, Health);
			PlayerHUD->SetMana(CurrentMana, Mana);
			PlayerHUD->AddToPlayerScreen();
		}
	}
}

void ATpsCloseCombatCharacter::SetupStats(){
	CurrentHealth = Health;
	CurrentMana = Mana;
}

void ATpsCloseCombatCharacter::OnStartGuardingFinished(UAnimMontage* Montage, bool bInterrupted){
	/*UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimMontage* IdleGuardAnimation = WeaponData->GetIdleGuardingAnimation();
	AnimInstance->Montage_Play(IdleGuardAnimation, 1.0f);*/
}

void ATpsCloseCombatCharacter::DodgeRoll(const FInputActionValue& Value){
	if (bPressedJump || IsDodgeRolling || IsAttacking){
		return;
	}

	if (auto* AnimInstance = GetMesh()->GetAnimInstance()){
		IsDodgeRolling = true;
		AnimInstance->Montage_Play(DodgeRollMontage);

		// Get the current movement input direction
		FVector DodgeDirection = FVector::ZeroVector;

		if (Controller != nullptr){
			// Assuming your MoveAction is set up with a 2D vector axis
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			FVector2D MovementVector = Value.Get<FVector2D>();

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// Calculate dodge direction
			DodgeDirection = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X).GetSafeNormal();
		}

		// If there is no input, default to forward dodge
		if (DodgeDirection.IsNearlyZero()){
			DodgeDirection = -GetActorForwardVector();
		}

		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Dodge Vec: " + DodgeDirection.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow,
		                                 TEXT("Forward Vec: " + GetActorForwardVector().ToString()));

		// Launch character in the dodge direction
		LaunchCharacter(DodgeDirection * 250, true, true);
	}
}

void ATpsCloseCombatCharacter::Jump(){
	if (IsDodgeRolling){
		return;
	}
	Super::Jump();
}

void ATpsCloseCombatCharacter::StopDodgeRoll(){
	IsDodgeRolling = false;
}

void ATpsCloseCombatCharacter::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	if (bIsLockedOn && CurrentTarget){
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (!PlayerCharacter || !PlayerController){
			return;
		}


		// Get player and target locations
		const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		const FVector TargetLocation = CurrentTarget->GetActorLocation();

		// Create a desired rotation based on target location
		const FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);

		// Smoothly interpolate camera rotation towards desired rotation
		const FRotator InterpolatedRotation = FMath::RInterpTo(PlayerController->GetControlRotation(), DesiredRotation,
		                                                       DeltaSeconds, LockOnRotationSpeed);

		// Set player controller rotation (adjust based on your camera setup)
		if (!IsDodgeRolling){
			PlayerController->SetControlRotation(InterpolatedRotation);
		}
		// Rotate player character towards the target
		const FRotator PlayerDesiredRotation = FRotator(0.0f, DesiredRotation.Yaw, 0.0f);
		// Only rotate around the Yaw axis
		const FRotator PlayerInterpolatedRotation = FMath::RInterpTo(PlayerCharacter->GetActorRotation(),
		                                                             PlayerDesiredRotation, DeltaSeconds,
		                                                             LockOnRotationSpeed);

		PlayerCharacter->SetActorRotation(PlayerInterpolatedRotation);
	}
}

void ATpsCloseCombatCharacter::OnRepHealth(){
	if (PlayerHUD){
		PlayerHUD->SetHealth(CurrentHealth, Health);
	}
}
