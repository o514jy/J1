#pragma once

#include "SkillBase.h"

class AuroraQ : public SkillBase
{
public:
	AuroraQ();
	~AuroraQ();

public:
	/** initialize **/
	virtual void SetInfo(CreatureRef owner, int32 templateId) override;

public:
	/** AnimEvent **/
	virtual void OnAttackEvent(int32 timeCount) override;

public:
	/** Process Skill **/
	virtual void DoSkill(const Protocol::C_SKILL& skillPkt) override;
};

