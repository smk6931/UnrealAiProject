// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryObject.h"
#include "Blueprint/UserWidget.h"
#include "Project/Character/Ui/MenuUi.h"


void UInventoryObject::Init()
{
	MenuUi = CreateWidget<UMenuUi>(GetWorld(), MenuUiFactory);
	MenuUi->AddToViewport();
	MenuUi->SetVisibility(ESlateVisibility::Hidden);
}
