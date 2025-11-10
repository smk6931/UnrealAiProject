// Fill out your copyright notice in the Description page of Project Settings.


#include "ApiMonsterObject.h"

#include "ApiItemObject.h"
#include "HttpModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void UApiMonsterObject::StartPollingMonsterImage(int32 id)
{
	GetWorld()->GetTimerManager().SetTimer(ImageGenerateTimer, [this, id]()
	{
		UE_LOG(LogTemp, Display, TEXT("Polling Monster Image 3초 타이머"));
		PollMonsterImageStatus(id);
	}, 3.0f, true); // 3초 
}

void UApiMonsterObject::PollMonsterImageStatus(int32 id)
{
	UE_LOG(LogTemp, Display, TEXT("PollMonsterImageStatus 실행"));
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(FString::Printf(TEXT("http://127.0.0.1:8000/monster/image/%d"), id));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TWeakObjectPtr<UApiMonsterObject> WeakThis(this);

	Request->OnProcessRequestComplete().BindLambda([WeakThis](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		if (!WeakThis.IsValid() || !Res.IsValid()) return;

		FString JsonResponse = Res->GetContentAsString();
		TSharedPtr<FJsonObject> JsonObj;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonResponse);

		if (FJsonSerializer::Deserialize(Reader, JsonObj) && JsonObj.IsValid())
		{
			FString Status = JsonObj->GetStringField(TEXT("status"));
			if (Status == "done")
			{
				FString ImageUrl = JsonObj->GetStringField(TEXT("image_url"));
				UE_LOG(LogTemp, Display, TEXT("✅ 이미지 생성 완료: %s"), *ImageUrl);

				FString Str = FString::Printf(TEXT("http://127.0.0.1:8000/%s"),*ImageUrl.Replace(TEXT("\\"), TEXT("/")));
				WeakThis->LoadImageFromUrl(Str);

				// WeakThis->LoadMonsterImageFromUrl(ImageUrl);
			    // ImgRequest->SetURL(FString::Printf(TEXT("http://127.0.0.1:8000/%s"), *Str));
			}
		}
	});
	Request->ProcessRequest();
}

void UApiMonsterObject::MonsterInfoResponse()
{
	UE_LOG(LogTemp, Warning, TEXT("MonsterInfoResponse"));
	
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TEXT("http://127.0.0.1:8000/monster"));
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		FString response;
		if (bSuccess && Res.IsValid() && Res->GetResponseCode() == 200)
		{
			UE_LOG(LogTemp, Warning, TEXT("MonsterInfoResponse성공 %s"),*Res->GetContentAsString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MonsterInfoResponse실패"));
		}
		AsyncTask(ENamedThreads::GameThread, [this,Res]()
		{
			OnMonsterInfoResponse.ExecuteIfBound(Res->GetContentAsString());
		});
	});
	HttpRequest->ProcessRequest();
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
	        	{
	        		OnMonsterTextureResponse.ExecuteIfBound(Texture);
	        	});
		    }
		}
	});
	Request->ProcessRequest();
}

void UApiMonsterObject::CreateMonsterAi()
{
	UE_LOG(LogTemp,Warning,TEXT("CreateMonsterAi"))
	
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(FString::Printf(TEXT("%s"),*MonsterGenerateUrl));
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
		WeakThis->StartPollingMonsterImage(Rows.response[0].id);
		WeakThis->OnMonsterInfoResponse.ExecuteIfBound(JsonResponse);
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
