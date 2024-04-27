#include "pch.h"
#include "NormalAttack.h"
#include "Data.h"
#include "Creature.h"
#include "BuffInstant.h"

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
	__super::OnAttackEvent(timeCount);

	cout << _owner->_objectId << "'s NormalAttack Skill OnAttackEvent " << timeCount << "\n";

	//// 스킬의 이펙트 범위 안에 들어왔는지 확인
	//int32 effectId = _skillData->EffectIdList[timeCount];
	//vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);
	//
	//// buff 처리
	//for (auto& object : objects)
	//{
	//	// 1) attack 시점에 사용할 buff 생성 
	//	BuffBaseRef buff = make_shared<BuffInstant>();
	//	buff->SetInfo(_skillData->BuffIdList[timeCount], object, static_pointer_cast<SkillBase>(shared_from_this()));
	//
	//	// 2) 들어온 object에게 buff 부여
	//	buff->ApplyBuff();
	//}
}

void NormalAttack::DoSkill(const Protocol::C_SKILL& skillPkt)
{
	__super::DoSkill(skillPkt);

	// 애니메이션 재생 (클라에서)

	// 공격 시점에 이벤트 발생

	// 끝날 때 이벤트 발생
}
