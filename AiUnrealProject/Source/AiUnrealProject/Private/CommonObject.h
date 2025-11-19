// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonObject.generated.h"

/**
 * 
 */
// USTRUCT(BlueprintType)
// struct FUrlStruct
// {
// 	GENERATED_BODY()
// public:
// 	UPROPERTY(BlueprintReadWrite)
// 	FString url = "http://127.0.0.1:8080/";
// };

UCLASS(Blueprintable)
class AIUNREALPROJECT_API UCommonObject : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite)
	FString Url = "http://127.0.0.1:8080";
};
