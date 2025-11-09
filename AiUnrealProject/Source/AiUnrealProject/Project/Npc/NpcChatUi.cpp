// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcChatUi.h"

#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Project/ApiObject/Npc/ApiNpcObject.h"

void UNpcChatUi::NativeConstruct()
{
	Super::NativeConstruct();
	TextPost->OnTextCommitted.AddDynamic(this, &UNpcChatUi::UiPostEnter);
}

void UNpcChatUi::UiPostEnter(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		UApiNpcObject* Api = NewObject<UApiNpcObject>();
		Api->OnNpcChatResponse.BindWeakLambda(this,[this](const FString& Result)
		{
			TextGet->SetText(FText::FromString(Result));
		});
		Api->SendNpcChat(Text.ToString());
	}
}
