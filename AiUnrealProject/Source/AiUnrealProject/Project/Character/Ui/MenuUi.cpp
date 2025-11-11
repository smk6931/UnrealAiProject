// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuUi.h"
#include "Info/ItemInfoUi.h"
#include "Info/MonsterInfoUi.h"
#include "Generate/GenerateItemUi.h"
#include "Generate/GenerateMonsterUi.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UMenuUi::NativeConstruct()
{
	Super::NativeConstruct();
	BpGenerateMonsterUi->MenuUi = this;
	BpGenerateItemUi->MenuUi = this;
	
	Button_Close->OnClicked.AddDynamic(this,&UMenuUi::OnCloseClick);
	Button_Item->OnClicked.AddDynamic(this,&UMenuUi::OnItemClick);
	Button_Monster->OnClicked.AddDynamic(this,&UMenuUi::OnMonsterClick);
	Button_Create->OnClicked.AddDynamic(this,&UMenuUi::OnCreateClick);
	Button_World->OnClicked.AddDynamic(this,&UMenuUi::OnWorldClick);
	
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

void UMenuUi::OnWorldClick()
{
	UE_LOG(LogTemp, Display, TEXT("ButtonMonsterClick"));
	// BpMonsterInfoUi->SetVisibility(ESlateVisibility::Visible);
	// BpMonsterInfoUi->LoadMonsters();
	Board->SetVisibility(ESlateVisibility::Hidden);
}

void UMenuUi::OnItemClick()
{
	UE_LOG(LogTemp, Display, TEXT("ButtonItemClick"));
	BpItemInfoUi->SetVisibility(ESlateVisibility::Visible);
	BpItemInfoUi->LoadItems();
	Board->SetVisibility(ESlateVisibility::Hidden);
}


void UMenuUi::ButtonCLose()
{
	Button_Close->SetVisibility(ESlateVisibility::Collapsed);
	Button_Create->SetVisibility(ESlateVisibility::Collapsed);
}


