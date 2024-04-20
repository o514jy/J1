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
	shared_ptr<Protocol::SkillInfo> skillInfo;

public:
	/** process **/
	bool GetCanUseSkillBySkillSlot(const Protocol::SkillSlot& skillSlot);

	void DoSkill(const Protocol::SkillSlot& skillSlot);

public:
	/** information **/
	CreatureRef _owner;

	/** player **/
	SkillBaseRef _normalAttackSkill;
	SkillBaseRef _qSkill;
	SkillBaseRef _wSkill;
	SkillBaseRef _eSkill;
	SkillBaseRef _rSkill;
	SkillBaseRef _dashSkill;

	
};
