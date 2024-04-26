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
	// ������ �⺻���� ���� ��Ŷ�� ������.
	Protocol::S_MOVE movePkt;
	{
		MonsterRef ownerMonster = static_pointer_cast<Monster>(_owner);
		ObjectRef target = ownerMonster->_targetObject;

		Protocol::PosInfo* posInfo = new Protocol::PosInfo();

		posInfo->set_object_id(ownerMonster->_objectId);

		// ������ ���� ��ġ
		posInfo->set_x(ownerMonster->posInfo->x());
		posInfo->set_y(ownerMonster->posInfo->y());
		posInfo->set_z(ownerMonster->posInfo->z());

		// ���Ͱ� �����ϴ� ��ġ (Ÿ���� ��ġ �������� �ӵ���ŭ)
		// ������ ���� ��ġ�� ���� �� �ֱ�
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
		// ���� ���� �̳��� ���Դٸ� ����
		_owner->SetState(Protocol::MoveState::MOVE_STATE_SKILL);
	}
	else
	{
		// ���� ���� ���̶�� ����
		if (distToTarget > _ownerMon->GetMonsterData()->SearchMaxDistance)
		{
			_ownerMon->SetTargetObject(nullptr);
			_ownerMon->SetState(Protocol::MoveState::MOVE_STATE_IDLE);
		}
	}
}
