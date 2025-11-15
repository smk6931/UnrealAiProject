// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Project/ApiObject/ApiWorldObject.h"
#include "WorldInfoUi.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class AIUNREALPROJECT_API UWorldInfoUi : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	class UMenuUi* MenuUi;

	UPROPERTY(EditAnywhere)
	FWorldRow WorldRow;
	
	UPROPERTY(EditAnywhere)
	FWorldRows WorldRows;
	
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* DetailPanel;

	UPROPERTY(EditAnywhere)
	class UApiWorldObject* Api;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* VScroll;
	
	// UPROPERTY(meta = (BindWidget))
	// class UTextBlock* WorldTitle;
	//
	// UPROPERTY(meta = (BindWidget))
	// class UButton* Button_Detail;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_GenerateMonster;

	UPROPERTY(meta = (BindWidget))
	class UBorder* RightBoard;
	
	UPROPERTY(meta = (BindWidget))
	class UBorder* LeftBoard;

	virtual void NativeConstruct() override;

	void LoadWorlds();

	UFUNCTION()
	void OnDetailClick();

	UFUNCTION()
	void OnGenerateMonsterClick();

	FWorldRow WorldPrev;
	FWorldRow WorldNext;
	
	UPROPERTY(EditAnywhere)
	TMap<UButton*, FWorldRow> ButtonWorldMap;
};
