#include "pch.h"
#include "BaseAIController.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "StartRoom.h"

BaseAIController::BaseAIController()
{
}

BaseAIController::~BaseAIController()
{
}

void BaseAIController::SetInfo(ObjectRef owner)
{
	_owner = owner;


}

void BaseAIController::BroadcastMove()
{
	// 몬스터의 기본적인 상태 패킷을 보낸다.
	Protocol::S_MOVE movePkt;
	{
		MonsterRef ownerMonster = static_pointer_cast<Monster>(_owner);
		ObjectRef target = ownerMonster->_targetObject;

		Protocol::PosInfo* posInfo = new Protocol::PosInfo();

		// 몬스터의 현재 위치
		posInfo->set_x(ownerMonster->posInfo->x());
		posInfo->set_y(ownerMonster->posInfo->y());
		posInfo->set_z(ownerMonster->posInfo->z());

		// 몬스터가 가야하는 위치 (타겟의 위치)
		posInfo->set_dest_x(target->posInfo->x());
		posInfo->set_dest_y(target->posInfo->y());
		posInfo->set_dest_z(target->posInfo->z());

		movePkt.set_allocated_info(posInfo);
	}

	RoomBaseRef room = _owner->room.load().lock();
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
	room->Broadcast(sendBuffer);
}

void BaseAIController::UpdateTick()
{
	if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_IDLE)
	{
		UpdateIdle();
	}
	else if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_RUN)
	{
		UpdateRun();
	}
	else if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_SKILL)
	{
		UpdateSkill();
	}
	else if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_DEAD)
	{
		UpdateDead();
	}
}

void BaseAIController::UpdateIdle()
{
}

void BaseAIController::UpdateRun()
{
}

void BaseAIController::UpdateSkill()
{
}

void BaseAIController::UpdateDead()
{
}
