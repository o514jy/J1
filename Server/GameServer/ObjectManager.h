#pragma once

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

public:
	ObjectRef CreateObject(GameSessionRef session, Protocol::ObjectType objectType, uint64 templateId);
	
	PlayerRef CreatePlayer(GameSessionRef session, int32 templateId);
	BossRef CreateBoss(int32 templateId);
	ProjectileRef CreateProjectile(int32 templateId, CreatureRef owner = nullptr, SkillBaseRef ownerSkill = nullptr, GimmickBaseRef ownerGimmick = nullptr, float posX = 0.f, float posY = 0.f, float posZ = 0.f);

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