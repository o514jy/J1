#include "pch.h"
#include "Data.h"
#include "SkillComponent.h"
#include "DataManager.h"
#include "SkillManager.h"
#include "SkillBase.h"
#include "Creature.h"
#include "RoomBase.h"

SkillComponent::SkillComponent()
{
	skillInfo = new Protocol::SkillInfo;
}

SkillComponent::~SkillComponent()
{
	skillInfo = nullptr;
}

void SkillComponent::SetInfo(CreatureRef owner, CreatureDataRef creatureData)
{
	_owner = owner;
	_owner->objectInfo->set_allocated_skill_info(skillInfo);

	skillInfo->set_object_id(_owner->_objectId);
	AddSkill(creatureData->SkillAttackId, Protocol::SkillSlot::SKILL_SLOT_ATTACK);

	if (creatureData->CreatureType == L"Player")
	{
		PlayerDataRef data = static_pointer_cast<PlayerData>(creatureData);

		skillInfo->set_skill_attack_id(data->SkillAttackId);
		skillInfo->set_skill_q_id(data->SkillQId);
		skillInfo->set_skill_w_id(data->SkillWId);
		skillInfo->set_skill_e_id(data->SkillEId);
		skillInfo->set_skill_r_id(data->SkillRId);
		skillInfo->set_skill_dash_id(data->SkillDashId);

		
		AddSkill(data->SkillQId, Protocol::SkillSlot::SKILL_SLOT_Q);
		AddSkill(data->SkillWId, Protocol::SkillSlot::SKILL_SLOT_W);
		AddSkill(data->SkillEId, Protocol::SkillSlot::SKILL_SLOT_E);
		AddSkill(data->SkillRId, Protocol::SkillSlot::SKILL_SLOT_R);
		AddSkill(data->SkillDashId, Protocol::SkillSlot::SKILL_SLOT_DASH);
	}
}

void SkillComponent::AddSkill(int32 templateId, Protocol::SkillSlot skillSlot)
{
	if (templateId < 1)
		return;

	SkillDataRef skillData = GDataManager->GetSkillDataById(templateId);
	if (skillData == nullptr)
	{
		cout << "AddSkill Failed " << templateId << "\n";
		return;
	}

	SkillBaseRef skill = GSkillManager->GenerateSkillById(templateId);
	if (skill == nullptr)
	{
		cout << "GenerateSkill Failed " << templateId << "\n";
		return;
	}

	if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_ATTACK)
	{
		_normalAttackSkill = skill;
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_Q)
	{
		_qSkill = skill;
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_W)
	{
		_wSkill = skill;
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_E)
	{
		_eSkill = skill;
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_R)
	{
		_rSkill = skill;
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_DASH)
	{
		_dashSkill = skill;
	}

	skill->SetInfo(_owner, templateId);
}

void SkillComponent::BroadcastSkill(const Protocol::C_SKILL& skillPkt)
{
	// broadcast skill
	{
		Protocol::S_SKILL pkt;
		pkt.set_slot(skillPkt.slot());
		pkt.set_object_id(skillPkt.object_id());
		Protocol::SimplePosInfo* simplePosInfo = pkt.mutable_simple_pos_info();
		simplePosInfo->CopyFrom(skillPkt.simple_pos_info());

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		RoomBaseRef room = _owner->room.load().lock();
		room->Broadcast(sendBuffer);
	}
}

bool SkillComponent::GetCanUseSkillBySkillSlot(const Protocol::SkillSlot& skillSlot)
{
	if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_ATTACK)
	{
		return _normalAttackSkill->GetCanUseSkill();
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_Q)
	{
		return _qSkill->GetCanUseSkill();
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_W)
	{
		return _wSkill->GetCanUseSkill();
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_E)
	{
		return _eSkill->GetCanUseSkill();
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_R)
	{
		return _rSkill->GetCanUseSkill();
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_DASH)
	{
		return _dashSkill->GetCanUseSkill();
	}
}

void SkillComponent::DoSkill(const Protocol::C_SKILL& skillPkt)
{
	Protocol::SkillSlot skillSlot = skillPkt.slot();
	Protocol::SimplePosInfo simplePosInfo = skillPkt.simple_pos_info();

	if (GetCanUseSkillBySkillSlot(skillSlot) == false)
		return;
	
	// notify all client to use skill
	BroadcastSkill(skillPkt);

	if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_ATTACK)
	{
		return _normalAttackSkill->DoSkill(skillPkt);
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_Q)
	{
		return _qSkill->DoSkill(skillPkt);
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_W)
	{
		return _wSkill->DoSkill(skillPkt);
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_E)
	{
		return _eSkill->DoSkill(skillPkt);
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_R)
	{
		return _rSkill->DoSkill(skillPkt);
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_DASH)
	{
		return _dashSkill->DoSkill(skillPkt);
	}
}
