// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateUi.h"

#include "Components/Button.h"
#include "Project/ApiObject/Npc/ApiMonsterObject.h"

void UGenerateUi::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Create->OnClicked.AddDynamic(this, &UGenerateUi::OnCreateClick);
}

void UGenerateUi::OnCreateClick()
{
	UE_LOG(LogTemp, Display, TEXT("OnCreateClick"));

	Api = NewObject<UApiMonsterObject>(this);
	Api->OnMonsterInfoResponse.BindLambda([this](FString String)
	{
		UE_LOG(LogTemp, Display, TEXT("OnMonsterInfoResponse:BindLambda %s"), *String);
	});
	Api->CreateMonsterAi();
}
