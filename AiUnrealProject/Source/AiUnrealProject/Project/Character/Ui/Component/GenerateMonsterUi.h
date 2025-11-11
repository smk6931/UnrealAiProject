// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenerateMonsterUi.generated.h"

class UApiMonsterObject;
/**
 * 
 */
UCLASS()
class AIUNREALPROJECT_API UGenerateMonsterUi : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UApiMonsterObject* Api;

	UPROPERTY()
	class UMenuUi* MenuUi;
	
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* LeftVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* RightVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Image;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Create;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Close;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Item;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ItemDropListScroll;

	virtual  void NativeConstruct() override;

	UFUNCTION()
	void OnCreateClick();

	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnItemClick();

	UFUNCTION()
	void OnImageClick();
	
};
