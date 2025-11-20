// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterFsm.generated.h"

class AMonster;

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Around UMETA(DisplayName = "Around"),
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AIUNREALPROJECT_API UMonsterFsm : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMonsterFsm();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	void MonsterIdle();
	void MonsterAround();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	EMonsterState MonsterState = EMonsterState::Idle;

	UPROPERTY(EditAnywhere)
	FVector MonsInitLoc;
	
	UPROPERTY(EditAnywhere)
	FRotator MonsterMoveRotator;
	
	UPROPERTY(EditAnywhere)
	float CurrentTime = 0.0f;
	
	UPROPERTY(EditAnywhere)
	AMonster* Monster;
};
