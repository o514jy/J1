#include "pch.h"
#include "SkillManager.h"
#include "NormalAttack.h"
#include "AuroraQ.h"
#include "PoisonRain.h"

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
	default:
		break;
	}

	return skill;
}
