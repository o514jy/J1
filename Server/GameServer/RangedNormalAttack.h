#pragma once

#include "SkillBase.h"

class RangedNormalAttack : public SkillBase
{
public:
	RangedNormalAttack();
	~RangedNormalAttack();

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
	FVector3 _destPos = FVector3();

	ProjectileRef _projectile;
};

