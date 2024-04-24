#include "pch.h"
#include "RoomBase.h"
#include "Object.h"
#include "StartRoom.h"

StartRoom::StartRoom()
{
	
}

StartRoom::~StartRoom()
{
}

StartRoomRef StartRoom::GetRoomRef()
{
	return static_pointer_cast<StartRoom>(shared_from_this());
}

void StartRoom::UpdateTick()
{
	//__super::UpdateTick();

	// all object in start room process update tick 
	for (auto& object : _objects)
	{
		object.second->UpdateTick();
	}

	//cout << "Update StartRoom" << "\n";
	DoTimer(200, &StartRoom::UpdateTick);
}

bool StartRoom::EnterRoom(ObjectRef object, bool randPos)
{
	bool success = __super::EnterRoom(object, randPos);

	// 보스 몬스터 소환
	{
		//Protocol::S_SPAWN spawnPkt;
		//
		//Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		//objectInfo->CopyFrom(*object->objectInfo);
		//
		//SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		//Broadcast(sendBuffer, object->objectInfo->object_id());
	}
	
	return success;
}

bool StartRoom::LeaveRoom(ObjectRef object)
{
	return false;
}
