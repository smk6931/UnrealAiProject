// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NpcChatUi.h"
#include "GameFramework/Character.h"
#include "Project/Inventory/Inventory.h"
#include "NpcCharacter.generated.h"

UCLASS()
class AIUNREALPROJECT_API ANpcCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANpcCharacter();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	FString NpcName = FString(TEXT(""));
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UNpcChatUi>NpcChatUiFactory;
	UPROPERTY(EditAnywhere)
	class UNpcChatUi* NpcChatUi;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventory>InventoryFactory;
	UPROPERTY(EditAnywhere)
	class UInventory* Inventory;
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void OpenQuest();

	void OpenItem();
};
