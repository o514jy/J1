#include "pch.h"
#include "BuffBase.h"
#include "SkillBase.h"
#include "DataManager.h"
#include "Object.h"
#include "Creature.h"
#include "BuffPolicy.h"
#include "Data.h"

BuffBase::BuffBase()
{
	_buffId = 0;
	_templateId = 0;
	_buffType = Protocol::BuffType::BUFF_TYPE_NONE;

	_owner = nullptr;
	_caster = nullptr;
	_buffData = nullptr;

	_policy = nullptr;

	_despawnTick = 0;
	
	_finalAmount = 0;

	_buffInfo = new Protocol::BuffInfo();
}

BuffBase::~BuffBase()
{
	_owner = nullptr;

	delete _buffInfo;
}

void BuffBase::SetInfo(uint64 buffId, int32 templateId, CreatureRef target, CreatureRef caster, BuffPolicyRef policy)
{
	_buffId = buffId;
	_templateId = templateId;
	_owner = target;
	_caster = caster;
	_buffData = GDataManager->GetBuffDataById(templateId);
	_buffType = _buffData->BuffType;
	
	if (_buffData->BuffDurationType == Protocol::BuffDurationType::BUFF_DURATION_TYPE_DURATION)
		_despawnTick = ::GetTickCount64() + _buffData->BuffDurationMagnitude;
}

float BuffBase::GetFinalAmount()
{
	return _finalAmount;
}

void BuffBase::ApplyBuff()
{
	if (_policy == nullptr)
		return;

	_policy->ApplyBuff(_caster, _owner, _buffData);
}

void BuffBase::RevertBuff()
{
	if (_policy == nullptr)
		return;

	_policy->RevertBuff(_owner, _buffData);
}

uint64 BuffBase::GetRemainingLifetimeInTicks()
{
	return max(0, (_despawnTick - GetTickCount64()));
}

float BuffBase::GetRemainingLifetimeInSeconds()
{
	return GetRemainingLifetimeInTicks() / 1000.0f;
}
