#include "pch.h"
#include "Object.h"
#include "DataManager.h"
#include "Data.h"
#include "SkillBase.h"

Object::Object()
{
	objectType = make_shared<Protocol::ObjectType>();
	objectInfo = make_shared<Protocol::ObjectInfo>();
	posInfo = make_shared<Protocol::PosInfo>();
	objectInfo->set_allocated_pos_info(posInfo.get());
	

	_statComponent = nullptr;

	_objectId = 0;
	_templateId = 0;
}

Object::~Object()
{
	objectType = nullptr;
	objectInfo = nullptr;
	posInfo = nullptr;

	_statComponent = nullptr;
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

void Object::OnDamaged(ObjectRef attacker, SkillBaseRef skill)
{
	if (attacker == nullptr)
		return;
}
