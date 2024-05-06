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

	// �����ڸ���Ʈ �ʱ�ȭ
	safePlayers.clear();

	// safe zone ����
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
		if (safePlayers.find(object->_objectId) != safePlayers.end())
		{
			continue;
		}

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

	BossRef boss = static_pointer_cast<Boss>(_owner);
	auto gimmicks = boss->GetGimmickComponent()->_gimmicks;
	if (gimmicks.find(_ownerGimmickDataId) == gimmicks.end())
	{
		// ���� ������ �ִ�.
		return;
	}
	else
	{
		ownerGimmick = static_pointer_cast<FindSafeZone>(gimmicks[_ownerGimmickDataId]);
	}
}
