#pragma once

class BaseAIController
{
public:
	BaseAIController();
	~BaseAIController();

public:
	/** initialize **/
	virtual void SetInfo(ObjectRef owner);

public:
	/** setter & getter **/
	void SetDistToTarget(float dist);
	float GetDistToTarget();

public:
	/** network **/
	void BroadcastMove();

public:
	/** finite state machine **/
	virtual void UpdateTick();

	virtual void UpdateIdle();
	virtual void UpdateRun();
	virtual void UpdateSkill();
	virtual void UpdateDead();

public:
	/** helper **/
	virtual void ChaseOrAttackTarget(float chaseRange, float attackRange);

public:
	/** cache **/
	float _distToTarget;

public:
	/** information **/
	ObjectRef _owner;
};

