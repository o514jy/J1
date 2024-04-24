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

	_impactPos = new Protocol::SimplePosInfo();
}

SkillBase::~SkillBase()
{
	_owner = nullptr;
	_skillData = nullptr;

	delete _impactPos;

	// TODO : Disable AnimEvent

}

void SkillBase::SetInfo(CreatureRef owner, int32 templateId)
{
	_owner = owner;
	_skillData = GDataManager->GetSkillDataById(templateId);

	_animImpactJobs.assign(_skillData->AnimImpactTimeList.size(), nullptr);
}

void SkillBase::OnAnimImpactTimeHandler()
{
	// ���������� ĵ������ �ʾҴٸ� ������ �ִ� job�� �����ֱ�
	OnAttackEvent(_timeCount++);
}

void SkillBase::OnAnimCompleteHandler()
{
	// clear timeCount
	_timeCount = 0;

	// clear job
	for (int32 i = 0; i < _animImpactJobs.size(); i++)
		_animImpactJobs[i] = nullptr;

	if (_owner == nullptr)
		return;

	if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_SKILL)
		_owner->SetState(Protocol::MoveState::MOVE_STATE_IDLE);

	DoTimer(_skillData->CoolTime, &SkillBase::SetCanUseSkill, true);

	// TODO : cc���·� ���� ĵ���̸� ���⼭ skill���°� �ƴ� ���̴�. �װͿ� ���� ó�� �ʿ�
}

void SkillBase::OnAttackEvent(int32 timeCount)
{
	// ���������� ����� �ϰ��� �����ϸ� �о��ֱ�
	_animImpactJobs[timeCount] = nullptr;
}

void SkillBase::DoSkill(const Protocol::C_SKILL& skillPkt)
{
	// start skill
	_owner->SetState(Protocol::MoveState::MOVE_STATE_SKILL);

	_impactPos->CopyFrom(skillPkt.simple_pos_info());

	vector<int32> impactList = _skillData->AnimImpactTimeList;
	for (int32 i = 0; i < impactList.size(); i++)
	{
		_animImpactJobs[i] = DoTimer(impactList[i], &SkillBase::OnAnimImpactTimeHandler);
	}

	DoTimer(_skillData->SkillDuration, &SkillBase::OnAnimCompleteHandler);

	// start cooltime
	SetCanUseSkill(false);
	
}

void SkillBase::CancledSkill()
{
	// destroy reserved event
	for (auto& job : _animImpactJobs)
	{
		if (job != nullptr)
		{
			job->SetCancled(true);
		}
	}

	// force process OnAnimCompleteHandler
	OnAnimCompleteHandler();
}

vector<ObjectRef> SkillBase::GatherObjectInEffectArea(int32 effectId)
{
	vector<ObjectRef> objects;
	
	RoomBaseRef room = _owner->room.load().lock();
	for (auto& item : room->_objects)
	{
		ObjectRef object = item.second;

		// ��ų ������ ����
		if (object->_objectId == _owner->_objectId)
			continue;

		// �Ʊ� ����

		EffectDataRef effectData = GDataManager->GetEffectDataById(effectId);
		wstring effectType = effectData->EffectType;
		if (effectType == L"Rectangle")
		{
			//RectangleEffectDataRef rectData = static_pointer_cast<RectangleEffectData>(effectData);
			//
			//// ��ų ��� ������ ���⺤�� ���ϱ�
			//pair<float, float> dirV = findNormalizedDirectionVector(
			//	make_pair(_owner->posInfo->x(), _owner->posInfo->y()), 
			//	make_pair(_impactPos->x(), _impactPos->y()));
			//
			//if (true == IsInRectangleArea(object, dirV, rectData->ForwardLength, rectData->BackwardLength, rectData->LeftLength, rectData->RightLength))
			//{
			//	objects.push_back(object);
			//}
		}
		else if (effectType == L"Pizza")
		{
			PizzaEffectDataRef pizzaData = static_pointer_cast<PizzaEffectData>(effectData);
			
			if (true == IsInPizzaArea(object, pizzaData->Radius, pizzaData->Theta))
			{
				objects.push_back(object);
			}
		}
	}

	return objects;
}

bool SkillBase::IsInPizzaArea(ObjectRef object, float radius, float theta)
{
	bool ret = false;
	
	pair<float, float> interVNormalized = Utils::DirectionVectorNormalized(make_pair(object->posInfo->x(), object->posInfo->y()), make_pair(_owner->posInfo->x(), _owner->posInfo->y()));
	pair<float, float> interV = Utils::DirectionVector(make_pair(object->posInfo->x(), object->posInfo->y()), make_pair(_owner->posInfo->x(), _owner->posInfo->y()));

	float interVLen = sqrt(interV.first * interV.first + interV.second * interV.second);

	pair<float, float> dirV = Utils::DirectionVectorNormalized(make_pair(_owner->posInfo->x(), _owner->posInfo->y()), make_pair(_impactPos->x(), _impactPos->y()));

	// object�� ������ ������ �Ÿ��� radius ���� ������
	if (interVLen <= radius)
	{
		// object�� �����ڸ� �մ� ���Ϳ� �������� ���⺤�͸� ����
		float dot = interVNormalized.first * dirV.first + interVNormalized.second * dirV.second;
		// theta ���ϱ�
		float tempTheta = acos(dot);
		float degree = tempTheta * (180.0 / (float)3.141592653589793238463);

		if (degree <= theta)
			ret = true;
		else
			ret = false;
	}
	else
	{
		ret = false;
	}

	return ret;
}

bool SkillBase::GetCanUseSkill()
{
	return _canUseSkill;
}

void SkillBase::SetCanUseSkill(bool flag)
{
	_canUseSkill = flag;
}
