#include "pch.h"
#include "RoomBase.h"
#include "StartRoom.h"
#include "RoomManager.h"

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}

void RoomManager::InitializeAllRoom()
{
	StartRoomRef startRoom = make_shared<StartRoom>();
	_rooms.insert(make_pair(Protocol::RoomType::ROOM_TYPE_START_ROOM, startRoom));
}

void RoomManager::AllUpdateTickStart()
{
	//cout << "Update RoomManager" << "\n";
	//DoTimer(1000, &RoomManager::AllUpdateTick);

	for (auto roomData : _rooms)
	{
		roomData.second->UpdateTick();
	}
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
