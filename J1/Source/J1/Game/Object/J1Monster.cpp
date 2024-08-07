#include "J1Monster.h"
#include "Controllers/J1MonsterController.h"

AJ1Monster::AJ1Monster()
{
	/** Ai Controller **/
	{
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
		AIControllerClass = AJ1MonsterController::StaticClass();
	}
}

AJ1Monster::~AJ1Monster()
{
}

void AJ1Monster::SetTargetPlayer(TObjectPtr<AJ1Player> InPlayer)
{
	if (InPlayer == nullptr)
	{
		SetMoveState(Protocol::MoveState::MOVE_STATE_IDLE);
	}
	else
	{
		SetMoveState(Protocol::MoveState::MOVE_STATE_RUN);
	}

	TargetPlayer = InPlayer;
}

void AJ1Monster::SetTempSyncPos(FVector InSyncPos)
{
	SetbSetSyncPos(true);
	TempSyncPos = InSyncPos;
}

FVector AJ1Monster::GetTempSyncPos()
{
	return TempSyncPos;
}

void AJ1Monster::SetbSetSyncPos(bool InbSyncPos)
{
	bSetSyncPos = InbSyncPos;
}

void AJ1Monster::SetInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	Super::SetInfo(InObjectInfo);

	
}
