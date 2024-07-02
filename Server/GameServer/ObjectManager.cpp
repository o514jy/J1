#include "pch.h"
#include "ObjectManager.h"
#include "SkillManager.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "Boss.h"
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
	WRITE_LOCK;

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

	AddObject(player);

	return player;
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

	AddObject(boss);

	return boss;
}

ProjectileRef ObjectManager::CreateProjectile(int32 templateId, CreatureRef owner, SkillBaseRef ownerSkill, GimmickBaseRef ownerGimmick, float posX, float posY, float posZ)
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

	// enter room
	RoomBaseRef ownerRoom = owner->room.load().lock();
	ownerRoom->AddObject(projectile);

	// enter game
	AddObject(projectile);

	if (ownerSkill != nullptr)
		projectile->SetInfo(owner, ownerSkill, templateId);
	else if (ownerGimmick != nullptr)
		projectile->SetInfo(static_pointer_cast<Boss>(owner), ownerGimmick, templateId);

	return projectile;
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

bool ObjectManager::AddObject(ObjectRef object)
{
	WRITE_LOCK;

	if (_objects.find(object->objectInfo->object_id()) != _objects.end())
		return false;

	_objects.insert(make_pair(object->objectInfo->object_id(), object));

	return true;
}

bool ObjectManager::RemoveObject(uint64 objectId)
{
	WRITE_LOCK;

	if (_objects.find(objectId) == _objects.end())
		return false;

	ObjectRef object = _objects[objectId];

	// room에 속해있을 경우 그 room의 목록에서 제거
	RoomBaseRef roomRef = object->room.load().lock();
	if (roomRef != nullptr)
	{
		//roomRef->DoAsync(&RoomBase::RemoveObject, objectId);
		roomRef->RemoveObject(objectId);
	}

	object->room.store(weak_ptr<RoomBase>()); // null로 밀어주기
	object->session = weak_ptr<GameSession>();
	object->_statComponent = nullptr;

	// 클리어
	object->Clear();

	_objects.erase(objectId);

	return true;
}

uint64 ObjectManager::GenerateIdLocked(Protocol::ObjectType objectType)
{
	WRITE_LOCK;

	//return ((uint64)objectType << 32) | (s_objectIdGenerator++);
	return (s_objectIdGenerator++);
}
