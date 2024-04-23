#pragma once

#include "JobQueue.h"

class BuffBase : public JobQueue
{
public:
	BuffBase();
	~BuffBase();

public:
	/** initialize **/
	virtual void SetInfo(int32 templateId, ObjectRef owner, SkillBaseRef skill);

public:
	/** setter & getter **/
	float GetFinalAmount();

public:
	/** network **/


public:
	/** process **/
	virtual void ApplyBuff();
	virtual void ClearBuff();

public:
	/** information **/
	ObjectRef _owner;
	SkillBaseRef _ownerSkill;
	BuffDataRef _buffData;

	float _finalAmount;

	Protocol::BuffInfo* _buffInfo;
};

