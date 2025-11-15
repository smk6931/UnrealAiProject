// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcCharacter.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/ShaderKeyGenerator.h"


// Sets default values
ANpcCharacter::ANpcCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANpcCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANpcCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANpcCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANpcCharacter::OpenQuest()
{
	UE_LOG(LogTemp, Display, TEXT("ANpcCharacter::OpenQuest"));
	if (!NpcChatUi)
	{
		NpcChatUi = CreateWidget<UNpcChatUi>(GetWorld(),NpcChatUiFactory);
		NpcChatUi->AddToViewport();
	}
	else
	{
		NpcChatUi->SetVisibility(NpcChatUi->GetVisibility() == ESlateVisibility::Visible?
		ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

void ANpcCharacter::OpenItem()
{
	UE_LOG(LogTemp, Display, TEXT("ANpcCharacter::OpenQuest"));
	if (!Inventory)
	{
		Inventory = CreateWidget<UInventory>(GetWorld(),InventoryFactory);
		Inventory->AddToViewport();
	}
	else
	{
		Inventory->SetVisibility(Inventory->GetVisibility() == ESlateVisibility::Visible?
		ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

