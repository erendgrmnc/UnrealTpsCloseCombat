// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UWeaponInterface.generated.h"

/**
 * 
 */
UINTERFACE()
class UWeaponInterface : public UInterface{
	GENERATED_BODY()
};

class IWeaponInterface{
	GENERATED_BODY()

public:
	virtual void PrimaryAction();
	virtual void SecondaryAction();
};