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

	// �ִϸ��̼� ��� (Ŭ�󿡼�)

	// ���� ������ �̺�Ʈ �߻�

	// ���� �� �̺�Ʈ �߻�
}
