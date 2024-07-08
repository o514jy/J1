#include "J1Monster.h"

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

void AJ1Monster::SetInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	Super::SetInfo(InObjectInfo);


}
