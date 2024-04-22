#include "pch.h"
#include "Object.h"
#include "DataManager.h"
#include "Data.h"
#include "SkillBase.h"
#include "StatComponent.h"

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

void Object::OnDamaged(ObjectRef attacker, BuffBaseRef buff)
{
	if (attacker == nullptr)
		return;

	StatComponentRef stats = attacker->GetStatComponent();
	// todo : calculate final damage
	float finalDamage = max(0, stats->GetAtk() - stats->GetDef());
	// calculate final hp
	float finalHp = max(0, stats->GetHp() - finalDamage);

	stats->SetHp(finalHp); // 0일때 사망처리는 sethp 쪽에서 해준다.
}
