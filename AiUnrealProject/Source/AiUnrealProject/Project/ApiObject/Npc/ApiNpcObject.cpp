// Fill out your copyright notice in the Description page of Project Settings.


#include "ApiNpcObject.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Project/Npc/NpcCharacter.h"

void UApiNpcObject::SendNpcChat(const FString& Question)
{
	UE_LOG(LogTemp, Display, TEXT("SendNpcChat"));
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TEXT("http://127.0.0.1:8000/npc/chat"));
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FNpcChatPost Post;
	Post.question = Question;

	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(Post, JsonString);
	HttpRequest->SetContentAsString(JsonString);
	
	HttpRequest->OnProcessRequestComplete().BindLambda([this](
		FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
	{
		FString Result;
		if (bSuccess && Res.IsValid() && Res->GetResponseCode() == 200)
		{
			FNpcChatResponse Parsed;
			FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Parsed);
			
			UE_LOG(LogTemp, Display, TEXT("NpcChat Received response: %s"), *Res->GetContentAsString());
			
			Result = Parsed.response;
			UE_LOG(LogTemp, Display, TEXT("SendNpcChat success %s"),*Result);
		} else { Result = TEXT("요청 실패"); }
		
		AsyncTask(ENamedThreads::GameThread, [this,Result]()
		{
			OnNpcChatResponse.ExecuteIfBound(Result);
		});
	});
	HttpRequest->ProcessRequest();
}
