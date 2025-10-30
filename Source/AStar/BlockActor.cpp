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
	// gCost (시작점에서 누적거리)
	float tempCost = FMath::Abs(s->indexY - indexY) + FMath::Abs(s->indexX - indexX);
	// 대각선일 경우 1.5 점 더하자.
	if (tempCost == 2) tempCost = 1.5f;
	float gCostTemp = s->gCostValue + tempCost;

	// hCost ( 목적지까지의 추정치 )
	float hCostTemp = FMath::Max(FMath::Abs(g->indexY - indexY), FMath::Abs(g->indexX - indexX));

	// fCost ( gCost + hCost )
	float fCostTemp = gCostTemp + hCostTemp;

	// 만약 fCostValue 이 -1 이라면 Cost 갱신
	// 만약 fCostTemp 가  fCostValue 보다 작으면 갱신
	// 만약 fCostTemp == fCostValue && gCostTemp 가 gCostValue 보다 크면 갱신
	if (fCostValue == -1 ||
		fCostTemp < fCostValue ||
		(fCostTemp == fCostValue && gCostTemp > gCostValue))
	{
		gCostValue = gCostTemp;
		hCostValue = hCostTemp;
		fCostValue = fCostTemp;

		gCost->SetText(FText::AsNumber(gCostValue));
		hCost->SetText(FText::AsNumber(hCostValue));
		fCost->SetText(FText::AsNumber(fCostValue));

		// 몇 번째 Block 기준으로 Cost 를 구했는가
		parentIndex = s->index;
	}
}


