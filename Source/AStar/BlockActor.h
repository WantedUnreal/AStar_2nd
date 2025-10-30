// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockActor.generated.h"

UCLASS()
class ASTAR_API ABlockActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlockActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 초기화 함수
	void Init(int32 nY, int nX);

	// 가로 몇 번째 인지
	int32 indexY;
	// 세로 몇 번째 인지
	int32 indexX;
	// Array 에서 몇 번째 인지
	int32 index;
	// 부모 idx (누구로 부터 cost 를 계산했냐?)
	int32 parentIndex = -1;

	// Outline
	UPROPERTY()
	class UImage* outline;
	// Cost
	UPROPERTY()
	class UTextBlock* fCost;
	float fCostValue = -1;
	UPROPERTY()
	class UTextBlock* gCost;
	float gCostValue = -1;
	UPROPERTY()
	class UTextBlock* hCost;
	float hCostValue = -1;

	// 이동 할 수 있는지 여부
	bool canMove = true;

	// Outline 색 변경 함수
	void ChangeColorOutline(FLinearColor color);
	// cost 구하는 함수
	void SetCost(ABlockActor* s, ABlockActor* g);
};


