// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateWorldUi.h"

#include "GenerateMonsterUi.h"
#include "JsonObjectConverter.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UGenerateWorldUi::NativeConstruct()
{
	Super::NativeConstruct();
	Api = NewObject<UApiWorldObject>(this);
	Button_Image->SetVisibility(ESlateVisibility::Collapsed);
	
	Button_Create->OnClicked.AddDynamic(this, &UGenerateWorldUi::OnCreateClick);
	Button_Close->OnClicked.AddDynamic(this, &UGenerateWorldUi::OnCreateClick); 
	Button_Item->OnClicked.AddDynamic(this, &UGenerateWorldUi::OnCreateClick); 
	Button_Image->OnClicked.AddDynamic(this, &UGenerateWorldUi::OnCreateClick);
	
	InputQuestion->OnTextCommitted.AddDynamic(this, &UGenerateWorldUi::OnInputQuestion);
	// InputQuestion->OnTextChanged.AddDynamic(this, &UMyWidget::OnInputChanged);
}

void UGenerateWorldUi::OnCreateClick()
{
}

void UGenerateWorldUi::OnCloseClick()
{
}

void UGenerateWorldUi::OnItemClick()
{
}

void UGenerateWorldUi::OnImageClick()
{
}

void UGenerateWorldUi::OnWorldClick()
{
}

void UGenerateWorldUi::OnInputQuestion(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		UTextBlock* ResponseWorld = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		ResponseWorld->SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 16));
		Api->OnWorldInfoResponse.BindLambda([this,ResponseWorld](FString String)
		{
			FWorldRows Rows;
			FJsonObjectConverter::JsonObjectStringToUStruct(String, &Rows);

			UE_LOG(LogTemp, Warning, TEXT("OnInputQuestion 바인딩후 Response응답 %s"), *String);
			
			UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Title->SetText(FText::FromString(Rows.Response[0].title));
			Title->SetFont(FCoreStyle::GetDefaultFontStyle("regular",16));
			
			UTextBlock* Content = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Content->SetText(FText::FromString(Rows.Response[0].content));
			Content->SetFont(FCoreStyle::GetDefaultFontStyle("regular",16));
			Content->SetAutoWrapText(true);
			
			this->LeftVerticalBox->AddChild(Title);
			this->LeftVerticalBox->AddChild(Content);
			
			ResponseWorld->SetText(FText::FromString(String));
			this->RightVerticalBox->AddChild(ResponseWorld);
		});
		UE_LOG(LogTemp, Display, TEXT("OnInputQuestion %s"), *Text.ToString());
		// Api->GenerateWorldPipeline(Text.ToString());
		Api->GenerateNewxWorld(Text.ToString());
	}
}
