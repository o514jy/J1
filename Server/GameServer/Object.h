#pragma once

#include "Data.h"
// enable_shared_from_this<Object>
class Object : public enable_shared_from_this<Object>
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

	void SetPosInfo(Protocol::PosInfo InPosInfo);
	Protocol::PosInfo* GetPosInfo();

	void SetAgentIdx(int32 idx);
	int32 GetAgentIdx() { return _agentIdx; }

	StatComponentRef GetStatComponent() { return _statComponent; }

	RoomBaseRef GetRoomRef();

	virtual void Clear();

public:
	/** information **/
	Protocol::ObjectType _objectType;
	Protocol::ObjectInfo* objectInfo;
	Protocol::PosInfo* posInfo;

	uint64 _objectId;
	int32 _templateId;
	int32 _agentIdx;

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
	bool _isCrowd = true;

public:
	weak_ptr<GameSession> session;
};

