// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Shield.h"

void AShield::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AShield::PrimaryAction(){
	IWeaponInterface::PrimaryAction();
}

void AShield::SecondaryAction(){
	IWeaponInterface::SecondaryAction();
}

void AShield::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	Super::OnWeaponOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AShield::BeginPlay(){
	Super::BeginPlay();
}
