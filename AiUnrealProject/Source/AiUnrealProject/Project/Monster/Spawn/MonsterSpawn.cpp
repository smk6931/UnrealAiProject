// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawn.h"

#include "JsonObjectConverter.h"
#include "Components/Image.h"
#include "Project/Monster/Parent/Monster.h"
#include "Project/Monster/Parent/MonsterFsm.h"
#include "Project/Monster/Parent/MonsterImage.h"


// Sets default values
AMonsterSpawn::AMonsterSpawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMonsterSpawn::BeginPlay()
{
	Super::BeginPlay();

	ApiMonster = NewObject<UApiMonsterObject>(this);
	ApiMonster->OnMonsterInfoResponse.BindLambda([this](FString String)
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(String, &MonsterRows);
	});
	ApiMonster->GetMonsterAll();
}

// Called every frame
void AMonsterSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentTime += GetWorld()->GetDeltaSeconds();
	if (CurrentTime >= 4 && Monsters.Num() < 8)
	{
		CurrentTime = 0;
		AMonster* Monster = GetWorld()->SpawnActor<AMonster>(MonsterFactory);
		Monster->SetActorLocation(GetActorLocation() + FVector(FMath::RandRange(-500,500),FMath::RandRange(-500,500),25));
		Monster->MonsterRawInit(MonsterRows.response[FMath::RandRange(0,MonsterRows.response.Num()-1)]);
		Monster->MonsterFsm->MonsInitLoc = Monster->GetActorLocation();
		
		ApiMonster->OnMonsterTextureResponse.BindLambda([this,Monster](UTexture2D* Texture)
		{
			if (!Monster)
			{
				UE_LOG(LogTemp, Error, TEXT("Monster is null")); return;
			}
			Monster->MonsterImage->Image->SetBrushFromTexture(Texture);
		});
		FString Str = Monster->MonsterRow.image_url.Replace(TEXT("\\"), TEXT("/"));
		UE_LOG(LogTemp, Display, TEXT("Monster Spawn Url %s/%s "),*ApiMonster->Url,*Str);
		ApiMonster->LoadImageFromUrl(FString::Printf(TEXT("%s/%s"),*ApiMonster->Url,*Monster->MonsterRow.image_url));
		
		Monsters.Add(Monster);
	}
}

