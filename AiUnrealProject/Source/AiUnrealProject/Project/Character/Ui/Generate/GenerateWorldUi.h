// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Project/ApiObject/Npc/ApiWorldObject.h"
#include "GenerateWorldUi.generated.h"

/**
 * 
 */
UCLASS()
class AIUNREALPROJECT_API UGenerateWorldUi : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FWorldRows WorldRows;

	UPROPERTY(EditAnywhere)
	FString JsonString;
	
	UPROPERTY()
	class UApiWorldObject* Api;

	UPROPERTY()
	class UMenuUi* MenuUi;
	
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WorldInfo;

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

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* InputQuestion;
	
	virtual  void NativeConstruct() override;

	UFUNCTION()
	void OnCreateClick();

	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnItemClick();

	UFUNCTION()
	void OnImageClick();

	UFUNCTION()
	void OnWorldClick();

	UFUNCTION()
	void OnInputQuestion(const FText& Text, ETextCommit::Type CommitMethod);
};
