// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterInfoUi.h"

#include "JsonObjectConverter.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Project/Util/UiUtil.h"

class UTextBlock;
class UImage;

void UMonsterInfoUi::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMonsterInfoUi::LoadMonsters()
{
	UApiMonsterObject* Api = NewObject<UApiMonsterObject>();
	Api->OnMonsterInfoResponse.BindLambda([this](FString String)
	{
		UE_LOG(LogTemp, Display, TEXT("Loading monsters %s"),*String);
		FMonsterRows Rows;
        FJsonObjectConverter::JsonObjectStringToUStruct(String, &Rows);
		UE_LOG(LogTemp, Display, TEXT("몬스터 배열 갯수는? [%d]"),Rows.response.Num());
		
		VScroll->ClearChildren();
		ButtonMonsterMap.Empty();

		auto MakeTextCell = [&](const FString& Value, float Width, FLinearColor Color = FLinearColor::White)
		{
			USizeBox* CellBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			CellBox->SetWidthOverride(Width);

			UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Text->SetText(FText::FromString(Value));
			Text->SetColorAndOpacity(FSlateColor(Color));
			Text->SetJustification(ETextJustify::Left);
			Text->SetAutoWrapText(false);
			Text->SetMargin(FMargin(5, 0, 5, 0));
			CellBox->AddChild(Text);
			return CellBox;
		};

		UHorizontalBox* RowFirst = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		RowFirst->AddChildToHorizontalBox(MakeTextCell("Image", 80.f,FLinearColor::Yellow));
		RowFirst->AddChildToHorizontalBox(MakeTextCell("Name", 300.f,FLinearColor::Yellow));
		RowFirst->AddChildToHorizontalBox(MakeTextCell("Level", 100.f,FLinearColor::Yellow));
		RowFirst->AddChildToHorizontalBox(MakeTextCell("Attack", 100.f,FLinearColor::Yellow));
		RowFirst->AddChildToHorizontalBox(MakeTextCell("Hp", 100.f,FLinearColor::Yellow));
		RowFirst->AddChildToHorizontalBox(MakeTextCell("Habitat", 300.f,FLinearColor::Yellow));
		VScroll->AddChild(RowFirst);

		for (const FMonsterRow& Monster : Rows.response)
		{
			UApiMonsterObject* Api = NewObject<UApiMonsterObject>();
			
			UButton* ClickButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
			UHorizontalBox* RowBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

			UUiUtil* Util = NewObject<UUiUtil>();
			Util->ButtonTransparentStyle(ClickButton);

			UImage* Icon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			UTexture2D* DummyTex = UTexture2D::CreateTransient(64, 64);
			Icon->SetBrushFromTexture(DummyTex);
			Icon->SetColorAndOpacity(FLinearColor::Gray);

			USizeBox* IconBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
			IconBox->SetWidthOverride(80.f);
			IconBox->SetHeightOverride(80.f);
			IconBox->AddChild(Icon);
			FString Str = Monster.image_url.Replace(TEXT("\\"), TEXT("/"));
            Api->OnMonsterTextureResponse.BindLambda([this, Icon](UTexture2D* Texture)
            {
	           Icon->SetBrushFromTexture(Texture); 
            });
			RowBox->AddChildToHorizontalBox(IconBox);

			RowBox->AddChildToHorizontalBox(MakeTextCell(Monster.Name, 300.f));
			RowBox->AddChildToHorizontalBox(MakeTextCell(FString::FromInt(Monster.level), 100.f));
			RowBox->AddChildToHorizontalBox(MakeTextCell(FString::FromInt(Monster.attack), 100.f));
			RowBox->AddChildToHorizontalBox(MakeTextCell(FString::FromInt(Monster.hp), 100.f));
			RowBox->AddChildToHorizontalBox(MakeTextCell(Monster.habitat, 300.f));

			ClickButton->AddChild(RowBox);
			VScroll->AddChild(ClickButton);

			ButtonMonsterMap.Add(ClickButton, Monster);

			ClickButton->OnClicked.AddDynamic(this, &UMonsterInfoUi::OnMonsterClicked);
			Api->LoadImageFromUrl(FString::Printf(TEXT("http://127.0.0.1:8000/%s"), *Str));
		}
	});
	Api->MonsterInfoResponse();
}

void UMonsterInfoUi::OnMonsterClicked()
{
	for (TPair<UButton*, FMonsterRow>& Map : ButtonMonsterMap)
	{
		UButton* Button = Map.Key;
		if (Button->HasKeyboardFocus())
		{
			const FMonsterRow& Row = Map.Value;
			DetailPanel->ClearChildren();

			UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Title->SetText(FText::FromString(Row.Name));
			Title->SetColorAndOpacity(FSlateColor(FLinearColor::White));

			UTextBlock* Desc = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Desc->SetText(FText::FromString(Row.description));
			Desc->SetAutoWrapText(true);
			Desc->SetMargin(FMargin(10.f));

			DetailPanel->AddChild(Title);
			DetailPanel->AddChild(Desc);
		}
	}
}
