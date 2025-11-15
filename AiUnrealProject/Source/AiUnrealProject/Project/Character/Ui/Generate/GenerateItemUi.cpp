// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateItemUi.h"

#include "JsonObjectConverter.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Project/Character/Ui/MenuUi.h"
#include "Project/Util/UiUtil.h"

class UUiUtil;
class USizeBox;

void UGenerateItemUi::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Close->OnClicked.AddDynamic(this,&UGenerateItemUi::OnCloseClick);
	Button_CreateItem->OnClicked.AddDynamic(this, &UGenerateItemUi::OnCreateItemClick);
	Button_Image->OnClicked.AddDynamic(this, &UGenerateItemUi::OnImageClick);

	Button_Image->SetVisibility(ESlateVisibility::Collapsed);
	Api = NewObject<UApiItemObject>(this);
}

void UGenerateItemUi::OnCloseClick()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UGenerateItemUi::OnCreateItemClick()
{
	MonsterId->SetVisibility(ESlateVisibility::Collapsed);
	MonsterName->SetVisibility(ESlateVisibility::Collapsed);
	
	Api->OnItemInfoResponse.BindLambda([this](FString String)
	{
		Button_Image->SetVisibility(ESlateVisibility::Visible);
		FItemRows Rows;
		FJsonObjectConverter::JsonObjectStringToUStruct(String, &Rows);
		ItemRows = Rows;
		
		UE_LOG(LogTemp,Warning,TEXT("람다안으로 아이템 생성 넘어옴 %s"), *Rows.response[0].Name);

		UUiUtil* Util = NewObject<UUiUtil>(this);
		int FonsSize = 15;
			UE_LOG(LogTemp, Display, TEXT("OnMonsterInfoResponse Rows 파싱 성공 %s"), *Rows.response[0].Name)
			MonsterName->SetText(FText::FromString(Rows.response[0].Name));
			
			UHorizontalBox* RowBoxName = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			USizeBox* NameT = Util->MakeTextCell(this, TEXT("이름"),150,FonsSize,FColor::Orange);
			USizeBox* Name = Util->MakeTextCell(this, Rows.response[0].Name,150,FonsSize);
			RowBoxName->AddChild(NameT);
			RowBoxName->AddChild(Name);
			LeftVerticalBox->AddChild(RowBoxName);

		    UHorizontalBox* RowBoxAttack = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			USizeBox* AttackT = Util->MakeTextCell(this, TEXT("종류"),100,FonsSize,FColor::Orange);
			USizeBox* Attack = Util->MakeTextCell(this, Rows.response[0].Type, 150,FonsSize);
			RowBoxAttack->AddChild(AttackT);
			RowBoxAttack->AddChild(Attack);
			LeftVerticalBox->AddChild(RowBoxAttack);


			UHorizontalBox* RowBoxLevel = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			USizeBox* LevelT = Util->MakeTextCell(this, TEXT("등급"),100,FonsSize,FColor::Orange);
			USizeBox* Level = Util->MakeTextCell(this, (Rows.response[0].Rarity), 150,FonsSize);
			RowBoxLevel->AddChild(LevelT);
			RowBoxLevel->AddChild(Level);
			LeftVerticalBox->AddChild(RowBoxLevel);

			
			USizeBox* DescT = Util->MakeTextCell(this, TEXT("몬스터 설명"),100,FonsSize,FColor::Orange);
			UTextBlock* Desc = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Desc->SetText(FText::FromString(Rows.response[0].Description));
			Desc->SetAutoWrapText(true);
			Desc->SetMargin(FMargin(20.f));
			Desc->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFontStyle("Regular", FonsSize)));
			
			RightVerticalBox->AddChild(DescT);
			RightVerticalBox->AddChild(Desc);
	});
	Api->GenerateItemsForMonsterIds(MonsterRows.response[0].id, 1, false);
}

void UGenerateItemUi::OnImageClick()
{
	// UApiItemObject* Api = NewObject<UApiItemObject>(this);
	Button_Image->SetVisibility(ESlateVisibility::Collapsed);
	Api->OnItemTextureResponse.BindLambda([this](UTexture2D* Texture)
    {
		UE_LOG(LogTemp,Warning,TEXT("OnImageClick OnTextureResponse BindLambda 성공"))
    	Icon->SetBrushFromTexture(Texture);
    });
	Api->GenerateItemImg(ItemRows.response[0].id);
}
