// Fill out your copyright notice in the Description page of Project Settings.


#include "UiUtil.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

class USizeBox;

void UUiUtil::ButtonTransparentStyle(UButton* Button)
{
	FButtonStyle TransparentStyle;
	TransparentStyle.SetNormal(FSlateNoResource());
	TransparentStyle.SetHovered(FSlateNoResource());
	TransparentStyle.SetPressed(FSlateNoResource());
	TransparentStyle.Normal.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
	TransparentStyle.Hovered.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0.1f));
	TransparentStyle.Pressed.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0.2f));
	Button->SetStyle(TransparentStyle);

}

USizeBox* UUiUtil::MakeTextCell(UUserWidget* Widget, const FString& Value, float Width, int FontSize, FLinearColor Color)
{
	USizeBox* CellBox = Widget->WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	CellBox->SetWidthOverride(Width);

	UTextBlock* Text = Widget->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	
	Text->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFontStyle("Regular", FontSize)));
	Text->SetText(FText::FromString(Value));
	Text->SetColorAndOpacity(FSlateColor(Color));
	Text->SetJustification(ETextJustify::Left);
	Text->SetAutoWrapText(false);
	Text->SetMargin(FMargin(5, 0, 5, 0));
	CellBox->AddChild(Text);
	return CellBox;
}

// auto MakeTextCell = [&](UUserWidget* Widget,const FString& Value, float Width, FLinearColor Color = FLinearColor::White)
// {
// 	USizeBox* CellBox = Widget->WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
// 	CellBox->SetWidthOverride(Width);
//
// 	UTextBlock* Text = Widget->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
// 	Text->SetText(FText::FromString(Value));
// 	Text->SetColorAndOpacity(FSlateColor(Color));
// 	Text->SetJustification(ETextJustify::Left);
// 	Text->SetAutoWrapText(false);
// 	Text->SetMargin(FMargin(5, 0, 5, 0));
// 	CellBox->AddChild(Text);
// 	return CellBox;
// };
