// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuUi.h"

#include "ItemInfoUi.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UMenuUi::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Close->OnClicked.AddDynamic(this,&UMenuUi::ButtonCloseClick);
	Button_Item->OnClicked.AddDynamic(this,&UMenuUi::ButtonItemClick);
	Button_Monster->OnClicked.AddDynamic(this,&UMenuUi::ButtonMonsterClick);
	BpItemInfoUi->SetVisibility(ESlateVisibility::Hidden);
}

void UMenuUi::ButtonCloseClick()
{
	UE_LOG(LogTemp, Display, TEXT("ButtonCloseClick"));
	BpItemInfoUi->SetVisibility(ESlateVisibility::Hidden);
	Board->SetVisibility(ESlateVisibility::Visible);
}

void UMenuUi::ButtonMonsterClick()
{
	UE_LOG(LogTemp, Display, TEXT("ButtonMonsterClick"));
}

void UMenuUi::ButtonItemClick()
{
	UE_LOG(LogTemp, Display, TEXT("ButtonItemClick"));
	BpItemInfoUi->SetVisibility(ESlateVisibility::Visible);
	Board->SetVisibility(ESlateVisibility::Hidden);
}


