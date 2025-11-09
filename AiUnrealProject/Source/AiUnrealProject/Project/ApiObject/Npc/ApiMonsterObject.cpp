// Fill out your copyright notice in the Description page of Project Settings.


#include "ApiMonsterObject.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"

void UApiMonsterObject::MonsterInfoResponse()
{
	UE_LOG(LogTemp, Warning, TEXT("MonsterInfoResponse"));
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TEXT("http://127.0.0.1:8000/monster"));
}

void UApiMonsterObject::LoadImageFromUrl(const FString& url)
{
}
