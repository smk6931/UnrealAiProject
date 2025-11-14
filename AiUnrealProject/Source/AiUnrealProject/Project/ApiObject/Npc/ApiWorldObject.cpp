// Fill out your copyright notice in the Description page of Project Settings.


#include "ApiWorldObject.h"

#include "ApiMonsterObject.h"
#include "ApiNpcObject.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void UApiWorldObject::GetRandomWorld()
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(TEXT("http://127.0.0.1:8000/world/random"));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		if (bSuccess && Res.IsValid())
		{
			FWorldRows Rows;
		    FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Rows);
			OnWorldInfoResponse.ExecuteIfBound(Res->GetContentAsString());
			UE_LOG(LogTemp,Warning,TEXT("GetRandWorld Json원본 : %s 구조체 변환 %s"),*Res->GetContentAsString() ,*Rows.Response[0].title);
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("GetRandWorld 응답 실패 %s"),*Res->GetContentAsString());
		}
	});
	Request->ProcessRequest();
}

void UApiWorldObject::GetWorldsAll()
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(TEXT("http://127.0.0.1:8000/world"));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		if (bSuccess && Res.IsValid())
		{
			FWorldRows Rows;
			FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Rows);
			OnWorldInfoResponse.ExecuteIfBound(Res->GetContentAsString());
			if (Rows.Response.Num() == 0)
			{ UE_LOG(LogTemp,Warning,TEXT("GetWorldsAll Rows 배열 비었음 %s"),*Res->GetContentAsString()) return; }
			
			UE_LOG(LogTemp,Warning,TEXT("GetWorldsAll Json원본 : %s 구조체 변환 %s"),*Res->GetContentAsString() ,*Rows.Response[0].title);
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("GetWorldsAll 응답 실패 %s"),*Res->GetContentAsString());
		}
	});
	Request->ProcessRequest();
}

void UApiWorldObject::GenerateWorldPipeline(FString Prompt)
{
	UE_LOG(LogTemp,Warning,TEXT("GenerateWorldPipeline"));
	FNpcChatPost Request;
	Request.question = Prompt;

	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(Request, JsonString);

	FHttpRequestRef Req = FHttpModule::Get().CreateRequest();
	Req->SetURL(TEXT("http://127.0.0.1:8000/world/generate"));
	Req->SetVerb(TEXT("POST"));
	Req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Req->SetContentAsString(JsonString);
	Req->SetTimeout(600.0f); 
	
	Req->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		UE_LOG(LogTemp,Warning,TEXT("GenerateWorldPipeline Request요청"));
		if (Req->GetStatus() == EHttpRequestStatus::Failed)
		{
			UE_LOG(LogTemp, Error, TEXT("GenerateWorldPipeline Failed: 타임아웃 가능성 높음"));
		}
		if (bSuccess && Res.IsValid())
		{
			UE_LOG(LogTemp,Warning,TEXT("GenerateWorldPipeline 세계관 %s"),*Res->GetContentAsString())
			OnWorldInfoResponse.ExecuteIfBound(Res->GetContentAsString());
		}
	});
	Req->ProcessRequest();
}
