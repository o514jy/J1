#include "pch.h"
#include "AuroraQ.h"
#include "Data.h"
#include "DataManager.h"
#include "BuffInstant.h"

AuroraQ::AuroraQ()
{
}

AuroraQ::~AuroraQ()
{
}

void AuroraQ::SetInfo(CreatureRef owner, int32 templateId)
{
	__super::SetInfo(owner, templateId);
}

void AuroraQ::OnAttackEvent(int32 timeCount)
{
	__super::OnAttackEvent(timeCount);

	// ��ų�� ����Ʈ ���� �ȿ� ���Դ��� Ȯ��
	int32 effectId = _skillData->EffectIdList[timeCount];
	vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);

	// buff ó��
	for (auto& object : objects)
	{
		// 1) attack ������ ����� buff ������ ���� ���� Ÿ�� �� ���ӽð� Ȯ��
		wstring buffDurationType = GDataManager->GetBuffDataById(_skillData->BuffIdList[timeCount])->BuffDurationType;

		// 2) ���� ���ӽð� Ÿ�Կ� �´� ���� ����
		BuffBaseRef buff = nullptr;
		if (buffDurationType == L"Instant")
		{
			buff = make_shared<BuffInstant>();
		}

		// 3) ���� �ʱ�ȭ �� ����
		buff->SetInfo(_skillData->BuffIdList[timeCount], object, static_pointer_cast<SkillBase>(shared_from_this()));

		// 4) ���� object���� buff �ο�
		buff->ApplyBuff();
	}
}

void AuroraQ::DoSkill(const Protocol::C_SKILL& skillPkt)
{
	__super::DoSkill(skillPkt);
}
