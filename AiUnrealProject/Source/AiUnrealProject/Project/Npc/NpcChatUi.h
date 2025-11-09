// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DECLARE_DELEGATE_OneParam(FOnNpcPost, FString string)

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NpcChatUi.generated.h"

/**
 * 
 */

UCLASS()
class AIUNREALPROJECT_API UNpcChatUi : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextGet;

	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* TextPost;

	FOnNpcPost OnNpcPost;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void UiPostEnter(const FText& Text, ETextCommit::Type CommitMethod);
};
