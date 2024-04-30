#pragma once

#include "SkillBase.h"

class PoisonRain : public SkillBase
{
public:
	PoisonRain();
	~PoisonRain();

public:
	/** initialize **/
	virtual void SetInfo(CreatureRef owner, int32 templateId) override;

public:
	/** AnimEvent **/
	virtual void OnAttackEvent(int32 timeCount) override;

public:
	/** Process Skill **/
	virtual void DoSkill(const Protocol::C_SKILL& skillPkt, Protocol::S_SKILL& skillPktToSend) override;

public:
	/** information **/
	int32 _projectileCount;

	/* projectile random pos */
	vector<pair<float, float>> randomPoses;
};

