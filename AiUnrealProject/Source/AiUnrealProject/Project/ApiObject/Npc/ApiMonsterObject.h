// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DECLARE_DELEGATE_OneParam(FOnMonsterInfoResponse, FString String)

DECLARE_DELEGATE_OneParam(FOnMonsterTextureResponse, UTexture2D* Texture)

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ApiMonsterObject.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMonsterRow
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString Name = "name";
	UPROPERTY(BlueprintReadWrite)
	int id = 1;
	UPROPERTY(BlueprintReadWrite)
	int level = 1;
	UPROPERTY(BlueprintReadWrite)
	int hp = 100;
	UPROPERTY(BlueprintReadWrite)
	int attack = 10;
	UPROPERTY(BlueprintReadWrite)
	FString habitat = "habitat";
	UPROPERTY(BlueprintReadWrite)
	FString description = "description";
	UPROPERTY(BlueprintReadWrite)
	TArray<int>drop_item_ids;
	UPROPERTY(BlueprintReadWrite)
	FString image_url = "image_url";
	UPROPERTY(BlueprintReadWrite)
	UTexture2D* image;
};

USTRUCT(BlueprintType)
struct FMonsterRows
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FMonsterRow> response;
};

UCLASS()
class AIUNREALPROJECT_API UApiMonsterObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FTimerHandle ImageGenerateTimer;

	UPROPERTY(EditAnywhere)
	FMonsterRows Rows;

	FOnMonsterInfoResponse OnMonsterInfoResponse;
	FOnMonsterTextureResponse OnMonsterTextureResponse;

	void StartPollingMonsterImage(int32 MonsterId);
	void PollMonsterImageStatus(int32 MonsterId);
	
	void MonsterInfoResponse();
	
	void LoadImageFromUrl(const FString& url);

	FString MonsterGenerateUrl = FString("http://127.0.0.1:8000/monster/generate");
	void CreateMonsterAi();
};

