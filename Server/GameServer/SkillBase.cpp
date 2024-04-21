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

		// todo : ��ų�� ����� �´��� �ƴ���(��������) üũ

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
	// ���� �߽ɰ� �簢���� ���� ����� �� ã��
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

	// ���� ���� ����� �� ���� �Ÿ� ���
	float distanceX = centerX - closestX;
	float distanceY = centerY - closestY;
	float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY));

	// �Ÿ��� ���������� �۰ų� ������ �浹
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
