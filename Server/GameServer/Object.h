#pragma once


class Object : public enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

	bool IsPlayer() { return _isPlayer; }

public:
	/** initialize **/
	virtual void SetInfo(int32 templateId);

public:
	/** setter & getter **/
	void SetState(Protocol::MoveState moveState);
	Protocol::MoveState GetState();



	StatComponentRef GetStatComponent() { return _statComponent; }

public:
	/** information **/
	shared_ptr<Protocol::ObjectType> objectType;
	shared_ptr<Protocol::ObjectInfo> objectInfo;
	shared_ptr<Protocol::PosInfo> posInfo;

	uint64 _objectId;
	int32 _templateId;

public:
	/** battle **/
	virtual void OnDamaged(ObjectRef attacker, SkillBaseRef skill);

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

