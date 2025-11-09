// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UiUtil.generated.h"

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
};
