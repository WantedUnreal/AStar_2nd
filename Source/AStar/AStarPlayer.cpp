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
		if (block->canMove)
		{
			goalBlockIdx = block->index;
		}
	}

	// 시작 지점과 도착 지점이 잘 설정되었다면
	if (goalBlockIdx != -1)
	{
		FindPath();
	}
}

void AAStarPlayer::FindPath()
{
	// openArray 에서 우선순위 되는 value 가져오고 Array 에서 제거
	openArray.HeapPop(currBlockIdx, FBlockCompare(blocks));

	AddOpenArray(FVector::ForwardVector);
	AddOpenArray(FVector::BackwardVector);
	AddOpenArray(FVector::RightVector);
	AddOpenArray(FVector::LeftVector);
	AddOpenArray(FVector::ForwardVector + FVector::RightVector);
	AddOpenArray(FVector::ForwardVector + FVector::LeftVector) ;
	AddOpenArray(FVector::BackwardVector + FVector::RightVector);
	AddOpenArray(FVector::BackwardVector + FVector::LeftVector);

	closeArray.Add(currBlockIdx);
	blocks[currBlockIdx]->ChangeColorOutline(FLinearColor::Gray);
}

void AAStarPlayer::AddOpenArray(FVector dir)
{
	// 다음 Y 축 방향 index
	int32 nextY = blocks[currBlockIdx]->indexY + dir.Y;
	// 다음 X 축 방향 index
	int32 nextX = blocks[currBlockIdx]->indexX + dir.X;
	// block 들 범위 밖에 있다면 함수를 나가자.
	if (CheckIndex(nextX, nextY) == false) return;

	// targetBlock index
	int32 targetBlockIdx = currBlockIdx;
	// 앞쪽
	if (dir.X == 1) targetBlockIdx += 10;
	// 뒤쪽
	else if (dir.X == -1) targetBlockIdx -= 10;
	// 오른쪽
	if (dir.Y == 1) targetBlockIdx ++;
	// 왼쪽
	else if (dir.Y == -1) targetBlockIdx--;

	// 만약에 갈 수 없는 Block 이라면 함수 나가자
	if (blocks[targetBlockIdx]->canMove == false) return;

	// targetBlockIdx 의 block  Cost 구하자.
	blocks[targetBlockIdx]->SetCost(blocks[currBlockIdx], blocks[goalBlockIdx]);

	// openArray 에 block 추가
	openArray.HeapPush(targetBlockIdx, FBlockCompare(blocks));

	blocks[targetBlockIdx]->ChangeColorOutline(FLinearColor::Blue);	
}

bool AAStarPlayer::CheckIndex(int32 nX, int32 nY)
{
	if (0 > nX || 10 <= nX) return false;
	if (0 > nY || 10 <= nY) return false;

	return true;
}

