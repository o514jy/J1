#pragma once

#include "JobQueue.h"

class ObjectManager : public JobQueue
{
public:
	ObjectManager();
	~ObjectManager();

public:
	ObjectRef CreateObject(GameSessionRef session, Protocol::ObjectType objectType, uint64 templateId);
	
	/** creature **/
	PlayerRef CreatePlayer(GameSessionRef session, int32 templateId);
	MonsterRef CreateMonster(int32 templateId, FVector3 spawnPos = FVector3(0.f, 0.f, 0.f), RoomBaseRef spawnedRoom = nullptr);
	BossRef CreateBoss(int32 templateId);

	/** projectile**/
	ProjectileRef CreateProjectile(int32 templateId, CreatureRef owner = nullptr, SkillBaseRef ownerSkill = nullptr, GimmickBaseRef ownerGimmick = nullptr, float posX = 0.f, float posY = 0.f, float posZ = 0.f, FVector3 destPos = FVector3());

	/** env **/
	PortalRef CreatePortal(int32 templateId, RoomBaseRef destRoom);

public:
	ObjectRef GetObjectById(uint64 objectId);

public:
	/** helper **/
	Protocol::ObjectType GetObjectTypeById(uint64 id);

public:
	void AddObject(ObjectRef object);
	void RemoveObject(uint64 objectId);

	uint64 GenerateIdLocked(Protocol::ObjectType objectType);

private:
	static uint64 s_objectIdGenerator;

private:
	USE_LOCK;
	unordered_map<uint64, ObjectRef> _objects;
};