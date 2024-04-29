#include "pch.h"
#include "Projectile.h"
#include "DataManager.h"
#include "StartRoom.h"
#include "Creature.h"

Projectile::Projectile()
{
	_projectileData = nullptr;
	_owner = nullptr;
	_ownerSkill = nullptr;

	_impactCount = 0;
}

Projectile::~Projectile()
{
	_projectileData = nullptr;
	_owner = nullptr;
	_ownerSkill = nullptr;
}

void Projectile::UpdateTick()
{
	__super::UpdateTick();
}

void Projectile::SetInfo(CreatureRef owner, SkillBaseRef ownerSkill, int32 templateId, float spawnPosX, float spawnPosY, float spawnPosZ)
{
	_owner = owner;
	_ownerSkill = ownerSkill;

	_objectId = objectInfo->object_id();
	_templateId = objectInfo->template_id();

	_projectileData = GDataManager->GetProjectileDataById(templateId);

	_impactCount = 0;
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
}

void Projectile::OnDurationCompleteHandler()
{
	_impactCount = 0;


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
