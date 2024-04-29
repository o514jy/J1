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

	// 스킬의 이펙트 범위 안에 들어왔는지 확인
	int32 effectId = _skillData->EffectIdList[timeCount];
	vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);

	// buff 처리
	for (auto& object : objects)
	{
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

void AuroraQ::DoSkill(const Protocol::C_SKILL& skillPkt)
{
	__super::DoSkill(skillPkt);
}
