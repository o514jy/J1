#include "pch.h"
#include "GimmickBase.h"
#include "DataManager.h"
#include "Data.h"
#include "SkillComponent.h"
#include "SkillBase.h"
#include "Boss.h"
#include "GimmickComponent.h"
#include "RoomBase.h"

GimmickBase::GimmickBase()
{
	_owner = nullptr;
	_gimmickData = nullptr;
	_eventCount = 0;
	_canUseGimmick = true;
}

GimmickBase::~GimmickBase()
{
	_owner = nullptr;
	_gimmickData = nullptr;
	_eventCount = 0;
	_canUseGimmick = false;
}

void GimmickBase::SetInfo(BossRef owner, int32 templateId)
{
	_owner = owner;

	_gimmickData = GDataManager->GetGimmickDataById(templateId);

	SetCanUseGimmick(true);
}

void GimmickBase::OnEventTimeHandler()
{
	OnEvent(_eventCount++);
}

void GimmickBase::OnEvent(int32 eventCount)
{
	// ���������� �̻��� ��Ȳ�̸� ����
	if (eventCount >= _gimmickData->MissionIdList.size())
		return;
}

void GimmickBase::OnDurationCompleteHandler()
{
	RoomBaseRef roomRef = _owner->GetRoomRef();

	// ������� ��� ��, ����� ��� �� ������� ���ư�
	_owner->GetGimmickComponent()->SetActiveGimmick(nullptr);

	Protocol::C_SKILL sendSkillPkt = MakeSkillPkt(_gimmickData->SkillIdList.back());
	_owner->GetSkillComponent()->DoSkill(sendSkillPkt);

	// ��Ÿ�� ���ư���
	if (roomRef != nullptr)
	{
		roomRef->DoTimer(_gimmickData->CoolTime, shared_from_this(), &GimmickBase::SetCanUseGimmick, true);
	}

	// ��� ������Ÿ�� ��Ȱ��ȭ


	_eventCount = 0;
}

Protocol::C_SKILL GimmickBase::MakeSkillPkt(int32 skillId, Protocol::SimplePosInfo simplePosInfo)
{
	Protocol::C_SKILL skillPkt;
	skillPkt.set_object_id(_owner->_objectId);
	skillPkt.set_slot(Protocol::SkillSlot::SKILL_SLOT_GIMMICK);
	{
		Protocol::SimplePosInfo* info = skillPkt.mutable_simple_pos_info();
		info->CopyFrom(simplePosInfo);
	}
	// gimmick
	skillPkt.set_skill_data_id(skillId);

	return skillPkt;
}

void GimmickBase::DoGimmick()
{
	RoomBaseRef roomRef = _owner->GetRoomRef();

	_owner->SetState(Protocol::MoveState::MOVE_STATE_GIMMICK);
	_owner->GetGimmickComponent()->SetActiveGimmick(static_pointer_cast<GimmickBase>(shared_from_this()));

	// �߰��� ȣ���� �Լ�

	// ���� �� ȣ���� �Լ�
	if (roomRef != nullptr)
	{
		roomRef->DoTimer(_gimmickData->Duration, shared_from_this(), &GimmickBase::OnDurationCompleteHandler);
	}

	// ����߿� ��� ȣ��x
	SetCanUseGimmick(false);
}

void GimmickBase::SetCanUseGimmick(bool flag)
{
	if (flag == true)
	{
		_owner->GetGimmickComponent()->_gimmickIdQueue.push(_gimmickData->DataId);
	}

	_canUseGimmick = flag;
}

bool GimmickBase::GetCanUseGimmick()
{
	return _canUseGimmick;
}
