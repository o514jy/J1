#pragma once

#include "JobQueue.h"

class Creature;
class SkillData;

class SkillBase : public JobQueue
{
public:
	SkillBase();
	~SkillBase();

public:
	/** initialize **/
	virtual void SetInfo(CreatureRef owner, int32 templateId);

protected:
	/** AnimEvent **/
	void OnAnimImpactTimeHandler();
	virtual void OnAnimCompleteHandler();

	virtual void OnAttackEvent(int32 timeCount);

public:
	/** Process Skill **/
	virtual void DoSkill(const Protocol::C_SKILL& skillPkt, Protocol::S_SKILL& skillPktToSend);

	// cancled by other cc
	virtual void CancledSkill();

public:
	/** projectile **/
	virtual ProjectileRef GenerateProjectile(int32 templateId, float spawnPosX, float spawnPosY, float spawnPosZ);

public:
	/** gather creature in effects **/
	vector<ObjectRef> GatherObjectInEffectArea(int32 effectId);

	/* is in area */
	bool IsInPizzaArea(ObjectRef object, float radius, float theta);
	bool IsInCircleArea(ObjectRef object, float radius);

public:
	/** exception check **/
	vector<JobRef> _animImpactJobs;

public:
	/** setter & geter **/
	void SetCanUseSkill(bool flag);
	bool GetCanUseSkill();

public:
	/** information **/
	CreatureRef _owner;
	SkillDataRef _skillData;
	
	int32 _timeCount; // index of AnimImpactTime

	bool _canUseSkill;

	Protocol::SimplePosInfo* _impactPos;
};

