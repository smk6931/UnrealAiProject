// Fill out your copyright notice in the Description page of Project Settings.


#include "ApiMonsterObject.h"

#include "ApiItemObject.h"
#include "HttpModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void UApiMonsterObject::GetMonsterAll()
{
	UE_LOG(LogTemp, Warning, TEXT("GetMonsterAll"));
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(FString::Printf(TEXT("%s/monster"),*Url));
	// HttpRequest->SetURL(TEXT("http://127.0.0.1:8000/monster"));
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		FString response;
		if (bSuccess && Res.IsValid() && Res->GetResponseCode() == 200)
		{ UE_LOG(LogTemp, Warning, TEXT("MonsterInfoResponse성공 %s"),*Res->GetContentAsString()); }
		else
		{ UE_LOG(LogTemp, Warning, TEXT("MonsterInfoResponse실패")); }
		AsyncTask(ENamedThreads::GameThread, [this,Res]()
		{ OnMonsterInfoResponse.ExecuteIfBound(Res->GetContentAsString()); });
	});
	HttpRequest->ProcessRequest();
}

void UApiMonsterObject::GetItemMonsterIimerCheck(int32 id)
{
	OnMonsterInfoResponse.BindWeakLambda(this,[this,id](FString String)
	{
		FMonsterRows Rows;
		FJsonObjectConverter::JsonObjectStringToUStruct(String,&Rows);
		UE_LOG(LogTemp,Warning,TEXT("GetItemMonsterIimerCheck MonsterUrl [%s]"),*Rows.response[0].image_url)
		
		if (Rows.response[0].image_url != FItemRow().image_url)
		{
			FString Str = FString::Printf(TEXT("%s/%s"),*Url,*MonsterRows.response[0].image_url.Replace(TEXT("\\"), TEXT("/")));
			LoadImageFromUrl(Str);
			GetWorld()->GetTimerManager().ClearTimer(ImageGenerateTimer);
		}
	});
		
	GetWorld()->GetTimerManager().SetTimer(ImageGenerateTimer, [this,id]()
	{
		GetMonsters(id);
	},3.0f, true);
}


void UApiMonsterObject::GetMonsters(int32 id)
{
	FString JsonString;
	FMonsterIds MonsterIds;
	MonsterIds.monster_ids.Add(id);
	FJsonObjectConverter::UStructToJsonObjectString(MonsterIds, JsonString);

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(FString::Printf(TEXT("%s/monster/get/monster_ids"), *Url));
	// Request->SetURL("http://127.0.0.1:8000/monster/get/monster_ids");
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(JsonString);
	Request->OnProcessRequestComplete().BindWeakLambda(this, [this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		if (!bSuccess || !Res.IsValid() || Res->GetResponseCode() != 200)
		{ UE_LOG(LogTemp, Warning, TEXT("GetMonsters 요청 실패 %s"), *Res->GetContentAsString()); return;}
		FMonsterRows Rows;
		FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Rows);
		if (Rows.response.Num() > 0)
		{
			UE_LOG(LogTemp,Warning,TEXT("GetMonsters 파싱성공 %s | %s"), *Rows.response[0].Name, *Rows.response[0].image_url)
			MonsterRows = Rows;
			OnMonsterInfoResponse.ExecuteIfBound(Res->GetContentAsString());
		}
	});
	Request->ProcessRequest();
}

void UApiMonsterObject::GenerateMonsterImg(int32 id)
{
	FString JsonString;
	FMonsterIds MonsterIds;
	MonsterIds.monster_ids.Add(id);
	FJsonObjectConverter::UStructToJsonObjectString(MonsterIds, JsonString);

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(FString::Printf(TEXT("%s/monster/image/generate"), *Url));
	// Request->SetURL(TEXT("http://127.0.0.1:8000/monster/image/generate"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();

	GetItemMonsterIimerCheck(id);
}

void UApiMonsterObject::LoadImageFromUrl(const FString& url)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadImageFromUrl 몬스터 이미지 요청"));
	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(url);
	Request->SetVerb(TEXT("GET"));
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		const TArray<uint8>& Data = Res->GetContent();
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> Wrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
		if (Wrapper.IsValid() && Wrapper->SetCompressed(Data.GetData(), Data.Num()))
		{
			TArray<uint8> RawData;;
		    if (Wrapper->GetRaw(ERGBFormat::BGRA,8,RawData))
		    {
		    	UTexture2D* Texture = UTexture2D::CreateTransient(
		    	Wrapper->GetWidth(),
		    	Wrapper->GetHeight(),
		    	PF_B8G8R8A8);
		    	void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	        	FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
	        	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
	        	Texture->UpdateResource();
	        	AsyncTask(ENamedThreads::GameThread, [this,Texture]()
	        	{ OnMonsterTextureResponse.ExecuteIfBound(Texture); });
		    }
		}
	});
	Request->ProcessRequest();
}

void UApiMonsterObject::CreateMonsterAi()
{
	UE_LOG(LogTemp,Warning,TEXT("CreateMonsterAi"))
	
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(FString::Printf(TEXT("%s/monster/generate"), *Url));
	// HttpRequest->SetURL("http://127.0.0.1:8000/monster/generate");
	HttpRequest->SetTimeout(120.0f); 
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TWeakObjectPtr<UApiMonsterObject> WeakThis(this);
	HttpRequest->OnProcessRequestComplete().BindLambda([WeakThis](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
   		FMonsterRows Rows;
   	    FString JsonResponse = Res->GetContentAsString();
		FJsonObjectConverter::JsonObjectStringToUStruct(JsonResponse, &Rows);

   	    UE_LOG(LogTemp, Display, TEXT("Monster 생성 응답: %s 몬스터 id는 %d 첫행 이름 %s"),*JsonResponse, Rows.response[0].id, *Rows.response[0].Name);
		WeakThis->OnMonsterInfoResponse.ExecuteIfBound(JsonResponse);
    });
	HttpRequest->ProcessRequest();
}

void UApiMonsterObject::GenerateMonster(FString String)
{
	UE_LOG(LogTemp,Warning,TEXT("GenerateMonster Json값 %s"),*String)
	
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL("http://127.0.0.1:8000/monster/generate");
	HttpRequest->SetTimeout(120.0f); 
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(String);

	TWeakObjectPtr<UApiMonsterObject> WeakThis(this);
	HttpRequest->OnProcessRequestComplete().BindLambda([WeakThis](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		if (!bSuccess){UE_LOG(LogTemp,Warning,TEXT("GenerateMonster bSuccess 실패")) return; }
		
		FMonsterRows Rows;
		FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Rows);
		if (Rows.response.Num() == 0)
		{
			UE_LOG(LogTemp,Warning,TEXT("GenerateMonster 파싱 실패 Json : %s"), *Res->GetContentAsString()) return;
		}
		UE_LOG(LogTemp, Display, TEXT("Monster 생성 Execute Delegate : %s \n 몬스터 id는 %d \n 첫행 이름 %s"),*Res->GetContentAsString(), Rows.response[0].id, *Rows.response[0].Name);
		WeakThis->OnMonsterInfoResponse.ExecuteIfBound(Res->GetContentAsString());
	});
	HttpRequest->ProcessRequest();
}


// void UApiMonsterObject::CreateMonsterAi()
// {
// 	UE_LOG(LogTemp,Warning,TEXT("CreateMonsterAi"))
// 	
// 	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
// 	HttpRequest->SetURL(FString::Printf(TEXT("%s"),*MonsterGenerateUrl));
// 	HttpRequest->SetTimeout(120.0f); 
// 	HttpRequest->SetVerb(TEXT("POST"));
// 	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
// 	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
// 	{
// 		FString response;
// 		if (bSuccess && Res.IsValid() && Res->GetResponseCode() == 200)
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("MonsterInfoResponse성공 %s"),*Res->GetContentAsString());
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("MonsterInfoResponse실패"));
// 		}
// 		AsyncTask(ENamedThreads::GameThread, [this,Res]()
// 		{
// 			OnMonsterInfoResponse.ExecuteIfBound(Res->GetContentAsString());
// 		});
// 	});
// 	HttpRequest->ProcessRequest();
// }
