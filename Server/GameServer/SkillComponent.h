#pragma once

class Creature;
class SkillBase;

class SkillComponent
{
public:
	SkillComponent();
	~SkillComponent();

public:
	/** initialize **/
	void SetInfo(CreatureRef owner, CreatureDataRef creatureData);

	void AddSkill(int32 templateId, Protocol::SkillSlot skillSlot);

public:
	/** network **/
	Protocol::SkillInfo* skillInfo;

	void BroadcastSkill(const Protocol::C_SKILL& skillPkt);

public:
	/** process **/
	bool GetCanUseSkillBySkillSlot(const Protocol::SkillSlot& skillSlot);

	void DoSkill(const Protocol::C_SKILL& skillPkt);

public:
	/** information **/
	CreatureRef _owner;

	SkillBaseRef _normalAttackSkill;

	/** player **/
	SkillBaseRef _qSkill;
	SkillBaseRef _wSkill;
	SkillBaseRef _eSkill;
	SkillBaseRef _rSkill;
	SkillBaseRef _dashSkill;

	/** boss **/

	
};

