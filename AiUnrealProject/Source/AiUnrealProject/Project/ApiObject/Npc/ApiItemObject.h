// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DECLARE_DELEGATE_OneParam(FOnItemInfoResponse, FString String)

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ApiItemObject.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemRow
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString Name = "name";

	UPROPERTY(BlueprintReadWrite)
	FString Rarity = "rarity";

	UPROPERTY(BlueprintReadWrite)
	FString Type = "Type";

	UPROPERTY(BlueprintReadWrite)
	FString Description = "description";

	UPROPERTY(BlueprintReadWrite)
	FString image_url = "image_url";

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* image;
};

USTRUCT(BlueprintType)
struct FItemRows
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FItemRow> response;
};

UCLASS()
class AIUNREALPROJECT_API UApiItemObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FItemRows Rows;
	
	FOnItemInfoResponse OnItemInfoResponse;

	void ItemInfoResponse();

	void ParseItemInfo();

	void LoadImageFromUrl(const FString& url);
};
