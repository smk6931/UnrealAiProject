// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateMonsterUi.h"

#include "GenerateItemUi.h"
#include "JsonObjectConverter.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Project/ApiObject/Npc/ApiMonsterObject.h"
#include "Project/Character/Ui/MenuUi.h"
#include "Project/Util/UiUtil.h"

void UGenerateMonsterUi::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Create->OnClicked.AddDynamic(this, &UGenerateMonsterUi::OnCreateClick);
	Button_Close->OnClicked.AddDynamic(this, &UGenerateMonsterUi::OnCloseClick);
	Button_Item->OnClicked.AddDynamic(this, &UGenerateMonsterUi::OnItemClick);
}

void UGenerateMonsterUi::OnCreateClick()
{
	UE_LOG(LogTemp, Display, TEXT("OnCreateClick"));

	Api = NewObject<UApiMonsterObject>(this);
	Api->OnMonsterTextureResponse.BindLambda([this](UTexture2D* Texture)
	{
		Icon->SetBrushFromTexture(Texture);
	});

	Api->OnMonsterInfoResponse.BindLambda([this](FString String)
	{
		FMonsterRows Rows;
		FJsonObjectConverter::JsonObjectStringToUStruct(String,&Rows);

		UUiUtil* Util = NewObject<UUiUtil>(this);
		
		if (Rows.response.Num() > 0)
		{
			int FonsSize = 15;
			UE_LOG(LogTemp, Display, TEXT("OnMonsterInfoResponse Rows 파싱 성공 %s"), *Rows.response[0].Name)
			Title->SetText(FText::FromString(Rows.response[0].Name));
			
			UHorizontalBox* RowBoxName = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			USizeBox* NameT = Util->MakeTextCell(this, TEXT("몬스터"),150,FonsSize,FColor::Orange);
			USizeBox* Name = Util->MakeTextCell(this, Rows.response[0].Name,150,FonsSize);
			RowBoxName->AddChild(NameT);
			RowBoxName->AddChild(Name);
			LeftVerticalBox->AddChild(RowBoxName);

			UHorizontalBox* RowBoxLevel = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			USizeBox* LevelT = Util->MakeTextCell(this, TEXT("레벨"),100,FonsSize,FColor::Orange);
			USizeBox* Level = Util->MakeTextCell(this, FString::FromInt(Rows.response[0].level), 150,FonsSize);
			RowBoxLevel->AddChild(LevelT);
			RowBoxLevel->AddChild(Level);
			LeftVerticalBox->AddChild(RowBoxLevel);

			UHorizontalBox* RowBoxAttack = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			USizeBox* AttackT = Util->MakeTextCell(this, TEXT("공격력"),100,FonsSize,FColor::Orange);
			USizeBox* Attack = Util->MakeTextCell(this, FString::FromInt(Rows.response[0].attack), 150,FonsSize);
			RowBoxAttack->AddChild(AttackT);
			RowBoxAttack->AddChild(Attack);
			LeftVerticalBox->AddChild(RowBoxAttack);

			UHorizontalBox* RowBoxHp = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			USizeBox* HpT = Util->MakeTextCell(this, TEXT("생명력"),100,FonsSize,FColor::Orange);
			USizeBox* Hp = Util->MakeTextCell(this, FString::FromInt(Rows.response[0].hp), 150,FonsSize);
			RowBoxHp->AddChild(HpT);
			RowBoxHp->AddChild(Hp);
			LeftVerticalBox->AddChild(RowBoxHp);

			UHorizontalBox* RowBoxhabitat = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
			USizeBox* HabitatT = Util->MakeTextCell(this, TEXT("서식지"),100,FonsSize,FColor::Orange);
			USizeBox* Habitat = Util->MakeTextCell(this, Rows.response[0].habitat, 150,FonsSize);
			RowBoxhabitat->AddChild(HabitatT);
			RowBoxhabitat->AddChild(Habitat);
			UTextBlock* T = Cast<UTextBlock>(Habitat->GetChildAt(0));
			T->SetAutoWrapText(true);
			LeftVerticalBox->AddChild(RowBoxhabitat);

			USizeBox* DescT = Util->MakeTextCell(this, TEXT("몬스터 설명"),100,FonsSize,FColor::Orange);
			UTextBlock* Desc = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Desc->SetText(FText::FromString(Rows.response[0].description));
			Desc->SetAutoWrapText(true);
			Desc->SetMargin(FMargin(20.f));
			Desc->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFontStyle("Regular", FonsSize)));

			// USizeBox* DropItemT= Util->MakeTextCell(this, TEXT("드랍 아이템"),100,FonsSize,FColor::Orange);
			
			RightVerticalBox->AddChild(DescT);
			RightVerticalBox->AddChild(Desc);
		}
		else{UE_LOG(LogTemp, Display, TEXT("OnMonsterInfoResponse Rows 안에 파싱 실패"));}
	});
	Api->CreateMonsterAi();
}

void UGenerateMonsterUi::OnCloseClick()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UGenerateMonsterUi::OnItemClick()
{
	MenuUi->BpGenerateItemUi->SetVisibility(ESlateVisibility::Visible);
}
