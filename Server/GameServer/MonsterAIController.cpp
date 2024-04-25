#include "pch.h"
#include "MonsterAIController.h"
#include "Player.h"
#include "Monster.h"
#include "StartRoom.h"

MonsterAIController::MonsterAIController()
{
}

MonsterAIController::~MonsterAIController()
{
}

void MonsterAIController::SetInfo(ObjectRef owner)
{
	__super::SetInfo(owner);
}

void MonsterAIController::UpdateTick()
{
	__super::UpdateTick();
}

void MonsterAIController::UpdateIdle()
{
	__super::UpdateIdle();

	RoomBaseRef room = _owner->room.load().lock();
	PlayerRef player = room->FindClosestPlayer(_owner);

	if (player != nullptr)
	{
		_owner->SetState(Protocol::MoveState::MOVE_STATE_RUN);
	}
}

void MonsterAIController::UpdateRun()
{
	__super::UpdateRun();

	MonsterRef _ownerMonster = static_pointer_cast<Monster>(_owner);
	PlayerRef targetPlayer = static_pointer_cast<Player>(_ownerMonster->_targetObject);

	if (targetPlayer == nullptr || targetPlayer->room.load().lock() == nullptr)
	{
		_ownerMonster->SetTargetObject(nullptr);
		_ownerMonster->SetState(Protocol::MoveState::MOVE_STATE_IDLE);
		BroadcastMove();
		return;
	}


}

void MonsterAIController::UpdateSkill()
{
	__super::UpdateSkill();
}

void MonsterAIController::UpdateDead()
{
	__super::UpdateDead();
}
