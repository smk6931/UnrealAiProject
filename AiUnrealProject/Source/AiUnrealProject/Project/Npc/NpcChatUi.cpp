// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcChatUi.h"

#include "JsonObjectConverter.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Project/ApiObject/ApiNpcObject.h"

class UApiNpcObject;

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
			FNpcChatResponse Response;
			FJsonObjectConverter::JsonObjectStringToUStruct(Result, &Response);
			UE_LOG(LogTemp,Warning,TEXT("NpcChatUi::UiPostEnter OnNpcChatResponse JsonString [%s]"), *Result);
			TextGet->SetText(FText::FromString(FString::Printf(TEXT(" 응답 %s \n\n 유사세계관 %s \n 유사도 %f \n"),*Response.reply, *Response.title, Response.similarity)));
			TextGet->SetAutoWrapText(true);
		});
		Api->SendNpcChat(Text.ToString());
	}
}
