#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "RoomBase.h"

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
	return ObjectRef();
}

PlayerRef ObjectManager::CreatePlayer(GameSessionRef session, int32 templateId)
{
	// ID 생성기
	const uint64 newId = GenerateIdLocked(Protocol::OBJECT_TYPE_CREATURE);

	PlayerRef player = make_shared<Player>();

	player->objectInfo->set_object_id(newId);
	player->objectInfo->set_template_id(templateId);
	player->objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_CREATURE);
	player->posInfo->set_object_id(newId);
	player->objectInfo->set_creature_type(Protocol::CreatureType::CREATURE_TYPE_PLAYER);

	player->session = session;
	session->player.store(player);

	player->SetInfo(templateId);

	AddObject(player);

	return player;
}

ObjectRef ObjectManager::GetObjectById(uint64 objectId)
{
	if (_objects.find(objectId) == _objects.end())
		return nullptr;

	return _objects[objectId];
}

Protocol::ObjectType ObjectManager::GetObjectTypeById(uint64 id)
{
	uint64 type = (id >> 32) & 0x7FFFFFFF;
	return (Protocol::ObjectType)type;
}

bool ObjectManager::AddObject(ObjectRef object)
{
	if (_objects.find(object->objectInfo->object_id()) != _objects.end())
		return false;

	_objects.insert(make_pair(object->objectInfo->object_id(), object));

	return true;
}

bool ObjectManager::RemoveObject(uint64 objectId)
{
	if (_objects.find(objectId) == _objects.end())
		return false;

	ObjectRef object = _objects[objectId];
	PlayerRef player = dynamic_pointer_cast<Player>(object);
	if (player)
		player->room.store(weak_ptr<RoomBase>()); // null로 밀어주기

	_objects.erase(objectId);

	return true;
}

uint64 ObjectManager::GenerateIdLocked(Protocol::ObjectType objectType)
{
	WRITE_LOCK;
	return ((uint64)objectType << 32) | (s_objectIdGenerator++);
}
