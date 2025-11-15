// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldInfoUi.h"

#include "GameDelegates.h"
#include "IPropertyTable.h"
#include "JsonObjectConverter.h"
#include "MonsterInfoUi.h"
#include "ToolMenusEditor.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Project/Character/Ui/MenuUi.h"
#include "Project/Character/Ui/Generate/GenerateMonsterUi.h"

void UWorldInfoUi::NativeConstruct()
{
	Super::NativeConstruct();
	Api = NewObject<UApiWorldObject>(this);

	Button_GenerateMonster->OnClicked.AddDynamic(this,&UWorldInfoUi::OnGenerateMonsterClick);
	RightBoard->SetVisibility(ESlateVisibility::Collapsed);
}

void UWorldInfoUi::LoadWorlds()
{
	VScroll->ClearChildren();
	Api->OnWorldInfoResponse.BindWeakLambda(this,[this](FString String)
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(String, &WorldRows);
		for (FWorldRow Row : WorldRows.Response)
		{
			UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Title->SetText(FText::FromString(Row.title));
			Title->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFontStyle("Regular", 16)));

			UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
			Button->AddChild(Title);
			
			FButtonStyle Style;
            Style.Normal.TintColor = FSlateColor(FLinearColor::Black);
            Button->SetStyle(Style);

			Button->OnClicked.AddDynamic(this, &UWorldInfoUi::OnDetailClick);
			ButtonWorldMap.Add(Button,Row);
			UScrollBoxSlot* Slot = Cast<UScrollBoxSlot>(VScroll->AddChild(Button));
            Slot->SetPadding(FMargin(5.f));
		}
	});
	Api->GetWorldsAll();
}

void UWorldInfoUi::OnDetailClick()
{
	RightBoard->SetVisibility(RightBoard->GetVisibility()==ESlateVisibility::Visible? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	
	DetailPanel->ClearChildren();
	UTextBlock* Content = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	Content->SetAutoWrapText(true);
	DetailPanel->AddChild(Content);

	for (TPair<UButton*, FWorldRow>& Map : ButtonWorldMap)
	{
		UButton* Button = Map.Key;
		if (Button->HasKeyboardFocus())
		{
			WorldRow = Map.Value;
			UTextBlock* GenerateTitle = Cast<UTextBlock>(Button_GenerateMonster->GetChildAt(0));
			GenerateTitle->SetText(FText::FromString(FString::Printf(TEXT("%s 몬스터 생성"),*Map.Value.title)));
			
			UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Title->SetColorAndOpacity(FColor::Yellow);
			Title->SetText(FText::FromString(FString::Printf(TEXT("%s"),*Map.Value.title)));
			Title->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFontStyle("Regular", 16)));
			
			FWorldRow& Row = Map.Value;
			UTextBlock* Desc = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Desc->SetText(FText::FromString(Row.content));
			Desc->SetAutoWrapText(true);
			Desc->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFontStyle("Regular", 16)));

			UVerticalBoxSlot* Vslot = Cast<UVerticalBoxSlot>( DetailPanel->AddChild(Title));
			Vslot->SetPadding(5.0f);
			DetailPanel->AddChild(Desc);
		}
	}
}

void UWorldInfoUi::OnGenerateMonsterClick()
{
	// RightBoard->SetVisibility(ESlateVisibility::Collapsed);
	UE_LOG(LogTemp, Display, TEXT("OnGenerateMonsterClick"));
	SetVisibility(ESlateVisibility::Collapsed);
	MenuUi->BpGenerateMonsterUi->SetVisibility(ESlateVisibility::Visible);
	MenuUi->BpGenerateMonsterUi->WorldInfo->SetText(FText::FromString(FString::Printf(TEXT("[%s] 몬스터 생성중"),*WorldRow.title)));
	
	FWorldRows Rows;
	Rows.Response.Add(WorldRow);
	
	FString String;
	FJsonObjectConverter::UStructToJsonObjectString(Rows, String);
	UE_LOG(LogTemp, Display, TEXT("OnGenerateMonsterClick 보내는 WorldJson %s"), *String);

	MenuUi->BpGenerateMonsterUi->JsonString = String;

	// UApiMonsterObject* ApiMonster = NewObject<UApiMonsterObject>(this);
	// MenuUi->BpGenerateMonsterUi->OnCreateClick();
	// ApiMonster->GenerateMonster(String);
}


// UButton* MonsterButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
// UTextBlock* MonsterButtonTitle = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
// MonsterButtonTitle->SetText(FText::FromString(FString::Printf(TEXT("[%s] 몬스터 생성"),*Map.Value.title)));
// MonsterButton->AddChild(MonsterButtonTitle);
// DetailPanel->AddChild(MonsterButton);