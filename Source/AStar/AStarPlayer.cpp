// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarPlayer.h"

#include "BlockActor.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

class FBlockCompare
{
public:
	const TArray<ABlockActor*>& blocks;
	FBlockCompare(const TArray<ABlockActor*>& inBlocks) : blocks(inBlocks) {} 
	bool operator() (const int32& a, const int32& b) const
	{
		// Sort 할 때 자리 바꾸는 조건
		// 최종 비용(fCost) 작은 값 앞으로
		if (blocks[a]->fCostValue < blocks[b]->fCostValue) return true;

		// 최종 비용(fCost) 가 같을 때
		// 시작 비용(gCost) 가 큰값 앞으로
		if (blocks[a]->fCostValue == blocks[b]->fCostValue &&
			blocks[a]->gCostValue > blocks[b]->gCostValue) return true;
		
		return false;
	}
};

// Sets default values
AAStarPlayer::AAStarPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAStarPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 10 x 10 으로 Block 만들자.
	ABlockActor* block = nullptr;
	FVector pos = FVector();
	for (int32 i = 0; i < 100; i++)
	{
		pos.Y = i % 10 * 100;
		pos.X = i / 10 * 100;
		block = GetWorld()->SpawnActor<ABlockActor>(blockFactory, pos, FRotator());
		block->Init(pos.Y, pos.X);
		blocks.Add(block);
	}

	// 마우스 보이게
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(pc);
	pc->SetShowMouseCursor(true);
}

// Called every frame
void AAStarPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{
		StartFindPath();
	}
}

// Called to bind functionality to input
void AAStarPlayer::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAStarPlayer::StartFindPath()
{
	// 시작 Block (Player 가 있는 곳의 Block)
	FVector pos = GetActorLocation();
	int32 nY = pos.Y / 100; // (Y / 가로 하나 크기)
	int32 nX = pos.X / 100; // (X / 세로 하나 크기)

	int32 startIdx = nY + nX * 10;
	UE_LOG(LogTemp, Warning, TEXT("START : %d"), startIdx);
	// 시작 idx 를  openArray 에 추가
	openArray.HeapPush(startIdx, FBlockCompare(blocks));

	// 도착 Block
	FVector start;
	FVector end;
	FVector dir;
	FCollisionQueryParams params;
	FHitResult hitInfo;

	// 마우스 클릭 위치를 3D 공간의 좌표, 방향 구하자.
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(start, dir);
	end = start + dir * 100000;
	params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, params);
	if (ABlockActor* block = Cast<ABlockActor>(hitInfo.GetActor()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Goal : %d"), block->index);
	}
}
