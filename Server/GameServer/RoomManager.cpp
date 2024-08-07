#include "pch.h"
#include "Object.h"
#include "Player.h"
#include "RoomBase.h"
#include "StartRoom.h"
#include "DungeonRoom.h"
#include "RoomManager.h"

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}

void RoomManager::InitializeAllRoom()
{
	// start room
	{
		StartRoomRef startRoom = make_shared<StartRoom>();
		string objFilePath = "../../Common/NavMesh/DevMap.obj";
		startRoom->roomName = "start room";
		startRoom->init(objFilePath);
		_rooms.insert(make_pair(Protocol::RoomType::ROOM_TYPE_START_ROOM, startRoom));
		startRoom->_roomType = Protocol::RoomType::ROOM_TYPE_START_ROOM;
	}

	// dungeon room one
	{
		DungeonRoomRef dungeonRoom = make_shared<DungeonRoom>();
		string objFilePath = "../../Common/NavMesh/DungeonMap_One.obj";
		dungeonRoom->roomName = "dungeon map one";
		dungeonRoom->init(objFilePath);
		_rooms.insert(make_pair(Protocol::RoomType::ROOM_TYPE_DUNGEON_ROOM, dungeonRoom));
		dungeonRoom->_roomType = Protocol::RoomType::ROOM_TYPE_DUNGEON_ROOM;
		dungeonRoom->SetInfo();
	}
}

void RoomManager::AllUpdateTickStart()
{
	//cout << "Update All Room" << "\n";
	//DoTimer(TICK_COUNT, &RoomManager::AllUpdateTickStart);

	for (auto roomData : _rooms)
	{
		roomData.second->UpdateTick();
	}
}

bool RoomManager::ChangeRoom(ObjectRef object, RoomBaseRef destRoom)
{
	if (object == nullptr)
		return false;

	const uint64 objectId = object->objectInfo->object_id();
	RoomBaseRef startRoom = object->GetRoomRef();

	cout << "Player : " << objectId << "'s Changing Room is started!!" << "\n";

	startRoom->DoAsync(&RoomBase::LeaveRoom, object);
	FVector3 pos = FVector3(-600.f, -400.f, 100.f);
	destRoom->DoAsync(&RoomBase::EnterRoom, object, false, pos);

	//// 다른 플레이어들에게 퇴장 사실을 알린다
	//{
	//	Protocol::S_DESPAWN despawnPkt;
	//	despawnPkt.add_object_ids(objectId);
	//
	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
	//	startRoom->DoAsync(&RoomBase::Broadcast, sendBuffer, objectId);
	//	//Broadcast(sendBuffer, objectId);
	//}
	//startRoom->DoAsync(&RoomBase::RemoveObject, objectId);
	//
	//// temp
	//// 시작 위치 조정
	//{
	//	object->posInfo->set_x(-600.f);
	//	object->posInfo->set_y(-400.f);
	//	object->posInfo->set_z(100.f);
	//	object->posInfo->set_dest_x(object->posInfo->x());
	//	object->posInfo->set_dest_y(object->posInfo->y());
	//	object->posInfo->set_dest_z(object->posInfo->z());
	//	object->posInfo->set_yaw(Utils::GetRandom(0.f, 100.f));
	//	object->posInfo->set_state(Protocol::MoveState::MOVE_STATE_IDLE);
	//}
	//
	//// 새로운 방에 입장시킨다.
	//destRoom->DoAsync(&RoomBase::AddObject, object);
	//
	//// 해당 플레이어에게 텔레포트를 실행하게 한다
	//{
	//	Protocol::S_TELEPORT teleportPkt;
	//	Protocol::ObjectInfo* info = teleportPkt.mutable_info();
	//	info->CopyFrom(*object->objectInfo);
	//
	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(teleportPkt);
	//	GameSessionRef session = object->session.lock();
	//	if (session != nullptr)
	//		session->Send(sendBuffer);
	//}
	//
	//// 입장 사실을 다른 플레이어에게 알린다
	//{
	//	Protocol::S_SPAWN spawnPkt;
	//	
	//	Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
	//	objectInfo->CopyFrom(*object->objectInfo);
	//
	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	//	destRoom->DoAsync(&RoomBase::Broadcast, sendBuffer, objectId);
	//}
	//
	//// 기존 입장한 플레이어 목록을 신입 플레이어에게 전송해준다
	//destRoom->DoAsync(&RoomBase::SendSpawnPktAboutOthers, object);

	return true;
}

RoomBaseRef RoomManager::GetRoom(uint64 roomId)
{
	if (_rooms.find(roomId) == _rooms.end())
		return nullptr;

	return _rooms[roomId];
}

StartRoomRef RoomManager::GetStartRoom()
{
	if (_rooms.find(Protocol::RoomType::ROOM_TYPE_START_ROOM) == _rooms.end())
		return nullptr;

	return static_pointer_cast<StartRoom>(_rooms[Protocol::RoomType::ROOM_TYPE_START_ROOM]);
}
