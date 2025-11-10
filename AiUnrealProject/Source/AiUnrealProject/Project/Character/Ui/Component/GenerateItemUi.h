// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Close;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_CreateItem;

	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnCreateItemClick();
};
