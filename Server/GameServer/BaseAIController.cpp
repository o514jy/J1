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

		// ���������� �ٲ� ��ġ �̸� �ݿ�
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
	// ������ �⺻���� ���� ��Ŷ�� ������.
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
		// ���� ���� �̳��� ���Դٸ� ���̻� �������� �ʰ�
		StopMovement();

		// ���� ���·� ��ȯ
		_owner->SetState(Protocol::MoveState::MOVE_STATE_SKILL);
	}
	else
	{
		// ���� ���� ���̶�� ����
		if (BossRef boss = dynamic_pointer_cast<Boss>(_owner))
			BroadcastMove();
		else
			RegisterTargetPosToNavDevice();

		// ���� ���� ���̸� idle��
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
