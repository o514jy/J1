#include "pch.h"
#include "Object.h"
#include "DataManager.h"
#include "Data.h"
#include "SkillBase.h"
#include "BuffBase.h"
#include "StatComponent.h"

Object::Object()
{
	objectInfo = new Protocol::ObjectInfo();
	posInfo = new Protocol::PosInfo();
	objectInfo->set_allocated_pos_info(posInfo);

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
}

Protocol::MoveState Object::GetState()
{
	return posInfo->state();
}

void Object::SetPosInfo(const Protocol::PosInfo& InPosInfo)
{
	posInfo->CopyFrom(InPosInfo);
}

Protocol::PosInfo* Object::GetPosInfo()
{
	return posInfo;
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
