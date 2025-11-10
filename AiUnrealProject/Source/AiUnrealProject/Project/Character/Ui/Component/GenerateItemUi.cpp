// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateItemUi.h"

#include "Components/Button.h"

void UGenerateItemUi::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Close->OnClicked.AddDynamic(this,UGenerateItemUi::OnCloseClick);
	Button_CreateItem->OnClicked.AddDynamic(this, UGenerateItemUi::OnCreateItemClick);

	
}

void UGenerateItemUi::OnCloseClick()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UGenerateItemUi::OnCreateItemClick()
{
	UApiItem
}
