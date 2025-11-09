// Fill out your copyright notice in the Description page of Project Settings.


#include "ApiItemObject.h"

#include "HttpModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void UApiItemObject::ItemInfoResponse()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemInfoResponse"));
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TEXT("http://127.0.0.1:8000/item"));
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		FString Response;
		if (bSuccess && Res.IsValid() && Res->GetResponseCode() == 200)
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemInfoResponse성공 %s"),*Res->GetContentAsString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemInfoResponse실패"));
		}
		AsyncTask(ENamedThreads::GameThread, [this,Res]()
		{
			OnItemInfoResponse.ExecuteIfBound(Res->GetContentAsString());
		});
	});
	HttpRequest->ProcessRequest();
}

void UApiItemObject::ParseItemInfo()
{
	ItemInfoResponse();
	OnItemInfoResponse.BindWeakLambda(this,[this](FString String)
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(String,&Rows);
		for (int i=0;i<Rows.response.Num();i++)
		{
			UE_LOG(LogTemp,Warning,TEXT("ParseITemInfo아이템목록 %d 번째 %s"),i, *Rows.response[i].Name)
		}
	});
}

void UApiItemObject::LoadImageFromUrl(const FString& url)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadImageFromUrl 이미지 요청"));
	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindLambda([this](
		FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSucceeded)
	{
		TArray<uint8> Data = Res->GetContent();
		TSharedPtr<IImageWrapper> Wrapper =
			FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper")).
		    CreateImageWrapper(EImageFormat::PNG);
		const TArray<uint8>* RGBA = nullptr;
		UTexture2D* Texture = UTexture2D::CreateTransient(
			Wrapper->GetWidth(), Wrapper->GetHeight(), PF_B8G8R8A8);
		void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(TextureData, RGBA->GetData(), RGBA->Num());
		Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
		Texture->UpdateResource();
	});
	Request->ProcessRequest();
}
