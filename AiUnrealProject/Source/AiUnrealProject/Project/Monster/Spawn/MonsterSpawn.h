// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project/ApiObject/ApiMonsterObject.h"
#include "MonsterSpawn.generated.h"


class AMonster;

UCLASS()
class AIUNREALPROJECT_API AMonsterSpawn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonsterSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FMonsterRows MonsterRows;

	UPROPERTY(EditAnywhere)
	UApiMonsterObject* ApiMonster;

    UPROPERTY(EditAnywhere)
	TSubclassOf<AMonster> MonsterFactory;
	
	UPROPERTY(EditAnywhere)
	TArray<AMonster*> Monsters;

	UPROPERTY(EditAnywhere)
	float CurrentTime;

	
};
