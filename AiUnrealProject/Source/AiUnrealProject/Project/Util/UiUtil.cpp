// Fill out your copyright notice in the Description page of Project Settings.


#include "UiUtil.h"

#include "Components/Button.h"

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
