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

	// 스킬의 이펙트 범위 안에 들어왔는지 확인
	int32 effectId = _skillData->EffectIdList[timeCount];
	vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);

	FindSafeZoneRef safeZone = ownerGimmick.lock();
	// 생존지대
	vector<uint64> safeIds = safeZone->GetSafePlayerList();

	// buff 처리
	for (auto& object : objects)
	{
		// 0) 생존지대에 있었으면 빼준다.
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

		// 1) attack 시점에 사용할 buff 생성을 위해 버프 타입 및 지속시간 확인
		wstring buffDurationType = GDataManager->GetBuffDataById(_skillData->BuffIdList[timeCount])->BuffDurationType;

		// 2) 버프 지속시간 타입에 맞는 버프 생성
		BuffBaseRef buff = nullptr;
		if (buffDurationType == L"Instant")
		{
			buff = make_shared<BuffInstant>();
		}

		// 3) 버프 초기화 및 설정
		buff->SetInfo(_skillData->BuffIdList[timeCount], object, static_pointer_cast<SkillBase>(shared_from_this()));

		// 4) 들어온 object에게 buff 부여
		buff->ApplyBuff();
	}
}

void SpreadCloud::DoSkill(const Protocol::C_SKILL& skillPkt, Protocol::S_SKILL& skillPktToSend)
{
	__super::DoSkill(skillPkt, skillPktToSend);
}
