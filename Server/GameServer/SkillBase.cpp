#include "pch.h"
#include "SkillBase.h"
#include "Creature.h"
#include "Data.h"
#include "DataManager.h"
#include "StartRoom.h"

SkillBase::SkillBase()
{
	_owner = nullptr;
	_skillData = nullptr;
	_timeCount = 0;
	_canUseSkill = true;
}

SkillBase::~SkillBase()
{
	_owner = nullptr;
	_skillData = nullptr;

	// TODO : Disable AnimEvent
}

void SkillBase::SetInfo(CreatureRef owner, int32 templateId)
{
	_owner = owner;
	_skillData = GDataManager->GetSkillDataById(templateId);
}

void SkillBase::OnAnimImpactTimeHandler()
{
	OnAttackEvent(_timeCount++);
}

void SkillBase::OnAnimCompleteHandler()
{
	_timeCount = 0;

	if (_owner == nullptr)
		return;

	if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_SKILL)
		_owner->SetState(Protocol::MoveState::MOVE_STATE_IDLE);

	// start cooltime
	DoTimer(_skillData->CoolTime, &SkillBase::SetCanUseSkill, true);
}

void SkillBase::OnAttackEvent(int32 timeCount)
{
}

void SkillBase::DoSkill()
{
	// start skill
	_owner->SetState(Protocol::MoveState::MOVE_STATE_SKILL);

	for (float time : _skillData->AnimImpactTimeList)
	{
		DoTimer(time, &SkillBase::OnAnimImpactTimeHandler);
	}

	DoTimer(_skillData->SkillDuration, &SkillBase::OnAnimCompleteHandler);

	// start cooltime
	SetCanUseSkill(false);
}

bool SkillBase::GetCanUseSkill()
{
	return _canUseSkill;
}

void SkillBase::SetCanUseSkill(bool flag)
{
	_canUseSkill = flag;
}
