// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Project/ApiObject/Npc/ApiItemObject.h"
#include "Project/ApiObject/Npc/ApiMonsterObject.h"
#include "GenerateItemUi.generated.h"

/**
 * 
 */
UCLASS()
class AIUNREALPROJECT_API UGenerateItemUi : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

	FMonsterRows MonsterRows;
	FItemRows ItemRows;

	UPROPERTY(EditAnywhere)
	UApiItemObject* Api;

	UPROPERTY()
	class UMenuUi* MenuUi;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Close;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_CreateItem;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Image;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MonsterName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MonsterId;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* RightVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* LeftVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;
	
	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnCreateItemClick();

	UFUNCTION()
	void OnImageClick();
};
