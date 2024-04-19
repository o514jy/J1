#include "J1SkillManager.h"
#include "J1SkillBase.h"
#include "J1NormalAttack.h"

TObjectPtr<UJ1SkillBase> UJ1SkillManager::GenerateSkillById(int32 InId)
{
	TObjectPtr<UJ1SkillBase> skill = nullptr;
	switch (InId)
	{
	case 1000:
		skill = NewObject<UJ1NormalAttack>();
		break;
	default:
		break;
	}

	return skill;
}
