#pragma once

class BaseAIController
{
public:
	BaseAIController();
	~BaseAIController();

public:
	/** initialize **/
	virtual void SetInfo(ObjectRef owner);

	virtual void Clear();

public:
	/** setter & getter **/
	void SetDistToTarget(float dist);
	float GetDistToTarget();

public:
	/** network **/
	void BroadcastMove(); // legacy
	
	void BroadcastNowPos();

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
	/** nav device **/
	void InitNavDevice(NavigationRef sample);

	void RegisterTargetPosToNavDevice();

	// register NowPos to MoveTarget to stop
	void StopMovement();

public:
	/** information **/
	ObjectRef _owner;

	// nav device for detour
	NavDeviceRef _navDevice;
};

