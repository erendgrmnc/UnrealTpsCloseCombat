// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Unarmed.h"

void AUnarmed::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AUnarmed::PrimaryAction(){
	IWeaponInterface::PrimaryAction();
}

void AUnarmed::SecondaryAction(){
	IWeaponInterface::SecondaryAction();
}

void AUnarmed::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	Super::OnWeaponOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AUnarmed::BeginPlay(){
	Super::BeginPlay();
}
