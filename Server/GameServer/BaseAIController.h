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
	/** information **/
	ObjectRef _owner;

	Protocol::MoveState* _state;
};

