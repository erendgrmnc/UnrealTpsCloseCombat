// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WukongPolearm.h"

// Sets default values
AWukongPolearm::AWukongPolearm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWukongPolearm::BeginPlay()
{
	ABaseWeapon::BeginPlay();
}

// Called every frame
void AWukongPolearm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWukongPolearm::PrimaryAction(){
	IWeaponInterface::PrimaryAction();
}

void AWukongPolearm::SecondaryAction(){
	IWeaponInterface::SecondaryAction();
}

void AWukongPolearm::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	Super::OnWeaponOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

