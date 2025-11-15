// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Project/ApiObject/ApiItemObject.h"
#include "ItemInfoUi.generated.h"

/**
 * 
 */

class UButton;

UCLASS()
class AIUNREALPROJECT_API UItemInfoUi : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* DetailPanel;
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* VScroll;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void LoadItems();

	UFUNCTION()
	void OnItemClick();

	UPROPERTY(EditAnywhere)
	TMap<UButton*, FItemRow> ButtonItemMap;
};
