// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuUi.generated.h"

/**
 * 
 */
UCLASS()
class AIUNREALPROJECT_API UMenuUi : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UItemInfoUi* BpItemInfoUi;

	UPROPERTY(meta = (BindWidget))
	class UMonsterInfoUi* BpMonsterInfoUi;

	UPROPERTY(meta = (BindWidget))
	class UWorldInfoUi* BpWorldInfoUi;

	UPROPERTY(meta = (BindWidget))
	class UGenerateMonsterUi* BpGenerateMonsterUi;

	UPROPERTY(meta = (BindWidget))
	class UGenerateItemUi* BpGenerateItemUi;

	UPROPERTY(meta = (BindWidget))
	class UGenerateWorldUi* BpGenerateWorldUi;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* BoardBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Close;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Create;

    UPROPERTY(meta = (BindWidget))
	class UButton* Button_CreateWorld;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Monster;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Item;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_World;


	UPROPERTY(meta = (BindWidget))
	class UBorder* Board;

	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnMonsterClick();

	UFUNCTION()
	void OnWorldClick();

	UFUNCTION()
	void OnCreateClick();

	UFUNCTION()
	void OnItemClick();

	void ButtonCLose();

	UFUNCTION()
	void OnCreateWorldClick();
};
