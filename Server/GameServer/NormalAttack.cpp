#include "pch.h"
#include "NormalAttack.h"
#include "Data.h"
#include "Creature.h"

NormalAttack::NormalAttack()
{
}

NormalAttack::~NormalAttack()
{
}

void NormalAttack::SetInfo(CreatureRef owner, int32 templateId)
{
	__super::SetInfo(owner, templateId);


}

void NormalAttack::OnAttackEvent(int32 timeCount)
{
	// max count 1
	if (timeCount >= _skillData->EffectIdList.size())
		return;

	int32 effectId = _skillData->EffectIdList[timeCount];
	vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);

	for (auto& object : objects)
	{
		object->OnDamaged(object, static_pointer_cast<SkillBase>(shared_from_this()));
	}
}

void NormalAttack::DoSkill()
{
	__super::DoSkill();

	// 애니메이션 재생 (클라에서)

	// 공격 시점에 이벤트 발생

	// 끝날 때 이벤트 발생
}
