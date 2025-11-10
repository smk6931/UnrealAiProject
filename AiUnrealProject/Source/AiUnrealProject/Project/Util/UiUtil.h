// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UiUtil.generated.h"

class USizeBox;
class UButton;
/**
 * 
 */
UCLASS()
class AIUNREALPROJECT_API UUiUtil : public UObject
{
	GENERATED_BODY()

public:
	void ButtonTransparentStyle(UButton* Button);

	USizeBox* MakeTextCell(UUserWidget* Widget, const FString& Value, float Width, int FontSize, FLinearColor Color = FLinearColor::White);
};
