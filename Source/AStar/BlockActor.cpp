// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockActor.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"


// Sets default values
ABlockActor::ABlockActor()
{
	// Set this actor to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABlockActor::BeginPlay()
{
	Super::BeginPlay();

	// Widget Component 가져오자.
	UWidgetComponent* compWidget = GetComponentByClass<UWidgetComponent>();
	UUserWidget* costUI = compWidget->GetWidget();

	// outline 가져오자.
	outline = Cast<UImage>(costUI->GetWidgetFromName(TEXT("Outline")));
	// cost Text 가져오자.
	fCost = Cast<UTextBlock>(costUI->GetWidgetFromName(TEXT("FCost")));
	gCost = Cast<UTextBlock>(costUI->GetWidgetFromName(TEXT("GCost")));
	hCost = Cast<UTextBlock>(costUI->GetWidgetFromName(TEXT("HCost")));

	// 랜덤하게 이동 여부 설정
	int32 rand = FMath::RandRange(0, 99);
	if (rand < 10)
	{
		canMove = false;
		ChangeColorOutline(FLinearColor::Black);
	}	
}

// Called every frame
void ABlockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlockActor::Init(int32 nY, int nX)
{
	indexY = nY / 100;
	indexX = nX / 100;
	index = indexY + indexX * 10;
}

void ABlockActor::ChangeColorOutline(FLinearColor color)
{
	
	// 색 변경
	outline->SetColorAndOpacity(color);
}

void ABlockActor::SetCost(ABlockActor* s, ABlockActor* g)
{
}


