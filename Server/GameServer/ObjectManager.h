#pragma once

class ObjectManager //: public JobQueue
{
public:
	ObjectManager();
	~ObjectManager();

public:
	ObjectRef CreateObject(GameSessionRef session, Protocol::ObjectType objectType, uint64 templateId);
	
	PlayerRef CreatePlayer(GameSessionRef session, int32 templateId);
	BossRef CreateBoss(int32 templateId);

public:
	ObjectRef GetObjectById(uint64 objectId);

public:
	/** helper **/
	Protocol::ObjectType GetObjectTypeById(uint64 id);

public:
	bool AddObject(ObjectRef object);
	bool RemoveObject(uint64 objectId);

	uint64 GenerateIdLocked(Protocol::ObjectType objectType);

private:
	static uint64 s_objectIdGenerator;

private:
	USE_LOCK;
	unordered_map<uint64, ObjectRef> _objects;
};