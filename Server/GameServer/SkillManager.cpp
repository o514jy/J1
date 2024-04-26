#include "pch.h"
#include "SkillManager.h"
#include "NormalAttack.h"

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
	case 1100:
		skill = make_shared<NormalAttack>();
		break;
	default:
		break;
	}

	return skill;
}
