#include "pch.h"
#include "GimmickBase.h"
#include "DataManager.h"
#include "Data.h"
#include "SkillComponent.h"
#include "SkillBase.h"
#include "Boss.h"
#include "GimmickComponent.h"

GimmickBase::GimmickBase()
{
	_owner = nullptr;
	_gimmickData = nullptr;
	_eventCount = 0;
}

GimmickBase::~GimmickBase()
{
	_owner = nullptr;
	_gimmickData = nullptr;
	_eventCount = 0;
}

void GimmickBase::SetInfo(BossRef owner, int32 templateId)
{
	_owner = owner;

	_gimmickData = GDataManager->GetGimmickDataById(templateId);


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
	// ������� ��� ��, ����� ��� �� ������� ���ư�
	_owner->GetGimmickComponent()->SetActiveGimmick(nullptr);

	Protocol::C_SKILL sendSkillPkt = MakeSkillPkt(_gimmickData->SkillIdList.back());
	_owner->GetSkillComponent()->DoSkill(sendSkillPkt);

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
	_owner->SetState(Protocol::MoveState::MOVE_STATE_GIMMICK);
	_owner->GetGimmickComponent()->SetActiveGimmick(static_pointer_cast<GimmickBase>(shared_from_this()));

	// �߰��� ȣ���� �Լ�

	// ���� �� ȣ���� �Լ�
	DoTimer(_gimmickData->Duration, &GimmickBase::OnDurationCompleteHandler);
}
