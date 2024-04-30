#include "J1SkillManager.h"
#include "J1SkillBase.h"
#include "J1NormalAttack.h"
#include "J1AuroraQ.h"
#include "J1PoisonRain.h"

TObjectPtr<UJ1SkillBase> UJ1SkillManager::GenerateSkillById(int32 InId)
{
	TObjectPtr<UJ1SkillBase> skill = nullptr;
	switch (InId)
	{
	case 1000:
		skill = NewObject<UJ1NormalAttack>();
		break;
	case 1001:
		skill = NewObject<UJ1AuroraQ>();
		break;
	case 1100:
		skill = NewObject<UJ1NormalAttack>();
		break;
	case 1101:
		skill = NewObject<UJ1PoisonRain>();
		break;
	default:
		break;
	}

	return skill;
}
