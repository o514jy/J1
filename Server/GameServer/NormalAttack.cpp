#include "pch.h"
#include "NormalAttack.h"
#include "Data.h"
#include "Creature.h"
#include "BuffBase.h"

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

	// ��ų�� ����Ʈ ���� �ȿ� ���Դ��� Ȯ��
	int32 effectId = _skillData->EffectIdList[timeCount];
	vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);

	// buff ó��
	for (auto& object : objects)
	{
		// 1) attack ������ ����� buff ���� 
		BuffBaseRef buff = make_shared<BuffBase>();
		buff->SetInfo(_skillData->BuffIdList[timeCount], object, static_pointer_cast<SkillBase>(shared_from_this()));

		// 2) ���� object���� buff �ο�
		buff->ApplyBuff();
	}
}

void NormalAttack::DoSkill()
{
	__super::DoSkill();

	// �ִϸ��̼� ��� (Ŭ�󿡼�)

	// ���� ������ �̺�Ʈ �߻�

	// ���� �� �̺�Ʈ �߻�
}
