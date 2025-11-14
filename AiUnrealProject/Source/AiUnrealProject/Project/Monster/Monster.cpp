// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "Components/WidgetComponent.h"


// Sets default values
AMonster::AMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MonsterUiComp = CreateDefaultSubobject<UWidgetComponent>("MonsterUiComp");
	MonsterUiComp->SetupAttachment(GetRootComponent());
	
	MonsterUiComp->SetWidgetSpace(EWidgetSpace::Screen); // 또는 World
	MonsterUiComp->SetDrawSize(FVector2D(300, 200));
	MonsterUiComp->SetPivot(FVector2D(0.5f, 0.f));
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	MonsterUi = CreateWidget<UMonsterUi>(GetWorld(), MonsterUiFactory);
	MonsterUiComp->SetWidget(MonsterUi);
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

