#include "pch.h"
#include "Object.h"
#include "DataManager.h"
#include "Data.h"
#include "SkillBase.h"
#include "BuffBase.h"
#include "StatComponent.h"
#include "RoomBase.h"

Object::Object()
{
	objectInfo = new Protocol::ObjectInfo();
	posInfo = new Protocol::PosInfo();
	objectInfo->set_allocated_pos_info(posInfo);
	_objectType = Protocol::ObjectType::OBJECT_TYPE_NONE;

	_statComponent = nullptr;

	_objectId = 0;
	_templateId = 0;
}

Object::~Object()
{
	delete objectInfo;

	_statComponent = nullptr;
}

void Object::UpdateTick()
{
	if (_statComponent != nullptr)
		_statComponent->UpdateTick();
}

void Object::SetInfo(int32 templateId)
{
	
}

void Object::SetState(Protocol::MoveState moveState)
{
	posInfo->set_state(moveState);

	//string s;
	//if (moveState == Protocol::MOVE_STATE_IDLE)
	//	s = "Idle";
	//else if (moveState == Protocol::MOVE_STATE_RUN)
	//	s = "Run";
	//else if (moveState == Protocol::MOVE_STATE_SKILL)
	//	s = "Skill";
	//else if (moveState == Protocol::MOVE_STATE_GIMMICK)
	//	s = "Gimmick";
	//else if (moveState == Protocol::MOVE_STATE_DEAD)
	//{
	//	s = "Dead";
	//
	//	Protocol::S_MOVE movePkt;
	//	Protocol::PosInfo* info = movePkt.mutable_info();
	//	info->CopyFrom(*posInfo);
	//
	//	RoomBaseRef roomRef = room.load().lock();
	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
	//	roomRef->Broadcast(sendBuffer);
	//}
	//else
	//	s = "None";
	//
	//cout << _objectId << "'s State Changed : " << s << "\n";

	
}

Protocol::MoveState Object::GetState()
{
	return posInfo->state();
}

void Object::SetPosInfo(Protocol::PosInfo InPosInfo)
{
	posInfo->CopyFrom(InPosInfo);
}

Protocol::PosInfo* Object::GetPosInfo()
{
	return posInfo;
}

void Object::SetAgentIdx(int32 idx)
{
	_agentIdx = idx;
}

RoomBaseRef Object::GetRoomRef()
{
	RoomBaseRef roomRef = room.load().lock();
	return roomRef;
}

void Object::Clear()
{
	_statComponent = nullptr;
}

void Object::OnDamaged(ObjectRef attacker, BuffBaseRef buff)
{
	if (attacker == nullptr)
		return;

	
}
