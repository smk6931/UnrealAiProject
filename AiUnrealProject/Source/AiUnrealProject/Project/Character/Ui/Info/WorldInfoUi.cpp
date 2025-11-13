// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldInfoUi.h"

#include "GameDelegates.h"
#include "JsonObjectConverter.h"
#include "MonsterInfoUi.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UWorldInfoUi::NativeConstruct()
{
	Super::NativeConstruct();
	Api = NewObject<UApiWorldObject>(this);

	Button_GenerateMonster->OnClicked.AddDynamic(this,&UWorldInfoUi::OnGenerateMonsterClick);
}

void UWorldInfoUi::LoadWorlds()
{
	// VScroll->ClearChildren();
	// ButtonWorldMap.Empty();
	
	Api->OnWorldInfoResponse.BindWeakLambda(this,[this](FString String)
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(String, &WorldRows);
		for (FWorldRow Row : WorldRows.Response)
		{
			UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Title->SetText(FText::FromString(Row.title));
			Title->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFontStyle("Regular", 20)));

			UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
			Button->AddChild(Title);
			
			FButtonStyle Style;
            Style.Normal.TintColor = FSlateColor(FLinearColor::Black);
            Style.Hovered.TintColor = FSlateColor(FLinearColor::Green);
            Style.Pressed.TintColor = FSlateColor(FLinearColor::Blue);
            Button->SetStyle(Style);

			Button->OnClicked.AddDynamic(this, &UWorldInfoUi::OnDetailClick);
			ButtonWorldMap.Add(Button,Row);
			VScroll->AddChild(Button);
		}
		WorldTitle->SetVisibility(ESlateVisibility::Collapsed);
	    Button_Detail->SetVisibility(ESlateVisibility::Collapsed);
	});
	Api->GetWorldsAll();
}

void UWorldInfoUi::OnDetailClick()
{
	DetailPanel->ClearChildren();
	UTextBlock* Content = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	Content->SetAutoWrapText(true);
	DetailPanel->AddChild(Content);

	for (TPair<UButton*, FWorldRow>& Map : ButtonWorldMap)
	{
		UButton* Button = Map.Key;
		if (Button->HasKeyboardFocus())
		{
			UButton* MonsterButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
			UTextBlock* MonsterButtonTitle = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			MonsterButtonTitle->SetText(FText::FromString(FString::Printf(TEXT("[%s] 몬스터 생성"),*Map.Value.title)));
			MonsterButton->AddChild(MonsterButtonTitle);
			DetailPanel->AddChild(MonsterButton);
			
			FWorldRow& Row = Map.Value;
			UTextBlock* Desc = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Desc->SetText(FText::FromString(Row.content));
			Desc->SetAutoWrapText(true);
			Desc->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFontStyle("Regular", 20)));

			DetailPanel->AddChild(Desc);
		}
	}
}

void UWorldInfoUi::OnGenerateMonsterClick()
{
	UE_LOG(LogTemp, Display, TEXT("OnGenerateMonsterClick"));

	
}


