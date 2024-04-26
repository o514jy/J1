#include "pch.h"
#include "BaseAIController.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "StartRoom.h"

BaseAIController::BaseAIController()
{
	_owner = nullptr;
}

BaseAIController::~BaseAIController()
{
	_owner = nullptr;
}

void BaseAIController::SetInfo(ObjectRef owner)
{
	_owner = owner;


}

void BaseAIController::SetDistToTarget(float dist)
{
	_distToTarget = dist;
}

float BaseAIController::GetDistToTarget()
{
	pair<float, float> ownerPos = make_pair(_owner->GetPosInfo()->x(), _owner->GetPosInfo()->y());
	
	MonsterRef ownerMon = static_pointer_cast<Monster>(_owner);
	pair<float, float> targetPos = make_pair(ownerMon->_targetObject->GetPosInfo()->x(), ownerMon->_targetObject->GetPosInfo()->y());
	
	return Utils::DirectionVectorLen(ownerPos, targetPos);
}

void BaseAIController::BroadcastMove()
{
	// 몬스터의 기본적인 상태 패킷을 보낸다.
	Protocol::S_MOVE movePkt;
	{
		MonsterRef ownerMonster = static_pointer_cast<Monster>(_owner);
		ObjectRef target = ownerMonster->_targetObject;

		Protocol::PosInfo* posInfo = new Protocol::PosInfo();

		posInfo->set_object_id(ownerMonster->_objectId);

		// 몬스터의 현재 위치
		posInfo->set_x(ownerMonster->posInfo->x());
		posInfo->set_y(ownerMonster->posInfo->y());
		posInfo->set_z(ownerMonster->posInfo->z());

		// 몬스터가 가야하는 위치 (타겟의 위치 방향으로 속도만큼)
		// 없으면 현재 위치로 같은 값 넣기
		if (target == nullptr)
		{
			posInfo->set_dest_x(ownerMonster->posInfo->x());
			posInfo->set_dest_y(ownerMonster->posInfo->y());
			posInfo->set_dest_z(ownerMonster->posInfo->z());
		}
		else
		{
			pair<float, float> nextPos;
			nextPos = Utils::FindNextTickPos2dToGo(posInfo, target->GetPosInfo(), ownerMonster->GetMonsterData()->MaxWalkSpeed);
			
			posInfo->set_dest_x(nextPos.first);
			posInfo->set_dest_y(nextPos.second);
			posInfo->set_dest_z(ownerMonster->posInfo->z());
		}
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

void BaseAIController::ChaseOrAttackTarget(float chaseRange, float attackRange)
{
	float distToTarget = GetDistToTarget();

	MonsterRef _ownerMon = static_pointer_cast<Monster>(_owner);

	if (distToTarget <= attackRange)
	{
		// 공격 범위 이내로 들어왔다면 공격
		_owner->SetState(Protocol::MoveState::MOVE_STATE_SKILL);
	}
	else
	{
		// 공격 범위 밖이라면 추적
		if (distToTarget > _ownerMon->GetMonsterData()->SearchMaxDistance)
		{
			_ownerMon->SetTargetObject(nullptr);
			_ownerMon->SetState(Protocol::MoveState::MOVE_STATE_IDLE);
		}
	}
}
