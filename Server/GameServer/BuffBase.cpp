#include "pch.h"
#include "BuffBase.h"
#include "SkillBase.h"
#include "DataManager.h"
#include "Object.h"
#include "Creature.h"

BuffBase::BuffBase()
{
	_owner = nullptr;
	_ownerSkill = nullptr;
	_buffData = nullptr;
	
	_finalAmount = 0;

	_buffInfo = new Protocol::BuffInfo();
}

BuffBase::~BuffBase()
{
	_owner = nullptr;
	_ownerSkill = nullptr;

	delete _buffInfo;
}

void BuffBase::SetInfo(int32 templateId, ObjectRef owner, SkillBaseRef skill)
{
	_owner = owner;
	_ownerSkill = skill;
	
	_buffData = GDataManager->GetBuffDataById(templateId);

	/* buffInfo */
	_buffInfo->set_template_id(templateId);
	_buffInfo->set_giver_object_id(_owner->_objectId);
	_buffInfo->set_giver_object_id(skill->_owner->_objectId);

	// temp : calculate final amount
	_finalAmount = skill->_owner->GetCreatureData()->Atk * _buffData->BuffAmountRate;
}

float BuffBase::GetFinalAmount()
{
	return _finalAmount;
}

void BuffBase::ApplyBuff()
{
}

void BuffBase::ClearBuff()
{
}
