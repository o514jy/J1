#include "J1SkillManager.h"
#include "J1SkillBase.h"
#include "J1NormalAttack.h"
#include "J1AuroraQ.h"
#include "J1PoisonRain.h"
#include "J1SpreadCloud.h"
#include "J1/Game/Gimmick/J1FindSafeZone.h"

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
	case 1010:
		skill = NewObject<UJ1NormalAttack>();
	case 1100:
		skill = NewObject<UJ1NormalAttack>();
		break;
	case 1101:
		skill = NewObject<UJ1PoisonRain>();
		break;
	case 1102:
		skill = NewObject<UJ1SpreadCloud>();
		break;
	default:
		break;
	}

	return skill;
}

TObjectPtr<UJ1GimmickBase> UJ1SkillManager::GenerateGimmickById(int32 InId)
{
	TObjectPtr<UJ1GimmickBase> gimmick = nullptr;
	switch (InId)
	{
	case 2000:
		gimmick = NewObject<UJ1FindSafeZone>();
		break;
	default:
		break;
	}

	return gimmick;
}
