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

vector<ObjectRef> SkillBase::GatherObjectInEffectArea(int32 effectId)
{
	vector<ObjectRef> objects;
	
	RoomBaseRef room = _owner->room.load().lock();
	for (auto& item : room->_objects)
	{
		ObjectRef object = item.second;

		// todo : 스킬의 대상이 맞는지 아닌지(적군인지) 체크

		EffectDataRef effectData = GDataManager->GetEffectDataById(effectId);
		wstring effectType = effectData->EffectType;
		if (effectType == L"Rectangle")
		{
			RectangleEffectDataRef rectData = static_pointer_cast<RectangleEffectData>(effectData);
			if (true == IsInRectangleArea(object, rectData->LeftUpPosY, rectData->LeftUpPosX, rectData->RightDownPosY, rectData->RightDownPosX))
			{
				objects.push_back(object);
			}
		}
		else if (effectType == L"Circle")
		{

		}
	}

	return objects;
}

bool SkillBase::IsInRectangleArea(ObjectRef object, float luY, float luX, float rdY, float rdX)
{
	// 원의 중심과 사각형의 가장 가까운 점 찾기
	float centerX = object->posInfo->x();
	float centerY = object->posInfo->y();

	luY += object->posInfo->y();
	luX += object->posInfo->x();
	rdY += object->posInfo->y();
	rdX += object->posInfo->x();

	// temp
	CreatureRef creature = static_pointer_cast<Creature>(object);
	CreatureDataRef data = creature->GetCreatureData();
	float radius = data->ColliderRadius;
	
	float closestX = max(luX, min(centerX, rdX));
	float closestY = max(rdY, min(centerY, luY));

	// 원과 가장 가까운 점 간의 거리 계산
	float distanceX = centerX - closestX;
	float distanceY = centerY - closestY;
	float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY));

	// 거리가 반지름보다 작거나 같으면 충돌
	if (distance <= radius) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool SkillBase::GetCanUseSkill()
{
	return _canUseSkill;
}

void SkillBase::SetCanUseSkill(bool flag)
{
	_canUseSkill = flag;
}
