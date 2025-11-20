// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterUi.h"
#include "GameFramework/Character.h"
#include "Project/ApiObject/ApiMonsterObject.h"
#include "Monster.generated.h"

class UMonsterImage;

UCLASS()
class AIUNREALPROJECT_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FMonsterRow MonsterRow;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* MonsterImageComp;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMonsterImage> MonsterImageFactory;
	UPROPERTY(EditAnywhere)
	class UMonsterImage* MonsterImage;


	UPROPERTY(EditAnywhere)
	class UWidgetComponent* MonsterUiComp;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMonsterUi> MonsterUiFactory;
	UPROPERTY(EditAnywhere)
	class UMonsterUi* MonsterUi;

	UPROPERTY(EditAnywhere)
	class UMonsterFsm* MonsterFsm;

	void MonsterRawInit(FMonsterRow Row);
};
