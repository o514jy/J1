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
	// ���������� �̻��� ��Ȳ�̸� ����
	if (impactCount >= _projectileData->ImpactTimeList.size())
		return;

	// ���������� ����� �ϰ��� �����ϸ� �о��ֱ�
	//_animImpactJobs[timeCount] = nullptr;

	// ���� ���� �� �ش��ϴ� object���� ���� �ο�
	int32 effectId = _projectileData->EffectIdList[_impactCount];
	vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);
	ProcessBuff(objects);

}

void Projectile::OnDurationCompleteHandler()
{
	_impactCount = 0;

	// ������ ó���� ������ ������ϴ� ���
	if (_projectileData->ImpactTimeList.size() == 0)
	{
		int32 effectId = _projectileData->EffectIdList[_impactCount];
		vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);
		ProcessBuff(objects);
	}

	// �� �������� �Ҹ�
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

		// ��ų ������ ����
		if (object->_objectId == _owner->_objectId)
			continue;

		// ũ���� �ƴϸ� ����
		if (object->_objectType != Protocol::ObjectType::OBJECT_TYPE_CREATURE)
			continue;

		// �Ʊ� ����

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
	// buff ó��
	for (auto& object : objects)
	{
		// 1) attack ������ ����� buff ������ ���� ���� Ÿ�� �� ���ӽð� Ȯ��
		wstring buffDurationType = GDataManager->GetBuffDataById(_projectileData->BuffIdList[_impactCount])->BuffDurationType;

		// 2) ���� ���ӽð� Ÿ�Կ� �´� ���� ����
		BuffBaseRef buff = nullptr;
		if (buffDurationType == L"Instant")
		{
			buff = make_shared<BuffInstant>();
		}

		// 3) ���� �ʱ�ȭ �� ����
		buff->SetInfo(_projectileData->BuffIdList[_impactCount], object, _ownerSkill);

		// 4) ���� object���� buff �ο�
		buff->ApplyBuff();
	}
}

bool Projectile::IsInCircleArea(ObjectRef object, float effectRadius/*ȿ�� �Ÿ�*/)
{
	bool ret = false;

	float dist = Utils::DirectionVectorLen(posInfo, object->posInfo);

	CreatureRef creature = static_pointer_cast<Creature>(object);
	float radiusSum = creature->GetCreatureData()->ColliderRadius + effectRadius;

	// �߽ɰ��� �Ÿ��� �������� �պ��� ���� ���
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
