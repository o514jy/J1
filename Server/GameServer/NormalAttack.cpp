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

	//// ��ų�� ����Ʈ ���� �ȿ� ���Դ��� Ȯ��
	//int32 effectId = _skillData->EffectIdList[timeCount];
	//vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);
	//
	//// buff ó��
	//for (auto& object : objects)
	//{
	//	// 1) attack ������ ����� buff ���� 
	//	BuffBaseRef buff = make_shared<BuffInstant>();
	//	buff->SetInfo(_skillData->BuffIdList[timeCount], object, static_pointer_cast<SkillBase>(shared_from_this()));
	//
	//	// 2) ���� object���� buff �ο�
	//	buff->ApplyBuff();
	//}
}

void NormalAttack::DoSkill(const Protocol::C_SKILL& skillPkt)
{
	__super::DoSkill(skillPkt);

	// �ִϸ��̼� ��� (Ŭ�󿡼�)

	// ���� ������ �̺�Ʈ �߻�

	// ���� �� �̺�Ʈ �߻�
}
