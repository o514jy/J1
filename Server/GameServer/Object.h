#pragma once

#include "Data.h"
#include "JobQueue.h"
// enable_shared_from_this<Object>
class Object : public JobQueue
{
public:
	Object();
	virtual ~Object();

	bool IsPlayer() { return _isPlayer; }

public:
	/** update tick **/
	virtual void UpdateTick();

public:
	/** initialize **/
	virtual void SetInfo(int32 templateId);

public:
	/** setter & getter **/
	void SetState(Protocol::MoveState moveState);
	Protocol::MoveState GetState();

	void SetPosInfo(const Protocol::PosInfo& InPosInfo);
	Protocol::PosInfo* GetPosInfo();

	StatComponentRef GetStatComponent() { return _statComponent; }

	virtual void Clear();

public:
	/** information **/
	Protocol::ObjectType _objectType;
	Protocol::ObjectInfo* objectInfo;
	Protocol::PosInfo* posInfo;

	uint64 _objectId;
	int32 _templateId;

public:
	/** battle **/
	virtual void OnDamaged(ObjectRef attacker, BuffBaseRef buff);

public:
	/* stat */
	StatComponentRef _statComponent;

public:
	atomic<weak_ptr<RoomBase>> room;

protected:
	bool _isPlayer = false;

public:
	weak_ptr<GameSession> session;
};

