// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Project/ApiObject/Npc/ApiWorldObject.h"
#include "WorldInfoUi.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class AIUNREALPROJECT_API UWorldInfoUi : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* DetailPanel;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* VScroll;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void LoadMonsters();

	UFUNCTION()
	void OnMonsterClicked();

	UPROPERTY(EditAnywhere)
	TMap<UButton*, FWorldRow> ButtonWorldMap;
};
