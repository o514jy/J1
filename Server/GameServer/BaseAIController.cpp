#include "pch.h"
#include "BaseAIController.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "StartRoom.h"
#include "NavDevice.h"
#include "Navigation.h"
#include "Boss.h"

BaseAIController::BaseAIController()
{
	_owner = nullptr;
	_navDevice = nullptr;
	_distToTarget = 0.f;
}

BaseAIController::~BaseAIController()
{
	_owner = nullptr;
	_navDevice = nullptr;
}

void BaseAIController::SetInfo(ObjectRef owner)
{
	_owner = owner;

	InitNavDevice(_owner->GetRoomRef()->GetNav());
}

void BaseAIController::Clear()
{
	_owner = nullptr;

	_navDevice->SubAgentToCrowd();
	_navDevice = nullptr;
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

		// 서버에서는 바뀔 위치 미리 반영
		ownerMonster->GetPosInfo()->set_x(posInfo->dest_x());
		ownerMonster->GetPosInfo()->set_y(posInfo->dest_y());
		ownerMonster->GetPosInfo()->set_z(posInfo->dest_z());
	}

	RoomBaseRef room = _owner->room.load().lock();
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
	room->Broadcast(sendBuffer);
}

void BaseAIController::BroadcastNowPos()
{
	//cout << "monster pos [ " << _owner->posInfo->x() << ", " << _owner->posInfo->y() << ", " << _owner->posInfo->z() << " ]\n";
	if (MonsterRef mon = static_pointer_cast<Monster>(_owner))
	{
		if (mon->_targetObject != nullptr)
		{
			//cout << "target  pos [ " << mon->_targetObject->posInfo->x() << ", " << mon->_targetObject->posInfo->y() << ", " << mon->_targetObject->posInfo->z() << " ]\n";
		}
	}
	// 몬스터의 기본적인 상태 패킷을 보낸다.
	if (_owner == nullptr)
		return;

	Protocol::S_MOVE movePkt;
	{
		Protocol::PosInfo* posInfo = movePkt.mutable_info();
		posInfo->CopyFrom(*_owner->GetPosInfo());
	}

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
	_owner->GetRoomRef()->Broadcast(sendBuffer);
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
	MonsterRef _ownerMon = static_pointer_cast<Monster>(_owner);

	//float distToTarget = GetDistToTarget();
	FVector3 dirV = FVector3(
		_ownerMon->_targetObject->posInfo->x() - _ownerMon->posInfo->x(),
		_ownerMon->_targetObject->posInfo->y() - _ownerMon->posInfo->y(),
		_ownerMon->_targetObject->posInfo->z() - _ownerMon->posInfo->z()
	);
	float distToTarget = max(0, dirV.Magnitude() - _ownerMon->_targetObject->_colliderRadius);

	if (distToTarget <= attackRange)
	{
		// 공격 범위 이내로 들어왔다면 더이상 움직이지 않고
		StopMovement();

		// 공격 상태로 전환
		_owner->SetState(Protocol::MoveState::MOVE_STATE_SKILL);
	}
	else
	{
		// 공격 범위 밖이라면 추적
		if (BossRef boss = dynamic_pointer_cast<Boss>(_owner))
			BroadcastMove();
		else
			RegisterTargetPosToNavDevice();

		// 추적 범위 밖이면 idle로
		if (distToTarget > chaseRange)
		{
			_ownerMon->SetTargetObject(nullptr);
			_ownerMon->SetState(Protocol::MoveState::MOVE_STATE_IDLE);
		}
	}
}

void BaseAIController::InitNavDevice(NavigationRef sample)
{
	if (_owner == nullptr)
		return;

	if (_navDevice == nullptr)
		_navDevice = make_shared<NavDevice>();

	_navDevice->init(sample.get(), _owner);
}

void BaseAIController::RegisterTargetPosToNavDevice()
{
	MonsterRef mon = static_pointer_cast<Monster>(_owner);
	if (mon == nullptr)
		return;
	if (mon->_targetObject == nullptr)
		return;

	FVector3 dPos = Utils::Unreal2RecastPoint(*mon->_targetObject->GetPosInfo());

	if (_navDevice != nullptr)
		_navDevice->SetMoveTarget(dPos);
	else
		int aef = 3;
}

void BaseAIController::StopMovement()
{
	FVector3 dPos = Utils::Unreal2RecastPoint(*_owner->GetPosInfo());
	_navDevice->SetMoveTarget(dPos);
}
