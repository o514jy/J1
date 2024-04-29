#include "pch.h"
#include "PoisonRain.h"
#include "Data.h"
#include "Creature.h"
#include "DataManager.h"
#include "Projectile.h"
#include "StartRoom.h"
#include "BuffInstant.h"
#include "ObjectManager.h"

PoisonRain::PoisonRain()
{
	_projectileCount = 0;
}

PoisonRain::~PoisonRain()
{
}

void PoisonRain::SetInfo(CreatureRef owner, int32 templateId)
{
	__super::SetInfo(owner, templateId);

	int32 vSize = static_cast<int32>(_skillData->AnimImpactTimeList.size());
	randomPoses.assign(vSize, make_pair(0.f, 0.f));
}

void PoisonRain::OnAttackEvent(int32 timeCount)
{
	__super::OnAttackEvent(timeCount);

	float nx = randomPoses[timeCount].first;
	float ny = randomPoses[timeCount].second;
	float nz = _owner->GetPosInfo()->z();

	int32 projId = _skillData->ProjectileIdList[timeCount];
	// spawn projectile
	ProjectileRef projectile = GenerateProjectile(projId, nx, ny, nz);
	projectile->SpawnProjectile();
}

void PoisonRain::DoSkill(const Protocol::C_SKILL& skillPkt)
{
	__super::DoSkill(skillPkt);

	for (int32 i = 0; i < randomPoses.size(); i++)
	{
		float nx = _owner->GetPosInfo()->x() + Utils::GetRandom<float>(-800.f, 800.f);
		float ny = _owner->GetPosInfo()->y() + Utils::GetRandom<float>(-800.f, 800.f);

		randomPoses[i] = make_pair(nx, ny);
	}
}
