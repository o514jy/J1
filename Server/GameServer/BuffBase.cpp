#include "pch.h"
#include "BuffBase.h"
#include "DataManager.h"
#include "Object.h"

BuffBase::BuffBase()
{
	_owner = nullptr;
	_ownerSkill = nullptr;
	_buffData = nullptr;
}

BuffBase::~BuffBase()
{
	_owner = nullptr;
	_ownerSkill = nullptr;
	_buffData = nullptr;
}

void BuffBase::SetInfo(int32 templateId, ObjectRef owner, SkillBaseRef skill)
{
	_owner = owner;
	_ownerSkill = skill;
	
	_buffData = GDataManager->GetBuffDataById(templateId);


}

void BuffBase::ApplyBuff()
{
}

void BuffBase::ClearBuff()
{
}
