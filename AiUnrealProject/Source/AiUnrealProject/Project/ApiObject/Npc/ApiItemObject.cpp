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

void UApiItemObject::LoadImageFromUrl(const FString& url)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadImageFromUrl 이미지 요청"));
	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindLambda([=, this](
		FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSucceeded)
	{
		if (!bSucceeded || !Res.IsValid())
		{ UE_LOG(LogTemp, Error, TEXT("❌ 이미지 요청 실패 또는 응답 없음"));
			return; }
		int32 Code = Res->GetResponseCode();
		if (Code != 200)
		{ UE_LOG(LogTemp, Error, TEXT("❌ HTTP 오류 코드: %d"), Code);
			return; }
		 TArray<uint8> Data = Res->GetContent();
		if (Data.Num() == 0)
		{ UE_LOG(LogTemp, Error, TEXT("❌ 이미지 데이터가 비어 있음"));
			return; }
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

		OnItemTextureResponse.ExecuteIfBound(Texture);
	});
	Request->ProcessRequest();
}

void UApiItemObject::GenerateItemsForMonsterIds(int id, int item_count, bool bimage)
{
	FString url = "http://127.0.0.1:8000/item/generate/monster_ids";
	
	FString String;
	FMonsterGenerateItemRequest Request;
	Request.monster_id = id;
	Request.item_count = item_count;
	Request.bimage = bimage;
	FJsonObjectConverter::UStructToJsonObjectString(Request, String);
	
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(FString::Printf(TEXT("%s"),*url));
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(String);
	UE_LOG(LogTemp, Display, TEXT("GenerateItemsForMonsterIds 보내는 제이슨%s"), *String);

	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSucceeded)
	{
		if (bSucceeded)
		{
			FItemRows Rows;
			FString Response = Res->GetContentAsString();
			FJsonObjectConverter::JsonObjectStringToUStruct(Response, &Rows);
			if (Rows.response.Num() > 0)
			{
				UE_LOG(LogTemp,Warning,TEXT("GenerateItemFor 성공 %s"), *Rows.response[0].Name)
				OnItemInfoResponse.ExecuteIfBound(Response);
				GetItemImageTimerCheck(Rows.response[0].id);
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("GenerateItemFor 실패 %s"), *Res->GetContentAsString())
			}
		}
	});
	HttpRequest->ProcessRequest();
}

void UApiItemObject::GetItemImageTimerCheck(int32 id)
{
	GetWorld()->GetTimerManager().SetTimer(ImageGenerateTimer,[this,id]()
	{
		FString JsonString;
		FItemIds ItemIds;
		ItemIds.item_ids[0] = id;
        FJsonObjectConverter::UStructToJsonObjectString(ItemIds, JsonString);
		
		UE_LOG(LogTemp, Display, TEXT("PollMonsterImageStatus 3초마다 실행"));
	    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
		
	    Request->SetURL(FString::Printf(TEXT("http://127.0.0.1:8000/item/get/item_ids")));
	    Request->SetVerb(TEXT("GET"));
	    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetContentAsString(JsonString);
		
	    TWeakObjectPtr<UApiItemObject> WeakThis(this);
    
	    Request->OnProcessRequestComplete().BindLambda([WeakThis](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	    {
	    	if (bSuccess || !WeakThis.IsValid() || !Res.IsValid()) return;
	    	
	    	FItemRows Rows;
	    	FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Rows);
	    	UE_LOG(LogTemp, Display, TEXT("GetItemImageTimerCheck Response값 %s \n 변환된 Rows [%s]"),*Res->GetContentAsString(), *Rows.response[0].Name);
	    	
	    	if (Rows.response[0].image_url == FItemRows().response[0].image_url)
	    	{ UE_LOG(LogTemp, Display, TEXT("GetItemImageTimerCheck 이미지 변환 아직 안됨"));return; }

	    	UE_LOG(LogTemp, Display, TEXT("GetItemImageTimerCheck 이미지 Url가져오기 성공 [%s]"),*Rows.response[0].image_url);
	    	FString Str = FString::Printf(TEXT("http://127.0.0.1:8000/%s"),*Rows.response[0].image_url.Replace(TEXT("\\"), TEXT("/")));
	    	WeakThis->LoadImageFromUrl(Str);
	    	WeakThis->GetWorld()->GetTimerManager().ClearTimer(WeakThis->ImageGenerateTimer);
	    	
	    });
	    Request->ProcessRequest();
	},3,true);
}




// void UApiItemObject::GetItemImage(int32 id)
// {
// 	UE_LOG(LogTemp, Display, TEXT("PollMonsterImageStatus 실행"));
// 	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
// 	Request->SetURL(FString::Printf(TEXT("http://127.0.0.1:8000/item/get/item_ids")));
// 	Request->SetVerb(TEXT("GET"));
// 	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
//
// 	TWeakObjectPtr<UApiItemObject> WeakThis(this);
//
// 	Request->OnProcessRequestComplete().BindLambda([WeakThis](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
// 	{
// 		if (!WeakThis.IsValid() || !Res.IsValid()) return;
//
// 		FString JsonResponse = Res->GetContentAsString();
// 		TSharedPtr<FJsonObject> JsonObj;
// 		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonResponse);
//
// 		if (FJsonSerializer::Deserialize(Reader, JsonObj) && JsonObj.IsValid())
// 		{
// 			FString Status = JsonObj->GetStringField(TEXT("status"));
// 			if (Status == "done")
// 			{
// 				UE_LOG(LogTemp, Display, TEXT("✅ 이미지 생성 완료: 타이머 삭제 %s"), *JsonObj->GetStringField(TEXT("image_url")));
// 			    WeakThis->GetWorld()->GetTimerManager().ClearTimer(WeakThis->ImageGenerateTimer);
// 				
// 				FString ImageUrl = JsonObj->GetStringField(TEXT("image_url"));
// 				FString Str = FString::Printf(TEXT("http://127.0.0.1:8000/%s"),*ImageUrl.Replace(TEXT("\\"), TEXT("/")));
// 				WeakThis->LoadImageFromUrl(Str);
// 			
// 			}
// 		}
// 	});
// 	Request->ProcessRequest();
// }
