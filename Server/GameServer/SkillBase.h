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
	void OnAnimCompleteHandler();

	virtual void OnAttackEvent(int32 timeCount);

public:
	/** Process Skill **/
	virtual void DoSkill();

public:
	/** gather creature in effects **/
	vector<ObjectRef> GatherObjectInEffectArea(int32 effectId);

	/* is in area */
	bool IsInRectangleArea(ObjectRef object, float luY, float luX, float rdY, float rdX);

public:
	/** exception check **/
	vector<JobRef> _jobs;

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
};
