#include "pch.h"
#include "ObjectManager.h"
#include "SkillManager.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "Boss.h"
#include "Portal.h"
#include "Env.h"
#include "Projectile.h"
#include "RoomBase.h"
#include "GimmickBase.h"

// [UNUSED(1)][TYPE(31)][ID(32)]
uint64 ObjectManager::s_objectIdGenerator = 1;

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

ObjectRef ObjectManager::CreateObject(GameSessionRef session, Protocol::ObjectType objectType, uint64 templateId)
{
	//WRITE_LOCK;

	return ObjectRef();
}

PlayerRef ObjectManager::CreatePlayer(GameSessionRef session, int32 templateId)
{
	//WRITE_LOCK;

	// ID 생성기
	const uint64 newId = GenerateIdLocked(Protocol::OBJECT_TYPE_CREATURE);

	PlayerRef player = make_shared<Player>();

	player->objectInfo->set_object_id(newId);
	player->objectInfo->set_template_id(templateId);
	player->objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_CREATURE);
	player->objectInfo->set_creature_type(Protocol::CreatureType::CREATURE_TYPE_PLAYER);

	player->posInfo->set_object_id(newId);
	player->posInfo->set_state(Protocol::MoveState::MOVE_STATE_IDLE);

	player->session = session;
	session->player.store(player);

	player->SetInfo(templateId);

	DoAsync(&ObjectManager::AddObject, static_pointer_cast<Object>(player));
	//AddObject(player);

	return player;
}

MonsterRef ObjectManager::CreateMonster(int32 templateId, FVector3 spawnPos, RoomBaseRef spawnedRoom)
{
	// ID 생성기
	const uint64 newId = GenerateIdLocked(Protocol::OBJECT_TYPE_CREATURE);

	MonsterRef monster = make_shared<Monster>();

	monster->objectInfo->set_object_id(newId);
	monster->objectInfo->set_template_id(templateId);
	monster->objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_CREATURE);
	monster->objectInfo->set_creature_type(Protocol::CreatureType::CREATURE_TYPE_MONSTER);
	monster->objectInfo->set_monster_type(Protocol::MonsterType::MONSTER_TYPE_GENERAL);

	monster->posInfo->set_object_id(newId);
	monster->posInfo->set_state(Protocol::MoveState::MOVE_STATE_IDLE);

	monster->posInfo->set_x(spawnPos.X);
	monster->posInfo->set_y(spawnPos.Y);
	monster->posInfo->set_z(spawnPos.Z);

	if (spawnedRoom != nullptr)
		monster->room = spawnedRoom;

	monster->SetInfo(templateId);

	DoAsync(&ObjectManager::AddObject, static_pointer_cast<Object>(monster));
	//AddObject(monster);

	return monster;
}

BossRef ObjectManager::CreateBoss(int32 templateId)
{
	// ID 생성기
	const uint64 newId = GenerateIdLocked(Protocol::OBJECT_TYPE_CREATURE);

	BossRef boss = make_shared<Boss>();

	boss->objectInfo->set_object_id(newId);
	boss->objectInfo->set_template_id(templateId);
	boss->objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_CREATURE);
	boss->objectInfo->set_creature_type(Protocol::CreatureType::CREATURE_TYPE_MONSTER);
	boss->objectInfo->set_monster_type(Protocol::MonsterType::MONSTER_TYPE_BOSS);

	boss->posInfo->set_object_id(newId);
	boss->posInfo->set_state(Protocol::MoveState::MOVE_STATE_IDLE);

	boss->SetInfo(templateId);

	DoAsync(&ObjectManager::AddObject, static_pointer_cast<Object>(boss));
	//AddObject(boss);

	return boss;
}

ProjectileRef ObjectManager::CreateProjectile(int32 templateId, CreatureRef owner, SkillBaseRef ownerSkill, GimmickBaseRef ownerGimmick, float posX, float posY, float posZ, FVector3 destPos)
{
	// ID 생성기
	const uint64 newId = GenerateIdLocked(Protocol::OBJECT_TYPE_PROJECTILE);

	ProjectileRef projectile = GSkillManager->GenerateProjectileById(templateId);

	projectile->objectInfo->set_object_id(newId);
	projectile->objectInfo->set_template_id(templateId);
	projectile->objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_PROJECTILE);
	projectile->_objectType = Protocol::ObjectType::OBJECT_TYPE_PROJECTILE;

	projectile->posInfo->set_object_id(newId);
	projectile->posInfo->set_state(Protocol::MoveState::MOVE_STATE_IDLE);
	projectile->posInfo->set_x(posX);
	projectile->posInfo->set_y(posY);
	projectile->posInfo->set_z(posZ);

	if (destPos != FVector3())
		projectile->_destPos = destPos;

	// enter room
	RoomBaseRef ownerRoom = owner->room.load().lock();
	ownerRoom->AddObject(projectile);

	// enter game
	DoAsync(&ObjectManager::AddObject, static_pointer_cast<Object>(projectile));
	//AddObject(projectile);

	if (ownerSkill != nullptr)
		projectile->SetInfo(owner, ownerSkill, templateId);
	else if (ownerGimmick != nullptr)
		projectile->SetInfo(static_pointer_cast<Boss>(owner), ownerGimmick, templateId);

	return projectile;
}

PortalRef ObjectManager::CreatePortal(int32 templateId, RoomBaseRef destRoom)
{
	const uint64 newId = GenerateIdLocked(Protocol::OBJECT_TYPE_ENV);

	PortalRef portal = make_shared<Portal>();

	portal->objectInfo->set_object_id(newId);
	portal->objectInfo->set_template_id(templateId);
	portal->objectInfo->set_object_type(Protocol::OBJECT_TYPE_ENV);
	portal->_objectType = Protocol::OBJECT_TYPE_ENV;

	portal->posInfo->set_object_id(newId);
	portal->posInfo->set_state(Protocol::MOVE_STATE_IDLE);

	portal->SetInfo(templateId);

	portal->SetDestRoom(destRoom);

	DoAsync(&ObjectManager::AddObject, static_pointer_cast<Object>(portal));
	//AddObject(portal);

	return portal;
}

ObjectRef ObjectManager::GetObjectById(uint64 objectId)
{
	WRITE_LOCK;

	if (_objects.find(objectId) == _objects.end())
		return nullptr;

	return _objects[objectId];
}

Protocol::ObjectType ObjectManager::GetObjectTypeById(uint64 id)
{
	WRITE_LOCK;

	uint64 type = (id >> 32) & 0x7FFFFFFF;
	return (Protocol::ObjectType)type;
}

void ObjectManager::AddObject(ObjectRef object)
{
	//WRITE_LOCK;

	if (_objects.find(object->objectInfo->object_id()) != _objects.end())
		return;

	_objects.insert(make_pair(object->objectInfo->object_id(), object));
}

void ObjectManager::RemoveObject(uint64 objectId)
{
	//WRITE_LOCK;

	if (_objects.find(objectId) == _objects.end())
		return;

	ObjectRef object = _objects[objectId];

	// room에 속해있을 경우 그 room의 목록에서 제거
	RoomBaseRef roomRef = object->room.load().lock();
	if (roomRef != nullptr)
	{
		roomRef->DoAsync(&RoomBase::RemoveObject, objectId);
		//roomRef->RemoveObject(objectId);
	}

	object->room.store(weak_ptr<RoomBase>()); // null로 밀어주기
	object->session = weak_ptr<GameSession>();

	// 클리어
	object->Clear();

	_objects.erase(objectId);
}

uint64 ObjectManager::GenerateIdLocked(Protocol::ObjectType objectType)
{
	WRITE_LOCK;

	return ((uint64)objectType << 32) | (s_objectIdGenerator++);
	//return (s_objectIdGenerator++);
}
