#include "pch.h"
#include "SpreadCloud.h"
#include "Boss.h"
#include "GimmickComponent.h"
#include "FindSafeZone.h"
#include "DataManager.h"
#include "BuffInstant.h"
#include "SafeZone.h"
#include "StartRoom.h"
#include "Projectile.h"

SpreadCloud::SpreadCloud()
{
	_ownerGimmickDataId = 0;
}

SpreadCloud::~SpreadCloud()
{
}

void SpreadCloud::SetInfo(CreatureRef owner, int32 templateId)
{
	__super::SetInfo(owner, templateId);

	_ownerGimmickDataId = FIND_SAFE_ZONE_DATA_ID;
}

void SpreadCloud::OnAnimCompleteHandler()
{
	__super::OnAnimCompleteHandler();

	if (_owner == nullptr)
		return;

	// 생존자리스트 초기화
	safePlayers.clear();

	// safe zone 제거
	//RoomBaseRef roomRef = _owner->room.load().lock();
	//for (int i = 0; i < roomRef->_objects.size(); i++)
	//{
	//	auto& item = roomRef->_objects[i];
	//	if (!object)
	//		continue;
	//	if (object->_objectType == Protocol::ObjectType::OBJECT_TYPE_PROJECTILE)
	//	{
	//		ProjectileRef proj = static_pointer_cast<Projectile>(object);
	//		proj->ForceDelete();
	//	}
	//}
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
		if (safePlayers.find(object->_objectId) != safePlayers.end())
		{
			continue;
		}

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

	BossRef boss = static_pointer_cast<Boss>(_owner);
	auto gimmicks = boss->GetGimmickComponent()->_gimmicks;
	if (gimmicks.find(_ownerGimmickDataId) == gimmicks.end())
	{
		// 뭔가 문제가 있다.
		return;
	}
	else
	{
		ownerGimmick = static_pointer_cast<FindSafeZone>(gimmicks[_ownerGimmickDataId]);
	}
}
