#include "pch.h"
#include "Projectile.h"
#include "DataManager.h"
#include "StartRoom.h"
#include "Creature.h"
#include "BuffInstant.h"
#include "GimmickBase.h"
#include "Boss.h"

Projectile::Projectile()
{
	_projectileData = nullptr;
	_owner = nullptr;
	_ownerSkill = nullptr;
	_ownerGimmick = nullptr;

	_impactCount = 0;
}

Projectile::~Projectile()
{
	_projectileData = nullptr;
	_owner = nullptr;
	_ownerSkill = nullptr;
	_ownerGimmick = nullptr;
}

void Projectile::UpdateTick()
{
	__super::UpdateTick();
}

void Projectile::SetInfo(CreatureRef owner, SkillBaseRef ownerSkill, int32 templateId)
{
	_owner = owner;
	_ownerSkill = ownerSkill;

	_objectId = objectInfo->object_id();
	_templateId = objectInfo->template_id();

	_projectileData = GDataManager->GetProjectileDataById(templateId);

	_impactCount = 0;
}

void Projectile::SetInfo(BossRef owner, GimmickBaseRef ownerGimmick, int32 templateId)
{
	_owner = owner;
	_ownerGimmick = ownerGimmick;

	_objectId = objectInfo->object_id();
	_templateId = objectInfo->template_id();

	_projectileData = GDataManager->GetProjectileDataById(templateId);

	_impactCount = 0;
}

void Projectile::Clear()
{
	__super::Clear();

	_projectileData = nullptr;
	_owner = nullptr;
	_ownerSkill = nullptr;
	_ownerGimmick = nullptr;
}

void Projectile::OnImpactTimeHandler()
{
	OnImpactEvent(_impactCount++);
}

void Projectile::OnImpactEvent(int32 impactCount)
{
	// 예외적으로 이상한 상황이면 종료
	if (impactCount >= _projectileData->ImpactTimeList.size())
		return;

	// 정상적으로 예약된 일감을 실행하면 밀어주기
	//_animImpactJobs[timeCount] = nullptr;

	// 영역 판정 후 해당하는 object에게 버프 부여
	int32 effectId = _projectileData->EffectIdList[_impactCount];
	vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);
	ProcessBuff(objects);

}

void Projectile::OnDurationCompleteHandler()
{
	_impactCount = 0;

	// 데미지 처리를 끝에서 해줘야하는 경우
	if (_projectileData->ImpactTimeList.size() == 0)
	{
		int32 effectId = _projectileData->EffectIdList[_impactCount];
		vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);
		ProcessBuff(objects);
	}

	// 다 끝났으니 소멸
	RoomBaseRef roomRef = this->room.load().lock();
	roomRef->RemoveObject(this->_objectId);
}

void Projectile::SpawnProjectile()
{
	_impactCount = 0;
	// do projectile
	for (int32 i = 0; i < _projectileData->ImpactTimeList.size(); i++)
	{
		DoTimer(_projectileData->ImpactTimeList[i], &Projectile::OnImpactTimeHandler);
	}

	DoTimer(_projectileData->Duration, &Projectile::OnDurationCompleteHandler);
	
}

vector<ObjectRef> Projectile::GatherObjectInEffectArea(int32 effectId)
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

		EffectDataRef effectData = GDataManager->GetEffectDataById(effectId);
		wstring effectType = effectData->EffectType;
		if (effectType == L"Rectangle")
		{

		}
		else if (effectType == L"Pizza")
		{
			
		}
		else if (effectType == L"Circle")
		{
			CircleEffectDataRef circleData = static_pointer_cast<CircleEffectData>(effectData);

			bool flag = IsInCircleArea(object, circleData->Radius);
			if (true == flag)
			{
				objects.push_back(object);
			}
		}
	}

	return objects;
}

void Projectile::ProcessBuff(vector<ObjectRef>& objects)
{
	// buff 처리
	for (auto& object : objects)
	{
		// 1) attack 시점에 사용할 buff 생성을 위해 버프 타입 및 지속시간 확인
		wstring buffDurationType = GDataManager->GetBuffDataById(_projectileData->BuffIdList[_impactCount])->BuffDurationType;

		// 2) 버프 지속시간 타입에 맞는 버프 생성
		BuffBaseRef buff = nullptr;
		if (buffDurationType == L"Instant")
		{
			buff = make_shared<BuffInstant>();
		}

		// 3) 버프 초기화 및 설정
		buff->SetInfo(_projectileData->BuffIdList[_impactCount], object, _ownerSkill);

		// 4) 들어온 object에게 buff 부여
		buff->ApplyBuff();
	}
}

bool Projectile::IsInCircleArea(ObjectRef object, float effectRadius/*효과 거리*/)
{
	bool ret = false;

	float dist = Utils::DirectionVectorLen(posInfo, object->posInfo);

	CreatureRef creature = static_pointer_cast<Creature>(object);
	float radiusSum = creature->GetCreatureData()->ColliderRadius + effectRadius;

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
