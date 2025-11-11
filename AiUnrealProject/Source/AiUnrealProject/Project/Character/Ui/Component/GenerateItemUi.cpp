// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateItemUi.h"

#include "JsonObjectConverter.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Project/ApiObject/Npc/ApiItemObject.h"

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
	Api->OnItemInfoResponse.BindLambda([this](FString String)
	{
		Button_Image->SetVisibility(ESlateVisibility::Visible);
		FItemRows Rows;
		FJsonObjectConverter::JsonObjectStringToUStruct(String, &Rows);
		ItemRows = Rows;
		
		UE_LOG(LogTemp,Warning,TEXT("람다안으로 아이템 생성 넘어옴 %s"), *Rows.response[0].Name);
		MonsterName->SetText(FText::FromString(String));
		
		UTextBlock* Text = NewObject<UTextBlock>();
		Text->SetText(FText::FromString(String));
		Text->SetAutoWrapText(true);
		Text->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFontStyle("Regular", 15)));
		RightVerticalBox->AddChild(Text);

		
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
