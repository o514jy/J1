#include "pch.h"
#include "GimmickBase.h"
#include "DataManager.h"
#include "Data.h"
#include "SkillComponent.h"
#include "SkillBase.h"
#include "Boss.h"

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

	Protocol::C_SKILL sendSkillPkt = MakeSkillPkt(_gimmickData->SkillIdList.back());
	_owner->GetSkillComponent()->DoSkill(sendSkillPkt);

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
	DoTimer(_gimmickData->Duration, &GimmickBase::OnDurationCompleteHandler);

	
}
