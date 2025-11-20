// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterUi.generated.h"

/**
 * 
 */
UCLASS()
class AIUNREALPROJECT_API UMonsterUi : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HpBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHp;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHp;
};
