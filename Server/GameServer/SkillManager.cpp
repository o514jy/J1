#include "pch.h"
#include "SkillManager.h"
#include "NormalAttack.h"
#include "AuroraQ.h"
#include "PoisonRain.h"
#include "SpreadCloud.h"
#include "Projectile.h"
#include "SafeZone.h"

SkillManager::SkillManager()
{
}

SkillManager::~SkillManager()
{
}

SkillBaseRef SkillManager::GenerateSkillById(int32 id)
{
	SkillBaseRef skill = nullptr;
	switch (id)
	{
	case 1000:
		skill = make_shared<NormalAttack>();
		break;
	case 1001:
		skill = make_shared<AuroraQ>();
		break;
	case 1100:
		skill = make_shared<NormalAttack>();
		break;
	case 1101:
		skill = make_shared<PoisonRain>();
		break;
	case 1102:
		skill = make_shared<SpreadCloud>();
		break;
	default:
		break;
	}

	return skill;
}

ProjectileRef SkillManager::GenerateProjectileById(int32 id)
{
	ProjectileRef projectile = nullptr;
	switch (id)
	{
	case 0:
		projectile = make_shared<Projectile>();
		break;
	case 1000001:
	case 1000002:
	case 1000003:
	case 1000004:
	case 1000005:
	case 1000006:
	case 1000007:
	case 1000008:
		projectile = make_shared<Projectile>();
		break;
	case 1000009:
		projectile = make_shared<SafeZone>();
		break;
	default:
		break;
	}

	return projectile;
}
