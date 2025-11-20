// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "MonsterFsm.h"
#include "MonsterImage.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"


// Sets default values
AMonster::AMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MonsterUiComp = CreateDefaultSubobject<UWidgetComponent>("MonsterUiComp");
	MonsterUiComp->SetupAttachment(GetRootComponent());
	
	MonsterImageComp = CreateDefaultSubobject<UWidgetComponent>("MonsterImageComp");
	MonsterImageComp->SetupAttachment(GetRootComponent());
	
	MonsterImageComp->SetWidgetSpace(EWidgetSpace::World); // 또는 World
	MonsterImageComp->SetDrawSize(FVector2D(300, 200));
	MonsterImageComp->SetPivot(FVector2D(0.5f, 0.f));
	MonsterImageComp->SetWorldRotation(FRotator(0,180,0));

	MonsterUiComp->SetWidgetSpace(EWidgetSpace::Screen);

	MonsterFsm = CreateDefaultSubobject<UMonsterFsm>(TEXT("MonsterFsm"));
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	MonsterUi = CreateWidget<UMonsterUi>(GetWorld(), MonsterUiFactory);
	MonsterUiComp->SetWidget(MonsterUi);
	MonsterUiComp->SetDrawSize(FVector2D(300,75));
	MonsterUiComp->SetRelativeLocation(FVector(0,0,150));
	
	MonsterImage = CreateWidget<UMonsterImage>(GetWorld(), MonsterImageFactory);
	MonsterImageComp->SetWidget(MonsterImage);
	
	MonsterImageComp->SetRelativeRotation(FRotator(30,-180,0));
	MonsterImageComp->SetRelativeLocation(FVector(0,0,150));
	MonsterImageComp->SetTwoSided(true);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMonster::MonsterRawInit(FMonsterRow Row)
{
	MonsterRow = Row;
	MonsterUi->Name->SetText(FText::FromString(Row.Name));
	MonsterUi->MaxHp->SetText(FText::AsNumber(Row.hp));
	MonsterUi->CurrentHp->SetText(FText::AsNumber(Row.hp));
	
}

