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
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(FString::Printf(TEXT("%s/item"), *Url));
	// Request->SetURL(TEXT("http://127.0.0.1:8000/item"));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		FString Response;
		if (bSuccess && Res.IsValid() && Res->GetResponseCode() == 200)
		{ UE_LOG(LogTemp, Warning, TEXT("ItemInfoResponse성공 %s"),*Res->GetContentAsString()); }
		else
		{ UE_LOG(LogTemp, Warning, TEXT("ItemInfoResponse실패")); }
		
		AsyncTask(ENamedThreads::GameThread, [this,Res]()
		{
			OnItemInfoResponse.ExecuteIfBound(Res->GetContentAsString());
		});
	});
	Request->ProcessRequest();
}

void UApiItemObject::LoadImageFromUrl(const FString& url)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadImageFromUrl 이미지 요청"));
	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(url);
	Request->SetVerb("GET");

	TWeakObjectPtr<UApiItemObject> WeakThis(this);
	Request->OnProcessRequestComplete().BindLambda([WeakThis](
		FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSucceeded)
	{
		if (!bSucceeded || !Res.IsValid())
		{ UE_LOG(LogTemp, Error, TEXT("❌ 이미지 요청 실패 또는 응답 없음")); return; }
		TArray<uint8> Data = Res->GetContent();
		if (Data.Num() == 0)
		{ UE_LOG(LogTemp, Error, TEXT("❌ 이미지 데이터가 비어 있음")); return; }

		TSharedPtr<IImageWrapper> Wrapper =
			FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"))
			.CreateImageWrapper(EImageFormat::PNG);

		if (!Wrapper.IsValid() || !Wrapper->SetCompressed(Data.GetData(), Data.Num())) // ✅ 수정됨
		{ UE_LOG(LogTemp, Error, TEXT("❌ PNG 압축 해제 실패")); return; }

		// ✅ [3] RGBA 디코드
		TArray<uint8> RGBA; if (!Wrapper->GetRaw(ERGBFormat::BGRA, 8, RGBA))
        { UE_LOG(LogTemp, Error, TEXT("❌ RGBA 변환 실패")); return; }

		// ✅ [4] Texture 생성 (PlatformData null 방지)
		UTexture2D* Texture = UTexture2D::CreateTransient(Wrapper->GetWidth(), Wrapper->GetHeight(), PF_B8G8R8A8);
		
		// ✅ [5] RGBA 데이터를 Texture로 복사
		void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(TextureData, RGBA.GetData(), RGBA.Num());
		Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

		UE_LOG(LogTemp, Warning, TEXT("LoadImageFromUrl 이거다음 Texture생성 및 델리게이트ExcuteIfBound"));
		Texture->UpdateResource();
		WeakThis->OnItemTextureResponse.ExecuteIfBound(Texture);
		// ✅ [6] 델리게이트 안전 실행
		// if (WeakThis.IsValid() && WeakThis->OnItemTextureResponse.IsBound()) { WeakThis->OnItemTextureResponse.Execute(Texture); }
	});
	Request->ProcessRequest();
}

void UApiItemObject::GenerateItemsForMonsterIds(int id, int item_count, bool bimage)
{
	// FString url = "http://127.0.0.1:8000/item/generate/monster_ids";
	FString url = FString::Printf(TEXT("%s/item/generate/monster_ids"), *Url);
	
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
			else { UE_LOG(LogTemp,Warning,TEXT("GenerateItemFor 실패 %s"), *Res->GetContentAsString()) }
		}
	});
	HttpRequest->ProcessRequest();
}

void UApiItemObject::GenerateItemImg(int32 id)
{
	FString JsonString;
	FItemIds ItemIds;
	ItemIds.item_ids.Add(id);
	FJsonObjectConverter::UStructToJsonObjectString(ItemIds, JsonString);
	UE_LOG(LogTemp, Display, TEXT("GenerateItemImg 아이템 생성 요청 Json파싱은? [%s]"),*JsonString);
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->SetURL(FString::Printf(TEXT("%s/item/image/generate"), *Url));
	// Request->SetURL(FString("http://127.0.0.1:8000/item/image/generate"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();
}

void UApiItemObject::GetItemImageTimerCheck(int32 id)
{
	this->AddToRoot();
	GetWorld()->GetTimerManager().SetTimer(ImageGenerateTimer, [this, id]()
	{
		FString JsonString;
		FItemIds ItemIds;
		ItemIds.item_ids.Add(id);
		FJsonObjectConverter::UStructToJsonObjectString(ItemIds, JsonString);
		FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(FString::Printf(TEXT("%s/item/get/item_ids"), *Url));
		Request->SetVerb(TEXT("POST"));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetContentAsString(JsonString);
		Request->OnProcessRequestComplete().BindWeakLambda(this, [this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
		{
			if (!bSuccess || !Res.IsValid()) return;
			FItemRows Rows;
			if (FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Rows) && Rows.response.Num() > 0)
			{
				if (Rows.response[0].image_url != FItemRow().image_url)
				{
					UE_LOG(LogTemp, Warning, TEXT("GenerateItemImg Image Url비교 %s | %s"),*Rows.response[0].image_url, *FItemRow().image_url );
					LoadImageFromUrl(FString::Printf(TEXT("%s/%s"),*Url, *Rows.response[0].image_url.Replace(TEXT("\\"), TEXT("/"))));

					GetWorld()->GetTimerManager().ClearTimer(ImageGenerateTimer);
					this->RemoveFromRoot();
				}
			}
		});
		Request->ProcessRequest();
	}, 3.f, true);
}



// void UApiItemObject::LoadImageFromUrl(const FString& url)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("LoadImageFromUrl 이미지 요청"));
// 	FHttpModule* Http = &FHttpModule::Get();
// 	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
// 	Request->SetURL(url);
// 	Request->SetVerb("GET");
// 	
// 	Request->OnProcessRequestComplete().BindLambda([=, this](
// 		FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSucceeded)
// 	{
// 		if (!bSucceeded || !Res.IsValid())
// 		{ UE_LOG(LogTemp, Error, TEXT("❌ 이미지 요청 실패 또는 응답 없음")) return; }
// 		if (Res->GetResponseCode() != 200)
// 		{ UE_LOG(LogTemp, Error, TEXT("❌ HTTP 오류 코드: %d"), Res->GetResponseCode()); return; }
// 		if (Res->GetContent().Num() == 0)
// 		{ UE_LOG(LogTemp, Error, TEXT("❌ 이미지 데이터가 비어 있음")); return; }
// 		
// 		TSharedPtr<IImageWrapper> Wrapper =
// 			FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper")).
// 			CreateImageWrapper(EImageFormat::PNG);
// 		
// 		const TArray<uint8>* RGBA = nullptr;
// 		UTexture2D* Texture = UTexture2D::CreateTransient(
// 			Wrapper->GetWidth(), Wrapper->GetHeight(), PF_B8G8R8A8);
// 		void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
// 		FMemory::Memcpy(TextureData, RGBA->GetData(), RGBA->Num());
// 		Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
// 		Texture->UpdateResource();
// 		OnItemTextureResponse.ExecuteIfBound(Texture);
// 	});
// 	Request->ProcessRequest();
// }



// void UApiItemObject::GetItemImageTimerCheck(int32 id)
// {
// 	GenerateItemImg(id);
// 	
// 	GetWorld()->GetTimerManager().SetTimer(ImageGenerateTimer,[this,id]()
// 	{
// 		this->AddToRoot();
// 		
// 		FString JsonString;
// 		FItemIds ItemIds;
// 		ItemIds.item_ids.Add(id);
// 		FJsonObjectConverter::UStructToJsonObjectString(ItemIds, JsonString);
// 		
// 		UE_LOG(LogTemp, Display, TEXT("PollMonsterImageStatus 3초마다 실행"));
// 		FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
// 		
// 		Request->SetURL(FString::Printf(TEXT("http://127.0.0.1:8000/item/get/item_ids")));
// 		Request->SetVerb(TEXT("POST"));
// 		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
// 		Request->SetContentAsString(JsonString);
// 		
// 		TWeakObjectPtr<UApiItemObject> WeakThis(this);
// 		Request->OnProcessRequestComplete().BindLambda([WeakThis](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
// 		{
// 			if (!bSuccess || !WeakThis.IsValid() || !Res.IsValid()) return;
// 			FItemRows Rows;
// 			FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Rows);
// 			UE_LOG(LogTemp, Display, TEXT("GetItemImageTimerCheck Response값 %s \n 변환된 Rows [%s]"),*Res->GetContentAsString(), *Rows.response[0].Name);
// 	    	
// 			if (Rows.response[0].image_url == FItemRows().response[0].image_url)
// 			{ UE_LOG(LogTemp, Display, TEXT("GetItemImageTimerCheck 이미지 변환 아직 안됨"));return; }
//
// 			UE_LOG(LogTemp, Display, TEXT("GetItemImageTimerCheck 이미지 Url가져오기 성공 [%s]"),*Rows.response[0].image_url);
// 			FString Str = FString::Printf(TEXT("http://127.0.0.1:8000/%s"),*Rows.response[0].image_url.Replace(TEXT("\\"), TEXT("/")));
// 			WeakThis->LoadImageFromUrl(Str);
// 			WeakThis->GetWorld()->GetTimerManager().ClearTimer(WeakThis->ImageGenerateTimer);
// 	    	
// 		});
// 		Request->ProcessRequest();
// 	},3,true);
// }




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
