// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DECLARE_DELEGATE_OneParam(FOnItemInfoResponse, FString String)
DECLARE_DELEGATE_OneParam(FOnItemTextureResponse, UTexture2D* Texture)
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ApiItemObject.generated.h"
#include "CommonObject.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemRow
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UCommonObject* CommonObject;
	
	UPROPERTY(EditAnywhere)
	int id = 1;
	
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

USTRUCT(BlueprintType)
struct FMonsterGenerateItemRequest
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int monster_id = 1;
	UPROPERTY(BlueprintReadWrite)
	int item_count = 1;
	UPROPERTY(BlueprintReadWrite)
	bool bimage = false;
};

USTRUCT(BlueprintType)
struct FItemIds
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<int32> item_ids;
};

UCLASS()
class AIUNREALPROJECT_API UApiItemObject : public UCommonObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FTimerHandle ImageGenerateTimer;
	
	UPROPERTY(EditAnywhere)
	FItemRows Rows;
	
	FOnItemInfoResponse OnItemInfoResponse;
    FOnItemTextureResponse OnItemTextureResponse;
	
	void ItemInfoResponse();
	
	void LoadImageFromUrl(const FString& url);

	FString GenerateItemsForMonsterIdsUrl = FString("http://127.0.0.1:8000/item/generate/monster_ids");
	
	void GenerateItemsForMonsterIds(int id = 1, int item_count = 1, bool bimage = false);

	void GenerateItemImg(int32 id);

	void GetItemImageTimerCheck(int32 id);
};
