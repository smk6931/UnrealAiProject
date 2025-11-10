// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuUi.h"

#include "ItemInfoUi.h"
#include "MonsterInfoUi.h"
#include "Component/GenerateItemUi.h"
#include "Component/GenerateMonsterUi.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UMenuUi::NativeConstruct()
{
	Super::NativeConstruct();
	BpGenerateMonsterUi->MenuUi = this;
	
	Button_Close->OnClicked.AddDynamic(this,&UMenuUi::OnCloseClick);
	Button_Item->OnClicked.AddDynamic(this,&UMenuUi::OnItemClick);
	Button_Monster->OnClicked.AddDynamic(this,&UMenuUi::OnMonsterClick);
	Button_Create->OnClicked.AddDynamic(this,&UMenuUi::OnCreateClick);
	
	BpItemInfoUi->SetVisibility(ESlateVisibility::Hidden);
	BpMonsterInfoUi->SetVisibility(ESlateVisibility::Hidden);
	BpGenerateMonsterUi->SetVisibility(ESlateVisibility::Hidden);
	BpGenerateItemUi->SetVisibility(ESlateVisibility::Hidden);
}

void UMenuUi::OnCloseClick()
{
	UE_LOG(LogTemp, Display, TEXT("ButtonCloseClick"));
	BpItemInfoUi->SetVisibility(ESlateVisibility::Hidden);
	BpMonsterInfoUi->SetVisibility(ESlateVisibility::Hidden);
	BpGenerateMonsterUi->SetVisibility(ESlateVisibility::Hidden);
	BpGenerateItemUi->SetVisibility(ESlateVisibility::Hidden);
	Board->SetVisibility(ESlateVisibility::Visible);
}

void UMenuUi::OnCreateClick()
{
	UE_LOG(LogTemp, Display, TEXT("ButtonCreateClick"));
	BpGenerateMonsterUi->SetVisibility(ESlateVisibility::Visible);
	BpGenerateItemUi->SetVisibility(ESlateVisibility::Collapsed);
	Board->SetVisibility(ESlateVisibility::Hidden);
}

void UMenuUi::OnMonsterClick()
{
	UE_LOG(LogTemp, Display, TEXT("ButtonMonsterClick"));
	BpMonsterInfoUi->SetVisibility(ESlateVisibility::Visible);
	BpMonsterInfoUi->LoadMonsters();
	Board->SetVisibility(ESlateVisibility::Hidden);
}

void UMenuUi::OnItemClick()
{
	UE_LOG(LogTemp, Display, TEXT("ButtonItemClick"));
	BpItemInfoUi->SetVisibility(ESlateVisibility::Visible);
	BpItemInfoUi->LoadItems();
	Board->SetVisibility(ESlateVisibility::Hidden);
}


