#include "pch.h"
#include "Data.h"
#include "SkillComponent.h"
#include "DataManager.h"
#include "SkillManager.h"
#include "SkillBase.h"
#include "Creature.h"

SkillComponent::SkillComponent()
{
	skillInfo = make_shared< Protocol::SkillInfo>();
}

SkillComponent::~SkillComponent()
{
	skillInfo = nullptr;
}

void SkillComponent::SetInfo(CreatureRef owner, CreatureDataRef creatureData)
{
	_owner = owner;

	if (creatureData->CreatureType == L"Player")
	{
		PlayerDataRef data = static_pointer_cast<PlayerData>(creatureData);

		skillInfo->set_object_id(_owner->_objectId);
		skillInfo->set_skill_attack_id(data->SkillAttackId);
		skillInfo->set_skill_q_id(data->SkillQId);
		skillInfo->set_skill_w_id(data->SkillWId);
		skillInfo->set_skill_e_id(data->SkillEId);
		skillInfo->set_skill_r_id(data->SkillRId);
		skillInfo->set_skill_dash_id(data->SkillDashId);

		AddSkill(data->SkillAttackId, Protocol::SkillSlot::SKILL_SLOT_ATTACK);
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

void SkillComponent::DoSkill(const Protocol::SkillSlot& skillSlot)
{
	if (GetCanUseSkillBySkillSlot(skillSlot) == false)
		return;

	if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_ATTACK)
	{
		return _normalAttackSkill->DoSkill();
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_Q)
	{
		return _qSkill->DoSkill();
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_W)
	{
		return _wSkill->DoSkill();
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_E)
	{
		return _eSkill->DoSkill();
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_R)
	{
		return _rSkill->DoSkill();
	}
	else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_DASH)
	{
		return _dashSkill->DoSkill();
	}
}