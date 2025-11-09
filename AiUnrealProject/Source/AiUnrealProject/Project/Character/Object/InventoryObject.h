// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryObject.generated.h"


class UMenuUi;
/**
 * 
 */
UCLASS(Blueprintable,EditInlineNew)
class AIUNREALPROJECT_API UInventoryObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuUi> MenuUiFactory;

	UPROPERTY(EditAnywhere)
	class UMenuUi* MenuUi;
	
	void Init();
};
