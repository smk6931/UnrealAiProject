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
	UPROPERTY(meta = (BindWidget))
	class UItemInfoUi* BpItemInfoUi;

	UPROPERTY(meta = (BindWidget))
	class UMonsterInfoUi* BpMonsterInfoUi;

	UPROPERTY(meta = (BindWidget))
	class UGenerateUi* BpBpGenerateUi;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Close;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Create;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Monster;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Item;

	UPROPERTY(meta = (BindWidget))
	class UBorder* Board;

	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnMonsterClick();

	UFUNCTION()
	void OnCreateClick();

	UFUNCTION()
	void OnItemClick();

	virtual void NativeConstruct() override;
};
