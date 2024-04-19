#include "pch.h"
#include "NormalAttack.h"

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

}

void NormalAttack::DoSkill()
{
	__super::DoSkill();

	// 애니메이션 재생 (클라에서)

	// 공격 시점에 이벤트 발생

	// 끝날 때 이벤트 발생
}
