#include "pch.h"
#include "SpreadCloud.h"
#include "Boss.h"
#include "GimmickComponent.h"
#include "FindSafeZone.h"
#include "DataManager.h"
#include "BuffInstant.h"
#include "SafeZone.h"

SpreadCloud::SpreadCloud()
{
}

SpreadCloud::~SpreadCloud()
{
}

void SpreadCloud::SetInfo(CreatureRef owner, int32 templateId)
{
	__super::SetInfo(owner, templateId);

	BossRef boss = static_pointer_cast<Boss>(owner);
	GimmickBaseRef gimmick = boss->GetGimmickComponent()->_canActiveGimmickList[FIND_SAFE_ZONE_DATA_ID];
	ownerGimmick = static_pointer_cast<FindSafeZone>(gimmick);
}

void SpreadCloud::OnAttackEvent(int32 timeCount)
{
	__super::OnAttackEvent(timeCount);

	// ��ų�� ����Ʈ ���� �ȿ� ���Դ��� Ȯ��
	int32 effectId = _skillData->EffectIdList[timeCount];
	vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);

	FindSafeZoneRef safeZone = ownerGimmick.lock();
	// ��������
	vector<uint64> safeIds = safeZone->GetSafePlayerList();

	// buff ó��
	for (auto& object : objects)
	{
		// 0) �������뿡 �־����� ���ش�.
		bool isFin = false;
		for (uint64 id : safeIds)
		{
			if (object->_objectId == id)
			{
				isFin = true;
				continue;
			}
		}
		if (isFin == true)
			continue;

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

void SpreadCloud::DoSkill(const Protocol::C_SKILL& skillPkt, Protocol::S_SKILL& skillPktToSend)
{
	__super::DoSkill(skillPkt, skillPktToSend);
}
