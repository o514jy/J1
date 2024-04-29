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
	// 예외적으로 이상한 상황이면 종료
	if (impactCount >= _projectileData->ImpactTimeList.size())
		return;

	// 정상적으로 예약된 일감을 실행하면 밀어주기
	//_animImpactJobs[timeCount] = nullptr;

	// 영역 판정 후 해당하는 object에게 버프 부여
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
