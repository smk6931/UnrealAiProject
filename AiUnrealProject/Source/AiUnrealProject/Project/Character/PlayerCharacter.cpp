// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "LandscapeEditTypes.h"
#include "Project/ApiObject/Npc/ApiItemObject.h"
#include "Project/Npc/NpcCharacter.h"
#include "Ui/ItemInfoUi.h"
#include "Ui/MenuUi.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	InventoryObject->Init();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
	{
		UMenuUi* Ui = InventoryObject->MenuUi;
		Ui->GetVisibility() == ESlateVisibility::Hidden ? Ui->SetVisibility(ESlateVisibility::Visible) : Ui->SetVisibility(ESlateVisibility::Hidden);
		Ui->BpItemInfoUi->LoadItems();

		//Api->ParseItemInfo();
		/*FItemRow Row;
		TArray<FItemRow> Items;
		Items.Add(Row);
		Ui->BpItemInfoUi->LoadItems(Items);*/
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{
		FHitResult Hit;
		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		if (Hit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter %s"),*Hit.GetActor()->GetName());
			if (ANpcCharacter* Npc = Cast<ANpcCharacter>(Hit.GetActor()))
			{
				if (Npc->NpcName == "Chat")
				{
					Npc->OpenQuest();
				}
				else if (Npc->NpcName == "Item")
				{
					Npc->OpenItem();
				}
			}
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

