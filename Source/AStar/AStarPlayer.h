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
	// 길찾기 시작
	void StartFindPath();
};


