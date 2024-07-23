#include "pch.h"
#include "Monster.h"
#include "RoomBase.h"
#include "MonsterAIController.h"

Monster::Monster()
{
	objectInfo->set_creature_type(Protocol::CreatureType::CREATURE_TYPE_MONSTER);
	objectInfo->set_monster_type(Protocol::MonsterType::MONSTER_TYPE_GENERAL);
	
	_skillComponent = nullptr;
	_aiController = nullptr;
	_targetObject = nullptr;
}

Monster::~Monster()
{
	_skillComponent = nullptr;
	_aiController = nullptr;
	_targetObject = nullptr;
}

void Monster::UpdateTick()
{
	__super::UpdateTick();

	if (_aiController != nullptr)
		_aiController->UpdateTick();
}

void Monster::SetInfo(int32 templateId)
{
	__super::SetInfo(templateId);

	// skill component
	_aiController = make_shared<MonsterAIController>();
	_aiController->SetInfo(static_pointer_cast<Object>(shared_from_this()));
}

void Monster::SetTargetObject(ObjectRef object)
{
	//if (_targetObject == nullptr || _targetObject != object)
	{
		// 타겟이 바뀌었다는 패킷을 보낸다.
		Protocol::S_CHANGE_TARGET pkt;
		{
			Protocol::ObjectInfo* info = pkt.mutable_info();
			info->CopyFrom(*objectInfo);

			if (object == nullptr)
				pkt.set_target_object_id(0);
			else
				pkt.set_target_object_id(object->_objectId);
		}
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GetRoomRef()->Broadcast(sendBuffer);
	}
	_targetObject = object;
}

void Monster::SetState(Protocol::MoveState moveState)
{
	__super::SetState(moveState);
}

void Monster::Clear()
{
	__super::Clear();

	_aiController->Clear();
	_aiController = nullptr;

	_targetObject = nullptr;
}
