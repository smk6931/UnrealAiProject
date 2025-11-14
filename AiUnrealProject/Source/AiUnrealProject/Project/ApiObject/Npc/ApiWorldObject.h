// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ApiWorldObject.generated.h"

DECLARE_DELEGATE_OneParam(FOnWorldInfoResponse, FString String)

/**
 * 
 */


USTRUCT(BlueprintType)
struct FWorldRow
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="WorldStory")
	int32 id = 0;
	UPROPERTY(BlueprintReadWrite, Category="WorldStory")
	FString title = "InitTitle";
	UPROPERTY(BlueprintReadWrite, Category="WorldStory")
	FString content = "InitContent";
	UPROPERTY(BlueprintReadWrite, Category="WorldStory")
	FString metadata = "InitMetadata";

	UPROPERTY(BlueprintReadWrite, Category="WorldStory")
	float similarity = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category="WorldStory")
	FString similar_reason = "InitSimilarReason";
};

USTRUCT(BlueprintType)
struct FWorldRows
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="WorldStory")
	TArray<FWorldRow> Response;
};

USTRUCT(BlueprintType)
struct FWorldRowsWithEmbed
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="WorldStory")
	TArray<FWorldRow> Response_Embed;
	
	UPROPERTY(BlueprintReadWrite, Category="WorldStory")
	TArray<FWorldRow> Response;
};

UCLASS()
class AIUNREALPROJECT_API UApiWorldObject : public UObject
{
	GENERATED_BODY()
public:

	FOnWorldInfoResponse OnWorldInfoResponse;
	
	void GetRandomWorld();
	void GetWorldsAll();

	void GenerateWorldPipeline(FString Prompt);
	void GenerateNextWorld(FString Prompt);
};
