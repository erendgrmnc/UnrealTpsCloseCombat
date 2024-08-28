// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PatrolPath.h"

APatrolPath::APatrolPath(){
	PrimaryActorTick.bCanEverTick = false;
}

FVector APatrolPath::GetPatrolPoint(const int index) const{
	return PatrolPoints[index];
}

int APatrolPath::GetPatrolPoinstSize() const{
	return PatrolPoints.Num();
}
