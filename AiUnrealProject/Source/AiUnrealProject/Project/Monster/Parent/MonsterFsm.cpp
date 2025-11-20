// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterFsm.h"

#include "Monster.h"

// Sets default values for this component's properties
UMonsterFsm::UMonsterFsm()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMonsterFsm::BeginPlay()
{
	Super::BeginPlay();

	Monster = Cast<AMonster>(GetOwner());
}

// Called every frame
void UMonsterFsm::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentTime += GetWorld()->GetDeltaSeconds();
	
	switch (MonsterState)
	{
		case EMonsterState::Idle:
		    MonsterIdle();
		    break;
	    case EMonsterState::Around:
		    MonsterAround();
		    break;
	}
}

void UMonsterFsm::MonsterIdle()
{
	if (CurrentTime > 2)
	{
		CurrentTime = 0;
		MonsterState = EMonsterState::Around;
		UE_LOG(LogTemp, Warning, TEXT("MonsterFsm::Around"));
	}
}

void UMonsterFsm::MonsterAround()
{
	if (CurrentTime > 2)
	{
		CurrentTime = 0;
		MonsterMoveRotator = FRotator(0, FMath::RandRange(0,180), 0);
		if (FVector::Dist(Monster->GetActorLocation(),MonsInitLoc) > 500)
		{
			MonsterMoveRotator = FRotator((MonsInitLoc - Monster->GetActorLocation()).Rotation());
		}
		UE_LOG(LogTemp, Warning, TEXT("MonsterFsm::Around %s"),*MonsterMoveRotator.ToString());
	}
	Monster->AddActorWorldOffset(MonsterMoveRotator.Vector() * 10);
}

