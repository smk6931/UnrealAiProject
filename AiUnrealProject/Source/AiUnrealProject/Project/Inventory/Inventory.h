// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Slot/Slot.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class AIUNREALPROJECT_API UInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* WrapBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USlot>SlotFactory;
	UPROPERTY(EditAnywhere)
	class USlot* SlotItem;

public:
	virtual void NativeConstruct() override;
};
