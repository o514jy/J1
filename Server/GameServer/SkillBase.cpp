#include "pch.h"
#include "SkillBase.h"
#include "Creature.h"
#include "Projectile.h"
#include "Data.h"
#include "DataManager.h"
#include "ObjectManager.h"
#include "StartRoom.h"
#include "BuffInstant.h"
#include "SkillComponent.h"

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
	// 정상적으로 캔슬되지 않았다면 가지고 있던 job을 놓아주기
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

	RoomBaseRef roomRef = _owner->room.load().lock();
	if (roomRef != nullptr)
	{
		roomRef->DoTimer(_skillData->CoolTime, shared_from_this(), & SkillBase::SetCanUseSkill, true);
	}
	//DoTimer(_skillData->CoolTime, &SkillBase::SetCanUseSkill, true);

	// 활성화중인 스킬에서 해제
	_owner->GetSkillComponent()->SetActiveSkill(nullptr);

	// TODO : cc상태로 인한 캔슬이면 여기서 skill상태가 아닐 것이다. 그것에 대한 처리 필요
}

void SkillBase::OnAttackEvent(int32 timeCount)
{
	// 예외적으로 이상한 상황이면 종료
	if (timeCount >= _skillData->EffectIdList.size())
		return;

	// 정상적으로 예약된 일감을 실행하면 밀어주기
	_animImpactJobs[timeCount] = nullptr;
}

void SkillBase::DoSkill(const Protocol::C_SKILL& skillPkt, Protocol::S_SKILL& skillPktToSend)
{
	// skill's dataId
	skillPktToSend.set_skill_data_id(_skillData->DataId);

	// start skill
	_owner->SetState(Protocol::MoveState::MOVE_STATE_SKILL);

	_impactPos->CopyFrom(skillPkt.simple_pos_info());

	RoomBaseRef roomRef = _owner->room.load().lock();
	if (roomRef != nullptr)
	{
		vector<int32> impactList = _skillData->AnimImpactTimeList;
		for (int32 i = 0; i < impactList.size(); i++)
		{
			_animImpactJobs[i] = roomRef->DoTimer(impactList[i], shared_from_this(), &SkillBase::OnAnimImpactTimeHandler);
		}
	
		roomRef->DoTimer(_skillData->SkillDuration, shared_from_this(), &SkillBase::OnAnimCompleteHandler);
	}

	//vector<int32> impactList = _skillData->AnimImpactTimeList;
	//for (int32 i = 0; i < impactList.size(); i++)
	//{
	//	_animImpactJobs[i] = DoTimer(impactList[i], &SkillBase::OnAnimImpactTimeHandler);
	//}
	//
	//DoTimer(_skillData->SkillDuration, &SkillBase::OnAnimCompleteHandler);


	// start cooltime
	SetCanUseSkill(false);
	
	// 활성화중인 스킬로 설정
	_owner->GetSkillComponent()->SetActiveSkill(static_pointer_cast<SkillBase>(shared_from_this()));
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

ProjectileRef SkillBase::GenerateProjectile(int32 templateId, float spawnPosX, float spawnPosY, float spawnPosZ)
{
	ProjectileRef projectile = GObjectManager->CreateProjectile(
		templateId, 
		_owner, 
		static_pointer_cast<SkillBase>(shared_from_this()),
		nullptr,
		spawnPosX,
		spawnPosY,
		spawnPosZ
	);

	return projectile;
}

vector<ObjectRef> SkillBase::GatherObjectInEffectArea(int32 effectId)
{
	vector<ObjectRef> objects;
	
	RoomBaseRef room = _owner->room.load().lock();
	for (auto& item : room->_objects)
	{
		ObjectRef object = item.second;

		// 스킬 시전자 제외
		if (object->_objectId == _owner->_objectId)
			continue;

		// 크리쳐 아니면 제외
		if (object->_objectType != Protocol::ObjectType::OBJECT_TYPE_CREATURE)
			continue;

		// 아군 제외
		CreatureRef creature = static_pointer_cast<Creature>(object);
		CreatureRef ownerCreature = static_pointer_cast<Creature>(_owner);
		if (creature->GetCreatureData()->CreatureType == ownerCreature->GetCreatureData()->CreatureType)
			continue;

		EffectDataRef effectData = GDataManager->GetEffectDataById(effectId);
		wstring effectType = effectData->EffectType;
		if (effectType == L"Rectangle")
		{
			//RectangleEffectDataRef rectData = static_pointer_cast<RectangleEffectData>(effectData);
			//
			//// 스킬 사용 방향의 방향벡터 구하기
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
		else if (effectType == L"Circle")
		{
			CircleEffectDataRef circleData = static_pointer_cast<CircleEffectData>(effectData);

			if (true == IsInCircleArea(object, circleData->Radius))
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
	
	pair<float, float> interVNormalized = Utils::DirectionVectorNormalized(make_pair(_owner->posInfo->x(), _owner->posInfo->y()), make_pair(object->posInfo->x(), object->posInfo->y()));
	pair<float, float> interV = Utils::DirectionVector(make_pair(object->posInfo->x(), object->posInfo->y()), make_pair(_owner->posInfo->x(), _owner->posInfo->y()));

	float interVLen = sqrt(interV.first * interV.first + interV.second * interV.second);

	pair<float, float> dirV = Utils::DirectionVectorNormalized(make_pair(_owner->posInfo->x(), _owner->posInfo->y()), make_pair(_impactPos->x(), _impactPos->y()));

	// object와 시전자 사이의 거리가 radius 보다 작으면
	if (interVLen <= radius)
	{
		// 시전자와 object를 잇는 벡터와 시전자의 방향벡터를 내적
		float dot = interVNormalized.first * dirV.first + interVNormalized.second * dirV.second;
		// theta 구하기
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

bool SkillBase::IsInCircleArea(ObjectRef object, float radius)
{
	bool ret = false;

	float dist = Utils::DirectionVectorLen(_owner->posInfo, object->posInfo);

	CreatureRef creature = static_pointer_cast<Creature>(object);
	float radiusSum = creature->GetCreatureData()->ColliderRadius + radius;

	// 중심간의 거리가 반지름의 합보다 작은 경우
	if (dist < radiusSum)
	{
		ret = true;
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
