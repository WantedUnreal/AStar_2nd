// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AStarPlayer.generated.h"

UCLASS()
class ASTAR_API AAStarPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAStarPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

	// block 블루프린트
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABlockActor> blockFactory;
	// block Array
	UPROPERTY()
	TArray<class ABlockActor*> blocks;
	// open Array (여기에 들어있는 block 들 기준으로 앞뒤좌우대각선들 Cost 구하자)
	TArray<int32> openArray;
	// close Array (검색 다한 Block)
	TArray<int32> closeArray;
	// 목적지 block index
	int32 goalBlockIdx = -1;
	// 현재 중심 block index
	int32 currBlockIdx;
	// 길찾기 시작
	void StartFindPath();
	// 앞뒤좌우대각선들 Cost 구하면서 길찾기
	void FindPath();
	// Cost를 구하고 OpenArray 에 추가하는 함수
	void AddOpenArray(FVector dir);
	// 세로 nX 번째, 가로 nY 번째 Block 10 X 10 안에 존재하는지 여부 
	bool CheckIndex(int32 nX, int32 nY);
};



