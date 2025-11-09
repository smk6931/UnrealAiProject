// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenerateUi.generated.h"

class UApiMonsterObject;
/**
 * 
 */
UCLASS()
class AIUNREALPROJECT_API UGenerateUi : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UApiMonsterObject* Api;
	
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* LeftVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* RightVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Create;

	virtual  void NativeConstruct() override;

	UFUNCTION()
	void OnCreateClick();
	
};
