#pragma once

DECLARE_DELEGATE_OneParam(FOnNpcChatResponse, FString String)
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonObject.h"

#include "ApiNpcObject.generated.h"

/**
 * 
 */

USTRUCT(Blueprintable, BlueprintType)
struct FNpcChatPost
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString question = FString(TEXT(""));
};

USTRUCT(Blueprintable, BlueprintType)
struct FNpcChatResponse
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString response = FString(TEXT(""));

	UPROPERTY(EditAnywhere)
	FString reply = "reply";
	UPROPERTY(EditAnywhere)
	FString title = "title";
	UPROPERTY(EditAnywhere)
	float similarity = 0.00f;
};

UCLASS()
class AIUNREALPROJECT_API UApiNpcObject : public UCommonObject
{
	GENERATED_BODY()
public:
	
	FOnNpcChatResponse OnNpcChatResponse;
	
	void SendNpcChat(const FString& Question);
};
